/*
	File:		RegistrationDialog.cpp

	Contains:	Implementation of the Registration dialog.

	Written by:	Richard Wesley

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights Reserved.

		30 Oct 2001		rmgw		first implemented.

*/

#include "RegistrationDialog.h"

#include "SerialNumber.h"

static const int kDialogBorder = 20;
static const int kDialogWidth = 330;
static const int kDialogHeight = 154;

static const int kButtonHeight = 20;
static const int kButtonWidth = 69;
static const int kButtonSpacing = 12;

static const int kTextHeight = 22;
static const int kTextSpacing = 4;

static const int kIconWidth = 32;
static const int kIconHeight = 32;
static const int kIconSpacing = 20;

// ---------------------------------------------------------------------------
//		¥ RegistrationDialog
// ---------------------------------------------------------------------------

RegistrationDialog::RegistrationDialog (

	ASInt32		inNotYetSecs) 
	
	: ADM::Dialog ("com.electricfish.colorpal.registration", 0, kADMModalDialogStyle, 0)
	
	, m_register (0)
	, m_cancel (0)
	, m_not_yet (0)
	, m_icon (0)
	, m_prompt (0)
	, m_serial (0)
	, m_url (0)
	, m_countdown (0)
	
	, mNotYetSecs (inNotYetSecs)
	, mCountdownSecs (inNotYetSecs)
	, mTimer (0)
	
	{ // begin RegistrationDialog
		
		SetUpGUI ();
		
		OnSerialChanged (m_serial, 0, 0);

	} // end RegistrationDialog
	
// ---------------------------------------------------------------------------
//		¥ ~RegistrationDialog
// ---------------------------------------------------------------------------

RegistrationDialog::~RegistrationDialog (void) 
	
	{ // begin ~RegistrationDialog
		
		if (mTimer) AbortTimer (mTimer);
		mTimer = 0;

		m_serial->RemoveAllCallbacks ();
		m_not_yet->RemoveAllCallbacks ();

		delete m_countdown;
		delete m_url;
		delete m_serial;
		delete m_prompt;
		delete m_icon;
		delete m_register;
		delete m_cancel;
		delete m_not_yet;
		
	} // end ~RegistrationDialog

// ---------------------------------------------------------------------------
//		¥ SetUpGUI
// ---------------------------------------------------------------------------

void
RegistrationDialog::SetUpGUI (void) 
	
	{ // begin SetUpGUI
		
		//	General
		int button_y = kDialogHeight - kButtonHeight - kDialogBorder;
		
		SetTitle ("Color Pal Registration");
		Size (kDialogWidth, kDialogHeight);
		
		//	Register
		m_register 		= new ADM::Button (this, kID_OK, "Register", ADM::Rect(0, button_y, kButtonWidth, kButtonHeight));
		m_register->Move (kDialogWidth - kDialogBorder - kButtonWidth, m_register->GetPosition().v);

		//	Cancel
		m_cancel 		= new ADM::Button (this, kID_Cancel, "Cancel", ADM::Rect(0, button_y, kButtonWidth, kButtonHeight));
		m_cancel->Move (m_register->GetPosition().h - kButtonWidth - kButtonSpacing, m_cancel->GetPosition().v);

		//	Not Yet
		m_not_yet 		= new ADM::Button (this, kID_NotYet, "Not Yet", ADM::Rect(0, button_y, kButtonWidth, kButtonHeight));
		m_not_yet->Move (kDialogBorder, m_not_yet->GetPosition().v);
		m_not_yet->AddCallback (m_not_yet->select, this, (ADM::CallbackMethod) OnNotYet);
		
		//	Icon
		ASRect 			r;
		r.top = kDialogBorder;
		r.left = kDialogBorder;
		r.bottom = r.top + kIconWidth;
		r.right = r.left + kIconHeight;
		
		//	Prompt
		r.left = r.right + kIconSpacing;
		r.right = kDialogWidth - kDialogBorder;
		r.bottom = r.top + kTextHeight;
		m_prompt = new ADM::Edit (this, kID_Prompt, r, ADM::Edit::Edit_StaticSingleLine);
		m_prompt->SetText ("Please enter your serial number:");
		
		//	Serial
		r.top = r.bottom + kTextSpacing;
		r.bottom = r.top + kTextHeight;
		m_serial = new ADM::Edit (this, kID_SerialNumber, r, ADM::Edit::Edit_SingleLine);
		m_serial->SetMaxLength (32);
		m_serial->Activate();
		m_serial->AddCallback (m_serial->changed, this, (ADM::CallbackMethod) OnSerialChanged);
	
		//	URL
		r.top = r.bottom + kTextSpacing;
		r.bottom = r.top + kTextHeight;
		m_url = new ADM::Edit (this, kID_URL, r, ADM::Edit::Edit_StaticSingleLine);
		m_url->SetText ("Register online at <http://www.kagi.com/?3UV>");
		m_url->SetFont (kADMPaletteFont);
		
		//	Countdown
		r.top = m_not_yet->GetPosition().v + 4;
		r.left = m_not_yet->GetPosition().h + m_not_yet->GetSize().h + kButtonSpacing;
		r.bottom = r.top + kTextHeight;
		r.right = r.left + 3 * kTextHeight;
		m_countdown = new ADM::Edit (this, kID_CountDown, r, ADM::Edit::Edit_StaticSingleLine);
		m_countdown->SetFont (kADMPaletteFont);
		
		if (mNotYetSecs) {
			m_countdown->Show ();
			m_not_yet->Show ();
			SetCountdownSecs (mNotYetSecs);
			} // if
			
		else {
			m_countdown->Hide ();
			m_not_yet->Hide ();
			} // else

	} // end SetUpGUI
	
