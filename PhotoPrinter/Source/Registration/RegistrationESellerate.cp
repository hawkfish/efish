/*
	File:		RegistrationESellerate.cp

	Contains:	Implementation of the eSellerate Registration singleton.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1999-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

         <5>    11/7/01		rmgw    Remove timeout.
         <4>    11/2/01		rmgw    Embed strings.
         <3>    11/1/01		rmgw    Add update string.
         <2>    11/1/01		rmgw    Wrap eSellerate data handle.
         <1>    11/1/01		rmgw    eSellerate changes.
*/


#include "Registration.h"

#include "esellerate.h"
#include "validate.h"

#include "SerialNumber.h"

#include "MPString.h"

//	=== Constants ===

enum RegStrings {
	kRegStringsIllegalIndex = 0,
	kErrorURLIndex,
	
	strn_Registration = 1300
	};

static const unsigned char
sRegFileName [] = "\pFinder EFK DB";

static const unsigned char
sPublisherID [] = "\pPUB483348526";

static const unsigned char
sPurchaseRefNum [] = "\pES480611750";

static const unsigned char
sUpdateRefNum [] = "\pES534434108";

static const unsigned char
sPreviewCertificate [] = "\pPC480611750-5916";

static const unsigned char
sPublisherKey [] = "\p65901";

static const unsigned char
sProductPrefix [] = "\pEFKILTM";

const	unsigned	char	
kXorMask = 'F';

#pragma mark ESellerate

class ESellerate
{

	ResultData   	resultData;

public:
	
	enum {
		kPurchase = true,
		kUpdate = false
		};
		
					ESellerate			(bool					inPurchase,
										 InputString			inPublisherID,
										 InputString			inPurchaseRefNum,
										 InputString			inPreviewCertificate = nil,
										 InputString			inErrorURL = nil,
										 InputString			inExtraData = nil);
	virtual			~ESellerate			(void);

	ArraySize		IndexSerialNumber 	(ArraySize    			arraySize,
										 SerialNumber 			serialNumberArray[],
										 ProductID    			productIDArray[] = nil,
										 NameBasedKey 			nameBasedKeyArray[] = nil);
	ArraySize		IndexSerialNumber 	(SerialNumber& 			serialNumberArray);
	
	ArraySize		IndexFileLocation 	(ArraySize    			arraySize,
										 FileLocation 			fileLocationArray[]);
	ArraySize		IndexFileLocation 	(FileLocation& 			fileLocation);
};


// ---------------------------------------------------------------------------
//		• ESellerate
// ---------------------------------------------------------------------------

ESellerate::ESellerate (

	bool				inPurchase,
	InputString			inPublisherID,
	InputString			inPurchaseRefNum,
	InputString			inPreviewCertificate,
	InputString			inErrorURL,
	InputString			inExtraData)
	
	: resultData (nil)
	
	{ // begin ESellerate		
		
		long 	rc;
		if (inPurchase)
			rc = ::Purchase (inPublisherID, inPurchaseRefNum, inPreviewCertificate, inErrorURL, inExtraData, &resultData);
		else rc = ::CheckForUpdate (inPublisherID, inPurchaseRefNum, inPreviewCertificate, inErrorURL, inExtraData, &resultData);
		
		ThrowIfOSErr_(rc);
	
	} // end ESellerate
	
// ---------------------------------------------------------------------------
//		• ~ESellerate
// ---------------------------------------------------------------------------

ESellerate::~ESellerate (void)

	{ // begin ~ESellerate
		
		::DeleteResultData (resultData);
		resultData = nil;
		
	} // end ~ESellerate
	
// ---------------------------------------------------------------------------
//		• IndexSerialNumber
// ---------------------------------------------------------------------------

ArraySize
ESellerate::IndexSerialNumber (

	ArraySize    arraySize,
	SerialNumber serialNumberArray[],
	ProductID    productIDArray[],
	NameBasedKey nameBasedKeyArray[])
	
	{ // begin IndexSerialNumber

		return IndexSerialNumberData (resultData, productIDArray, serialNumberArray, nameBasedKeyArray, arraySize);

	} // end IndexSerialNumber

// ---------------------------------------------------------------------------
//		• IndexSerialNumber
// ---------------------------------------------------------------------------

ArraySize
ESellerate::IndexSerialNumber (

	SerialNumber& serialNumber)
	
	{ // begin IndexSerialNumber

		return IndexSerialNumber (1, &serialNumber);

	} // end IndexSerialNumber

// ---------------------------------------------------------------------------
//		• IndexFileLocation
// ---------------------------------------------------------------------------

ArraySize
ESellerate::IndexFileLocation (

	ArraySize    arraySize,
	FileLocation fileLocationArray[])
	
	{ // begin IndexFileLocation

		return IndexFileLocationData (resultData, fileLocationArray, arraySize);

	} // end IndexFileLocation

// ---------------------------------------------------------------------------
//		• IndexFileLocation
// ---------------------------------------------------------------------------

ArraySize
ESellerate::IndexFileLocation (

	FileLocation& fileLocation)
	
	{ // begin IndexFileLocation

		return IndexFileLocation (1, &fileLocation);

	} // end IndexFileLocation

#pragma mark -
#pragma mark PurchaseDialog

#include "EDialog.h"

class PurchaseDialog : public EDialog

{

	public:
		
		static	Boolean			Purchase				(void);
		
								PurchaseDialog			(LCommander*		inSuper,
														 Boolean			inNotYet,
														 short				inEventMask = everyEvent);
		virtual					~PurchaseDialog			(void);
		
