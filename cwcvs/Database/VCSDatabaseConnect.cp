#include "VCSDatabaseConnect.h"

#include "VCSError.h"
#include "VCSPrefs.h"
#include "VCSResult.h"
#include "VCSTask.h"
#include "CVSCommand.h"
#include "StFile.h"
#include "StHandle.h"
#include "StHandleState.h"
#include "StPtr.h"
#include "cvs.h"

#include <Files.h>
#include <Folders.h>
#include <Script.h>

//	=== Constants ===

static const	unsigned	char
sCVSROOTKey[] = "\pCVSROOT";

static const	unsigned	char
sCVSPassFile[] = "\p.cvspass";

// ---------------------------------------------------------------------------
//		¥ VCSDatabaseConnect
// ---------------------------------------------------------------------------

VCSDatabaseConnect::VCSDatabaseConnect (

	VCSContext&	inContext)
	
	: VCSCommand (inContext, true)
	
	{ // begin VCSDatabaseConnect
		
	} // end VCSDatabaseConnect

// ---------------------------------------------------------------------------
//		¥ ~VCSDatabaseConnect
// ---------------------------------------------------------------------------

VCSDatabaseConnect::~VCSDatabaseConnect (void)
	
	{ // begin ~VCSDatabaseConnect
				
	} // end ~VCSDatabaseConnect

// ---------------------------------------------------------------------------
//		¥ DoRequest
// ---------------------------------------------------------------------------

CWVCSCommandStatus 
VCSDatabaseConnect::DoRequest (void)
	
	{ // begin DoRequest
		
		static	const	char	eol = 0x0D;
		static	const	char	sep = ' ';
		
		//	Prepare
		CWVCSDatabaseConnection	db;
		mContext.GetDatabase (db);
		
		VCSTask (mContext, kTaskStrings, kConnectTask, db.sDatabasePath.name);
		
		//	Launch the app
		ProcessSerialNumber	psn;
		if (noErr != VCSRaiseOSErr (mContext, CVSGetProcess (&psn))) 
			return cwCommandStatusFailed;
		
		//	Get CVSROOT
		Str255				cvsrootStr;
		if (!VCSPrefsGetEnv (mContext, sCVSROOTKey, cvsrootStr, nil)) 
			return cwCommandStatusFailed;
		
		//	Get CVS_PASSWD and scramble it
		StPtr				cvspass (::scramble (db.pPassword));
		
		//	Make the new line
		Handle	h = ::NewHandle (cvsrootStr[0] + sizeof (sep) + ::strlen (cvspass));
		if (noErr != VCSRaiseOSErr (mContext, ::MemError ())) return cwCommandStatusFailed;
		StHandle			newLine (h);
		::BlockMoveData (cvsrootStr + 1, *newLine, cvsrootStr[0]);
		::BlockMoveData (&sep, *newLine + cvsrootStr[0], sizeof (sep));
		::BlockMoveData (cvspass, *newLine + cvsrootStr[0] + sizeof (sep), ::strlen (cvspass));
		
		//	Find the prefs folder
		FSSpec				prefsSpec;
		if (noErr != VCSRaiseOSErr (mContext, ::FindFolder (kOnSystemDisk, kPreferencesFolderType, kCreateFolder, &prefsSpec.vRefNum, &prefsSpec.parID))) 
			return cwCommandStatusFailed;
		
		//	Find the temp folder
		FSSpec				tempSpec;
		if (noErr != VCSRaiseOSErr (mContext, ::FindFolder (kOnSystemDisk, kTemporaryFolderType, kCreateFolder, &tempSpec.vRefNum, &tempSpec.parID))) 
			return cwCommandStatusFailed;

		//	Find the password file
		FSSpec				oldSpec;
		if (noErr != ::FSMakeFSSpec (prefsSpec.vRefNum, prefsSpec.parID, sCVSPassFile, &oldSpec)) {
			//	Create it if it doesn't exist
			if (noErr != VCSRaiseOSErr (mContext, ::FSpCreate (&oldSpec, 'MWIE', 'TEXT', smDefault))) 
				return cwCommandStatusFailed;
			} // if
		
		//	Create the temp file
		FSSpec				newSpec;
		if (noErr == ::FSMakeFSSpec (tempSpec.vRefNum, tempSpec.parID, sCVSPassFile, &newSpec)) {
			//	Delete it if it exists
			if (noErr != VCSRaiseOSErr (mContext, ::FSpDelete (&newSpec)))
				return cwCommandStatusFailed;
			} // if
		
		if (noErr != VCSRaiseOSErr (mContext, ::FSpCreate (&newSpec, 'MWIE', 'TEXT', smDefault))) 
			return cwCommandStatusFailed;
		
		//	Process the file
		Boolean				changed = false;
		{
			//	Open the temp file
			short				refNum;
			if (noErr != VCSRaiseOSErr (mContext, ::FSpOpenDF (&newSpec, fsRdWrPerm, &refNum))) return cwCommandStatusFailed;
			StFile				newFile (refNum);
			Size				writeSize = 0;
			
			//	Read the password file
			if (noErr != VCSRaiseOSErr (mContext, ::ReadFileContents (&h, &oldSpec))) return cwCommandStatusFailed;
			StHandle			contents (h);

			//	Scan it, checking each line against what we have
			Boolean				found = false;
			while (contents.GetSize ()) {
				if (noErr != VCSRaiseOSErr (mContext, GetNextLine (&h, contents))) return cwCommandStatusFailed;
				StHandle		line (h);
				Size			writeSize = line.GetSize ();

				if (changed ||
					(false == (found = found || (0 == ::memcmp (*line, cvsrootStr + 1, cvsrootStr[0])))) ||
					((writeSize == newLine.GetSize ()) && (0 == ::memcmp (*line, *newLine, writeSize)))) {
					StHandleState	lineState (line);
					line.Lock ();
					if (noErr != VCSRaiseOSErr (mContext, newFile.Write (writeSize, *line))) return cwCommandStatusFailed;
					} // if
				
				else {
					writeSize = newLine.GetSize ();
					StHandleState	newLineState (newLine);
					newLine.Lock ();
					if (noErr != VCSRaiseOSErr (mContext, newFile.Write (writeSize, *newLine))) return cwCommandStatusFailed;
					changed = found = true;
					} // else
					
				writeSize = sizeof (eol);
				if (noErr != VCSRaiseOSErr (mContext, newFile.Write (writeSize, &eol))) return cwCommandStatusFailed;
				} // while
				
			if (!found) {
				writeSize = newLine.GetSize ();
				StHandleState	newLineState (newLine);
				newLine.Lock ();
				if (noErr != VCSRaiseOSErr (mContext, newFile.Write (writeSize, *newLine))) return cwCommandStatusFailed;
				changed = found = true;

				writeSize = sizeof (eol);
				if (noErr != VCSRaiseOSErr (mContext, newFile.Write (writeSize, &eol))) return cwCommandStatusFailed;
				} // if
			}
		
		//	Swap them if we changed it
		if (changed) {
			if (noErr != VCSRaiseOSErr (mContext, ::FSpExchangeFiles (&newSpec, &oldSpec))) return cwCommandStatusFailed;
			::FSpDelete (&newSpec);
			} // if
			
		//	Success!
		return cwCommandStatusSucceeded;

	} // end DoRequest

