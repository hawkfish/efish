/*
	File:		RegistrationESellerate.cp

	Contains:	Implementation of the eSellerate Registration singleton.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1999-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

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
	kRegFileNameIndex = 1,
	kPublisherIDIndex,
	kPurchaseRefNumIndex,
	kPreviewCertificateIndex,
	kErrorURLIndex,
	kPublisherKeyIndex,
	kProductPrefixIndex,
	
	strn_Registration = 1300
	};
	
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
#pragma mark RegistrationDialog

#include "EDialog.h"
#include <LPeriodical.h>

class RegistrationDialog 	: public EDialog
							, public LPeriodical
{

	protected:
	
		UInt32					mNotYetTicks;
		UInt32					mStartTicks;
		
		void					SetupGUI				(void);

	public:
		
		static	Boolean			Purchase				(void);
		
								RegistrationDialog		(LCommander*		inSuper,
														 UInt32				inNotYetTicks = 0,
														 short				inEventMask = everyEvent);
		virtual					~RegistrationDialog		(void);
		
		virtual void			SpendTime				(const EventRecord&	inMacEvent);

		virtual	Boolean			Run						(void);
	};

const	ResIDT		PPob_RegistrationDialog		= 1300;

const	PaneIDT		pane_Purchase				= 'ok  ';
const	PaneIDT		pane_NotYet					= 'nyet';
const	PaneIDT		pane_Countdown				= 'down';

const	MessageT	msg_NotYet					= -1301;

// ---------------------------------------------------------------------------
//		• RegistrationDialog
// ---------------------------------------------------------------------------

RegistrationDialog::RegistrationDialog (
	
	LCommander*		inSuper,
	UInt32			inNotYetTicks,
	short			inEventMask)
	
	: EDialog (PPob_RegistrationDialog, inSuper, inEventMask)
	
	, mNotYetTicks (inNotYetTicks)
	, mStartTicks (0)
	
	{ // begin RegistrationDialog		

		SetupGUI ();
		
	} // end RegistrationDialog
	
// ---------------------------------------------------------------------------
//		• ~RegistrationDialog
// ---------------------------------------------------------------------------

RegistrationDialog::~RegistrationDialog (void)

	{ // begin ~RegistrationDialog
		
	} // end ~RegistrationDialog
	
// ---------------------------------------------------------------------------
//		• SetupGUI
// ---------------------------------------------------------------------------

void
RegistrationDialog::SetupGUI (void) 

	{ // begin SetupGUI
		
		UReanimator::LinkListenerToBroadcasters (this, GetDialog (), PPob_RegistrationDialog);
		
		LPane*	countDown = GetDialog ()->FindPaneByID (pane_Countdown);
		LPane*	notYet = GetDialog ()->FindPaneByID (pane_NotYet);
		if (mNotYetTicks) {
			countDown->Show ();
			notYet->Show ();
			} // if
			
		else {
			countDown->Hide ();
			notYet->Hide ();
			} // else
			
	} // end SetupGUI

// ---------------------------------------------------------------------------
//		• SpendTime
// ---------------------------------------------------------------------------

void 
RegistrationDialog::SpendTime (

	const EventRecord&	/*inMacEvent*/)
	
	{ // begin SpendTime
		
		const	long	soFarTicks = ::TickCount () - mStartTicks;
		LPane*			notYet = GetDialog ()->FindPaneByID (pane_NotYet);
		LPane*			countDown = GetDialog ()->FindPaneByID (pane_Countdown);
		
		if (soFarTicks >= mNotYetTicks) {
			notYet->Enable ();
			countDown->Hide ();
			StopIdling ();
			} // if

		else {
			notYet->Disable ();
			
			const	SInt32	newValue = (mNotYetTicks - soFarTicks + 59) / 60;
			if (countDown->GetValue () != newValue) countDown->SetValue (newValue);
			
			countDown->Show ();
			} // if
		
	} // end SpendTime

// ---------------------------------------------------------------------------
//		• Run
// ---------------------------------------------------------------------------

Boolean	
RegistrationDialog::Run (void)
	
	{ // begin Run
		
		//	Show the dialog
		GetDialog ()->Show ();
		
		//	Start the countdown
		mStartTicks = ::TickCount ();
		StartIdling ();
		
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
RegistrationDialog::Purchase (void)
	
	{ // begin Purchase
		
		try {
			/*1* PURCHASE example */	
			/*
			Command the Software Delivery Wizard to perform the purchase specified.
			*/
			ESellerate		resultData (ESellerate::kPurchase,
										MPString (strn_Registration, kPublisherIDIndex),
										MPString (strn_Registration, kPurchaseRefNumIndex),
										MPString (strn_Registration, kPreviewCertificateIndex),
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
		UInt32			fileSecs = ERegistrationFile (MPString (strn_Registration, kRegFileNameIndex)).GetRegTime ();
		
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
			ERegistrationFile	regFile (MPString (strn_Registration, kRegFileNameIndex));
			
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
    					MPString (strn_Registration, kPublisherKeyIndex),
    					nil 		// no expiration reference needed, since no Duration in the Sales Manager
  						);
			if (!valid) return false;
			
			//	Check the prefix
			MPString	prefix (strn_Registration, kProductPrefixIndex);
			serial[0] = prefix.Length ();
			if (0 != prefix.Compare (MPString (serial))) return false;
			
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
		ERegistrationFile	regFile (MPString (strn_Registration, kRegFileNameIndex));
		
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
	UInt32			inNotYetTicks,
	short			inEventMask)
	
	{ // begin RunDialog		
		
		if (IsRegistered ()) return true;
		
		if (IsExpired ()) inNotYetTicks = 0;
		
		if (inNotYetTicks)
			return RegistrationDialog (inSuper, inNotYetTicks, inEventMask).Run ();
		
		else return RegistrationDialog::Purchase ();
		
	} // end RunDialog
