/*
	File:		StDisableBroadcaster.cp

	Contains:	Implementation of a broadcast disabler.

	Written by:	Richard Wesley

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		20 Jul 2001		rmgw	Created. 
		
	Description:
	
		This class is designed to temprarily disable PowerPlant broadcasters.
		
		Typical usage would be:
		
		{	
			StDisableBroadcaster		broadcaster (theControl);
			
			theControl->SetValue (newValue);
		}
		 
*/

#include "StDisableBroadcaster.h"

// ---------------------------------------------------------------------------
//	¥ StDisableBroadcaster													  [public]
// ---------------------------------------------------------------------------

StDisableBroadcaster::StDisableBroadcaster (

	LBroadcaster*			inTarget)
	
	: mTarget (inTarget)
	, mWasBroadcasting (mTarget->IsBroadcasting ())

	{ // begin StDisableBroadcaster
		
		mTarget->StopBroadcasting ();
		
	} // end StDisableBroadcaster

// ---------------------------------------------------------------------------
//	¥ ~StDisableBroadcaster													  [public]
// ---------------------------------------------------------------------------

StDisableBroadcaster::~StDisableBroadcaster (void)
	
	{ // begin ~StDisableBroadcaster
		
		if (mWasBroadcasting) mTarget->StartBroadcasting ();
		
	} // end ~StDisableBroadcaster

