/*
	File:		HotChip.h

	Contains:	Interface to the Hot Chip item.

	Written by:	Richard Wesley

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights Reserved.

*/

#pragma once

#include "adm_item.h"

class HotChip : public ADM::Item
{
	
		//	Data
	ASRGBColor		mColor;
		
		//	Illegal
					HotChip		(const	HotChip&	other);
	HotChip&		operator=	(const	HotChip&	other);

protected:

		//	Event Handling
	virtual void 	Draw		(ADM::Drawer&		drawer);
	virtual void 	Notify		(ADMNotifierRef 	notifier);

public:
	
		//	Construction/Destruction
					HotChip		(ADM::Dialog*		parent,
								 ASInt32 			itemID,
								 const ASRect&		rect,
								 const ASRGBColor*	inColor = 0);
	virtual			~HotChip	(void);
		
		//	Access
	ASRGBColor		GetColor	(void) const {return mColor;};
	void			SetColor	(const	ASRGBColor&	inColor);
	
		//	Event Tracking
	virtual	bool	Track		(ADM::Tracker&		tracker);
};
