/*
	File:		EUserMessage.cp

	Contains:	Implementation of a user message.

	Written by:	Richard Wesley

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		16 Jul 2001		rmgw	Create user message system. 
*/

#include "EUserMessage.h"

// ---------------------------------------------------------------------------
//	¥ EUserMessage													  [public]
// ---------------------------------------------------------------------------

EUserMessage::EUserMessage (

	ResID				inIconID,
	TextRef				inTitle,
	TextRef				inMessage,
	TextRef				inDetails)
	
	: mIconID (inIconID)
	, mTitle (inTitle)
	, mMessage (inMessage)
	, mDetails (inDetails)

	{ // begin EUserMessage
	
	} // end EUserMessage

// ---------------------------------------------------------------------------
//	¥ EUserMessage													  [public]
// ---------------------------------------------------------------------------

EUserMessage::EUserMessage (

	const	EUserMessage&	other)
	
	: mIconID (other.mIconID)
	, mTitle (other.mTitle)
	, mMessage (other.mMessage)
	, mDetails (other.mDetails)

	{ // begin EUserMessage
	
	} // end EUserMessage

// ---------------------------------------------------------------------------
//	¥ ~EUserMessage													  [public]
// ---------------------------------------------------------------------------

EUserMessage::~EUserMessage (void)
	
	{ // begin ~EUserMessage
	
	} // end ~EUserMessage

// ---------------------------------------------------------------------------
//	¥ operator=														  [public]
// ---------------------------------------------------------------------------

EUserMessage&
EUserMessage::operator= (

	const	EUserMessage&	other)
	
	{ // begin operator=
		
		if (this != &other) {
			mIconID = other.mIconID;
			mTitle = other.mTitle;
			mMessage = other.mMessage;
			mDetails = other.mDetails;
			} // if
			
		return *this;
	} // end operator=
