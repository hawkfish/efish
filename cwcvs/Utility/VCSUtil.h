#pragma once

#include "VCSContext.h"

#include <AEDataModel.h>

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

	StringPtr stringOut, 
	short strListID, 
	short strIndex,
	ConstStr255Param param1 = nil, 
	ConstStr255Param param2 = nil,
	ConstStr255Param param3 = nil, 
	ConstStr255Param param4 = nil);

pascal Boolean 
VCSIdleProc		(EventRecord*			theEvent, 
				 long*					sleepTime, 
				 RgnHandle*				mouseRgn);
	
OSErr 
VCSSendOutputCommand (

	const 	VCSContext&		inPB,
	const	AEDescList*		command,
	const	FSSpec* 		cwd,
	Handle*					output);

OSErr 
VCSSendCommand	(

	const 	VCSContext&		inPB,
	const	AEDescList*		command,
	const	FSSpec* 		cwd);
