/*
	File:		TransparentGrafPortView.h

	Contains:	An LGrafPortView which doesn't erase its background

	Written by:	David Dunham

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

	07 Aug 01	drd		Created
*/

#pragma once

#include <LGrafPortView.h>

class TransparentGrafPortView : public LGrafPortView
{
	typedef	LGrafPortView	inherited;

public:
	enum { class_ID = FOUR_CHAR_CODE('Tgvw') };

					TransparentGrafPortView(LStream* inStream);
	virtual			~TransparentGrafPortView();

	virtual	void	DrawSelf();
	virtual void	UpdatePort();

			void	SetBackgroundBrush(const ThemeBrush inBrush)	{ fBackgroundBrush = inBrush; }

protected:
	ThemeBrush		fBackgroundBrush;
};
