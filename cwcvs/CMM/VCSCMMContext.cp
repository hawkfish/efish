/*
	File:		VCSCMMContext.cp

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


#include "VCSCMMContext.h"

#include "VCSPrompt.h"
#include "VCSUtil.h"
#include "MoreFilesExtras.h"

#include <Dialogs.h>
#include <Navigation.h>
#include <Folders.h>
#include <Resources.h>
#include <TextUtils.h>

#include <string.h>

//	Constants

enum {
	kPrefsFolderName = 1,
	kVCSName,
	
	kCMPreferencesStrings = 16010
	};

static unsigned char
sVCSName [] = "\pVCSName";

static unsigned char
sDatabaseName [] = "\pCMDatabase";

static unsigned char
sLocalRootName [] = "\pCMLocalRoot";

static unsigned char
sUserName [] = "\pCMUsername";

static const unsigned char
sPasswordName [] = "\pCMPassword";

static const unsigned char
sCommentName [] = "\pCMComment";

const 	OSType 	kCMProjectCreator 	= 'CMPj';
const 	OSType 	kCMProjectType 		= 'CMP¼';

const	ResType	type_PrefsResource	= 'pref';

// ---------------------------------------------------------------------------
//		€ FindPreferencesFolder
// ---------------------------------------------------------------------------

static OSErr 
FindPreferencesFolder (

	short&	vRefNum, 
	long&	folderID)

	{ // begin FindPreferencesFolder
	
		OSErr 	e = noErr;
		
		do {
			FSSpec 	ourFolder;
			if (noErr != (e = ::FindFolder (kOnSystemDisk, kPreferencesFolderType, kCreateFolder, &ourFolder.vRefNum, &ourFolder.parID))) break;

			::GetIndString (ourFolder.name, kCMPreferencesStrings, kPrefsFolderName);
			
			Boolean isFolder;
			switch (::FSpGetDirectoryID (&ourFolder, &folderID, &isFolder)) {
				case dirNFErr:
				case fnfErr:
					e = ::FSpDirCreate (&ourFolder, smSystemScript, &folderID);
					break;
					
				default:
					if (!isFolder) e = dirNFErr;
					break;
				} // switch
			if (noErr != e) break;
			
			vRefNum = ourFolder.vRefNum;
			} while (false);
			
		return e;
	
	} // end FindPreferencesFolder

// ---------------------------------------------------------------------------
//		€ FindProjectFile
// ---------------------------------------------------------------------------

OSErr
VCSCMMContext::FindProjectFile (

	FSSpec&				outProjectFile,
	const	FSSpec&		inContext)
		
	{ // begin FindProjectFile
		
		OSErr		e = noErr;
		
		FSSpec**	contextParents = nil;
		Size		parentCount = 0;
		do {
			//	Make the context list
			FSSpec	parent = inContext;
			if (noErr != (e = ::PtrToHand ((Ptr) &parent, (Handle*) &contextParents, sizeof (parent)))) break;
			++parentCount;
			
			while (parent.parID != fsRtDirID) {
				if (noErr != (e = ::FSMakeFSSpec (parent.vRefNum, parent.parID, nil, &parent))) break;
				if (noErr != (e = ::PtrAndHand ((Ptr) &parent, (Handle) contextParents, sizeof (parent)))) break;
				++parentCount;
				} // while
			if (noErr != e) break;
			
			// find the preferences folder
			short vRefNum;
			long folderID;
			if (noErr != (e = ::FindPreferencesFolder (vRefNum, folderID))) break;

			// loop over the projects in the preferences folder
			CInfoPBRec pb;
			for (pb.hFileInfo.ioFDirIndex = 1; e == noErr; ++pb.hFileInfo.ioFDirIndex) {
				//	Get the file info
				outProjectFile.name[0] = 0;
				pb.hFileInfo.ioNamePtr = outProjectFile.name;
				pb.hFileInfo.ioVRefNum = vRefNum;
				pb.hFileInfo.ioDirID = folderID;
				if (noErr != (e = ::PBGetCatInfoSync (&pb))) continue;
				
				//	Skip folders
				if (0 != (pb.hFileInfo.ioFlAttrib & ioDirMask)) continue;

				//	Skip non-project files
				if (pb.hFileInfo.ioFlFndrInfo.fdType != kCMProjectType) continue;

				//	Found it!
				outProjectFile.vRefNum = vRefNum;
				outProjectFile.parID = folderID;
				
				//	Open the file
				short	resRefNum = ::FSpOpenResFile (&outProjectFile, fsRdPerm);
				if (resRefNum < 0) continue;
				
				//	Check the plugin type matches
				StringHandle	str = (StringHandle) ::GetNamedResource ('STR ', sVCSName);
				if (!str) continue;
				::HLock ((Handle) str);
				
				Str255			vcsName;
				::GetIndString (vcsName, kCMPreferencesStrings, kVCSName);
				if (!::EqualString (*str, vcsName, true, true)) {
					::CloseResFile (resRefNum);
					continue;
					} // if
					
				//	Resolve the local root
				Boolean		wasChanged;
				AliasHandle	alias = (AliasHandle) ::GetNamedResource ('alis', sLocalRootName);
				FSSpec		localRoot;
				e = alias ? ::ResolveAlias (nil, alias, &localRoot, &wasChanged) : resNotFound;
				
				//	Close the file
				::CloseResFile (resRefNum);
				if (noErr != e) continue;
				
				//	Walk up the parents looking for a match
				for (Size p = 0; p < parentCount; ++p) {
					parent = (*contextParents)[p];
					if (!::FSpEqual (parent, localRoot)) continue;
					
					//	Found it!
					::DisposeHandle ((Handle) contextParents);
					contextParents = nil;
					return noErr;
					} // for
				} // for
			
			e = fnfErr;
			} while (false);
		
		if (contextParents) ::DisposeHandle ((Handle) contextParents);
		contextParents = nil;
		
		return e;
			
	} // end FindProjectFile

// ---------------------------------------------------------------------------
//		€ SaveProjectFile
// ---------------------------------------------------------------------------

OSErr
VCSCMMContext::SaveProjectFile (
	
	VCSContext&			inContext,
	ResID				inPrefsStrings,
	const	FSSpec&		inProjectSpec)
		
	{ // begin SaveProjectFile
		
		OSErr		e = noErr;
		
		SInt16		refNum = -1;
		ResID		rID;
		AliasHandle	alias = nil;
		Handle		rsrc = nil;
		
		do {
			//	Get the database info
			CWVCSDatabaseConnection	db;
			inContext.GetDatabase (db);
			
			//	Get the project info
			CWFileSpec	projectFileSpec;
			inContext.GetProjectFile (projectFileSpec);
			
			//	Make the temp spec
			FSSpec				tempSpec = inProjectSpec;
			if (noErr != (e = ::FindFolder (tempSpec.vRefNum, kTemporaryFolderType, kCreateFolder, &tempSpec.vRefNum, &tempSpec.parID))) break;
			
			//	Make the project file
			::FSpCreateResFile (&tempSpec, kCMProjectCreator, kCMProjectType, smDefault);
			if (noErr != (e = ::ResError ())) break;
			
			//	Open the file
			refNum = ::FSpOpenResFile (&tempSpec, fsRdWrPerm);
			if (noErr != (e = ::ResError ())) break;
			
			//	Add the plugin type
			Str255			vcsName;
			::GetIndString (vcsName, kCMPreferencesStrings, kVCSName);
			if (noErr != (e = ::PtrToHand (vcsName, &rsrc, 1 + vcsName[0]))) break;
			while ((rID = ::Unique1ID ('STR ')) < 128) continue;
			::AddResource (rsrc, 'STR ', rID, sVCSName);
			rsrc = nil;
					
			//	Add the database alias
			if (noErr != (e = ::NewAlias (nil, &db.sDatabasePath, &alias))) break;
			while ((rID = ::Unique1ID ('alis')) < 128) continue;
			::AddResource ((Handle) alias, 'alis', rID, sDatabaseName);
			if (noErr != (e = ::ResError ())) break;
			alias = nil;

			//	Add the local root alias
			if (noErr != (e = ::NewAlias (nil, &db.sProjectRoot, &alias))) break;
			while ((rID = ::Unique1ID ('alis')) < 128) continue;
			::AddResource ((Handle) alias, 'alis', rID, sLocalRootName);
			if (noErr != (e = ::ResError ())) break;
			alias = nil;
			
			//	Add the username
			if (noErr != (e = ::PtrToHand (db.pUsername, &rsrc, 1 + ::strlen (db.pUsername)))) break;
			::C2PStr (*rsrc);
			while ((rID = ::Unique1ID ('STR ')) < 128) continue;
			::AddResource (rsrc, 'STR ', rID, sUserName);
			rsrc = nil;
			
			//	Add the password
			if (noErr != (e = ::PtrToHand (db.pPassword, &rsrc, 1 + ::strlen (db.pPassword)))) break;
			::C2PStr (*rsrc);
			while ((rID = ::Unique1ID ('STR ')) < 128) continue;
			::AddResource (rsrc, 'STR ', rID, sPasswordName);
			rsrc = nil;
			
			//	Add the prefs
			for (short i = 1; ; ++i) {
				//	Get the name
				Str255	rName;
				::GetIndString (rName, inPrefsStrings, i);
				if (!rName[0]) break;
				
				//	Get the prefs
				CWMemHandle	h = inContext.GetNamedPreferences (::P2CStr (rName));
				::C2PStr ((char*) rName);
				
				//	Make a handle
				long		prefsSize = inContext.GetMemHandleSize (h);
				e = ::PtrToHand (inContext.LockMemHandle (h), &rsrc, prefsSize);
				inContext.UnlockMemHandle (h);
				if (noErr != e) break;

				//	Add the prefs
				while ((rID = ::Unique1ID (type_PrefsResource)) < 128) continue;
				::AddResource (rsrc, type_PrefsResource, rID, rName);
				rsrc = nil;
				} // for
			if (noErr != e) break;
			
			//	Close the file
			::CloseResFile (refNum);
			refNum = -1;
			
			//	Swap with the original
			FSSpec	testSpec;
			if (::FSMakeFSSpec (inProjectSpec.vRefNum, inProjectSpec.parID, inProjectSpec.name, &testSpec)) {
				if (noErr != (e = ::FSMakeFSSpec (inProjectSpec.vRefNum, inProjectSpec.parID, nil, &testSpec))) break;
				if (noErr != (e = ::FSpCatMove (&tempSpec, &testSpec))) break;
				} // if
				
			else {
				if (noErr != (e = ::FSpExchangeFiles (&tempSpec, &inProjectSpec))) break;
				} // else
			} while (false);
		
		if (rsrc) ::DisposeHandle (rsrc);
		rsrc = nil;

		if (alias) ::DisposeHandle ((Handle) alias);
		alias = nil;
		
		if (-1 != refNum) ::CloseResFile (refNum);
		refNum = -1;
		
		return e;
			
	} // end SaveProjectFile

// ---------------------------------------------------------------------------
//		€ MakeProjectFile
// ---------------------------------------------------------------------------

OSErr
VCSCMMContext::MakeProjectFile (
	
	VCSContext&			inContext,
	ResID				inPrefsStrings,
	ConstStr255Param	inProjectName,
	Boolean				inPrompt)
		
	{ // begin MakeProjectFile
		
		OSErr	e = noErr;
		
		AEDesc	defaultLocation = {typeNull, nil};
		
		do {
			//	Get the project info
			CWFileSpec	projectFileSpec;
			inContext.GetProjectFile (projectFileSpec);
			if (!inProjectName) inProjectName = projectFileSpec.name;
			
			// 	Make the project spec
			FSSpec	projectSpec;
			if (noErr != (e = ::FindPreferencesFolder (projectSpec.vRefNum, projectSpec.parID))) break;
			::BlockMoveData (inProjectName + 1, projectSpec.name + 1, projectSpec.name[0] = inProjectName[0]);
			
			//	Make the location
			FSSpec	dirSpec;
			if (noErr != (e = ::FSMakeFSSpec (projectSpec.vRefNum, projectSpec.parID, nil, &dirSpec))) break;
			if (noErr != (e = ::AECreateDesc (typeFSS, &dirSpec, sizeof (dirSpec), &defaultLocation))) break;
			
			if (inPrompt && NavServicesAvailable ()) {
				//	Ask for the location
				NavDialogOptions	dialogOptions;
				if (noErr != (e = ::NavGetDefaultDialogOptions (&dialogOptions))) break;
				dialogOptions.dialogOptionFlags |= kNavNoTypePopup;
				dialogOptions.dialogOptionFlags &= ~kNavAllowMultipleFiles;
				::BlockMoveData (inProjectName + 1, dialogOptions.savedFileName + 1, dialogOptions.savedFileName[0] = inProjectName[0]);
				
				NavReplyRecord		reply;
				if (noErr != (e = ::NavPutFile (&defaultLocation, &reply, &dialogOptions, nil, kCMProjectType, kCMProjectCreator, nil))) break;
				
				//	Extract it
				AEKeyword			theAEKeyword;
				DescType			typeCode;
				Size				actualSize;
				if (noErr != (e = ::AEGetNthPtr (&reply.selection, 1, typeFSS, &theAEKeyword, &typeCode, &projectSpec, sizeof (projectSpec), &actualSize))) break;
				} // if
				
			//	Save it
			if (noErr != (e = SaveProjectFile (inContext, inPrefsStrings, projectSpec))) break;
			} while (false);
		
		::AEDisposeDesc (&defaultLocation);
		
		return e;
		
	} // end MakeProjectFile
		
#pragma mark -

// ---------------------------------------------------------------------------
//		€ VCSCMMContext
// ---------------------------------------------------------------------------

VCSCMMContext::VCSCMMContext (
	
	const	FSSpec*						inContext,
	long								inRequest,
	Boolean								inAdvanced,
	Boolean								inRecursive,
	Boolean								inSupported)
	
	: VCSDialogContext (inContext, inRequest, inAdvanced, inRecursive, inSupported)

	{ // begin VCSCMMContext
		
		//	Username
		StringHandle	str = (StringHandle) ::GetNamedResource ('STR ', sUserName);
		if (str) {
			::BlockMoveData (*str + 1, mUserName, **str);
			mUserName[**str] = 0;
			} // if
		
		//	Password
		str = (StringHandle) ::GetNamedResource ('STR ', sPasswordName);
		if (str) {
			::BlockMoveData (*str + 1, mPassword, **str);
			mPassword[**str] = 0;
			} // if
		
		//	Comment
		str = (StringHandle) ::GetNamedResource ('STR ', sCommentName);
		if (str) 
			::BlockMoveData (*str + 1, mLastComment + 1, mLastComment[0] = **str);
		
	} // end VCSCMMContext

// ---------------------------------------------------------------------------
//		€ ~VCSCMMContext
// ---------------------------------------------------------------------------

VCSCMMContext::~VCSCMMContext (void)
	
	{ // begin ~VCSCMMContext
		
		if (mHaveResults) mResultsDialog.DoModalDialog ();
		
		do {
			Handle	rsrc = ::GetNamedResource ('STR ', sCommentName);
			if (!rsrc) {
				ResID		rID;
				while ((rID = ::Unique1ID ('STR ')) < 128) continue;
				rsrc = ::NewHandleClear (1);
				::AddResource (rsrc, 'STR ', rID, sCommentName);
				if (noErr != ::ResError ()) break;
				} // if
			
			if (noErr != (::PtrToXHand (mLastComment, rsrc, 1 + mLastComment[0]))) break;
			::ChangedResource (rsrc);
			} while (false);
		
	} // end VCSCMMContext

// ---------------------------------------------------------------------------
//		€ GetDatabase
// ---------------------------------------------------------------------------

void
VCSCMMContext::GetDatabase (

	CWVCSDatabaseConnection&	db) const
	
	{ // begin GetDatabase
		
		//	Database
		Boolean	wasChanged;
		AliasHandle	alias = (AliasHandle) ::GetNamedResource ('alis', sDatabaseName);
		if (alias) {
			::ResolveAlias (nil, alias, &db.sDatabasePath, &wasChanged);
			if (wasChanged) ::ChangedResource ((Handle) alias);
			} // if
		
		//	Local root
		alias = (AliasHandle) ::GetNamedResource ('alis', sLocalRootName);
		if (alias) {
			::ResolveAlias (nil, alias, &db.sProjectRoot, &wasChanged);
			if (wasChanged) ::ChangedResource ((Handle) alias);
			} // if
		
		//	Username
		db.pUsername = (char*) mUserName;
		
		//	Password
		db.pPassword = (char*) mPassword;

	} // end GetDatabase

// ---------------------------------------------------------------------------
//		€ GetNamedPreferences
// ---------------------------------------------------------------------------

CWMemHandle
VCSCMMContext::GetNamedPreferences (

	const char* prefsname) const
	
	{ // begin GetNamedPreferences
		
		Str255	resName;
		::BlockMoveData (prefsname, resName + 1, resName[0] = ::strlen (prefsname));
		
		return (CWMemHandle) ::GetNamedResource (type_PrefsResource, resName);

	} // end GetNamedPreferences

// ---------------------------------------------------------------------------
//		€ GetProjectFile
// ---------------------------------------------------------------------------

void
VCSCMMContext::GetProjectFile (

	CWFileSpec& 	outProjectSpec) const
	
	{ // begin GetProjectFile
		
		::memset (&outProjectSpec, 0, sizeof (outProjectSpec));

		FCBPBRec	pb;
		::memset (&pb, 0, sizeof (pb));
		pb.ioRefNum = ::CurResFile ();
		pb.ioNamePtr = outProjectSpec.name;
		
		if (noErr != ::PBGetFCBInfoSync (&pb)) return;
		
		outProjectSpec.vRefNum = pb.ioFCBVRefNum;
		outProjectSpec.parID = pb.ioFCBParID;
		
	} // end GetProjectFile

