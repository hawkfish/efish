/*
	File:		RegistrationSerial.cp

	Contains:	Implementation of the Serial Number Registration singleton.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1999-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

         <3>    10/30/01    rmgw    Use botleneck in RegisterSerialNumber.
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

#include "MFileSpec.h"
#include "MPString.h"
#include "MSpecialFolder.h"

#include <UResourceMgr.h>

//	=== Types ===

typedef	unsigned	long	
t_secs;
		
//	=== Constants ===

const	ResIDT		strn_Registration		= 1300;

enum {
	kRegFileNameIndex = 1
	};
	
const	unsigned	char	
kXorMask = 'F';

// ---------------------------------------------------------------------------
//		• operator!=
// ---------------------------------------------------------------------------

static int
operator!= (

	const	NumVersion&	lhs,
	const	NumVersion&	rhs)
	
	{ // begin operator!=
		
		//	majorRev
		if (lhs.majorRev > rhs.majorRev)
			return 1;
			
		if (lhs.majorRev < rhs.majorRev)
			return -1;
			
		//	minorAndBugRev
		if (lhs.minorAndBugRev > rhs.minorAndBugRev)
			return 1;
			
		if (lhs.minorAndBugRev < rhs.minorAndBugRev)
			return -1;
			
		//	stage
		if (lhs.stage > rhs.stage)
			return 1;
			
		if (lhs.stage < rhs.stage)
			return -1;
		
		if (lhs.stage == finalStage) 
			return 0;
		
		//	nonRelRev
		if (lhs.nonRelRev > rhs.nonRelRev)
			return 1;
			
		if (lhs.nonRelRev < rhs.nonRelRev)
			return -1;
		
		//	Matches
		return 0;
		
	} // end operator!=
	
// ---------------------------------------------------------------------------
//		• UpdateLicenseFile
// ---------------------------------------------------------------------------

static t_secs
UpdateLicenseFile (

	FSSpec&	outSpec)
	
	{ // begin UpdateLicenseFile
		
		try {
			//	Find the registration file
			MFileSpec		regSpec (MSpecialFolder (), MPString (strn_Registration, kRegFileNameIndex), false); 
			if (!regSpec.Exists ()) regSpec.CreateResFile ('reg#');
			
			//	Hide it
			FInfo			fInfo;
			regSpec.GetFinderInfo (fInfo);
			fInfo.fdFlags |= kIsInvisible;
			regSpec.SetFinderInfo (fInfo);
			
			//	Get the dates
			CInfoPBRec		pb;
			regSpec.GetCatInfo (pb);
			
			//	Get the app vers resource
			StResource		appVersRsrc ('vers', 1);
			StHandleLocker	appVersLock (appVersRsrc);
			VersRecHndl		appVers (reinterpret_cast<VersRecHndl> (appVersRsrc.Get ()));
			
			//	Get the reg vers resource
			StCurResFile	saveResFile;
			MResFile		regFile (regSpec);
			StNewResource	regVersRsrc ('vers', 1);
			
			//	Compare them
			if (regVersRsrc.ResourceExisted ()) {
				//	Reg vers existed, so compare
				VersRecHndl	regVers (reinterpret_cast<VersRecHndl> (regVersRsrc.Get ()));
				int			regComp = ((**regVers).numericVersion != (**appVers).numericVersion);
				
				if (regComp < 0) {
					//	reg earlier than app, so reset dates
					::GetDateTime (&pb.hFileInfo.ioFlCrDat);
					pb.hFileInfo.ioFlMdDat = pb.hFileInfo.ioFlCrDat;
					regSpec.SetCatInfo (pb);
					} // if
				} // if
				
			//	Make the reg vers match us
			::PtrToXHand (*appVersRsrc, regVersRsrc.Get (), ::GetHandleSize (appVersRsrc));
			
			//	Return the license file
			outSpec = regSpec;
			
			return pb.hFileInfo.ioFlCrDat;
			} // try
			
		catch (...) {
			return 0;
			} // catch
			
	} // end UpdateLicenseFile
	
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
//		• IsExpired
// ---------------------------------------------------------------------------

Boolean
Registration::IsExpired (void) 
	
	{ // begin IsExpired
	
		//	Find the registration file
		FSSpec			regSpec;
		t_secs			fileSecs = UpdateLicenseFile (regSpec);
		
		//	Get the current time
		t_secs			nowSecs;
		::GetDateTime (&nowSecs);
		
		//	Add 30 days
		t_secs			expireSecs = fileSecs + 30L * 24L * 60L * 60L;
		
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

			FSSpec			regSpec; 
			if (!UpdateLicenseFile (regSpec)) return false;
			
			StCurResFile	saveResFile;
			MResFile		regFile (regSpec);
			
			Handle			h = ::Get1Resource ('STR ', 128);
			ThrowIfResFail_(h);
			
			//	Check prefs serial
			StringHandle	s = reinterpret_cast<StringHandle> (h);
			Str255			serial;
			::BlockMoveData (*s, serial, **s + 1);
			::XorSerial (serial, kXorMask);
			if (::TestSerial (serial)) return true;
			
			::RemoveResource (h);
			::DisposeHandle (h);
			h = nil;
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
		
		FSSpec			regSpec; 
		if (!UpdateLicenseFile (regSpec)) return;
			
		StCurResFile	saveResFile;
		MResFile		regFile (regSpec);
		
		StNewResource	h ('STR ', 128, StrLength (inSerial) + 1, true);
		::XorSerial (inSerial, kXorMask);
		::BlockMoveData (inSerial, *h, StrLength (inSerial) + 1);
		
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
