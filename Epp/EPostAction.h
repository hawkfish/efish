/*
	File:		EPostAction.h

	Contains:	Interface to a smart action poster.

	Written by:	Richard Wesley

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		20 Jul 2001		rmgw	Created. 
*/

#pragma once

#include <LListener.h>

class EPostAction : public LListener

	{
		//	Illegal
								EPostAction		(const	EPostAction&	other);
		EPostAction&			operator=		(const	EPostAction&	other);
	
	protected:
	
		LCommander*				mTarget;
		LAction*				mAction;
		
	public:
		
			//	Construction/Destruction/Copying
		explicit				EPostAction		(LCommander*			inTarget);
		virtual					~EPostAction	(void);
			
			//	Access
		LCommander*				GetTarget		(void) const {return mTarget;};
		
			//	LAction Pointer emulation
		EPostAction&			operator=		(LAction*				inAction);
		operator				LAction*		(void) const;
			
			//	LListener
		virtual void			ListenToMessage	(MessageT				inMessage,
												 void*					ioParam);
	};
