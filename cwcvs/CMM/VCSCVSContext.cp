/*
	File:		VCSCVSContext.cp

	Contains:	Implementation of the CMM VCS context.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1997-9 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

        <12>      5/5/99    rmgw    Don't abort AppleEvent!
        <11>      5/5/99    rmgw    Split saving and prompting dialog.
        <10>      4/7/99    rmgw    Use display dialog for results windows under 32K
         <9>     3/31/99    rmgw    Check for cmd-period.
         <8>     3/31/99    rmgw    Add AE idling.
         <7>     3/31/99    rmgw    Fix comment result.
         <6>     3/30/99    rmgw    Crash recovery merging.
         <5>     3/22/99    rmgw    Add results dialog.
         <4>     3/19/99    rmgw    First pass at GUI.
         <3>     3/19/99    rmgw    Implement CM project documents.
         <2>     3/18/99    rmgw    Make file optional for handshake.
         <1>     3/17/99    rmgw    first checked in.
*/


#include "VCSCVSContext.h"

#include "VCSPrompt.h"
#include "VCSResult.h"
#include "VCSUtil.h"
#include "IterateDirectory.h"
#include "MoreFilesExtras.h"

#include <Dialogs.h>
#include <Navigation.h>
#include <Folders.h>
#include <Resources.h>
#include <TextUtils.h>

#include <string.h>

//	Constants

enum {
	kIllegalPanel = 0,
	
	kEnvironmentPanel,
	kOptionsPanel,
	
	kPanelStrings = 16000
	};

const	ResType	type_PrefsResource	= 'pref';

static unsigned char
sCVSName [] = "\pCVS";

static unsigned char
sRootName [] = "\pRoot";

// ---------------------------------------------------------------------------
//		¥ FindCVSDir
// ---------------------------------------------------------------------------

OSErr
VCSCVSContext::FindCVSDir (
	
	FSSpec&				outCVSDir,
	const	FSSpec&		inContext)
	
	{ // begin FindCVSDir
		
		OSErr	e = noErr;
		
		do {
			Boolean	isDir;
			long	dirID;
			if (noErr != (e = ::FSpGetDirectoryID (&inContext, &dirID, &isDir))) break;

			outCVSDir = inContext;
			if (isDir) outCVSDir.parID = dirID;
			if (noErr != (e = ::FSMakeFSSpec (outCVSDir.vRefNum, outCVSDir.parID, sCVSName, &outCVSDir))) break;
			} while (false);
			
		return e;

	} // end FindCVSDir

// ---------------------------------------------------------------------------
//		¥ FindLocalRoot
// ---------------------------------------------------------------------------

OSErr
VCSCVSContext::FindLocalRoot (
	
	FSSpec&				outLocalRoot,
	const	FSSpec&		inContext)
	
	{ // begin FindLocalRoot
		
		OSErr	e = noErr;
		
		outLocalRoot = inContext;
		do {
			//	Find the CVS directory
			FSSpec	cvsDir;
			if (noErr != (e = FindCVSDir (cvsDir, outLocalRoot))) break;
			
			for (;;) {
				if (noErr != (e = ::FSMakeFSSpec (cvsDir.vRefNum, cvsDir.parID, nil, &outLocalRoot))) break;
				
				if (noErr != ::FSMakeFSSpec (outLocalRoot.vRefNum, outLocalRoot.parID, sCVSName, &cvsDir)) break;
				} // for
			} while (false);
			
		return e;
		
	} // end FindLocalRoot

#pragma mark -

// ---------------------------------------------------------------------------
//		¥ VCSCVSContext
// ---------------------------------------------------------------------------

