/*
	File:		ColorPal.h

	Contains:	Interface to the ColorPal plugin class.

	Written by:	Richard Wesley

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights Reserved.

*/

#pragma once

#include "adm.h"

#include "AE_GeneralPlug.h"

#include <string>

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
	
		//	Construction/Destruction
					ColorPal				(SPBasicSuite*		inSP, 
											 AEGP_PluginID		inPluginID,
											 const A_char*		inResPath);
	virtual 		~ColorPal				(void);
		
		//	Access
	ColorPalette*	GetPalette				(void) const {return mDialog;};
		
		//	Hooks
	void			Command					(void);
	bool			UpdateMenu				(void);
};