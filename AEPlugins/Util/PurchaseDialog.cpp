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

	const	char*		inPrompt,
	bool 				inCanTry) 
	
	: ADM::Dialog ("com.electricfish.colorpal.purchase", 0, kADMModalDialogStyle, 0)
	
	, m_purchase (0)
	, m_cancel (0)
	, m_try (0)
	, m_prompt (0)
	
	{ // begin PurchaseDialog
		
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
		m_try 		= new ADM::Button (this, kID_Try, "Try", ADM::Rect(0, button_y, kButtonWidth, kButtonHeight));
		m_try->Move (kDialogBorder, m_try->GetPosition().v);
		m_try->AddCallback (m_try->select, this, (ADM::CallbackMethod) OnNotYet);
		
		//	Prompt
		ASRect 			r;
		r.top = kDialogBorder;
		r.left = kDialogBorder;
		r.right = kDialogWidth - kDialogBorder;
		r.bottom = r.top + 3 * kTextHeight;
		m_prompt = new ADM::Edit (this, kID_Prompt, r, ADM::Edit::Edit_StaticSingleLine);
		m_prompt->SetText (inPrompt);
		
		if (inCanTry) 
			m_try->Show ();
		else m_try->Hide ();
		
	} // end PurchaseDialog
	
// ---------------------------------------------------------------------------
//		¥ ~PurchaseDialog
// ---------------------------------------------------------------------------

PurchaseDialog::~PurchaseDialog (void) 
	
	{ // begin ~PurchaseDialog
		
		m_try->RemoveAllCallbacks ();

		delete m_prompt;
		delete m_purchase;
		delete m_cancel;
		delete m_try;
		
	} // end ~PurchaseDialog

// ---------------------------------------------------------------------------
//		¥ OnNotYet
// ---------------------------------------------------------------------------

void	
PurchaseDialog::OnNotYet (

	ADM::CallbackObject*	caller, 
	void *					userData, 
	void *					callData)
	
	{ // begin OnNotYet
		
		EndModal (kID_Try);
		
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
					
				case kID_Try:
					return true;
				} // switch
			} // for

	} // end Run