VCSCVSContext::VCSCVSContext (
	
	h_CWVCSItemState					inItems,
	long								inRequest,
	Boolean								inAdvanced,
	Boolean								inRecursive,
	Boolean								inSupported)
	
	: VCSDialogContext (inItems, inRequest, inAdvanced, inRecursive, inSupported)

	, mEnvList ((EnvironmentListHandle) ::NewHandleClear (sizeof (EnvironmentList)))
	
	{ // begin VCSCVSContext
		
		//	Set up the local root
		CheckResult (FindLocalRoot (mLocalRoot, (**inItems).item.fsItem));
		
		//	Build the environment list
		EnvListClear (mEnvList);
		
		//	Find the CVS directory
		Boolean	isDir;
		FSSpec	cvsSpec = mLocalRoot;
		CheckResult (::FSpGetDirectoryID (&mLocalRoot, &cvsSpec.parID, &isDir));
		CheckResult (::FSMakeFSSpec (cvsSpec.vRefNum, cvsSpec.parID, sCVSName, &cvsSpec));
		
		//	Find the CVS:Root file
		FSSpec	rootSpec = cvsSpec;
		CheckResult (::FSpGetDirectoryID (&cvsSpec, &rootSpec.parID, &isDir));
		CheckResult (::FSMakeFSSpec (rootSpec.vRefNum, rootSpec.parID, sRootName, &rootSpec));
		
		//	Read the Root file
		Handle		root = nil;
		CheckResult (ReadFileContents (&root, &rootSpec));
			
		//	Find the file data
		while (0 != GetHandleSize (root)) {
			Handle	line = nil;
			if (noErr != GetNextLine (&line, root)) break;
			
			Str255	value;
			::BlockMoveData (*line, value + 1, value[0] = ::GetHandleSize (line));
			::DisposeHandle (line);
			line = nil;
			
			EnvListSet (mEnvList, "\pCVSROOT", value);
			break;
			} // while
		::DisposeHandle (root);
		root = nil;
		
	} // end VCSCVSContext

// ---------------------------------------------------------------------------
//		¥ ~VCSCVSContext
// ---------------------------------------------------------------------------

VCSCVSContext::~VCSCVSContext (void)
	
	{ // begin ~VCSCVSContext
		
		::DisposeHandle ((Handle) mEnvList);
		mEnvList = nil;
		
	} // end VCSCVSContext

// ---------------------------------------------------------------------------
//		¥ GetDatabase
// ---------------------------------------------------------------------------

void
VCSCVSContext::GetDatabase (

	CWVCSDatabaseConnection&	db) const
	
	{ // begin GetDatabase
		
		//	Database
		db.sDatabasePath = mLocalRoot;
		
		//	Local root
		db.sProjectRoot = mLocalRoot;
		
		//	Username
		db.pUsername = (char*) mUserName;
		
		//	Password
		db.pPassword = (char*) mPassword;

	} // end GetDatabase

// ---------------------------------------------------------------------------
//		¥ GetNamedPreferences
// ---------------------------------------------------------------------------

CWMemHandle
VCSCVSContext::GetNamedPreferences (

	const char* prefsname) const
	
	{ // begin GetNamedPreferences
		
		Str255	resName;
		::BlockMoveData (prefsname, resName + 1, resName[0] = ::strlen (prefsname));
		
		Str255	panelName;
		::GetIndString (panelName, kPanelStrings, kEnvironmentPanel);
		if (::EqualString (panelName, resName, false, true)) return (CWMemHandle) mEnvList;
		
		return (CWMemHandle) ::GetNamedResource (type_PrefsResource, resName);

	} // end GetNamedPreferences

// ---------------------------------------------------------------------------
//		¥ FindProjectFilterProc
// ---------------------------------------------------------------------------

static pascal void 
FindProjectFilterProc (

	const CInfoPBRec * const 	cpbPtr,
	long 						dirID,
	Boolean*					quitFlag,
	void*						yourDataPtr)
	
	{ // begin FindProjectFilterProc
		
		if ((cpbPtr->hFileInfo.ioFlAttrib & ioDirMask) != 0) return;
		if (cpbPtr->hFileInfo.ioFlFndrInfo.fdType != 'MMPr') return;
		
		FSSpec*					projectSpec = (FSSpec*) yourDataPtr;
		if (noErr == ::FSMakeFSSpec (cpbPtr->hFileInfo.ioVRefNum, dirID, cpbPtr->hFileInfo.ioNamePtr, projectSpec)) 
			*quitFlag = true;
		
	} // end FindProjectFilterProc
											  
// ---------------------------------------------------------------------------
//		¥ GetProjectFile
// ---------------------------------------------------------------------------

void
VCSCVSContext::GetProjectFile (

	CWFileSpec& 	outProjectSpec) const
	
	{ // begin GetProjectFile
		
		outProjectSpec = mLocalRoot;
		::FSpIterateDirectory (&mLocalRoot, 3, FindProjectFilterProc, &outProjectSpec);
		
	} // end GetProjectFile

