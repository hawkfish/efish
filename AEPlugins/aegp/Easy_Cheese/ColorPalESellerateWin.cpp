/*
	File:		ColorPalESellerateWin.cpp

	Contains:	Implementation of the eSellerate Registration singleton.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1999-2001 by Electric Fish, Inc.  All Rights Reserved.

*/

#include "Registration.h"

#include "ESellerateEngine.h"
#include "RegistrationStorage.h"

#include <string.h>

static const char
sPublisherID [] = "PUB483348526";

static const char
sPurchaseRefNum [] = "ES760709705";

static const char
sUpdateRefNum [] = "";

static const char
sPreviewCertificate [] = "PC760709705-9591";

static const char
sPublisherKey [] = "65901";

static const char
sProductPrefix [] = "pEFCPWIN";

static const char
sErrorURL [] = "http://www.electricfish.com/products/colorpal/PurchaseErr.html";

//	=== Constants ===

#define UNREGISTER 0

// ---------------------------------------------------------------------------
//		¥ Initialize
// ---------------------------------------------------------------------------

bool
Registration::Initialize (

	const 	char*		inResPath) 
	
	{ // begin Initialize
		
#if UNREGISTER
		RegistrationStorage		reg (sSection, sCurrentVersion);
		
		reg.DeleteTrialVersion ();
		reg.DeleteRegTime ();
		reg.DeleteRegString ();
		reg.DeleteHost ();
#endif		
		//	Install the engine
		bool	result = (FAILURE != ::InstallEngine ());
		
		return (result);
		
	} // end Initialize
	
// ---------------------------------------------------------------------------
//		¥ TestSerialNumber
// ---------------------------------------------------------------------------

bool
Registration::TestSerialNumber (

	const	char*	inSerial)
	
	{ // begin TestSerialNumber
		
		try {
			/*4* VALIDATE SERIAL NUMBER example */
  			ReturnValue valid = ::ValidateSerialNumber (
    					inSerial, 	// serial number string
    					nil, 		// no Name-based Key string needed, since None set in the Sales Manager
    					 			// Publisher Key string, optional but used for tighter validation
    					sPublisherKey,
    					nil 		// no expiration reference needed, since no Duration in the Sales Manager
  						);
			if (!valid) return false;
			
			//	Check the prefix
			if (strncmp (inSerial, sProductPrefix, strlen (sProductPrefix))) return false;
			
			//	Made it, so we are registered!
			return true;
			} // try
		
		catch (...) {
			} // catch
			
		return false;
		
	} // end TestSerialNumber
	
// ---------------------------------------------------------------------------
//		¥ DoPurchaseDialog
// ---------------------------------------------------------------------------

bool	
Registration::DoPurchaseDialog (void)
	
	{ // begin DoPurchaseDialog
		
		try {
			/*1* PURCHASE example */	
			/*
			Command the Software Delivery Wizard to perform the purchase specified.
			*/
			ESellerateEngine	resultData (ESellerateEngine::kPurchase,
											sPublisherID,
											sPurchaseRefNum,
											sPreviewCertificate,
											sErrorURL);
			
			SerialNumber		serialNumber;
			if (0 == resultData.IndexSerialNumber (serialNumber)) return false;
			
			Registration::RegisterSerialNumber (serialNumber);
			
			return true;
			} // try
			 
		catch (...) {
			return false;
			} // catch
			
	} // end DoPurchaseDialog

