/*
	File:		EPostAction.cp

	Contains:	Implementation of a smart action poster.

	Written by:	Richard Wesley

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		20 Jul 2001		rmgw	Created. 
		
	Description:
	
		This class is designed to be used with "state recording" actions.
		The destructor posts the action, and it listens for grow zone problems.
		It also has LAction* semantics to make it easy to drop in.
		
		Typical usage would be:
		
		{	
			EPostAction		postAction (theUndoTarget);
			
			try {postAction = new MyStateAction (...);} catch (...) {}
			
			//	Perform the action
			
		}	//	Destructor posts action, or 0 if we are low on memory or some such.
		 
*/

#include "EPostAction.h"

#include <LGrowZone.h>

// ---------------------------------------------------------------------------
//	¥ EPostAction													  [public]
// ---------------------------------------------------------------------------

EPostAction::EPostAction (

	LCommander*			inTarget)
	
	: mTarget (inTarget)
	, mAction (0)

	{ // begin EPostAction
	
		LGrowZone::GetGrowZone()->AddListener (this);

	} // end EPostAction

// ---------------------------------------------------------------------------
//	¥ ~EPostAction													  [public]
// ---------------------------------------------------------------------------

EPostAction::~EPostAction (void)
	
	{ // begin ~EPostAction
		
		mTarget->PostAction (mAction);
		
	} // end ~EPostAction

// ---------------------------------------------------------------------------
//	¥ operator=														  [public]
// ---------------------------------------------------------------------------

EPostAction&
EPostAction::operator= (

	LAction*	inAction)
	
	{ // begin operator=
		
		if (inAction != mAction) {
			delete mAction;
			mAction = inAction;
			} // if
			
		return *this;

	} // end operator=

// ---------------------------------------------------------------------------
//	¥ ListenToMessage													  [public]
// ---------------------------------------------------------------------------

void
EPostAction::ListenToMessage (

	MessageT	inMessage, 
	void *		ioParam) 
	
	{ // begin ListenToMessage
		
		switch (inMessage) {
			case msg_GrowZone:
				{
				SInt32*	bytesNeeded = (SInt32*) ioParam;
				if (*bytesNeeded < 0) break;
				
				long	startFree = FreeMem ();
				
				delete mAction;
				mAction = 0;
				
				*bytesNeeded = FreeMem () - startFree;
				break;
				} // case
			} // switch
			
	} // ListenToMessage

