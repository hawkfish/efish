/*
	File:		FPEditText.cp

	Contains:	LEditText smart enough to handle floating point numbers (cheezy US style)
	Written by:	Dav Lion

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

	21 mar 2001		dml			created

*/

#include "FPEditText.h"

// ---------------------------------------------------------------------------
//	¥ FPEditText								Stream Constructor		  [public]
// ---------------------------------------------------------------------------

FPEditText::FPEditText(
	LStream*	inStream,
	ClassIDT	inImpID)

	: LEditText(inStream, inImpID)
	, mHasDecimal (false)
{
	SetupKeyFilter();
}


// ---------------------------------------------------------------------------
//	¥ FPEditText								Parameterized Constructor [public]
// ---------------------------------------------------------------------------

FPEditText::FPEditText(
	const SPaneInfo&	inPaneInfo,
	LCommander*			inSuperCommander,
	ConstStringPtr		inInitialText,
	ResIDT				inTextTraitsID,
	MessageT			inMessage,
	SInt16				inMaxChars,
	UInt8				inAttributes,
	TEKeyFilterFunc		inKeyFilter,
	bool				inPasswordField,
	ClassIDT			inImpID)
	: LEditText (inPaneInfo, inSuperCommander, inInitialText, inTextTraitsID, inMessage,
				inMaxChars, inAttributes, inKeyFilter, inPasswordField, inImpID)
	, mHasDecimal (false)
{
	SetupKeyFilter();
}//end big ct


FPEditText::~FPEditText() {
	}//end dt

EKeyStatus	
FPEditText::FloatingPointField(TEHandle		/*inMacTEH*/,
								UInt16			inKeyCode,
								UInt16			&ioCharCode,
								EventModifiers	/*inModifiers*/) {

	EKeyStatus	theKeyStatus = keyStatus_PassUp;

	if (UKeyFilters::IsTEDeleteKey(inKeyCode)) {
		theKeyStatus = keyStatus_TEDelete;

	} else if (UKeyFilters::IsTECursorKey(inKeyCode)) {
		theKeyStatus = keyStatus_TECursor;

	} else if (UKeyFilters::IsExtraEditKey(inKeyCode)) {
		theKeyStatus = keyStatus_ExtraEdit;

	} else if (((ioCharCode >= '0') &&
			   (ioCharCode <= '9')) ||
			   (ioCharCode == '.')) {
		theKeyStatus = keyStatus_Input;
	}
	 else {
		theKeyStatus = keyStatus_Reject;
	 	}//else unhappy so beep

	return theKeyStatus;
}//end FloatingPointField



Boolean		
FPEditText::HandleKeyPress(const EventRecord&	inKeyEvent) {
	UInt16		theChar		 ((UInt16) (inKeyEvent.message & charCodeMask));

	Boolean keyHandled (LEditText::HandleKeyPress(inKeyEvent));
	if (keyHandled) {	
		if (theChar == '.')
			mHasDecimal = true;
		SetupKeyFilter();
		}//endif handled
		
	return keyHandled;
	}//end HandleKeyPress




void
FPEditText::SetupKeyFilter() {
	if (mHasDecimal)
		SetKeyFilter(UKeyFilters::IntegerField);
	else
		SetKeyFilter(FloatingPointField);
	}//end SetupKeyFilter