#include "CVSLog.h"

#include <string.h>

#include <Files.h>
#include <Folders.h>
#include <Script.h>

static unsigned char
sLogFileName [] = "\pCWCVS.log";

static unsigned char
sEOL = '\r';

// ---------------------------------------------------------------------------
//		¥ LogHandle
// ---------------------------------------------------------------------------

static OSErr
LogHandle (

	short	fRefNum,
	Handle	h)
	
	{ // begin LogHandle
		
		OSErr		e = noErr;
		
		SignedByte	state = HGetState (h);
		Size		inOutCount = GetHandleSize (h);
		
		HLock (h);
		e = FSWrite (fRefNum, &inOutCount, *h);
	
	CleanUp:
	
		HSetState (h, state);
		
		return e;
		
	} // end LogHandle
	
// ---------------------------------------------------------------------------
//		¥ LogChar
// ---------------------------------------------------------------------------

static OSErr
LogChar (

	short	fRefNum,
	char	c)
	
	{ // begin LogChar
		
		Size		inOutCount = sizeof (c);
		
		return FSWrite (fRefNum, &inOutCount, &c);
	
	} // end LogChar
	
// ---------------------------------------------------------------------------
//		¥ LogCString
// ---------------------------------------------------------------------------

static OSErr
LogCString (

	short			fRefNum,
	const	char*	s)
	
	{ // begin LogCString
		
		Size		inOutCount = strlen (s);
		
		return FSWrite (fRefNum, &inOutCount, s);
	
	} // end LogCString
	
// ---------------------------------------------------------------------------
//		¥ LogPString
// ---------------------------------------------------------------------------

static OSErr
LogPString (

	short				fRefNum,
	ConstStr255Param	s)
	
	{ // begin LogPString
		
		Size		inOutCount = *s;
		
		return FSWrite (fRefNum, &inOutCount, s + 1);
	
	} // end LogPString
	
// ---------------------------------------------------------------------------
//		¥ LogTimeStamp
// ---------------------------------------------------------------------------

static OSErr
LogTimeStamp (

	short				fRefNum,
	const	char*		s)
	
	{ // begin LogTimeStamp
		
		OSErr				e = noErr;
		
		Str255				p;
		unsigned	long	rawSecs;
		
		GetDateTime (&rawSecs);
		
		if (noErr != (e = LogCString (fRefNum, "*** "))) goto CleanUp;
		if (noErr != (e = LogCString (fRefNum, s))) goto CleanUp;
		if (noErr != (e = LogChar (fRefNum, ' '))) goto CleanUp;
		
		IUDateString (rawSecs, shortDate, p);
		if (noErr != (e = LogPString (fRefNum, p))) goto CleanUp;
		if (noErr != (e = LogChar (fRefNum, ' '))) goto CleanUp;

		IUTimeString (rawSecs, true, p);
		if (noErr != (e = LogPString (fRefNum, p))) goto CleanUp;
		if (noErr != (e = LogCString (fRefNum, " ***"))) goto CleanUp;
		if (noErr != (e = LogChar (fRefNum, sEOL))) goto CleanUp;
	
	CleanUp:
	
		return e;
		
	} // end LogTimeStamp
	
// ---------------------------------------------------------------------------
//		¥ MakeLogFSSpec
// ---------------------------------------------------------------------------

static OSErr
MakeLogFSSpec (

	FSSpec*	outSpec)
	
	{ // begin MakeLogFSSpec
		
		OSErr	e = noErr;
		
		if (noErr != (e = FindFolder (kOnSystemDisk, kPreferencesFolderType, kCreateFolder, &outSpec->vRefNum, &outSpec->parID))) goto CleanUp;
		if (noErr != (e = FSMakeFSSpec (outSpec->vRefNum, outSpec->parID, sLogFileName, outSpec))) goto CleanUp;
	
	CleanUp:
	
		return e;
		
	} // end MakeLogFSSpec

#pragma mark -

// ---------------------------------------------------------------------------
//		¥ CVSLogAppleEvent
// ---------------------------------------------------------------------------

