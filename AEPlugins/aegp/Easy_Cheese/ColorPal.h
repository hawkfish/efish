/*
	File:		ColorPal.h

	Contains:	Interface to the ColorPal plugin class.

	Written by:	Richard Wesley

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights Reserved.

*/

#pragma once

#include "adm.h"

#include "AE_GeneralPlug.h"

class ColorPalette;

class ColorPal : public ADM::Host

{
		//	Data
	AEGP_PluginID 	mPluginID;
	ColorPalette*	mDialog;
		
		//	Illegal
					ColorPal				(const	ColorPal&	other);
	ColorPal&		operator=				(const	ColorPal&	other);
		
public:
	
		//	Class Variables
	static	const	ASInt32					sCurrentVersion;
	static	const	char					sSection[];
	
		//	Construction/Destruction
					ColorPal				(SPBasicSuite*		inSP, 
											 AEGP_PluginID		inPluginID);
	virtual 		~ColorPal				(void);
		
		//	Access
	ColorPalette*	GetPalette				(void) const {return mDialog;};
		
		//	Registration
	bool			IsExpired				(void);
	bool			IsRegistered			(void);
	void			RegisterSerialNumber	(const	char*		inSerial);
	bool			DoRegistrationDialog	(ASInt32			inNotYetSecs = 10);

		//	Hooks
	void			Command					(void);
	bool			UpdateMenu				(void);
};