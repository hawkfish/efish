/*
	File:		PopupBevelButton.h

	Contains:	Definition of a bevel button which acts more like a popup menu.

	Written by:	David Dunham

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		21 Mar 2001		drd		Created
*/

#pragma once

#include <LBevelButton.h>

class	Layout;

class PopupBevelButton : public LBevelButton
{
public:
	enum { class_ID = FOUR_CHAR_CODE('PBBt') };

						PopupBevelButton	(LStream*	inStream);
	virtual				~PopupBevelButton	(void);

	// LControl
	virtual void		HotSpotResult( SInt16 inHotSpot );

	// LMenuController
	virtual void		SetCurrentMenuItem(SInt16 inItem);
};
