/*
	File:		FPEditText.h

	Contains:	LEditText smart enough to handle floating point numbers (cheezy US style)
	Written by:	Dav Lion

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

	21 mar 2001		dml			created

*/
#pragma once

#include <LEditText.h>


class FPEditText : public LEditText {
	protected:
		bool	mHasDecimal;

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
		
	};//end FPEditText