/*
	File:		FPEditText.cp

	Contains:	LEditText smart enough to handle floating point numbers (cheezy US style)
	Written by:	Dav Lion

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

	04 oct 2001		dml		accept negative floats!
	09 Aug 2001		drd		Removed dependency on Toolbox++ and <string.h>
	07 Aug 2001		drd		294 Added gDecimalPoint so we aren't tied to USA
	21 mar 2001		dml		created
*/

#include "FPEditText.h"

char	FPEditText::gDecimalPoint = 0;

// ---------------------------------------------------------------------------
//	¥ FPEditText								Stream Constructor		  [public]
// ---------------------------------------------------------------------------

FPEditText::FPEditText(
	LStream*	inStream,
	ClassIDT	inImpID)

	: LEditText(inStream, inImpID)
	, mHasDecimal (false)
{
	this->InitializeDecimalPoint();
	this->SetupKeyFilter();
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
	this->InitializeDecimalPoint();
	this->SetupKeyFilter();
}//end big ct


FPEditText::~FPEditText() {
}//end dt

/*
FloatingPointField
	Key Filter for Floating Point characters
*/
EKeyStatus	
FPEditText::FloatingPointField(TEHandle		/*inMacTEH*/,
								UInt16			inKeyCode,
								UInt16			&ioCharCode,
								EventModifiers	/*inModifiers*/)
{
	EKeyStatus	theKeyStatus = keyStatus_PassUp;

	if (UKeyFilters::IsTEDeleteKey(inKeyCode)) {
		theKeyStatus = keyStatus_TEDelete;

	} else if (UKeyFilters::IsTECursorKey(inKeyCode)) {
		theKeyStatus = keyStatus_TECursor;

	} else if (UKeyFilters::IsExtraEditKey(inKeyCode)) {
		theKeyStatus = keyStatus_ExtraEdit;

	} else if (((ioCharCode >= '0') &&
			   (ioCharCode <= '9')) ||
			   (ioCharCode == '-') 	||
			   (ioCharCode == gDecimalPoint)) {
		theKeyStatus = keyStatus_Input;
	}
	 else if (UKeyFilters::IsActionKey(inKeyCode)) {
	 	theKeyStatus = keyStatus_PassUp;
	 }
	 else {
		theKeyStatus = keyStatus_Reject;
	 	}//else unhappy so beep

	return theKeyStatus;
}//end FloatingPointField


	
								
/*
HandleKeyPress {OVERRIDE}
*/
Boolean		
FPEditText::HandleKeyPress(const EventRecord&	inKeyEvent) {
	UInt16			theChar((UInt16) (inKeyEvent.message & charCodeMask));

	Boolean keyHandled (LEditText::HandleKeyPress(inKeyEvent));
	if (keyHandled) {	
		LStr255		theText;
		this->GetDescriptor(theText);
		mHasDecimal = theText.Find(gDecimalPoint) != 0;
		this->SetupKeyFilter();
	}//endif handled
		
	return keyHandled;
}//end HandleKeyPress

/*
InitializeDecimalPoint
	Lazy instantiation
*/
void
FPEditText::InitializeDecimalPoint()
{
	if (gDecimalPoint == 0) {
		Intl0Hndl	itl0 = (Intl0Hndl) ::GetIntlResource(0);	// Get Ôitl0Õ resource
		gDecimalPoint = (*itl0)->decimalPt;
	}
} // InitializeDecimalPoint

/*
SetupKeyFilter
*/
void
FPEditText::SetupKeyFilter() {
	if (mHasDecimal)
		this->SetKeyFilter(UKeyFilters::IntegerField);
	else
		this->SetKeyFilter(FloatingPointField);
}//end SetupKeyFilter
