// ===========================================================================
//	VCSResult.h ©1997 Electric Fish, Inc. All rights reserved.
// ===========================================================================

#pragma once

#include "VCSContext.h"

OSErr
GetNextString (

	Handle*					outLine,
	Handle					inText,
	char					inDelim);

OSErr
GetNextLine (

	Handle*					outLine,
	Handle					inText);

OSErr 
ReadFileContents (
	
	Handle*					outData,	
	const	FSSpec*			inSpec);

Boolean
FSpEqual (

	const	FSSpec*			spec1,
	const	FSSpec*			spec2);
	
OSErr
VCSUpdateFileStatus (

	const VCSContext&		inPB,
	const	FSSpec*			root,
	Handle					line);

void 
VCSDisplayResult (

	const VCSContext&		inPB,
	short					inSeverity,
	short					inStringID,
	short					inStringIndex,
	Handle					message);
	
void 
VCSDisplayError (

	const VCSContext&		inPB,
	short					inSeverity,
	short					inStringID,
	short					inStringIndex,
	Handle					message);

OSErr
VCSCheckCmdOutputLine (

	const VCSContext&		inPB,
	ConstStr255Param		inCmd,
	Handle					line);

OSErr
VCSCheckCmdOutput (

	const VCSContext&		inPB,
	ConstStr255Param		inCmd,
	Handle					output);
