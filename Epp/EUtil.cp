/*
	File:		EUtil.cp

	Contains:	utility routines

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

	12 Jul 2000		drd		SizeFromMenu
	12 jul 2000		dml		added ERect32 version of FitRectInside
*/

#include "EUtil.h"
#include <algorithm.h>

//-----------------------------------------------------
//BestFit
//-----------------------------------------------------
void 
EUtil::BestFit 		(	SInt32&	outWidth, 
						SInt32&	outHeight,
						const	SInt32		boundingWidth,
						const	SInt32		boundingHeight,
						const	SInt32		objectWidth,
						const	SInt32		objectHeight,
						const	bool 		okToExpand)

	{ // begin BestFit
		
	do {	
		if (objectWidth > 0) {
			outWidth = okToExpand ? max(boundingWidth, objectWidth) : min(objectWidth, boundingWidth);
			outHeight = (outWidth *  objectHeight ) / objectWidth;
			
			if (outHeight <= boundingHeight ) 
				break;
			} // if
		
		if (objectHeight > 0) {
			outHeight = okToExpand ? max(boundingHeight, objectHeight) : min(boundingHeight, objectHeight) ;
			outWidth = (outHeight *  objectWidth ) / objectHeight ;
			break;
			} // if
			
		outWidth = outHeight = 0;
		} while (false);
		
		
	} // end BestFit
	
	
void
EUtil::FitRectInside(const MRect& target,
					  const MRect& bounding,
					  MRect& outDestRect,
					  const bool okToExpand) {

	SInt32 bestWidth;
	SInt32 bestHeight;
	
	EUtil::BestFit(bestWidth, 
					bestHeight,
					bounding.Width(),
					bounding.Height(),
					target.Width(),
					target.Height(),
					okToExpand);

	outDestRect.SetWidth(bestWidth);
	outDestRect.SetHeight(bestHeight);
}// end FitRectInside


void
EUtil::FitRectInside(const ERect32& target,
					  const ERect32& bounding,
					  ERect32& outDestRect,
					  const bool okToExpand) {

	SInt32 bestWidth;
	SInt32 bestHeight;
	
	EUtil::BestFit(bestWidth, 
					bestHeight,
					bounding.Width(),
					bounding.Height(),
					target.Width(),
					target.Height(),
					okToExpand);

	outDestRect.SetWidth(bestWidth);
	outDestRect.SetHeight(bestHeight);
}// end FitRectInside

/*
SizeFromMenu
	Given a menu and item, return the font size. This assumes the menu reads
	Ò9 pointÓ Ñ donÕt use it for the Other item.
*/
SInt16
EUtil::SizeFromMenu(const SInt16 inMenuItem, MenuHandle inMenu)
{
	Str255		theMenuText;
	int			i;

	// Get the menu handle if we need to
	// ??? might want to optimize this in a global, as in PensŽeUtil
	if (inMenu == nil) {
		LMenu *		theMenu = LMenuBar::GetCurrentMenuBar()->FetchMenu(MENU_Size);
		inMenu = theMenu->GetMacMenuH();
		Assert_(inMenu != nil);
	}

	// Get the menu item text.
	::GetMenuItemText(inMenu, inMenuItem, theMenuText);

	// Get the size referred to by the menu item.
	// First, truncate the string at a space
	for (i = 1; i < theMenuText[0]; i++) {
		if (theMenuText[i] == ' ') {
			theMenuText[0] = i - 1;
			break;
		}
	}
	SInt32	theSize;
	::StringToNum(theMenuText, &theSize);

	return theSize;
} // SizeFromMenu
