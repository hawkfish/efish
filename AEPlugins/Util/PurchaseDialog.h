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
	ButtonRef		m_try;
	EditRef			m_prompt;
		
		//	Illegal
					PurchaseDialog		(const	PurchaseDialog&	other);
	PurchaseDialog&	operator=			(const	PurchaseDialog&	other);

protected:
		
		//	Message handlers
	void			OnNotYet			(ADM::CallbackObject*	caller, 
										 void *					userData, 
										 void *					callData);
	
public:
	
	enum Items {
		kID_Purchase = 1,
		kID_Cancel,
		kID_Try,
		kID_Prompt,
		kID_Fnord
		};

		//	Construction/Destruction
					PurchaseDialog		(const	char*			inPrompt,
										 bool 					inCanTry = true);
	virtual			~PurchaseDialog		(void);

		//	Dialog management
	bool			Run					(void);
};
