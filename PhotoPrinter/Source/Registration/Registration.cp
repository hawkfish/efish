/*
	File:		Registration.cp

	Contains:	Implementation of the preregistered Registration singleton.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1999-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

         <7>    11/16/01    rmgw    Add update dialog. Bug #374.
         <6>    11/14/01    rmgw    Soup up a la Color Pal.
         <5>     11/1/01	rmgw    Add Initialize.
         <4>    10/29/01    rmgw    Convert to Bart version.
         <3>     12/9/01    rmgw    Make registration more generic.  Bug #229.
         <2>     12/9/99    rmgw    Factor registration from dialog.
         <1>     12/8/99    rmgw    first checked in.
*/


#include "Registration.h"

// ---------------------------------------------------------------------------
//		¥ Initialize
// ---------------------------------------------------------------------------

Boolean
Registration::Initialize (void) 
	
	{ // begin Initialize
		
		return true;
		
	} // end Initialize
	
// ---------------------------------------------------------------------------
//		¥ IsRegistered
// ---------------------------------------------------------------------------

Boolean
Registration::IsRegistered (void)
	
	{ // begin IsRegistered
		
		return true;
		
	} // end IsRegistered

// ---------------------------------------------------------------------------
//		¥ IsExpired
// ---------------------------------------------------------------------------

Boolean
Registration::IsExpired (void) 
	
	{ // begin IsExpired
	
		return false;	
	
	} // end IsExpired
	
// ---------------------------------------------------------------------------
//		¥ TestSerialNumber
// ---------------------------------------------------------------------------

Boolean
Registration::TestSerialNumber (

	StringPtr	)
	
	{ // begin TestSerialNumber
		
		return true;
		
	} // end TestSerialNumber
	
// ---------------------------------------------------------------------------
//		¥ RegisterSerialNumber
// ---------------------------------------------------------------------------

void
Registration::RegisterSerialNumber (

	StringPtr	)
	
	{ // begin RegisterSerialNumber
		
	} // end RegisterSerialNumber
	
// ---------------------------------------------------------------------------
//		¥ GetSerialNumber
// ---------------------------------------------------------------------------

Boolean
Registration::GetSerialNumber (

	StringPtr	)
	
	{ // begin GetSerialNumber
		
		return false;
		
	} // end GetSerialNumber
	
// ---------------------------------------------------------------------------
//		¥ DoStartupDialog
// ---------------------------------------------------------------------------

Boolean
Registration::DoStartupDialog (
	
	LCommander*		,
	UInt32			,
	short			)
	
	{ // begin RunDialog		
		
		return IsRegistered ();
		
	} // end DoStartupDialog

// ---------------------------------------------------------------------------
//		¥ DoPurchaseDialog
// ---------------------------------------------------------------------------

Boolean
Registration::DoPurchaseDialog (

	LCommander*)
	
	{ // begin DoPurchaseDialog		
		
		return IsRegistered ();
		
	} // end DoPurchaseDialog

// ---------------------------------------------------------------------------
//		¥ DoUpdateDialog
// ---------------------------------------------------------------------------

Boolean
Registration::DoUpdateDialog (

	LCommander*		)
	
	{ // begin DoUpdateDialog		
		
		return false;
		
	} // end DoUpdateDialog
