/*
	File:		RegistrationESellerate.cp

	Contains:	Implementation of the eSellerate Registration singleton.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1999-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

        <11>    11/16/01    rmgw    Add trial prompt variations.  Bug #373.
        <10>    11/15/01    rmgw    Add unregistered SN string.
         <9>    11/15/01    rmgw    Use new UseTime property.
         <8>    11/14/01    rmgw    Soup up a la Color Pal.
         <7>    11/9/01		rmgw    Make Preview nil.
         <6>    11/9/01		rmgw    Live ammo.
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

#define PREVIEW 0
#ifndef PREVIEW
	#define PREVIEW PP_DEBUG
#endif

enum RegStrings {
	kRegStringsIllegalIndex = 0,
	kErrorURLIndex,
	kUnregisteredSNIndex,
	
	kStartupTrialMessageIndex,
	kStartupExpiredMessageIndex,
	
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

#if PREVIEW
static const unsigned char
sPreviewCertificate [] = "\pPC480611750-5916";
#else
static const unsigned char*
sPreviewCertificate = nil;
#endif

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
#pragma mark StartupDialog

#include "EDialog.h"

class StartupDialog : public EDialog

{

	public:
		
								StartupDialog			(LCommander*		inSuper,
														 Boolean			inCanTry,
														 short				inEventMask = everyEvent);
		virtual					~StartupDialog			(void);
		
		virtual	Boolean			Run						(void);
	};

const	ResIDT		PPob_PurchaseDialog		= 1300;

const	PaneIDT		pane_Purchase				= 'ok  ';
const	PaneIDT		pane_Try					= 'nyet';
const	PaneIDT		pane_Prompt					= 'pmpt';

const	MessageT	msg_Try					= -1301;

// ---------------------------------------------------------------------------
//		• StartupDialog
// ---------------------------------------------------------------------------

StartupDialog::StartupDialog (
	
	LCommander*		inSuper,
	Boolean			inCanTry,
	short			inEventMask)
	
	: EDialog (PPob_PurchaseDialog, inSuper, inEventMask)
	
	{ // begin StartupDialog		

		UReanimator::LinkListenerToBroadcasters (this, GetDialog (), PPob_PurchaseDialog);
		
		LPane*	tryButton = GetDialog ()->FindPaneByID (pane_Try);
		if (inCanTry) 
			tryButton->Show ();
		else tryButton->Hide ();
		
		FindPaneByID (pane_Prompt)->SetDescriptor (MPString (strn_Registration, inCanTry ? kStartupTrialMessageIndex : kStartupExpiredMessageIndex));
		
	} // end StartupDialog
	
// ---------------------------------------------------------------------------
//		• ~StartupDialog
// ---------------------------------------------------------------------------

StartupDialog::~StartupDialog (void)

	{ // begin ~StartupDialog
		
	} // end ~StartupDialog
	
// ---------------------------------------------------------------------------
//		• Run
// ---------------------------------------------------------------------------

Boolean	
StartupDialog::Run (void)
	
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
					if (!Registration::DoPurchaseDialog (this)) {
						GetDialog ()->Show ();
						GetDialog ()->Select ();
						continue;
						} // if
					//	Fall through…
					
				case msg_Try:
					return true;
				} // switch
			} // for
			
	} // end Run

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
		ERegistrationFile	reg (sRegFileName);
		
		//	Get the times
		UInt32			regSecs = reg.GetRegTime ();
		UInt32			useSecs = reg.GetUseTime ();
		
		//	Add 30 days
		UInt32			expireSecs = regSecs + 30L * 24L * 60L * 60L;
		
		return (expireSecs <= useSecs);	
	
	} // end IsExpired
	
// ---------------------------------------------------------------------------
//		• IsRegistered
// ---------------------------------------------------------------------------

Boolean
Registration::IsRegistered (void)
	
	{ // begin IsRegistered
		
		//	Get serial number
		Str255			serial;
		GetSerialNumber (serial);

		//	Validate it
		return TestSerialNumber (serial);
		
	} // end IsRegistered
	
// ---------------------------------------------------------------------------
//		• TestSerialNumber
// ---------------------------------------------------------------------------

Boolean
Registration::TestSerialNumber (

	StringPtr	inSerial)
	
	{ // begin TestSerialNumber
		
		//	Get serial number
		Str255			serial;
		::BlockMoveData (inSerial + 1, serial + 1, serial[0] = inSerial[0]);

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
		
	} // end TestSerialNumber
	
// ---------------------------------------------------------------------------
//		• GetSerialNumber
// ---------------------------------------------------------------------------

Boolean
Registration::GetSerialNumber (

	StringPtr	outSerial)
	
	{ // begin GetSerialNumber
		
		try {
			//	Get the reg file
			ERegistrationFile	regFile (sRegFileName);
		
			//	Add the new SN
			regFile.GetRegString (outSerial);
			::XorSerial (outSerial, kXorMask);
			
			return true;
			} // try
			
		catch (...) {
			::GetIndString (outSerial, strn_Registration, kUnregisteredSNIndex);
			return true;
			} // catch
			
		return false;
		
	} // end GetSerialNumber
	
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
//		• DoStartupDialog
// ---------------------------------------------------------------------------

Boolean
Registration::DoStartupDialog (
	
	LCommander*		inSuper,
	UInt32			/*inNotYetTicks*/,
	short			inEventMask)
	
	{ // begin DoStartupDialog		
		
		if (IsRegistered ()) return true;
		
		return StartupDialog (inSuper, !IsExpired (), inEventMask).Run ();
		
	} // end DoStartupDialog

// ---------------------------------------------------------------------------
//		• DoPurchaseDialog
// ---------------------------------------------------------------------------

Boolean
Registration::DoPurchaseDialog (

	LCommander*)
	
	{ // begin DoPurchaseDialog		
		
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
			
	} // end DoPurchaseDialog
