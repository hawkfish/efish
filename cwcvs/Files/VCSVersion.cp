#include "VCSVersion.h"

#include "VCSError.h"
#include "VCSPrefs.h"
#include "VCSResult.h"
#include "VCSTask.h"
#include "VCSUtil.h"

#include "CVSCommand.h"

#include "FullPath.h"
#include "MoreFilesExtras.h"

#include <stat.h>
#include <string.h>
#include <time.mac.h>

#include <TextUtils.h>

//	=== Constants ===

// ---------------------------------------------------------------------------
//		¥ SetFinderLabel
// ---------------------------------------------------------------------------

static OSErr 
SetFinderLabel (
	
	const	FSSpec*	spec, 
	short 			label)
	
	{ // begin SetFinderLabel
		
		OSErr	e = noErr;
		
		if (noErr != (e = FSpChangeFDFlags (spec, false, kColor))) goto CleanUp;
		if (noErr != (e = FSpChangeFDFlags (spec, true, (label << 1) & kColor))) goto CleanUp;
		
		/* can't bump fsRtParID */
		if (fsRtParID == spec->parID) goto CleanUp;

		/* bump the parent directory's mod date to wake up the Finder */
		/* to the change we just made */
		if (noErr != (e = BumpDate (spec->vRefNum, spec->parID, nil))) goto CleanUp;
	
	CleanUp:
	
		return e;
		
	} // end SetFinderLabel
	
// ---------------------------------------------------------------------------
//		¥ VCSVersion
// ---------------------------------------------------------------------------

VCSVersion::VCSVersion (

	VCSContext&	inContext)
	
	: VCSFileCommand (inContext, true, true)
		
	{ // begin VCSVersion
		
	} // end VCSVersion

// ---------------------------------------------------------------------------
//		¥ ~VCSVersion
// ---------------------------------------------------------------------------

VCSVersion::~VCSVersion (void)
	
	{ // begin ~VCSVersion
		
	} // end ~VCSVersion

// ---------------------------------------------------------------------------
//		¥ IterateFile
// ---------------------------------------------------------------------------

void 
VCSVersion::IterateFile (

	const CInfoPBRec& 	 	cpbPtr,
	long					dirID,
	Boolean&				quitFlag)

	{ // begin IterateFile
		
		VCSTask 	task (mContext, kTaskStrings, kVersionTask, cpbPtr.hFileInfo.ioNamePtr);
		
		VCSFileCommand::IterateFile (cpbPtr, dirID, quitFlag);
		
	} // end IterateFile
	
// ---------------------------------------------------------------------------
//		¥ CalcTimeStamp
// ---------------------------------------------------------------------------

static const char*
CalcTimeStamp (

	time_t	inTime)
	
	{ // begin CalcTimeStamp
	
		char*				result;
		struct	tm*			test_tm = gmtime (&inTime);
		
		if (test_tm) {
			struct	tm	local_tm = *test_tm;
	    	
	    	result = asctime (&local_tm);
			} // if
		
		else result = ctime (&inTime);
		
		result[24] = 0;
		
		return result;
		
	} // end CalcTimeStamp
	
// ---------------------------------------------------------------------------
//		¥ ParseEntriesLine
// ---------------------------------------------------------------------------

static Boolean
ParseEntriesLine (

	const	FSSpec*			inSpec,
	Handle					line,
	Handle*					outDate,
	Handle*					outVersion,
	Handle*					outKeywords)
	
	{ // begin ParseEntriesLine
	
		const	char		slash = '/';
		
		Str255				fileName;
		long				slashPos;
		
		/*	
		/CVSVCS.mcp/1.2/Fri Dec 10 23:43:38 1993/-kb/

		/VCSAbout.c/1.1/Sat Dec 11 23:41:43 1993//
		/VCSAbout.c/1.1/Wed Dec 10 15:41:43 1997//

		/VCSHistory.c/1.4/Fri Dec 10 22:36:38 1993//
		/VCSHistory.c/1.4/Tue Dec  9 14:36:38 1997//

		/VCSVersion.c/1.4/Fri Dec 10 23:20:32 1993//
		/VCSVersion.c/1.4/Thu Dec 11 09:16:32 1993//
		*/

		//	Parse the Command
		slashPos = Munger (line, 0, &slash, sizeof (slash), nil, 0);
		if (slashPos < 0) return false;
		Munger (line, 0, nil, slashPos + 1, &slash, 0);

		//	Parse the file name
		slashPos = Munger (line, 0, &slash, sizeof (slash), nil, 0);
		if (slashPos < 0) return false;
		BlockMoveData (*line, fileName + 1, fileName[0] = slashPos);
		if (!EqualString (fileName, inSpec->name, false, true)) return false;
		Munger (line, 0, nil, slashPos + 1, &slash, 0);
		
		//	Parse the version
		slashPos = Munger (line, 0, &slash, sizeof (slash), nil, 0);
		if (slashPos < 0) return false;
		if (outVersion) {
			HLock (line);
			PtrToHand (*line, outVersion, slashPos);
			HUnlock (line);
			} // if
		Munger (line, 0, nil, slashPos + 1, &slash, 0);
		
		//	Parse the timestamp
		slashPos = Munger (line, 0, &slash, sizeof (slash), nil, 0);
		if (slashPos < 0) return false;
		if (outDate) {
			HLock (line);
			PtrToHand (*line, outDate, slashPos);
			HUnlock (line);
			} // if
		Munger (line, 0, nil, slashPos + 1, &slash, 0);
		
		//	Parse the keywords
		slashPos = Munger (line, 0, &slash, sizeof (slash), nil, 0);
		if (slashPos < 0) return false;
		if (outKeywords) {
			HLock (line);
			PtrToHand (*line, outKeywords, slashPos);
			HUnlock (line);
			} // if
		Munger (line, 0, nil, slashPos + 1, &slash, 0);
		
		return true;
	
	} // end ParseEntriesLine
	
