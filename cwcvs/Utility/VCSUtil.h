#pragma once

#include "VCSContext.h"

#include <AEDataModel.h>

enum ScanCodes {
	kCmdScan = 0x37,
	kShiftScan = 0x38,
	kAlphaScan = 0x39,
	kOptionScan = 0x3A,
	kControlScan = 0x3B,
	
	kFnordScan
	};

StringPtr
AppendPString (

	StringPtr			s,
	ConstStr255Param	c);

OSErr 
ReplaceString (

	Handle baseText, 
	ConstStr255Param key, 
	ConstStr255Param substitutionText);

OSErr 
ReplaceInIndString (

	StringPtr 				stringOut, 
	short 					strListID, 
	short 					strIndex,
	ConstStr255Param 		param1 = nil, 
	ConstStr255Param 		param2 = nil,
	ConstStr255Param 		param3 = nil, 
	ConstStr255Param 		param4 = nil);

Boolean
FSpEqual (

	const	FSSpec&			a,
	const	FSSpec&			b);
	
Boolean 
CheckScanCode (

	short					inScan);
	
pascal Boolean 
VCSIdleProc		(EventRecord*			theEvent, 
				 long*					sleepTime, 
				 RgnHandle*				mouseRgn);
	
OSErr 
VCSSendOutputCommand (

	VCSContext&				inPB,
	const	AEDescList*		command,
	const	FSSpec* 		cwd,
	Handle*					output);

OSErr 
VCSSendCommand	(

	VCSContext&				inPB,
	const	AEDescList*		command,
	const	FSSpec* 		cwd);
