/*
	File:		VCSContext.cp

	Contains:	Implementation of the CWVCSContext wrapper.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1998 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

         <4>     3/31/99    rmgw    Add AE idling.
         <3>    11/10/98    rmgw    Convert to abstract base class.
         <2>     11/9/98    rmgw    Add CheckOptionalResult.
         <1>     10/8/98    rmgw    Convert to v7 API.
*/


#include "VCSContext.h"

#include <SetUpA4.h>

VCSContext*
VCSContext::sAECallback = 0;

// ---------------------------------------------------------------------------
//		¥ AEIdleProc
// ---------------------------------------------------------------------------

pascal Boolean 
VCSContext::AEIdleProc (

	EventRecord*	theEvent, 
	long *			sleepTime, 
	RgnHandle*		mouseRgn)
	
	{ // begin AEIdleProc
		
		CWResult	result = cwNoErr;
		
		EnterCallback();
		
		if (sAECallback) result = sAECallback->OnAEIdle (*theEvent, *sleepTime, *mouseRgn);
		
		ExitCallback();
		
		return result != cwNoErr;
	
	} // end AEIdleProc

// ---------------------------------------------------------------------------
//		¥ VCSContext
// ---------------------------------------------------------------------------

VCSContext::VCSContext (void)
	
	{ // begin VCSContext
		
	} // end VCSContext

// ---------------------------------------------------------------------------
//		¥ ~VCSContext
// ---------------------------------------------------------------------------

VCSContext::~VCSContext (void)
	
	{ // begin ~VCSContext
		
		if (sAECallback == this) sAECallback = 0;
		
	} // end VCSContext
	
// ---------------------------------------------------------------------------
//		¥ BeginAEIdle
// ---------------------------------------------------------------------------

AEIdleUPP 
VCSContext::BeginAEIdle (void)
	
	{ // begin BeginAEIdle
		
		PrepareCallback();
		
		if (sAECallback) return 0;
		
		sAECallback = this;
		
		return NewAEIdleUPP (AEIdleProc);
	
	} // end BeginAEIdle

// ---------------------------------------------------------------------------
//		¥ EndAEIdle
// ---------------------------------------------------------------------------

void
VCSContext::EndAEIdle (

	AEIdleUPP&	inIdleProc)
	
	{ // begin EndAEIdle
		
		if (sAECallback != this) return;
		
		sAECallback = 0;
		
		if (inIdleProc) DisposeAEIdleUPP (inIdleProc);
		inIdleProc = 0;
	
	} // end EndAEIdle



