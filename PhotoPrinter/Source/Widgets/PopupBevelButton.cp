/*
	File:		PopupBevelButton.cp

	Contains:	Implementation of a bevel button which acts more like a popup menu.

	Written by:	David Dunham

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		22 May 2001		drd		SetCurrentMenuItem maintains checkmark
		27 Mar 2001		drd		Rearranged to avoid compiler warning
		21 Mar 2001		drd		Created
*/

#include "PopupBevelButton.h"
#include <Menus.h>

/*
PopupBevelButton
*/
PopupBevelButton::PopupBevelButton	(LStream*	inStream)
	: LBevelButton(inStream)
{
} // PopupBevelButton

/*
~PopupBevelButton
*/
PopupBevelButton::~PopupBevelButton	(void)
{
} // ~PopupBevelButton

/*
HotSpotResult {OVERRIDE}
	Respond to a click in a BevelButton
*/
void
PopupBevelButton::HotSpotResult(SInt16 inHotSpot)
{
	LBevelButton::HotSpotResult(inHotSpot);

	// Unfortunately, the inherited method simply sets mMenuChoice, rather than using
	// an accessor. So we use the accessor
	switch (mBehavior) {
		case kControlBehaviorPopup:
			this->SetCurrentMenuItem(mMenuChoice);
			break;
	}
} // HotSpotResult

/*
SetCurrentMenuItem {OVERRIDE}
	Be sure the icon matches the menu, and that the correct item is checked in the menu
*/
void
PopupBevelButton::SetCurrentMenuItem(SInt16 inItem)
{
	LBevelButton::SetCurrentMenuItem(inItem);

	MenuRef			menu = this->GetMacMenuH();
	SInt16			nItems = ::CountMenuItems(menu);
	SInt16			index;
	for (index = 1; index <= nItems; index++) {
		char		mark = ' ';
		if (index == inItem)
			mark = checkMark;
		::SetItemMark(menu, index, mark);
	}

	// Update our icon
	::GetItemIcon(menu, inItem, &index);
	if (index != 0) {
		ControlButtonContentInfo	ci;
		ci.contentType = kControlContentCIconRes;
		ci.u.resID = index + 256;
		this->SetContentInfo(ci);
	}

#ifdef MORE_API
	UInt8			iconType;
	Handle			icon;
	OSErr			err = ::GetMenuItemIconHandle(menu, inItem, &iconType, &icon);
	ThrowIfOSErr_(err);
	if (iconType != kMenuNoIcon) {
		ControlButtonContentInfo	ci;
		ci.u.ICONHandle = icon;
		this->SetContentInfo(ci);
	}
#endif
} // SetCurrentMenuItem