		virtual	Boolean			Run						(void);
	};

const	ResIDT		PPob_PurchaseDialog		= 1300;

const	PaneIDT		pane_Purchase				= 'ok  ';
const	PaneIDT		pane_NotYet					= 'nyet';

const	MessageT	msg_NotYet					= -1301;

// ---------------------------------------------------------------------------
//		• PurchaseDialog
// ---------------------------------------------------------------------------

PurchaseDialog::PurchaseDialog (
	
	LCommander*		inSuper,
	Boolean			inNotYet,
	short			inEventMask)
	
	: EDialog (PPob_PurchaseDialog, inSuper, inEventMask)
	
	{ // begin PurchaseDialog		

		UReanimator::LinkListenerToBroadcasters (this, GetDialog (), PPob_PurchaseDialog);
		
		LPane*	notYet = GetDialog ()->FindPaneByID (pane_NotYet);
		if (inNotYet) 
			notYet->Show ();
		else notYet->Hide ();
					
	} // end PurchaseDialog
	
// ---------------------------------------------------------------------------
//		• ~PurchaseDialog
// ---------------------------------------------------------------------------

PurchaseDialog::~PurchaseDialog (void)

	{ // begin ~PurchaseDialog
		
	} // end ~PurchaseDialog
	
// ---------------------------------------------------------------------------
//		• Run
// ---------------------------------------------------------------------------

Boolean	
PurchaseDialog::Run (void)
	
	{ // begin Run
		
		//	Show the dialog
		GetDialog ()->Show ();
		
		for (;;) {
			switch (DoDialog ()) {
				case msg_Cancel:
				case cmd_Close:
					return false;
					
				case msg_OK:
					GetDialog ()->Hide ();
					if (!Purchase ()) {
						GetDialog ()->Show ();
						GetDialog ()->Select ();
						continue;
						} // if
					//	Fall through…
					
				case msg_NotYet:
					return true;
				} // switch
			} // for
			
	} // end Run

// ---------------------------------------------------------------------------
//		• Purchase
// ---------------------------------------------------------------------------

Boolean	
PurchaseDialog::Purchase (void)
	
	{ // begin Purchase
		
		try {
			/*1* PURCHASE example */	
			/*
			Command the Software Delivery Wizard to perform the purchase specified.
			*/
			ESellerate		resultData (ESellerate::kPurchase,
										sPublisherID,
										sPurchaseRefNum,
										sPreviewCertificate,
										MPString (strn_Registration, kErrorURLIndex));
			
			SerialNumber	serialNumber;
			if (0 == resultData.IndexSerialNumber (serialNumber)) return false;
			
			MPString		serialCopy (serialNumber);
			Registration::RegisterSerialNumber (serialCopy);
			
			return true;
			} // try
			 
		catch (...) {
			return false;
			} // catch
			
	} // end Purchase

#pragma mark -

#include "ERegistrationFile.h"

// ---------------------------------------------------------------------------
//		• Initialize
// ---------------------------------------------------------------------------

Boolean
Registration::Initialize (void) 
	
	{ // begin Initialize
		
		return (FAILURE != ::InstallEngine ());
		
	} // end Initialize
	
// ---------------------------------------------------------------------------
//		• IsExpired
// ---------------------------------------------------------------------------

Boolean
Registration::IsExpired (void) 
	
	{ // begin IsExpired
	
		//	Find the registration file
		UInt32			fileSecs = ERegistrationFile (sRegFileName).GetRegTime ();
		
		//	Get the current time
		UInt32			nowSecs;
		::GetDateTime (&nowSecs);
		
		//	Add 30 days
		UInt32			expireSecs = fileSecs + 30L * 24L * 60L * 60L;
		
		return (expireSecs <= nowSecs);	
	
	} // end IsExpired
	
// ---------------------------------------------------------------------------
//		• IsRegistered
// ---------------------------------------------------------------------------

Boolean
Registration::IsRegistered (void)
	
	{ // begin IsRegistered
		
		try {
			StDisableDebugThrow_();
			
			//	Get the reg file
			ERegistrationFile	regFile (sRegFileName);
			
			//	Get serial number
			Str255			serial;
			regFile.GetRegString (serial);
			::XorSerial (serial, kXorMask);

			//	Validate it
			/*4* VALIDATE SERIAL NUMBER example */
  			ReturnValue valid = ValidateSerialNumber (
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
		
	} // end IsRegistered
	
// ---------------------------------------------------------------------------
//		• RegisterSerialNumber
// ---------------------------------------------------------------------------

void
Registration::RegisterSerialNumber (

	StringPtr	inSerial)
	
	{ // begin RegisterSerialNumber
		
		//	Get the reg file
		ERegistrationFile	regFile (sRegFileName);
		
		//	Add the new SN
		::XorSerial (inSerial, kXorMask);
		regFile.SetRegString (inSerial);
		::XorSerial (inSerial, kXorMask);
		
	} // end RegisterSerialNumber
	
// ---------------------------------------------------------------------------
//		• RunDialog
// ---------------------------------------------------------------------------

Boolean
Registration::RunDialog (
	
	LCommander*		inSuper,
	UInt32			/*inNotYetTicks*/,
	short			inEventMask)
	
	{ // begin RunDialog		
		
		if (IsRegistered ()) return true;
		
		return PurchaseDialog (inSuper, !IsExpired (), inEventMask).Run ();
		
	} // end RunDialog
