/*
	File:		EUserMessage.cp

	Contains:	Implementation of a user message.

	Written by:	Richard Wesley

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		26 Jul 2001		rmgw	Add C string utilities. 
		17 Jul 2001		rmgw	Fix null param expansion. 
		16 Jul 2001		rmgw	Add pascal string utilities. 
		16 Jul 2001		rmgw	Add SetParamText utilities. 
		16 Jul 2001		rmgw	Create user message system. 
*/

#include "EUserMessage.h"

#include <cstring>

// ---------------------------------------------------------------------------
//	¥ SetParamText													  [public]
// ---------------------------------------------------------------------------

EUserMessage::TextRef
EUserMessage::SetParamText (

	TextRef					inText,
	const	char*			param0,
	const	char*			param1, 
	const	char*			param2, 
	const	char*			param3)
	
	{ // begin SetParamText
		
		const	char*		params[] = {param0, param1, param2, param3};
		Str15				key = "\p^0";
		short				e;
		
		for (int i = 0; i < (sizeof (params) / sizeof (params[0])); ++i) {
			const	char		null = 0;
			const	char*		param = params[i] ? params[i] : &null;
			MNewHandle			hValue (param, std::strlen (param));
			key[key[0]] = '0' + i;
			do {
				e = ::ReplaceText (*inText, hValue, key);
				} while (e > 0);
			
			ThrowIfOSErr_(e);
			} // for
		
		return inText;
		
	} // end SetParamText

// ---------------------------------------------------------------------------
//	¥ SetParamText													  [public]
// ---------------------------------------------------------------------------

EUserMessage::TextRef
EUserMessage::SetParamText (

	ResID					inTextID,
	const	char*			param0,
	const	char*			param1, 
	const	char*			param2, 
	const	char*			param3)
	
	{ // begin SetParamText
		
		TextRef			inText (new MNewHandle (::GetResource ('TEXT', inTextID)));
		::DetachResource (*inText);
		
		return SetParamText (inText, param0, param1, param2, param3);
		
	} // end SetParamText

// ---------------------------------------------------------------------------
//	¥ SetParamText													  [public]
// ---------------------------------------------------------------------------

EUserMessage::TextRef
EUserMessage::SetParamText (

	const	char*			inString,
	const	char*			param0,
	const	char*			param1, 
	const	char*			param2, 
	const	char*			param3)
	
	{ // begin SetParamText
		
		TextRef			inText (new MNewHandle (inString, std::strlen (inString)));
		::DetachResource (*inText);
		
		return SetParamText (inText, param0, param1, param2, param3);
		
	} // end SetParamText

#pragma mark -

// ---------------------------------------------------------------------------
//	¥ SetParamText													  [public]
// ---------------------------------------------------------------------------

EUserMessage::TextRef
EUserMessage::SetParamText (

	TextRef					inText,
	ConstStr255Param		param0,
	ConstStr255Param		param1, 
	ConstStr255Param		param2, 
	ConstStr255Param		param3)
	
	{ // begin SetParamText
		
		ConstStr255Param	params[] = {param0, param1, param2, param3};
		Str15				key = "\p^0";
		short				e;
		
		for (int i = 0; i < (sizeof (params) / sizeof (params[0])); ++i) {
			unsigned	char	null = 0;
			ConstStr255Param	param = params[i] ? params[i] : &null;
			MNewHandle			hValue (param + 1, param[0]);
			key[key[0]] = '0' + i;
			do {
				e = ::ReplaceText (*inText, hValue, key);
				} while (e > 0);
			
			ThrowIfOSErr_(e);
			} // for
		
		return inText;
		
	} // end SetParamText

// ---------------------------------------------------------------------------
//	¥ SetParamText													  [public]
// ---------------------------------------------------------------------------

EUserMessage::TextRef
EUserMessage::SetParamText (

	ResID					inTextID,
	ConstStr255Param		param0,
	ConstStr255Param		param1, 
	ConstStr255Param		param2, 
	ConstStr255Param		param3)
	
	{ // begin SetParamText
		
		TextRef			inText (new MNewHandle (::GetResource ('TEXT', inTextID)));
		::DetachResource (*inText);
		
		return SetParamText (inText, param0, param1, param2, param3);
		
	} // end SetParamText

// ---------------------------------------------------------------------------
//	¥ SetParamText													  [public]
// ---------------------------------------------------------------------------

EUserMessage::TextRef
EUserMessage::SetParamText (

	ConstStr255Param		inString,
	ConstStr255Param		param0,
	ConstStr255Param		param1, 
	ConstStr255Param		param2, 
	ConstStr255Param		param3)
	
	{ // begin SetParamText
		
		TextRef			inText (new MNewHandle (inString + 1, inString[0]));
		::DetachResource (*inText);
		
		return SetParamText (inText, param0, param1, param2, param3);
		
	} // end SetParamText

#pragma mark -

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
//	Utility constructor to simulate a typical alert invokation:
//
//		ParamText (param0, param1, param2, param3);
//		StopAlert (resID, nil);
//
//	Becomes
//
//		EUserMessage	msg (resID, kStopIcon, param0, param1, param2, param3);

EUserMessage::EUserMessage (

	ResID					inTextID,
	ResID					inIconID,
	ConstStr255Param		param0,
	ConstStr255Param		param1, 
	ConstStr255Param		param2, 
	ConstStr255Param		param3)
	
	: mIconID (inIconID)
	, mMessage (SetParamText (inTextID, param0, param1, param2, param3))

	{ // begin EUserMessage
		
	} // end EUserMessage

// ---------------------------------------------------------------------------
//	¥ EUserMessage													  [public]
// ---------------------------------------------------------------------------
//	Utility constructor to display a string with optional text substitution:
//
//		EUserMessage (msg, kNoteIcon);

EUserMessage::EUserMessage (

	ConstStr255Param		inString,
	ResID					inIconID,
	ConstStr255Param		param0,
	ConstStr255Param		param1, 
	ConstStr255Param		param2, 
	ConstStr255Param		param3)
	
	: mIconID (inIconID)
	, mMessage (SetParamText (inString, param0, param1, param2, param3))

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
