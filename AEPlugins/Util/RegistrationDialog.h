/*
	File:		RegistrationDialog.h

	Contains:	Interface to the Color Palette dialog.

	Written by:	Richard Wesley

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights Reserved.

*/

#pragma once

#include "adm_dialog.h"

#include "adm_button.h"
#include "adm_edit.h"

class RegistrationDialog : public ADM::Dialog

{

		//	Items
	typedef	ADM::Button*	ButtonRef;
	typedef	ADM::Item*		ItemRef;
	typedef	ADM::Edit*		EditRef;
	
	ButtonRef		m_register;
	ButtonRef		m_cancel;
	ButtonRef		m_not_yet;
	ItemRef			m_icon;
	EditRef			m_prompt;
	EditRef			m_serial;
	EditRef			m_url;
	EditRef			m_countdown;
		
		//	Countdown
	ASInt32			mNotYetSecs;
	ASInt32			mCountdownSecs;
	ADMTimerRef		mTimer;
	
		//	Illegal
					RegistrationDialog	(const	RegistrationDialog&	other);
	RegistrationDialog&	operator=		(const	RegistrationDialog&	other);

protected:
		
		//	Message handlers
	void			SetUpGUI			(void);
	void			OnSerialChanged		(CallbackObject*	caller, 
										 void *				userData, 
										 void *				callData);
	void			OnNotYet			(CallbackObject*	caller, 
										 void *				userData, 
										 void *				callData);
	bool			OnRegister			(void);
	
public:
	
	enum Items {
		kID_OK = 1,
		kID_Cancel,
		kID_NotYet,
		kID_Icon,
		kID_Prompt,
		kID_SerialNumber,
		kID_URL,
		kID_CountDown,
		kID_Fnord
		};

		//	Construction/Destruction
					RegistrationDialog	(ASInt32 			inNotYetSecs = 10);
	virtual			~RegistrationDialog	(void);

		//	Access
	std::string 	GetSerialNumber		(void) const;
	
	ASInt32			GetCountdownSecs	(void) const {return mCountdownSecs;};
	void			SetCountdownSecs	(ASInt32	inCountdownSecs);
	
		//	Dialog management
	virtual	bool	Timer				(ADMTimerRef 		timer);
	int				Run					(void);
};
