/*
	File:		Registration.h

	Contains:	Interface to the Registration singleton.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1999-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

*/


#pragma once

#include "ASTypes.h"

class Registration

{

protected:
	
public:
		
		//	Class Variables
	static	const	ASInt32	sCurrentVersion;
	static	const	char	sSection[];
	
	static	bool			Initialize				(const 	char*		inResPath);
	
	static	bool			IsExpired				(void);
	static	bool			IsRegistered			(void);

	static	bool			TestSerialNumber 		(const	char*		inSerial);
	static	void			RegisterSerialNumber 	(const	char*		inSerial);

	static	bool			DoRegistrationDialog	(ASInt32			inNotYetSecs = 10);
	static	bool			DoPurchaseDialog		(void);
};