OSErr
CVSLogAppleEvent (

	const	AppleEvent*	inEvent)
	
	{ // begin CVSLogAppleEvent
		
		OSErr	e = noErr;
		
		FSSpec	logSpec;
		long	argCount;
		long	i;
		
		//	Stuff to clean up
		short	fRefNum = -1;
		AEDesc	args = {typeNull, nil};
		AEDesc	arg = {typeNull, nil};
		
		//	Open the file
		if (noErr != MakeLogFSSpec (&logSpec))
			FSpCreate (&logSpec, 'CWIE', 'TEXT', smDefault);
		
		if (noErr != (e = FSpOpenDF (&logSpec, fsRdWrPerm, &fRefNum))) goto CleanUp;
		if (noErr != (e = SetFPos (fRefNum, fsFromLEOF, 0))) goto CleanUp;
		
		//	Log the time stamp
		if (noErr != (e = LogTimeStamp (fRefNum, "Command"))) goto CleanUp;

		//	Log the environment (optional)
		if (noErr == AEGetParamDesc (inEvent, 'ENVT', typeAEList, &args)) {
			if (noErr != (e = AECountItems (&args, &argCount))) goto CleanUp;
			if (noErr != (e = LogCString (fRefNum, "setenv"))) goto CleanUp;
			if (noErr != (e = LogChar (fRefNum, sEOL))) goto CleanUp;
			
			for (i = 1; i <= argCount; ++i) {
				AEKeyword		key;
				
				if (noErr != (e = AEGetNthDesc (&args, i, typeChar, &key, &arg))) goto CleanUp;
				if (noErr != (e = LogHandle (fRefNum, arg.dataHandle))) goto CleanUp;
				if (noErr != (e = LogChar (fRefNum, (i % 2) ? '=' : sEOL))) goto CleanUp;

				AEDisposeDesc (&arg);
				} // for
			
			AEDisposeDesc (&args);
			} // if
			
		// Log the cwd (optional)
		if (noErr == AEGetParamDesc (inEvent, 'SPWD', typeChar, &arg)) {
			if (noErr != (e = LogCString (fRefNum, "pwd"))) goto CleanUp;
			if (noErr != (e = LogChar (fRefNum, sEOL))) goto CleanUp;
			if (noErr != (e = LogHandle (fRefNum, arg.dataHandle))) goto CleanUp;
			if (noErr != (e = LogChar (fRefNum, sEOL))) goto CleanUp;
			} // if
		
		//	Log the command line (required)
		if (noErr != (e = AEGetParamDesc (inEvent, '----', typeAEList, &args))) goto CleanUp;
		if (noErr != (e = AECountItems (&args, &argCount))) goto CleanUp;
		if (noErr != (e = LogCString (fRefNum, "cvs "))) goto CleanUp;
		
		for (i = 1; i <= argCount; ++i) {
			AEKeyword		key;
			
			if (noErr != (e = AEGetNthDesc (&args, i, typeChar, &key, &arg))) goto CleanUp;
			if (noErr != (e = LogHandle (fRefNum, arg.dataHandle))) goto CleanUp;
			if (noErr != (e = LogChar (fRefNum, (argCount == i) ? sEOL : ' '))) goto CleanUp;

			AEDisposeDesc (&arg);
			} // for
		
		AEDisposeDesc (&args);
		
	CleanUp:
	
		AEDisposeDesc (&args);
		AEDisposeDesc (&arg);
		
		if (fRefNum != -1) FSClose (fRefNum);
		fRefNum = -1;
		
		return e;
		
	} // end CVSLogAppleEvent
	
// ---------------------------------------------------------------------------
//		¥ CVSLogResult
// ---------------------------------------------------------------------------

OSErr
CVSLogResult (

	Handle	inResult)
	
	{ // begin CVSLogResult
		
		OSErr	e = noErr;
		
		FSSpec	logSpec;
		
		//	Stuff to clean up
		short	fRefNum = -1;
		
		if (!inResult) return noErr;
		
		//	Open the file
		if (noErr != MakeLogFSSpec (&logSpec))
			FSpCreate (&logSpec, 'CWIE', 'TEXT', smDefault);
		
		if (noErr != (e = FSpOpenDF (&logSpec, fsRdWrPerm, &fRefNum))) goto CleanUp;
		if (noErr != (e = SetFPos (fRefNum, fsFromLEOF, 0))) goto CleanUp;
		
		//	Log the time stamp
		if (noErr != (e = LogTimeStamp (fRefNum, "Result"))) goto CleanUp;

		//	Log the result
		if (noErr != (e = LogHandle (fRefNum, inResult))) goto CleanUp;

	CleanUp:
	
		if (fRefNum != -1) FSClose (fRefNum);
		fRefNum = -1;
		
		return e;
		
	} // end CVSLogResult
