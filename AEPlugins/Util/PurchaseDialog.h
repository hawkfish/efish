/*
	File:		PurchaseDialog.h

	Contains:	Interface to the Color Palette dialog.

	Written by:	Richard Wesley

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights Reserved.

*/

#pragma once

#include "adm_dialog.h"

#include "adm_button.h"
#include "adm_edit.h"

class PurchaseDialog : public ADM::Dialog

{

		//	Items
	typedef	ADM::Button*	ButtonRef;
	typedef	ADM::Item*		ItemRef;
	typedef	ADM::Edit*		EditRef;
	
	ButtonRef		m_purchase;
	ButtonRef		m_cancel;
	ButtonRef		m_not_yet;
	EditRef			m_prompt;
	EditRef			m_countdown;
		
		//	Countdown
	ASInt32			mNotYetSecs;
	ASInt32			mCountdownSecs;
	ADMTimerRef		mTimer;
	
		//	Illegal
					PurchaseDialog		(const	PurchaseDialog&	other);
	PurchaseDialog&	operator=			(const	PurchaseDialog&	other);

protected:
		
		//	Message handlers
	void			SetUpGUI			(void);
	void			OnNotYet			(ADM::CallbackObject*	caller, 
										 void *					userData, 
										 void *					callData);
	
public:
	
	enum Items {
		kID_Purchase = 1,
		kID_Cancel,
		kID_NotYet,
		kID_Prompt,
		kID_CountDown,
		kID_Fnord
		};

		//	Construction/Destruction
					PurchaseDialog		(ASInt32 			inNotYetSecs = 10);
	virtual			~PurchaseDialog		(void);

		//	Access
	ASInt32			GetCountdownSecs	(void) const {return mCountdownSecs;};
	void			SetCountdownSecs	(ASInt32			inCountdownSecs);
	
		//	Dialog management
	virtual	bool	Timer				(ADMTimerRef 		timer);
	bool			Run					(void);
};
