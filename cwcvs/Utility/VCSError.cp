/*
	File:		VCSError.c

	Contains:	<contents>

	Written by:	Richard Wesley

	Copyright:	Copyright ©1997-8 Electric Fish Inc.  All Rights Reserved.

	Change History (most recent first):

         <5>     10/8/98    rmgw    Convert to v7 API.
         <4>     9/22/98    rmgw    Convert to CWP3.1.
         <3>      2/6/98    rmgw    Convert VCSTask to stack object.
         <2>    11/13/97    rmgw    Implement our task stack.
         <1>    10/29/97    rmgw    first checked in
*/


// ===========================================================================
//	VCSError.c ©1997 Electric Fish, Inc. All rights reserved.
// ===========================================================================

#include "VCSError.h"

#include "VCSTask.h"
#include "VCSUtil.h"
 
#include <TextUtils.h>

// ---------------------------------------------------------------------------
//		€ VCSResTextError
// ---------------------------------------------------------------------------

void 
VCSResTextError (
	
	VCSContext&				inPB,
	short					strListID, 
	short					strIndex, 
	OSErr					callerErr,
	StringPtr				param1, 
	StringPtr				param2,
	StringPtr				param3, 
	StringPtr				param4)

	{ // begin VCSResTextError
	
		Str255 message;
		Str255 errorString;
		
		if (callerErr != noErr)
			NumToString (callerErr, errorString);
		else errorString[0] = 0;
		 
		ReplaceInIndString (message, strListID, strIndex, param1, param2, param3, param4);	
		
		inPB.MessageOutput (messagetypeError, p2cstr (message), p2cstr (errorString), callerErr);
		
	} // end VCSResTextError

// ---------------------------------------------------------------------------
//		€ VCSRaiseOSErr
// ---------------------------------------------------------------------------

OSErr
VCSRaiseOSErr (

	VCSContext&				inPB,
	OSErr					e)
	
	{ // begin VCSRaiseOSErr
		
		Str255	taskName;
		if (noErr == e) return e;
		
		VCSResTextError (inPB,
						 kErrorStrings, 
						 kLongTaskErr, 
						 e,
						 VCSTask::GetTaskName (taskName), 
						 nil, 
						 nil, 
						 nil);
		
		return e;
		
	} // end VCSRaiseOSErr
	
// ---------------------------------------------------------------------------
//		€ VCSRaiseString
// ---------------------------------------------------------------------------

void
VCSRaiseString (

	VCSContext&				inPB,
	short					strListID, 
	short					strIndex, 
	StringPtr				param1, 
	StringPtr				param2,
	StringPtr				param3, 
	StringPtr				param4)
	
	{ // begin VCSRaiseString
		
		Str255	taskName;
		Str255	pTaskErrorString;
		ReplaceInIndString (pTaskErrorString, strListID, strIndex,
					   param1, param2, param3, param4);
		
		VCSResTextError (inPB,
						 kErrorStrings, 
						 kLongTaskErr, 
						 noErr,
						 VCSTask::GetTaskName (taskName), 
						 pTaskErrorString, 
						 nil, 
						 nil);
		
	} // end VCSRaiseString
