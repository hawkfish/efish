/*
	File:		EUserMessageServer.h

	Contains:	Interface to the user message server.

	Written by:	Richard Wesley

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		16 Jul 2001		rmgw	Create user message system. 
		
	Notes:

		This is a singleton pattern to avoid user confusion (messages being delivered out
		of order.
		
		Please do not create new variants of Queue/DisplayUserMessage.  If you want to
		simplify use of some common message format, add utilities/constructors to 
		EUserMessage instead.
*/

#pragma once

#include <LPeriodical.h>

#include "EUserMessage.h"

#include "MNMRec.h"

#include <vector>

class EUserMessageServer : public LPeriodical

	{
	
			//	Illegal
									EUserMessageServer	(const	EUserMessageServer&	other);
		EUserMessageServer&			operator=			(const	EUserMessageServer&	other);
		
	protected:
		
			//	Types
		typedef	std::vector<EUserMessage>	MessageList;
			
			//	Singleton
		static	EUserMessageServer*	sSingleton;
		
			//	Message list
		MessageList					mPendingMessages;

			//	Dialog parameters
		LCommander*					mDialogSuper;
		ResIDT						mNotifyIconID;
		ResIDT						mDialogID;
			
			//	Notification
		HORef<MNMRec>				mNotification;
		
			//	Message display
		virtual	void				ShowMessages		(const	MessageList&	inMessages);
		virtual	void				AttemptMessages		(void);
		
	public:
									
			//	Singleton
		static	EUserMessageServer*	GetSingleton		(void) {return sSingleton;};
		
			//	Construction/Destruction
		explicit					EUserMessageServer	(LCommander*			inDialogSuper = 0,
														 ResIDT					inNotifyIconID = 128,
														 ResIDT					inDialogID = 10000);
		virtual						~EUserMessageServer	(void);
		
			//	LPeriodical
		virtual	void				SpendTime			(const EventRecord& 	inMacEvent);

			//	Message submission
		virtual	void				QueueUserMessage	(const	EUserMessage&	inMessage);
		virtual	void				DisplayUserMessage	(const	EUserMessage&	inMessage);
	};
