// ===========================================================================
//	VCSUtil.c ©1997-8 Electric Fish, Inc. All rights reserved.
// ===========================================================================

#include "VCSUtil.h"

#include "VCSError.h"
#include "VCSResult.h"
#include "VCSTask.h"

#include "CVSCommand.h"
#include "StHandle.h"

#include "MoreFilesExtras.h"

#include <Errors.h>
#include <TextUtils.h>

//	=== Constants ===

// ---------------------------------------------------------------------------
//		´ AppendPString
// ---------------------------------------------------------------------------

StringPtr
AppendPString (

	StringPtr			s,
	ConstStr255Param	c)
	
	{ // begin AppendPString
		
		BlockMoveData (c + 1, s + s[0] + 1, c[0]);
		s[0] += c[0];

		return s;
		
	} // end AppendPString
	
// ---------------------------------------------------------------------------
//		´ ReplaceString
// ---------------------------------------------------------------------------

OSErr 
ReplaceString (

	Handle 				baseText, 
	ConstStr255Param	key, 
	ConstStr255Param	substitutionText)

	{ // begin ReplaceString

		OSErr	e = noErr;
		Handle 	h = nil;
		
		if (noErr != (e = PtrToHand(substitutionText + 1, &h, substitutionText[0]))) goto CleanUp;
		if (noErr > (e = ReplaceText (baseText, h, (StringPtr) key))) goto CleanUp;
		
		e = noErr;
		
	CleanUp:
	
		if (h) DisposeHandle (h);
		h = nil;
		
		return e;
		
	} // end ReplaceString

// ---------------------------------------------------------------------------
//		´ ReplaceInIndString
// ---------------------------------------------------------------------------

OSErr 
ReplaceInIndString (

	StringPtr			stringOut, 
	short 				strListID, 
	short 				strIndex,
	ConstStr255Param	param1, 
	ConstStr255Param	param2,
	ConstStr255Param	param3, 
	ConstStr255Param	param4)
	
	{ // begin ReplaceInIndString
	
		OSErr e = noErr;

		Handle h = nil;
		
		GetIndString (stringOut, strListID, strIndex);
		if (noErr != (e = PtrToHand (stringOut + 1, &h, stringOut[0]))) goto CleanUp;
		
		if (param1 == nil) param1 = "\p";
		if (noErr > (e = ReplaceString (h, "\p<1>", param1))) goto CleanUp;

		if (param2 == nil) param2 = "\p";
		if (noErr > (e = ReplaceString (h, "\p<2>", param2))) goto CleanUp;

		if (param3 == nil) param3 = "\p";
		if (noErr > (e = ReplaceString (h, "\p<3>", param3))) goto CleanUp;

		if (param4 == nil) param4 = "\p";
		if (noErr > (e = ReplaceString (h, "\p<4>", param4))) goto CleanUp;

		BlockMoveData (*h, stringOut + 1, stringOut[0] = GetHandleSize(h));
		
	CleanUp:
	
		if (h) DisposeHandle (h);
		h = nil;
		
		return e;
	
	} // end ReplaceInIndString

// ---------------------------------------------------------------------------
//		´ VCSIdleProc
// ---------------------------------------------------------------------------

pascal Boolean 
VCSIdleProc (

	EventRecord*	theEvent, 
	long *			sleepTime, 
	RgnHandle*		mouseRgn)
	
	{ // begin VCSIdleProc
		
		if (theEvent->what == nullEvent) {
			*mouseRgn = nil;
			*sleepTime = 6;
			} // if
		
		return false;
	
	} // end VCSIdleProc

// ---------------------------------------------------------------------------
//		´ VCSSendOutputCommand
// ---------------------------------------------------------------------------

OSErr 
VCSSendOutputCommand (

	const 	VCSContext&		inPB,
	const	AEDescList*		command,
	const	FSSpec*			cwd,
	Handle*					output)
	
	{ // begin VCSSendOutputCommand
	
		OSErr				e = noErr;
		
		ProcessSerialNumber	psn;
				
		*output = nil;
		
		{
			// find MacCVS
			VCSTask 	task(inPB, kTaskStrings, kFindingClientTask);
			if (noErr != (e = CVSGetProcess (&psn))) return VCSRaiseOSErr (inPB, e);
			}
			
		// send the command to SourceServer
		switch (e = CVSSendCommand (inPB, output, &psn, command, cwd)) {
			case noErr:
			case userCanceledErr:
				break;
				
			default:
				//	Report any errors			
				VCSRaiseOSErr (inPB, e);
				break;
			} // switch
		
		return e;
		
	} // end VCSSendOutputCommand

// ---------------------------------------------------------------------------
//		´ VCSSendCommand
// ---------------------------------------------------------------------------

OSErr 
VCSSendCommand (

	const 	VCSContext&		inPB,
	const	AEDescList*		command,
	const	FSSpec*			cwd)
	
	{ // begin VCSSendCommand
	
		OSErr				e = noErr;
		
		//	Stuff to clean up
		StHandle			output;
		
		if (noErr != (e = VCSSendOutputCommand (inPB, command, cwd, &output.mH))) return e;
		
		if ((nil != output) && (0 != GetHandleSize (output))) {
			long				theCount;
			long				i;
			
			// Pull the command line from the event
			if (noErr != (e = AECountItems (command, &theCount))) return e;
			for (i = 0; i < theCount; ++i) {
				AEKeyword	key = typeNull;
				DescType	typeCode;
				Size		actualSize = 0;
				Str255		cmdName;
				
				if (noErr != (e = AEGetNthPtr (command, i + 1, typeChar, &key, &typeCode, cmdName + 1, sizeof (cmdName) - 1, &actualSize))) return e;
				cmdName[0] = actualSize;
				if (cmdName[1] == '-') continue;
				
				if (noErr != (e = VCSCheckCmdOutput (inPB, cmdName, output))) return e;
				break;
				} // for
			} // if
			
		return e;
		
	} // end VCSSendCommand

