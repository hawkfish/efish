#include "CVSCommand.h"

#include "CVSLog.h"
#include "VCSDialogPrep.h"
#include "VCSPrefs.h"

#include "SSSignatureToApp.h"
#include "MoreFilesExtras.h"
#include "VCSResult.h"
#include "Kerberos.h"

#include <string.h>

#include <Errors.h>
#include <Folders.h>
#include <TextUtils.h>

//	===	Constants ===

const	OSType	
kMacCVSCreator = 'mCVS';

static const	unsigned	char
sCVSROOTKey[] = "\pCVSROOT";

// ---------------------------------------------------------------------------
//		´ ProcessIsRunning
// ---------------------------------------------------------------------------

static Boolean			
ProcessIsRunning (

	const	ProcessSerialNumber*	inPSN) 
	
	{ // begin ProcessIsRunning

		ProcessSerialNumber		psn = {0, 0};
		
		for (;;) {
			Boolean	same;
			if (noErr != GetNextProcess (&psn)) return false;
			if (noErr != SameProcess (&psn, inPSN, &same)) return false;
			
			if (same) return true;
			} // for

		return false;
	
	} // end ProcessIsRunning

// ---------------------------------------------------------------------------
//		´ GetCVSVariable
// ---------------------------------------------------------------------------

static OSErr 
GetCVSVariable (
	
	AEDesc*				outCVSVariable,	
	const	FSSpec*		inCVSDir,
	ConstStr255Param	inCVSKey)
	
	{ // begin GetCVSVariable
	
		OSErr	e = noErr;
		
		FSSpec	keySpec;
		Boolean	isDirectory;
		
		//	Make the keyfile spec
		if (noErr != (e = FSpGetDirectoryID (inCVSDir, &keySpec.parID, &isDirectory))) goto CleanUp;
		if (noErr != (e = FSMakeFSSpec (inCVSDir->vRefNum, keySpec.parID, inCVSKey, &keySpec))) goto CleanUp;

		//	Open the file
		if (noErr != (e = ReadFileContents (&outCVSVariable->dataHandle, &keySpec))) goto CleanUp;
		
		outCVSVariable->descriptorType = typeChar;
		
	CleanUp:
		
		return e;

	} // end GetCVSVariable

// ---------------------------------------------------------------------------
//		´ MakePathHandle
// ---------------------------------------------------------------------------

static OSErr
MakePathHandle (

	Handle*			outPath,
	const	FSSpec*	inSpec)
	
	{ // begin MakePathHandle
		
		OSErr			e = noErr;
		
		FSSpec			tempSpec = *inSpec;
		const	char	sep = ':';
		
		if (noErr != (e = PtrToHand (tempSpec.name + 1, outPath, tempSpec.name[0]))) goto CleanUp;
		if (tempSpec.parID == fsRtParID) {
			if (noErr != (e = PtrAndHand (&sep, *outPath, sizeof (sep)))) goto CleanUp;
			
			goto CleanUp;
			} // if
			
		while (tempSpec.parID != fsRtParID) {
			if (noErr != (e = FSMakeFSSpec (tempSpec.vRefNum, tempSpec.parID, nil, &tempSpec))) goto CleanUp;
			
			tempSpec.name[++tempSpec.name[0]] = sep;
			(void) Munger (*outPath, 0, nil, 0, &tempSpec.name[1], tempSpec.name[0]);
			if (noErr != (e = MemError ())) goto CleanUp;
			} // while
			
	CleanUp:
		
		if (e) {
			if (*outPath) DisposeHandle (*outPath);
			*outPath = nil;
			} // if
			
		return e;
		
	} // end MakePathHandle
	
#pragma mark -

// ---------------------------------------------------------------------------
//		´ CVSCreateCommand
// ---------------------------------------------------------------------------

OSErr 
CVSCreateCommand (

	AEDesc*			outCommand, 
	const	char*	inName)

	{ // begin CVSCreateCommand
	
		OSErr 	e = noErr;
		
		outCommand->descriptorType = typeNull;
		outCommand->dataHandle = nil;
		
		if (noErr != (e = AECreateList (nil, 0 , false, outCommand))) goto CleanUp;
		if (noErr != (e = CVSAddCStringArg (outCommand, inName))) goto CleanUp;

	CleanUp:
	
		if (noErr != e) AEDisposeDesc (outCommand);
		
		return e;
	
	} // end CVSCreateCommand

