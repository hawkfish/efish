/*
	File:		EUtil.cp

	Contains:	utility routines

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

	25 aug 2000		dml		fix BringFinderToFront
	17 Aug 2000		rmgw	Use MPSN in BringFinderToFront.
	14 Aug 2000		drd		BringFinderToFront
	20 Jul 2000		drd		AlignToScreen, GetMonitorRect account for menu bar
	18 Jul 2000		drd		AlignToScreen, GetMonitorRect
	12 Jul 2000		drd		SizeFromMenu
	12 jul 2000		dml		added ERect32 version of FitRectInside
*/

#include "EUtil.h"

#include "MNewRegion.h"
#include "MProcesses.h"

#include <UWindows.h>

#include <Icons.h>							// Has alignment constants

#include <algorithm.h>

SInt16		EUtil::gScreenInset = kDefaultScreenInset;

/*
AlignToScreen
	!!! To do: center alignments
*/
void
EUtil::AlignToScreen(LWindow* inWindow, const AlignmentType inAlign)
{
	MRect		monitorRect;
	GetMonitorRect(inWindow->GetMacWindow(), monitorRect, kSubtractMenuBar);

	MNewRegion	structRgn;
	::GetWindowRegion(inWindow->GetMacWindow(), kWindowStructureRgn, structRgn);
	MRect		outerRect(structRgn.Bounds());
	MRect		innerRect;
	inWindow->GetGlobalBounds(innerRect);
	SInt16		newTop = innerRect.top;
	SInt16		newLeft = innerRect.left;
	MRect		border;
	border.left = innerRect.left - outerRect.left;
	border.top = innerRect.top - outerRect.top;
	border.right = outerRect.right - innerRect.right;
	border.bottom = outerRect.bottom - innerRect.bottom;

	// Determine top
	switch (inAlign) {
		case kAlignTop:
		case kAlignCenterTop:
		case kAlignTopLeft:
		case kAlignTopRight:
			newTop = monitorRect.top + gScreenInset + border.top;
			break;

		case kAlignBottom:
		case kAlignCenterBottom:
		case kAlignBottomLeft:
		case kAlignBottomRight:
			newTop = monitorRect.bottom - gScreenInset - innerRect.Height() - border.bottom;
			break;
	}

	// Determine left
	switch (inAlign) {
		case kAlignLeft:
		case kAlignCenterLeft:
		case kAlignTopLeft:
		case kAlignBottomLeft:
			newLeft = monitorRect.left + gScreenInset + border.left;
			break;

		case kAlignRight:
		case kAlignCenterRight:
		case kAlignTopRight:
		case kAlignBottomRight:
			newLeft = monitorRect.right - gScreenInset - innerRect.Width() - border.right;
			break;
	}

	inWindow->MoveWindowTo(newLeft, newTop);
} // AlignToScreen

//-----------------------------------------------------
// BestFit
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
	
/*
BringFinderToFront
	From "Using the Mac OS 8.5 Window Manager" [Listing 2-6]
	Will throw (procNotFound) if Finder is not running
*/
void	EUtil::BringFinderToFront(void)
{
	const OSType			kFinderSignature = 'MACS';
	const OSType			kFinderType = 'FNDR';

	for (MPSN psn; psn.Next ();) {
		MProcessInfo	info (psn);
		if (info.Signature () != kFinderSignature) continue;
		if (info.Type () != kFinderType) continue;
		
		psn.SetFront ();
		return;
		} // for
	
	Throw_(procNotFound);
	
} // BringFinderToFront

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
GetMonitorRect
	Determines which monitor contains the greatest portion of the passed-in window
	and returns the rect describing that device.

	Original by Mike Shields as part of UWindowStagger
*/
void
EUtil::GetMonitorRect(WindowPtr inWindow, Rect& outMonitorRect, const bool inMenuBar)
{
	GDHandle		monitor;
	
	if (inWindow == nil) {
		// if there are no open windows, use the main monitor
		monitor = ::GetMainDevice();
	} else {
		// otherwise, find out which monitor contains the most of the frontmost 
		// window and then get info for it
		monitor = UWindows::FindDominantDevice(UWindows::GetWindowContentRect(inWindow));
		if (monitor == nil) {
			// This window isn't on ANY monitor, so just use the main one.
			monitor = ::GetMainDevice();
		}
	}
	SignalIfNot_(monitor);
	outMonitorRect = (**monitor).gdRect;	// ??? will this become opaque?

	if (inMenuBar == kSubtractMenuBar && monitor == ::GetMainDevice()) {
		outMonitorRect.top += ::GetMBarHeight();
	}
} // GetMonitorRect

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
