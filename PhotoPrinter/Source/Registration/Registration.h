/*
	File:		RegistrationDialog.h

	Contains:	Interface to the Registration dialog.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1999 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

         <2>     12/9/99    rmgw    Factor registration from dialog.
         <1>     12/8/99    rmgw    first checked in.
*/


#pragma once

class Registration

	{

	protected:
	
	public:
		
		static	Boolean			IsRegistered			(void);
		static	void			RegisterSerialNumber	(StringPtr			inSerial);
		static	Boolean			RunDialog				(LCommander*		inSuper,
														 UInt32				inNotYetTicks = 0,
														 short				inEventMask = everyEvent);
	};