// ---------------------------------------------------------------------------
//		¥ ParseEntriesFile
// ---------------------------------------------------------------------------

OSErr
VCSVersion::ParseEntriesFile (

	const	FSSpec*			inSpec,
	Handle*					outDate,
	Handle*					outVersion,
	Handle*					outKeywords)
	
	{ // begin ParseEntriesFile
		
		OSErr				e = noErr;
		
		Handle				entries = nil;

		if (outDate) *outDate = nil;
		if (outVersion) *outVersion = nil;
		
		//	Read the Entries file
		do {
			//	File not found is _not_ an error
			//	It just means that the directory had not been added
			FSSpec				entriesSpec;
			if (noErr != ::FSMakeFSSpec (inSpec->vRefNum, inSpec->parID, "\p:CVS:ENTRIES", &entriesSpec)) break;
			
			//	Being unable to read the file _is_ an error
			if (noErr != (e = ReadFileContents (&entries, &entriesSpec))) break;
				
			//	Find the file data
			while (0 != GetHandleSize (entries)) {
				Handle	line = nil;
				if (noErr != GetNextLine (&line, entries)) break;
				
				Boolean	found = ParseEntriesLine (inSpec, line, outDate, outVersion, outKeywords);
				
				DisposeHandle (line);
				line = nil;
				
				if (found) break;
				} // while
			} while (false);
			
		if (entries != nil) DisposeHandle (entries);
		entries = nil;
		
		return e;
		
	} // end ParseEntriesFile

// ---------------------------------------------------------------------------
//		¥ GetCheckoutState
// ---------------------------------------------------------------------------

CWVCSCheckoutState 
VCSVersion::GetCheckoutState (
	
	const	FSSpec*			inSpec)

	{ // begin GetCheckoutState
	
		const	char		null = 0;

		CWVCSCheckoutState	eCheckoutState = cwCheckoutStateNotInDatabase;
		
		Handle				fullPath = nil;
		Handle				versionText = nil;
		Handle				dateText = nil;
		
		do {
			short				fullPathLength;
			if (noErr != VCSRaiseOSErr (mContext, FSpGetFullPath (inSpec, &fullPathLength, &fullPath))) break;
			if (noErr != VCSRaiseOSErr (mContext, PtrAndHand (&null, fullPath, sizeof (null)))) break;
			HLock (fullPath);

			//	Get the Unix time data
			struct	stat		file_stat;
			stat (*fullPath, &file_stat);
#if __MSL__ == 0x6000
			file_stat.st_mtime -= _mac_msl_epoch_offset_;
			file_stat.st_mtime -= _mac_msl_epoch_offset_;
#endif			
			//	Read the Entries file
			if (noErr != VCSRaiseOSErr (mContext, ParseEntriesFile (inSpec, &dateText, &versionText, 0))) break;
			
			//	Extract the state from the version and date
			if (versionText && dateText) {
				if (**versionText == '-') {
					//	Negative version is uncommited delete
					eCheckoutState = cwCheckoutStateNotInDatabase;
					} // if
					
				else {
					if (noErr != VCSRaiseOSErr (mContext, PtrAndHand (&null, dateText, sizeof (null)))) break;
					
					HLock (dateText);
					
					//	Compare it to the file version.
					if (strcmp (CalcTimeStamp (file_stat.st_mtime), *dateText))
						eCheckoutState = cwCheckoutStateCheckedOut;
					
					else eCheckoutState = (noErr == FSpCheckObjectLock (inSpec))
										? cwCheckoutStateCheckedOut
										: cwCheckoutStateNotCheckedOut;
					} // else
				} // if
			} while (false);
			
		if (fullPath != nil) DisposeHandle (fullPath);
		fullPath = nil;

		if (versionText != nil) DisposeHandle (versionText);
		versionText = nil;
		
		if (dateText != nil) DisposeHandle (dateText);
		dateText = nil;

		return eCheckoutState;
		
	} // end GetCheckoutState

// ---------------------------------------------------------------------------
//		¥ ProcessRegularFile
// ---------------------------------------------------------------------------

CWVCSItemStatus 
VCSVersion::ProcessRegularFile (
	
	CWVCSItem& 		inItem)

	{ // begin ProcessRegularFile

		//	Prepare

		inItem.eItemStatus = cwItemStatusSucceeded;
		
		do {
			CWVCSCheckoutState	eCheckoutState = GetCheckoutState (&inItem.fsItem);
			CWVCSVersion		version;
			version.eVersionForm = cwVersionFormNumeric;
			version.sVersionData.numeric = 0;

			//	Update the IDE
			mContext.UpdateCheckoutState (inItem.fsItem, eCheckoutState, version);
		
			//	Update the finder
			FinderLabelIndex	finderLabelIndex = kOrphanedLabelIndex;

			switch (eCheckoutState) {
				case cwCheckoutStateCheckedOut:
					finderLabelIndex = kCheckedOutLabelIndex;
					break;
					
				case cwCheckoutStateNotCheckedOut:
					finderLabelIndex = kCheckedInLabelIndex;
					break;
				} // switch
				
			short				finderLabel;
			if (VCSGetFinderLabel (mContext, finderLabelIndex, &finderLabel))
				SetFinderLabel (&inItem.fsItem, finderLabel);
			} while (false);
			
	CleanUp:
	
		return inItem.eItemStatus;

	} // end ProcessRegularFile
