/*
	File:		PurchaseDialog.cpp

	Contains:	Implementation of the Registration dialog.

	Written by:	Richard Wesley

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights Reserved.

		30 Oct 2001		rmgw		first implemented.

*/

#include "PurchaseDialog.h"

#include "Registration.h"

using namespace std;

static const int kDialogBorder = 20;
static const int kDialogWidth = 330;
static const int kDialogHeight = 154;

static const int kButtonHeight = 20;
static const int kButtonWidth = 69;
static const int kButtonSpacing = 12;

static const int kTextHeight = 22;
static const int kTextSpacing = 4;

// ---------------------------------------------------------------------------
//		¥ PurchaseDialog
// ---------------------------------------------------------------------------

PurchaseDialog::PurchaseDialog (

	ASInt32		inNotYetSecs) 
	
	: ADM::Dialog ("com.electricfish.colorpal.purchase", 0, kADMModalDialogStyle, 0)
	
	, m_purchase (0)
	, m_cancel (0)
	, m_not_yet (0)
	, m_prompt (0)
	, m_countdown (0)
	
	, mNotYetSecs (inNotYetSecs)
	, mCountdownSecs (inNotYetSecs)
	, mTimer (0)
	
	{ // begin PurchaseDialog
		
		SetUpGUI ();
		
	} // end PurchaseDialog
	
// ---------------------------------------------------------------------------
//		¥ ~PurchaseDialog
// ---------------------------------------------------------------------------

PurchaseDialog::~PurchaseDialog (void) 
	
	{ // begin ~PurchaseDialog
		
		if (mTimer) AbortTimer (mTimer);
		mTimer = 0;

		m_not_yet->RemoveAllCallbacks ();

		delete m_countdown;
		delete m_prompt;
		delete m_purchase;
		delete m_cancel;
		delete m_not_yet;
		
	} // end ~PurchaseDialog

// ---------------------------------------------------------------------------
//		¥ SetUpGUI
// ---------------------------------------------------------------------------

void
PurchaseDialog::SetUpGUI (void) 
	
	{ // begin SetUpGUI
		
		//	General
		int button_y = kDialogHeight - kButtonHeight - kDialogBorder;
		
		SetTitle ("Color Pal Registration");
		Size (kDialogWidth, kDialogHeight);
		
		//	Register
		m_purchase 		= new ADM::Button (this, kID_Purchase, "Purchase", ADM::Rect(0, button_y, kButtonWidth, kButtonHeight));
		m_purchase->Move (kDialogWidth - kDialogBorder - kButtonWidth, m_purchase->GetPosition().v);

		//	Cancel
		m_cancel 		= new ADM::Button (this, kID_Cancel, "Cancel", ADM::Rect(0, button_y, kButtonWidth, kButtonHeight));
		m_cancel->Move (m_purchase->GetPosition().h - kButtonWidth - kButtonSpacing, m_cancel->GetPosition().v);

		//	Not Yet
		m_not_yet 		= new ADM::Button (this, kID_NotYet, "Not Yet", ADM::Rect(0, button_y, kButtonWidth, kButtonHeight));
		m_not_yet->Move (kDialogBorder, m_not_yet->GetPosition().v);
		m_not_yet->AddCallback (m_not_yet->select, this, (ADM::CallbackMethod) OnNotYet);
		
		//	Prompt
		ASRect 			r;
		r.top = kDialogBorder;
		r.left = kDialogBorder;
		r.right = kDialogWidth - kDialogBorder;
		r.bottom = r.top + 3 * kTextHeight;
		m_prompt = new ADM::Edit (this, kID_Prompt, r, ADM::Edit::Edit_StaticSingleLine);
		m_prompt->SetText ("This is an unrgistered trial version of Color Pal.");
		
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
PurchaseDialog::SetCountdownSecs (

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
PurchaseDialog::Timer (

	ADMTimerRef 	timer)
	
	{ // begin Timer
		
		mTimer = 0;
		
		SetCountdownSecs (mCountdownSecs - 1);
	
		return false;
		
	} // end Timer

// ---------------------------------------------------------------------------
//		¥ OnNotYet
// ---------------------------------------------------------------------------

void	
PurchaseDialog::OnNotYet (

	ADM::CallbackObject*	caller, 
	void *					userData, 
	void *					callData)
	
	{ // begin OnNotYet
		
		EndModal (kID_NotYet);
		
	} // end OnNotYet
	
// ---------------------------------------------------------------------------
//		¥ Run
// ---------------------------------------------------------------------------

bool
PurchaseDialog::Run (void) 

	{ // begin Run
		
		for (;;) {
			switch (DoModal ()) {
				case kID_Cancel:
					return false;
					
				case kID_Purchase:
					Hide ();
					if (!Registration::DoPurchaseDialog ()) {
						Show ();
						Activate ();
						continue;
						} // if
					//	Fall through
					
				case kID_NotYet:
					return true;
				} // switch
			} // for

	} // end Run
