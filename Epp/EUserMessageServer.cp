/*
	File:		EUserMessageServer.cp

	Contains:	Implementation of the user message server.

	Written by:	Richard Wesley

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		20 Aug 2001		rmgw	Cache notification icon suite. 
		16 Jul 2001		rmgw	Fix various AttemptMessages problems. 
		16 Jul 2001		rmgw	Better signature for ShowMessages; define sSingleton. 
		16 Jul 2001		rmgw	Create user message system. 
*/

#include "EUserMessageServer.h"

#include "EUserMessageDialog.h"

#include "MProcesses.h"

//	=== Class Variables ===

EUserMessageServer*
EUserMessageServer::sSingleton = 0;

// ---------------------------------------------------------------------------
//	¥ EUserMessageServer											  [public]
// ---------------------------------------------------------------------------

EUserMessageServer::EUserMessageServer (

	LCommander*			inDialogSuper,
	ResIDT				inNotifyIconID,
	ResIDT				inDialogID)
	
	: mDialogSuper (inDialogSuper)
	, mDialogID (inDialogID)
	, mNotifyIcon (new MIconSuite (inNotifyIconID, kSelectorAllSmallData))
	
	{ // begin EUserMessageServer
		
		Assert_(sSingleton == 0);
		sSingleton = this;
		
		StartRepeating ();
		
	} // end EUserMessageServer

// ---------------------------------------------------------------------------
//	¥ ~EUserMessageServer											  [public]
// ---------------------------------------------------------------------------

EUserMessageServer::~EUserMessageServer (void)
	
	{ // begin ~EUserMessageServer
	
		Assert_(sSingleton == this);
		sSingleton = 0;

	} // end ~EUserMessageServer

#pragma mark -

// ---------------------------------------------------------------------------
//	¥ ShowMessages													  [public]
// ---------------------------------------------------------------------------

void
EUserMessageServer::ShowMessages (
	
	ResIDT					inDialogID,
	LCommander*				inDialogSuper,
	const	MessageList&	inMessages)
	
	{ // begin ShowMessages
		
		EUserMessageDialog	dlog (inDialogID, inDialogSuper, inMessages);
		dlog.Run ();

	} // end ShowMessages

// ---------------------------------------------------------------------------
//	¥ AttemptMessages													  [public]
// ---------------------------------------------------------------------------
//	Displays all the pending messages, unless we shouldn't.  We shouldn't if
//		a) we are in the background
//		b) we are in a silent AppleEvent
//	If we shouldn't, we put up a notification; otherwise we put up a dialog.
//	Of source, we do nothing if there are no messages.

void
EUserMessageServer::AttemptMessages (void)
	
	{ // begin AttemptMessages
		
		//	Anything to do?
		if (0 == mPendingMessages.size ()) return;
		
		//	Can we interact?
		MPSN	currentProcess (kCurrentProcess);
		MPSN	frontProcess;
		frontProcess.SetFront ();	//	Make it the front process
		
		AEInteractAllowed	level;
		AEGetInteractionAllowed (&level);
		
		//	If we can't, put up a notification and get out.
		if ((level == kAEInteractWithSelf) || (currentProcess != frontProcess)) {
			//	Make sure we have an NMRec up.
			if (!mNotification)
				mNotification = new MNMRec (*mNotifyIcon);
			
			return;
			} // if
		
		//	Clear any existing notification
		mNotification = 0;
		
		//	Don't allow reentrancy
		StopRepeating ();
		
		//	Remove the messages and show them
		MessageList		dialogList (mPendingMessages.begin (), mPendingMessages.end ());
		mPendingMessages.clear ();
		
		ShowMessages (mDialogID, mDialogSuper ? mDialogSuper : LCommander::GetTarget (), dialogList);
		
		//	Ready for further punishment
		StartRepeating ();
		
	} // end AttemptMessages

// ---------------------------------------------------------------------------
//	¥ SpendTime														  [public]
// ---------------------------------------------------------------------------

void
EUserMessageServer::SpendTime (

	const EventRecord& 	/*inMacEvent*/)
	
	{ // begin SpendTime
		
		AttemptMessages ();
		
	} // end SpendTime

// ---------------------------------------------------------------------------
//	¥ QueueUserMessage												  [public]
// ---------------------------------------------------------------------------

void
EUserMessageServer::QueueUserMessage (

	const	EUserMessage&	inMessage)
	
	{ // begin QueueUserMessage
		
		mPendingMessages.push_back (inMessage);
		
	} // end QueueUserMessage

// ---------------------------------------------------------------------------
//	¥ DisplayUserMessage											  [public]
// ---------------------------------------------------------------------------

void
EUserMessageServer::DisplayUserMessage (

	const	EUserMessage&	inMessage)
	
	{ // begin DisplayUserMessage
		
		QueueUserMessage (inMessage);
		
		AttemptMessages ();
		
	} // end DisplayUserMessage

