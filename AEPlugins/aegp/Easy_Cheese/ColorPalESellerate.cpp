/*
	File:		ColorPalESellerate.cpp

	Contains:	Implementation of the eSellerate Registration singleton.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1999-2001 by Electric Fish, Inc.  All Rights Reserved.

*/

#include "Registration.h"

#include "PurchaseDialog.h"
#include "RegistrationStorage.h"

// ---------------------------------------------------------------------------
//		¥ IsExpired
// ---------------------------------------------------------------------------

bool
Registration::IsExpired (void) 
	
	{ // begin IsExpired
	
		RegistrationStorage		reg (sSection, sCurrentVersion);
		
		//	Sanity check
		if (!reg.IsCorrectHost ()) {
			reg.DeleteTrialVersion ();
			reg.DeleteRegTime ();
			} // if
			
		//	Get the version
		ASInt32				stampVersion (reg.GetTrialVersion ());
		
		//	Update the time stamp
		time_t				nowSecs = time (0);
		if (stampVersion < sCurrentVersion)
			reg.SetRegTime (nowSecs);
		
		//	Get the time stamp
		time_t				stampSecs = reg.GetRegTime ();
		
		//	Add 7 days
		tm					expire = *gmtime (&stampSecs);
		expire.tm_mday += 7;
		time_t				expireSecs = mktime (&expire);
			
		//	Compare
		return difftime (nowSecs, expireSecs) > 0;	
	
	} // end IsExpired
	
// ---------------------------------------------------------------------------
//		¥ IsRegistered
// ---------------------------------------------------------------------------

bool
Registration::IsRegistered (void)
	
	{ // begin IsRegistered
		
		RegistrationStorage		reg (sSection, sCurrentVersion);
	
		//	Sanity check
		if (!reg.IsCorrectHost ()) return false;
		
		//	Get the registration key
		char					serial [256];
		if (!reg.GetRegString (sizeof (serial), serial)) return false;
	
		//	Test it
		return TestSerialNumber (serial);
		
	} // end IsRegistered
	
// ---------------------------------------------------------------------------
//		¥ RegisterSerialNumber
// ---------------------------------------------------------------------------

void
Registration::RegisterSerialNumber (

	const	char*	inSerial)
	
	{ // begin RegisterSerialNumber
		
		RegistrationStorage		reg (sSection, sCurrentVersion);
		
		reg.SetRegString (inSerial);
		
	} // end RegisterSerialNumber
	
// ---------------------------------------------------------------------------
//		¥ DoRegistrationDialog
// ---------------------------------------------------------------------------

bool
Registration::DoRegistrationDialog (
	
	ASInt32			inNotYetSecs)
	
	{ // begin DoRegistrationDialog		
		
		if (IsRegistered ()) return true;
		
		if (IsExpired ()) inNotYetSecs = 0;
		
		if (inNotYetSecs)
			return PurchaseDialog (inNotYetSecs).Run ();
		
		else return Registration::DoPurchaseDialog ();
		
	} // end DoRegistrationDialog
	
