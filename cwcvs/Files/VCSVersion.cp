#include "VCSVersion.h"

#include "VCSError.h"
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
//		€ VCSVersion
// ---------------------------------------------------------------------------

VCSVersion::VCSVersion (

	VCSContext&	inContext)
	
	: VCSFileCommand (inContext, true)
		
	{ // begin VCSVersion
		
	} // end VCSVersion

// ---------------------------------------------------------------------------
//		€ ~VCSVersion
// ---------------------------------------------------------------------------

VCSVersion::~VCSVersion (void)
	
	{ // begin ~VCSVersion
		
	} // end ~VCSVersion

// ---------------------------------------------------------------------------
//		€ CalcTimeStamp
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
//		€ ParseTimeStamp
// ---------------------------------------------------------------------------

static Boolean
ParseTimeStamp (

	long*					outState,
	const	FSSpec*			inSpec,
	Handle					line,
	const	struct	stat*	file_stat)
	
	{ // begin ParseTimeStamp
	
		const	char		slash = '/';
		
		Str255				fileName;
		long				slashPos;
		
		/*	
		/CVSVCS.mcp/1.2/Fri Dec 10 23:43:38 1993//

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
		if (**line == '-') {
			//	Negative version is uncommited delete
			*outState = cwCheckoutStateNotInDatabase;
			return true;
			} // if
		Munger (line, 0, nil, slashPos + 1, &slash, 0);
		
		//	Parse the timestamp
		slashPos = Munger (line, 0, &slash, sizeof (slash), nil, 0);
		if (slashPos < 0) return false;
		(*line)[slashPos] = 0;
		HLock (line);
		
		//	Compare it to the file version.
		if (strcmp (CalcTimeStamp (file_stat->st_mtime - _mac_unix_epoch_offset_), *line) && 
			strcmp (CalcTimeStamp (file_stat->st_mtime), *line))
			*outState = cwCheckoutStateCheckedOut;
		
		else *outState = (noErr == FSpCheckObjectLock (inSpec))
							? cwCheckoutStateCheckedOut
							: cwCheckoutStateNotCheckedOut;
		
		return true;
	
	} // end ParseTimeStamp
	
// ---------------------------------------------------------------------------
//		€ ProcessRegularFile
// ---------------------------------------------------------------------------

CWVCSItemStatus 
VCSVersion::ProcessRegularFile (
	
	CWVCSItem& 		inItem)

	{ // begin ProcessRegularFile

		const	char		null = 0;
		
		//	Stuff to clean up
		Handle				fullPath = nil;
		Handle				entries = nil;
		
		//	Prepare
		inItem.eItemStatus = cwItemStatusSucceeded;
		CWVCSCheckoutState	eCheckoutState = cwCheckoutStateNotInDatabase;
		CWVCSVersion		version;
		version.eVersionForm = cwVersionFormNumeric;
		version.sVersionData.numeric = 0;
		
		//	Get the Unix time data
		do {
			short				fullPathLength;
			if (noErr != FSpGetFullPath (&inItem.fsItem, &fullPathLength, &fullPath)) break;
			if (noErr != VCSRaiseOSErr (mContext, PtrAndHand (&null, fullPath, sizeof (null)))) break;
			HLock (fullPath);

			struct	stat		file_stat;
			stat (*fullPath, &file_stat);
			
			//	Read the Entries file
			FSSpec				entriesSpec;
			if (noErr == ::FSMakeFSSpec (inItem.fsItem.vRefNum, inItem.fsItem.parID, "\p:CVS:ENTRIES", &entriesSpec)) {
				if (noErr != VCSRaiseOSErr (mContext, ReadFileContents (&entries, &entriesSpec))) break;
				
				//	Find the file data
				while (0 != GetHandleSize (entries)) {
					Handle	line = nil;
					if (noErr != GetNextLine (&line, entries)) break;
					
					ParseTimeStamp (&eCheckoutState, &inItem.fsItem, line, &file_stat);
					
					DisposeHandle (line);
					line = nil;
					} // while
				} // if
			// else no CVS:ENTRIES => not in DB
			
			//	Update the IDE
			mContext.UpdateCheckoutState (inItem.fsItem, eCheckoutState, version);
			} while (false);
			
	CleanUp:
	
		if (fullPath != nil) DisposeHandle (fullPath);
		fullPath = nil;

		if (entries != nil) DisposeHandle (entries);
		entries = nil;
		
		return inItem.eItemStatus;

	} // end ProcessRegularFile