// ---------------------------------------------------------------------------
//		´ CVSDisposeCommand
// ---------------------------------------------------------------------------

OSErr 
CVSDisposeCommand (

	AEDesc*			inCommand)

	{ // begin CVSDisposeCommand
	
		return AEDisposeDesc (inCommand);
	
	} // end CVSDisposeCommand

// ---------------------------------------------------------------------------
//		´ CVSAddCStringArg
// ---------------------------------------------------------------------------

OSErr 
CVSAddCStringArg (

	AEDesc*				command, 
	const	char*		string)

	{ // begin CVSAddCStringArg
	
		return AEPutPtr (command, 0, typeChar, string, strlen (string));
	
	} // end CVSAddCStringArg

// ---------------------------------------------------------------------------
//		´ CVSAddPStringArg
// ---------------------------------------------------------------------------

OSErr 
CVSAddPStringArg (

	AEDesc*				command, 
	ConstStr255Param	string)

	{ // begin CVSAddPStringArg
	
		return AEPutPtr (command, 0, typeChar, string + 1, string[0]);
	
	} // end CVSAddPStringArg

// ---------------------------------------------------------------------------
//		´ CVSAddCommentArg
// ---------------------------------------------------------------------------

OSErr 
CVSAddCommentArg (

	AEDesc*				outCommand, 
	ConstStr255Param	inComment)
	
	{// begin CVSAddCommentArg
		
		OSErr 		e = noErr;
		
		if (inComment[0] == 0) return noErr;
		
		if (noErr != (e = CVSAddCStringArg (outCommand, "-m"))) goto CleanUp;
		if (noErr != (e = CVSAddPStringArg (outCommand, inComment))) goto CleanUp;
		
	CleanUp:
		
		return e;
	
	} // end CVSAddCommentArg

// ---------------------------------------------------------------------------
//		¥ CVSAddListArgs
// ---------------------------------------------------------------------------

OSErr
CVSAddListArgs (

	AEDesc*			inCommand,
	AEDescList* 	inList)
	
	{ // begin CVSAddListArgs
		
		OSErr	e = noErr;
		long	listCount = 0;
		long	i;
		if (noErr != (e = AECountItems (inList, &listCount))) goto CleanUp;
		for (i = 1; i <= listCount; ++i) {
			AEKeyword	theKeyword;
			AEDesc		theDesc;
			if (noErr != (e = AEGetNthDesc (inList, i, typeChar, &theKeyword, &theDesc))) goto CleanUp;
			e = AEPutDesc (inCommand, 0, &theDesc);
			AEDisposeDesc (&theDesc);
			if (noErr != e) goto CleanUp;
			} // for
			
	CleanUp:
	
		return e;
		
	} // end CVSAddListArgs
	
// ---------------------------------------------------------------------------
//		´ CVSGetProcess
// ---------------------------------------------------------------------------

OSErr 
CVSGetProcess (
	
	ProcessSerialNumber*	psn)
	
	{ // begin CVSGetProcess
		
		FSSpec				appSpec; /* SignatureToApp requires this, documentation to the contrary.... */

		return SignatureToApp (kMacCVSCreator, NULL, psn, &appSpec, nil,
							  Sig2App_LaunchApplication, launchContinue + launchDontSwitch);
	
	} // end CVSGetProcess
	
// ---------------------------------------------------------------------------
//		´ CVSSendCommand
// ---------------------------------------------------------------------------

