/*
	File:		FPEditText.h

	Contains:	LEditText smart enough to handle floating point numbers

	Written by:	Dav Lion

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

	07 Aug 2001		drd		294 Added gDecimalPoint so we aren't tied to USA
	21 mar 2001		dml		created
*/

#pragma once

#include <LEditText.h>

class FPEditText : public LEditText {
	protected:
		bool	mHasDecimal;

		void	InitializeDecimalPoint();
		void	SetupKeyFilter();

	public:
		enum {
			class_ID = FOUR_CHAR_CODE('FPet')
		};
		
						FPEditText(LStream*		inStream,
								ClassIDT		inImpID = imp_class_ID);

						FPEditText(const SPaneInfo&	inPaneInfo,
								LCommander*			inSuperCommander,
								ConstStringPtr		inInitialText,
								ResIDT				inTextTraitsID,
								MessageT			inMessage,
								SInt16				inMaxChars,
								UInt8				inAttributes,
								TEKeyFilterFunc		inKeyFilter,
								bool				inPasswordField = false,
								ClassIDT			inImpID = imp_class_ID);

	virtual				~FPEditText();
		
	static EKeyStatus	FloatingPointField(TEHandle		/*inMacTEH*/,
											UInt16			inKeyCode,
											UInt16			&ioCharCode,
											EventModifiers	/*inModifiers*/);

	virtual Boolean		HandleKeyPress(const EventRecord&	inKeyEvent);

	protected:
	static	char	gDecimalPoint;
};//end FPEditText
