/*
	File:		RegistrationSerial.cp

	Contains:	Implementation of the Serial Number Registration singleton.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1999-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

         <5>    11/1/01   	rmgw    Factor ERegistrationFile; add Initialize.
         <3>    10/30/01    rmgw    Use bottleneck in RegisterSerialNumber.
         <2>    10/30/01    rmgw    Hide license file and add vers resource.
         <1>    10/29/01    rmgw    Created from old Registration.cp.
*/


#include "Registration.h"

#include "SerialNumber.h"

#include "MResFile.h"

#pragma mark RegistrationDialog

//	=== Constants ===

const	ResIDT		PPob_RegistrationDialog		= 1300;

const	PaneIDT		pane_OK						= 'ok  ';
const	PaneIDT		pane_NotYet					= 'nyet';
const	PaneIDT		pane_SerialNumber			= 'reg#';
const	PaneIDT		pane_WebRegister			= 'rWeb';
const	PaneIDT		pane_Countdown				= 'down';

const	MessageT	msg_NotYet					= -1301;
const	MessageT	msg_SerialNumber			= -1302;
const	MessageT	msg_WebRegister				= -1303;

#include "EURLDialogHandler.h"
#include <LPeriodical.h>

class RegistrationDialog 	: public EURLDialogHandler 
							, public LPeriodical
	{

	protected:
	
		UInt32					mNotYetTicks;
		UInt32					mStartTicks;
		
		void					SetupGUI				(void);

		virtual	void			OnSerialChanged			(void);
		virtual	Boolean			OnOK					(void);


	public:
		
								RegistrationDialog		(LCommander*		inSuper,
														 UInt32				inNotYetTicks = 0,
														 short				inEventMask = everyEvent);
		virtual					~RegistrationDialog		(void);
		
		void 					GetSerialNumber 		(Str255				outSerial);
		
		virtual void			SpendTime				(const EventRecord&	inMacEvent);

		virtual	Boolean			Run						(void);
	};

#pragma mark -

// ---------------------------------------------------------------------------
//		• RegistrationDialog
// ---------------------------------------------------------------------------

RegistrationDialog::RegistrationDialog (
	
	LCommander*		inSuper,
	UInt32			inNotYetTicks,
	short			inEventMask)
	
	: EURLDialogHandler (PPob_RegistrationDialog, inSuper, inEventMask)
	
	, mNotYetTicks (inNotYetTicks)
	, mStartTicks (0)
	
	{ // begin RegistrationDialog		

		SetupGUI ();
		
		OnSerialChanged ();
		
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
		
		SetupURL (pane_WebRegister);
		
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
		if (::TestSerial (serial))
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
		if (!::TestSerial (serial)) {
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
		
		//	Start the countdown
		mStartTicks = ::TickCount ();
		StartIdling ();
		
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

#include "MFileSpec.h"
#include "MPString.h"
#include "MSpecialFolder.h"

#include <UResourceMgr.h>

//	=== Constants ===

const	ResIDT		
strn_Registration		= 1300;

enum {
	kRegFileNameIndex = 1
	};
	
const	unsigned	char	
kXorMask = 'F';

// ---------------------------------------------------------------------------
//		• DebugSerial
// ---------------------------------------------------------------------------

void
DebugSerial (

	const	char*		/*inTag*/,
	ConstStr255Param	/*inSerial*/)
	
	{ // begin DebugSerial
	
	} // end DebugSerial
	
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
			
			//	Check it
			::XorSerial (serial, kXorMask);
			if (::TestSerial (serial)) return true;
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
		
		return RegistrationDialog (inSuper, IsExpired () ? 0 : inNotYetTicks, inEventMask).Run ();
		
	} // end RunDialog
