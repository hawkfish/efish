/*
	File:		ColorPalESellerateMac.cpp

	Contains:	Implementation of the eSellerate Registration singleton.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1999-2001 by Electric Fish, Inc.  All Rights Reserved.

*/

#include "Registration.h"

#pragma mark StHideFloaters

#include <MacWindows.h>

class StHideFloaters

{
	
	bool				mWereVisible;
	
public:

						StHideFloaters	(void);
	virtual				~StHideFloaters	(void);
};


// ---------------------------------------------------------------------------
//		¥ StHideFloaters
// ---------------------------------------------------------------------------

StHideFloaters::StHideFloaters (void) 
	
	: mWereVisible (::AreFloatingWindowsVisible ())
	
	{ // begin StHideFloaters
		
		if (mWereVisible) ::HideFloatingWindows ();
		
	} // end StHideFloaters
	
// ---------------------------------------------------------------------------
//		¥ ~StHideFloaters
// ---------------------------------------------------------------------------

StHideFloaters::~StHideFloaters (void) 
	
	{ // begin ~StHideFloaters
	
		if (mWereVisible) ::ShowFloatingWindows ();

	} // end ~StHideFloaters

#pragma mark -

#include "ESellerateEngine.h"
#include "RegistrationStorage.h"

#include <string.h>

//	=== Constants ===

#define UNREGISTER 0
#define PREVIEW 0

static const unsigned char
sPublisherID [] = "\pPUB483348526";

static const unsigned char
sPurchaseRefNum [] = "\pES706980238";

static const unsigned char
sUpdateRefNum [] = "\p";

static const unsigned char
sPreviewCertificate [] =
#if PREVIEW 
"\pPC706980238-0518";
#else
"\p";
#endif

static const unsigned char
sPublisherKey [] = "\p65901";

static const unsigned char
sProductPrefix [] = "\pEFCPMAC";

static const unsigned char
sErrorURL [] = "\phttp://www.electricfish.com/products/colorpal/PurchaseErr.html";

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
		//	Convert the path to Pascal
		Str255	resPath;
		::BlockMoveData (inResPath, resPath + 1, resPath[0] = strlen (inResPath));
		
		//	Convert the path to an FSSpec
		FSSpec	resSpec;
		if (noErr != ::FSMakeFSSpec (0, 0, resPath, &resSpec)) return false;
		
		//	Open it
		UInt16	saveResFile = ::CurResFile ();
		UInt16	resRefNum = FSpOpenResFile (&resSpec, fsRdPerm);
		if (noErr != ::ResError ()) return false;
		
		//	Install the engine
		bool	result = (FAILURE != ::InstallEngine ());
		
		//	Clean up
		::CloseResFile (resRefNum);
		::UseResFile (saveResFile);
		
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
			//	Validate it
			Str255	serial;
			::BlockMoveData (inSerial, &serial[1], serial[0] = strlen (inSerial));
			
			/*4* VALIDATE SERIAL NUMBER example */
  			ReturnValue valid = ::ValidateSerialNumber (
    					serial, 	// serial number string
    					nil, 		// no Name-based Key string needed, since None set in the Sales Manager
    					 			// Publisher Key string, optional but used for tighter validation
    					sPublisherKey,
    					nil 		// no expiration reference needed, since no Duration in the Sales Manager
  						);
			if (!valid) return false;
			
			//	Check the prefix
			serial[0] = StrLength (sProductPrefix);
			if (!::EqualString (sProductPrefix, serial, true, true)) return false;
			
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
			StHideFloaters		hideFloaters;
			
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
			
			char				serialCopy[256];
			::BlockMoveData (serialNumber + 1, serialCopy, StrLength (serialNumber));
			serialCopy[StrLength (serialNumber)] = 0;
			Registration::RegisterSerialNumber (serialCopy);
			
			return true;
			} // try
			 
		catch (...) {
			return false;
			} // catch
			
	} // end DoPurchaseDialog

