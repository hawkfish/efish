/*
	File:		RegistrationDialog.h

	Contains:	Interface to the Registration dialog.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1999 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		<3>		5 dec 2000 	dml		add IsTimeLimited predicate (expiration and registration separation)
         <2>     12/9/99    rmgw    Factor registration from dialog.
         <1>     12/8/99    rmgw    first checked in.
*/


#pragma once

class Registration

	{

	protected:
	
	public:
		
		static	Boolean			IsTimeLimited			(void);
		static	Boolean			IsRegistered			(void);
		static	void			RegisterSerialNumber	(StringPtr			inSerial);
		static	Boolean			RunDialog				(LCommander*		inSuper,
														 UInt32				inNotYetTicks = 0,
														 short				inEventMask = everyEvent);
	};
