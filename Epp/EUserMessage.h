/*
	File:		EUserMessage.h

	Contains:	Interface to a user message.

	Written by:	Richard Wesley

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

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
			
			//	Construction/Destruction/Copying
		explicit				EUserMessage	(ResID					inIconID = kStopIcon,
												 TextRef				inTitle = 0,
												 TextRef				inMessage = 0,
												 TextRef				inDetails = 0);
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
												{return mTitle;};
		void					SetMessage		(TextRef				inMessage)
												{mMessage = inMessage;}; 
		
		TextRef					GetDetails		(void) const
												{return mTitle;};
		void					SetDetails		(TextRef				inDetails)
												{mDetails = inDetails;}; 
		
	};
