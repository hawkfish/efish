/*
	File:		RegistrationDialog.h

	Contains:	Interface to the Registration singleton.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1999-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

         <6>    11/14/01    rmgw    Soup up a la Color Pal.
         <5>    11/01/01    rmgw    Add Initialize.
         <4>    10/29/01    rmgw    Flip IsTimeLimited to IsExpired.
		 <3>	5 dec 2000 	dml		add IsTimeLimited predicate (expiration and registration separation)
         <2>     12/9/99    rmgw    Factor registration from dialog.
         <1>     12/8/99    rmgw    first checked in.
*/


#pragma once

class Registration

	{

	protected:
	
	public:
		
		static	Boolean			Initialize				(void);
		
		static	Boolean			IsExpired				(void);
		static	Boolean			IsRegistered			(void);
		
		static	Boolean			TestSerialNumber 		(StringPtr			inSerial);
		static	void			RegisterSerialNumber 	(StringPtr			inSerial);
		static	Boolean			GetSerialNumber 		(StringPtr			outSerial);

		static	Boolean			DoStartupDialog			(LCommander*		inSuper,
														 UInt32				inNotYetTicks = 0,
														 short				inEventMask = everyEvent);
		static	Boolean			DoPurchaseDialog		(LCommander*		inSuper);
	};