OSErr
CVSSendCommand (
	
	const 	VCSContext&				inPB,
	Handle*							outResult,
	const	ProcessSerialNumber*	inPSN,
	const	AEDesc*					inCommand,
	const	FSSpec*					inCVSLocal)

	{ // begin CVSSendCommand
		
		OSErr					e = noErr;
		
		const	long			mode ='FILE';
		const	Boolean			bufferSetting = true;
		FSSpec					tempSpec;
		Str255					cvsrootStr;
		
		//	Stuff to clean up
		AEDescList				envList = {typeNull, nil};
		
		AEDesc					localPath = {typeNull, nil};
		AEDesc					tempPath = {typeNull, nil};
		
		AppleEvent				theEvent = {typeNull, nil};
		AppleEvent				theReply = {typeNull, nil};
		AEDesc					address = {typeNull, nil};
		
		*outResult = nil;
		
		//	Check for CVSROOT
		if (!VCSPrefsGetEnv (inPB, sCVSROOTKey, cvsrootStr, nil)) return paramErr;
		
		//	Build the environment list
		if (noErr != (e = VCSPrefsMakeEnvDescList (inPB, &envList))) goto CleanUp;

		//	Build the local path spec
		if (noErr != (e = MakePathHandle (&localPath.dataHandle, inCVSLocal))) goto CleanUp;
		localPath.descriptorType = typeChar;
		
		//	Build the temp file spec
		if (noErr != (e = FindFolder (kOnSystemDisk, kTemporaryFolderType, kCreateFolder, &tempSpec.vRefNum, &tempSpec.parID))) goto CleanUp;
		NumToString (TickCount (), tempSpec.name);
		if (noErr != (e = MakePathHandle (&tempPath.dataHandle, &tempSpec))) goto CleanUp;
		tempPath.descriptorType = typeChar;
		
		// Construct the AppleEvent being sent to MacCVS.
		if (noErr != (e = AECreateDesc (typeProcessSerialNumber, inPSN, sizeof(*inPSN), &address))) goto CleanUp;
		if (noErr != (e = AECreateAppleEvent ('misc','dosc', &address, kAutoGenerateReturnID, kAnyTransactionID, &theEvent))) goto CleanUp;
		
		if (noErr != (e = AEPutKeyDesc (&theEvent, keyDirectObject, inCommand))) goto CleanUp;
		if (noErr != (e = AEPutKeyPtr (&theEvent,'MODE', typeEnumerated, &mode, sizeof (mode)))) goto CleanUp;
		if (noErr != (e = AEPutKeyPtr (&theEvent,'LBUF', typeBoolean, &bufferSetting, sizeof (bufferSetting)))) goto CleanUp;
		if (noErr != (e = AEPutKeyDesc (&theEvent,'SPWD', &localPath))) goto CleanUp;
		if (noErr != (e = AEPutKeyDesc (&theEvent, 'ENVT', &envList))) goto CleanUp;
		if (noErr != (e = AEPutKeyDesc (&theEvent, 'FILE', &tempPath))) goto CleanUp;
		
		//	Authenticate Kerberos
		{
			char*	cvsroot = p2cstr (cvsrootStr);
			if (strstr (cvsroot, ":kserver:") && (noErr != KClientStatus ())) {
				KClientKey	key;
				VCSDialogPrep	prep (inPB);
				KClientLogin (&ksession, &key);
				} // if
			c2pstr ((char*) cvsroot);
		}
		
		//	Send the event
		CVSLogAppleEvent (&theEvent);
		if (noErr != (e = AESend (&theEvent, &theReply, kAENoReply, kAEHighPriority, 0, 0L, 0L))) goto CleanUp;
		
		//	Wait for it to finish
		while (ProcessIsRunning (inPSN) && !inPB.YieldTime ()) {
			EventRecord theEvent;
			WaitNextEvent (0, &theEvent, 6, nil);
			if (noErr != ReadFileContents (outResult, &tempSpec)) continue;
			
			FSpDelete (&tempSpec);
			CVSLogResult (*outResult);
			goto CleanUp;
			} // while
		
		if (noErr != ReadFileContents (outResult, &tempSpec)) goto CleanUp;
		
		FSpDelete (&tempSpec);
		CVSLogResult (*outResult);
		
	CleanUp:
		
		AEDisposeDesc (&address);
		AEDisposeDesc (&theReply);
		AEDisposeDesc (&theEvent);

		AEDisposeDesc (&localPath);
		AEDisposeDesc (&tempPath);

		AEDisposeDesc (&envList);
		
		return e;
	
	} // end CVSSendCommand