// ---------------------------------------------------------------------------
//		¥ SetCountdownSecs
// ---------------------------------------------------------------------------

void	
RegistrationDialog::SetCountdownSecs (

	ASInt32			inCountdownSecs)
	
	{ // begin SetCountdownSecs
		
		//	Kill the old one
		if (mTimer) AbortTimer (mTimer);
		mTimer = 0;
		
		//	Change the countdown time
		mCountdownSecs = inCountdownSecs;
		
		if (!mCountdownSecs) {
			m_not_yet->Enable ();
			m_countdown->Hide ();
			} // if

		else {
			m_not_yet->Disable ();
			
			char	buffer[32];
			sprintf (buffer, "%ld", mCountdownSecs);
			m_countdown->SetText (buffer);
			
			m_countdown->Show ();
			
			mTimer = CreateTimer (1000);
			} // if
	
	} // end SetCountdownSecs

// ---------------------------------------------------------------------------
//		¥ Timer
// ---------------------------------------------------------------------------

bool	
RegistrationDialog::Timer (

	ADMTimerRef 	timer)
	
	{ // begin Timer
		
		mTimer = 0;
		
		SetCountdownSecs (mCountdownSecs - 1);
	
		return false;
		
	} // end Timer

// ---------------------------------------------------------------------------
//		¥ GetSerialNumber
// ---------------------------------------------------------------------------

std::string 
RegistrationDialog::GetSerialNumber (void) const
	
	{ // begin GetSerialNumber

		return m_serial->GetText ();
	
	} // end GetSerialNumber
	
// ---------------------------------------------------------------------------
//		¥ OnSerialChanged
// ---------------------------------------------------------------------------

void	
RegistrationDialog::OnSerialChanged (

	ADM::CallbackObject*	caller, 
	void *					userData, 
	void *					callData)
			
	{ // begin OnSerialChanged
		
		std::string	serial (GetSerialNumber ());
		if (::TestSerial (serial.c_str ()))
			m_register->Enable ();
		else m_register->Disable ();
		
	} // end OnSerialChanged
	
// ---------------------------------------------------------------------------
//		¥ OnNotYet
// ---------------------------------------------------------------------------

void	
RegistrationDialog::OnNotYet (

	ADM::CallbackObject*	caller, 
	void *					userData, 
	void *					callData)
	
	{ // begin OnNotYet
		
		EndModal (kID_NotYet);
		
	} // end OnNotYet
	
// ---------------------------------------------------------------------------
//		¥ OnRegister
// ---------------------------------------------------------------------------

bool	
RegistrationDialog::OnRegister (void)
	
	{ // begin OnRegister
		
		std::string	serial (GetSerialNumber ());
		if (!::TestSerial (serial.c_str ())) {
			//::SysBeep (1);
			return false;
			} // if
		
		return true;
		
	} // end OnRegister

// ---------------------------------------------------------------------------
//		¥ Run
// ---------------------------------------------------------------------------

int
RegistrationDialog::Run (void) 

	{ // begin Run
		
		for (;;) {
			switch (DoModal ()) {
				case kID_Cancel:
					return kID_Cancel;
					
				case kID_OK:
					if (!OnRegister ()) continue;
					return kID_OK;
					
				case kID_NotYet:
					return kID_NotYet;
				} // switch
			} // for

	} // end Run
