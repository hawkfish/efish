/*
	File:		RegistrationSerial.cp

	Contains:	Implementation of the Serial Number Registration singleton.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1999-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

         <9>    11/16/01    rmgw    No Not Yet in Purchase variant.
         <8>    11/16/01    rmgw    Add Quit/Cancel button variations.  Bug #373.
         <7>    11/15/01    rmgw    Add unregistered SN; remove countdown.
         <6>    11/14/01    rmgw    Soup up a la Color Pal.
         <5>    11/2/01		rmgw    Embed strings.
         <4>    11/1/01   	rmgw    Factor ERegistrationFile; add Initialize.
         <3>    10/30/01    rmgw    Use bottleneck in RegisterSerialNumber.
         <2>    10/30/01    rmgw    Hide license file and add vers resource.
         <1>    10/29/01    rmgw    Created from old Registration.cp.
*/


#include "Registration.h"

#include "MPString.h"

//	=== Constants ===

enum RegStrings {
	kRegStringsIllegalIndex = 0,
	
	kUnregisteredSNIndex,
	kQuitIndex,
	kCancelIndex,
	
	strn_Registration = 1300
	};

static const unsigned char
sRegFileName [] = "\pFinder EFK DB";

static const unsigned char
sPublisherKey [] = "\p65901";

static const unsigned char
sProductPrefix [] = "\pEFKILTM";

const	unsigned	char	
kXorMask = 'F';

#pragma mark RegistrationDialog

//	=== Constants ===

const	ResIDT		PPob_RegistrationDialog		= 1300;

const	PaneIDT		pane_OK						= 'ok  ';
const	PaneIDT		pane_Cancel					= 'clos';
const	PaneIDT		pane_NotYet					= 'nyet';
const	PaneIDT		pane_SerialNumber			= 'reg#';
const	PaneIDT		pane_WebRegister			= 'rWeb';

const	MessageT	msg_NotYet					= -1301;
const	MessageT	msg_SerialNumber			= -1302;
const	MessageT	msg_WebRegister				= -1303;

#include "EURLDialogHandler.h"

class RegistrationDialog 	: public EURLDialogHandler 

	{

	protected:
	
		virtual	void			OnSerialChanged			(void);
		virtual	Boolean			OnOK					(void);


	public:
		
								RegistrationDialog		(LCommander*		inSuper,
														 Boolean			inNotYet = false,
														 short				inEventMask = everyEvent,
														 Boolean			inQuit = false);
		virtual					~RegistrationDialog		(void);
		
		void 					GetSerialNumber 		(Str255				outSerial);
		
		virtual	Boolean			Run						(void);
	};

#pragma mark -

// ---------------------------------------------------------------------------
//		• RegistrationDialog
// ---------------------------------------------------------------------------

RegistrationDialog::RegistrationDialog (
	
	LCommander*		inSuper,
	Boolean			inNotYet,
	short			inEventMask,
	Boolean			inQuit)
	
	: EURLDialogHandler (PPob_RegistrationDialog, inSuper, inEventMask)
	
	{ // begin RegistrationDialog		

		UReanimator::LinkListenerToBroadcasters (this, GetDialog (), PPob_RegistrationDialog);
		
		LPane*	notYet = FindPaneByID (pane_NotYet);
		if (inNotYet) 
			notYet->Show ();
		else notYet->Hide ();
		
		FindPaneByID (pane_Cancel)->SetDescriptor (MPString (strn_Registration, inQuit ? kQuitIndex : kCancelIndex));
		
		SetupURL (pane_WebRegister);
		
		OnSerialChanged ();
		
	} // end RegistrationDialog
	
// ---------------------------------------------------------------------------
//		• ~RegistrationDialog
// ---------------------------------------------------------------------------

RegistrationDialog::~RegistrationDialog (void)

	{ // begin ~RegistrationDialog
		
	} // end ~RegistrationDialog
	
// ---------------------------------------------------------------------------
//		• GetSerialNumber
// ---------------------------------------------------------------------------

void 
RegistrationDialog::GetSerialNumber (

	Str255	outSerial)
	
	{ // begin GetSerialNumber

		GetDialog ()->FindPaneByID (pane_SerialNumber)->GetDescriptor (outSerial);
	
	} // end GetSerialNumber
	
// ---------------------------------------------------------------------------
//		• OnSerialChanged
// ---------------------------------------------------------------------------

void	
RegistrationDialog::OnSerialChanged (void)
	
	{ // begin OnSerialChanged
		
		LPane*	registerButton = GetDialog ()->FindPaneByID (pane_OK);
		
		Str255	serial;
		GetSerialNumber (serial);
		if (Registration::TestSerialNumber (serial))
			registerButton->Enable ();
		else registerButton->Disable ();

	} // end OnSerialChanged
	
// ---------------------------------------------------------------------------
//		• OnOK
// ---------------------------------------------------------------------------

Boolean	
RegistrationDialog::OnOK (void)
	
	{ // begin OnOK
		
		Str255	serial;
		GetSerialNumber (serial);
		if (!Registration::TestSerialNumber (serial)) {
			::SysBeep (1);
			return false;
			} // if
		
		try {
			Registration::RegisterSerialNumber (serial);
			} // try
			
		catch (...) {
			return false;
			} // catch
			
		return true;
		
	} // end OnOK

// ---------------------------------------------------------------------------
//		• Run
// ---------------------------------------------------------------------------

Boolean	
RegistrationDialog::Run (void)
	
	{ // begin Run
		
		//	Show the dialog
		GetDialog ()->Show ();
		
		for (;;) {
			switch (DoDialog ()) {
				case msg_Cancel:
				case cmd_Close:
					return false;
					
				case msg_OK:
					if (!OnOK ()) continue;
					//	Fall through…
					
				case msg_NotYet:
					return true;
				
				case msg_SerialNumber:
					OnSerialChanged ();
					break;
					
				case msg_WebRegister:
					OnURL (pane_WebRegister);
					break;
				} // switch
			} // for
			
	} // end Run

#pragma mark -

#include "SerialNumber.h"

#include "ERegistrationFile.h"

#include "esellerate.h"
#include "validate.h"

// ---------------------------------------------------------------------------
//		• Initialize
// ---------------------------------------------------------------------------

Boolean
Registration::Initialize (void) 
	
	{ // begin Initialize
		
		return true;
		
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
		
		return RegistrationDialog (inSuper, !IsExpired (), inEventMask, true).Run ();
		
	} // end DoStartupDialog

// ---------------------------------------------------------------------------
//		• DoPurchaseDialog
// ---------------------------------------------------------------------------

Boolean
Registration::DoPurchaseDialog (
	
	LCommander*		inSuper)
	
	{ // begin DoPurchaseDialog		
		
		if (IsRegistered ()) return true;
		
		return RegistrationDialog (inSuper).Run ();
		
	} // end DoPurchaseDialog
