/*
	File:		EUserMessage.h

	Contains:	Interface to a user message.

	Written by:	Richard Wesley

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		26 Jul 2001		rmgw	Add C string utilities. 
		16 Jul 2001		rmgw	Add pascal string utilities. 
		16 Jul 2001		rmgw	Add SetParamText utilities. 
		16 Jul 2001		rmgw	Create user message system. 
*/

#pragma once

#include "MNewHandle.h"
#include "HORef.h"

#include <Dialogs.h>
#include <Resources.h>

class EUserMessage

	{
	
		typedef	HORef<MHandle>	TextRef;
		
		ResID		mIconID;
		TextRef		mTitle;
		TextRef		mMessage;
		TextRef		mDetails;
		
	public:
			
			//	C string utilities
		static	TextRef			SetParamText	(TextRef				ioText,
												 const	char*			param0 = nil,
												 const	char*			param1 = nil, 
												 const	char*			param2 = nil, 
												 const	char*			param3 = nil);
		static	TextRef			SetParamText	(ResID					inTextID,
												 const	char*			param0 = nil,
												 const	char*			param1 = nil, 
												 const	char*			param2 = nil, 
												 const	char*			param3 = nil);
		static	TextRef			SetParamText	(const	char*			inMessage,
												 const	char*			param0 = nil,
												 const	char*			param1 = nil, 
												 const	char*			param2 = nil, 
												 const	char*			param3 = nil);

			//	Pascal string utilities
		static	TextRef			SetParamText	(TextRef				ioText,
												 ConstStr255Param		param0 = nil,
												 ConstStr255Param		param1 = nil, 
												 ConstStr255Param		param2 = nil, 
												 ConstStr255Param		param3 = nil);
		static	TextRef			SetParamText	(ResID					inTextID,
												 ConstStr255Param		param0 = nil,
												 ConstStr255Param		param1 = nil, 
												 ConstStr255Param		param2 = nil, 
												 ConstStr255Param		param3 = nil);
		static	TextRef			SetParamText	(ConstStr255Param		inMessage,
												 ConstStr255Param		param0 = nil,
												 ConstStr255Param		param1 = nil, 
												 ConstStr255Param		param2 = nil, 
												 ConstStr255Param		param3 = nil);

			//	Construction/Destruction/Copying
		explicit				EUserMessage	(ResID					inIconID = kStopIcon,
												 TextRef				inTitle = 0,
												 TextRef				inMessage = 0,
												 TextRef				inDetails = 0);
		explicit				EUserMessage	(ResID					inTextID,
												 ResID					inIconID = kStopIcon,
												 ConstStr255Param		param0 = nil,
												 ConstStr255Param		param1 = nil, 
												 ConstStr255Param		param2 = nil, 
												 ConstStr255Param		param3 = nil);
		explicit				EUserMessage	(ConstStr255Param		inMessage,
												 ResID					inIconID = kStopIcon,
												 ConstStr255Param		param0 = nil,
												 ConstStr255Param		param1 = nil, 
												 ConstStr255Param		param2 = nil, 
												 ConstStr255Param		param3 = nil);
								EUserMessage	(const	EUserMessage&	other);
		virtual					~EUserMessage	(void);
		
		EUserMessage&			operator=		(const	EUserMessage&	other);
			
			//	Access
		ResID					GetIconID		(void) const 
												{return mIconID;};
		void					SetIconID		(ResID					inIconID)
												{mIconID = inIconID;};
		
		TextRef					GetTitle		(void) const
												{return mTitle;};
		void					SetTitle		(TextRef				inTitle)
												{mTitle = inTitle;}; 
		
		TextRef					GetMessage		(void) const
												{return mMessage;};
		void					SetMessage		(TextRef				inMessage)
												{mMessage = inMessage;}; 
		
		TextRef					GetDetails		(void) const
												{return mDetails;};
		void					SetDetails		(TextRef				inDetails)
												{mDetails = inDetails;}; 
		
	};
