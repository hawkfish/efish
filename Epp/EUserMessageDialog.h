/*
	File:		EUserMessageDialog.h

	Contains:	Interface to the user message display dialog.

	Written by:	Richard Wesley

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		16 Jul 2001		rmgw	Create user message system. 
*/

#pragma once

#include "EDialog.h"

#include "EUserMessage.h"

#include <vector>

class EUserMessageDialog : public EDialog

	{
			//	Illegal
									EUserMessageDialog	(const	EUserMessageDialog&	other);
		EUserMessageDialog&			operator=			(const	EUserMessageDialog&	other);
		
	protected:

			//	Types
		typedef	std::vector<EUserMessage>	MessageList;
		typedef	MessageList::const_iterator	MessagePosition;
		typedef	EUserMessage::TextRef		TextRef;
		
			//	Internal state
		EUserMessage				mDefaults;
		const	MessageList&		mMessages;
		MessagePosition				mCurrentPosition;
		
			//	Details pane
		virtual	bool				DetailsAreVisible	(void) const;
		virtual	void				ShowDetails			(void);
		virtual	void				HideDetails			(void);
			
			//	Contents manipulation
		virtual	void				GetCurrentMessage	(EUserMessage&			outMessage) const;
		virtual	void				SetCurrentMessage	(const	EUserMessage&	inMessage);
		
			//	State iteration
		virtual	bool				SetCurrentPosition	(MessagePosition		inCurrent);
		virtual	bool				OnNext				(void);
		virtual	bool				OnBack				(void);
		
	public:
			
			//	Construction/Destruction
		explicit					EUserMessageDialog	(ResIDT					inDialogResID,
														 LCommander*			inSuper,
														 const	MessageList&	inMessages,
														 short					inEventMask = everyEvent);
		virtual						~EUserMessageDialog	(void);
			
			//	Dialog execution
		virtual	void				Run					(void);

	};
	