// ===========================================================================
//	VCSPrompt.h ©1997 Electric Fish, Inc. All rights reserved.
// ===========================================================================

#pragma once

#include "VCSContext.h"

enum PromptStrings {
	
	kDeletePrompt = 1,
	kCheckoutCommentFailedPrompt,
	kDiscardChangesPrompt,
	kCancelCheckedInPrompt,
	kCheckinCommentPrompt,
	kCheckoutCommentPrompt,
	kAddHeaderCommentPrompt,
	
	kPromptStringsID = 16002,
	
	kFnordPrompt
	};
	
typedef enum {
	kPromptYes, 
	kPromptNo, 
	kPromptCancel 
	} PromptResponse;

void
ClearPromptResponses (void);

PromptResponse 
VCSPromptYesNoCancel (

	const VCSContext&		inPB,
	short 					strnID, 
	short 					strIndex,
	ConstStr255Param 		param1, 
	ConstStr255Param 		param2,
	ConstStr255Param 		param3, 
	ConstStr255Param 		param4);

PromptResponse 
VCSPromptYesNo (

	const VCSContext&		inPB,
	short 					strnID, 
	short 					strIndex,
	ConstStr255Param 		param1, 
	ConstStr255Param 		param2,
	ConstStr255Param 		param3, 
	ConstStr255Param 		param4);

short 
VCSPromptString (
	
	const VCSContext&		inPB,
	short					inDLOGid,
	StringPtr 				outString,
	Size					outSize);

Boolean 
VCSPromptComment (

	const VCSContext&		inPB,
	short					inPromptIndex, 
	ConstStr255Param		fileName, 
	Str255 					comment);
