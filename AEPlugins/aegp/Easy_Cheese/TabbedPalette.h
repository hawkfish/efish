/*
	File:		TabbedPalette.h

	Contains:	Interface to a Tabbed Palette dialog.

	Written by:	Richard Wesley

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights Reserved.

*/

#pragma once

#include "adm_dialog.h"

class TabbedPalette : public ADM::Dialog

{
	
		//	Data
		
		//	Illegal
					TabbedPalette	(const	TabbedPalette&	other);
	TabbedPalette&	operator=		(const	TabbedPalette&	other);
		
public:
	
		//	Constants
	static	const	char	sTabGroupName [];
	
		//	Construction/Destruction
					TabbedPalette	(const std::string&	name,
									 ASInt32 			id = 0, 
									 ASInt32 			options = 0);
	virtual			~TabbedPalette	(void);
};
