/*
	File:		PhotoWindow.h

	Contains:	Subclass of LWindow with different zoom behavior

	Written by:	Dav Lion

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):
	
	23 Feb 2001		drd		Hack CalcStandardBounds to leave space at bottom
	12 Sep 2000 	dml		created
	
*/

#include "PhotoWindow.h"
#include "MRect.h"
#include "PhotoPrinter.h"
#include "PhotoPrintDoc.h"
#include "PhotoUtility.h"
PhotoWindow::PhotoWindow(LStream* inStream) 
	: LWindow(inStream)
	, mDoc (nil)
{

}//end stream ct


PhotoWindow::~PhotoWindow()
{
}//end dt


/*
* PhotoWindow calculates the "standard" (zoom) bounds differently than LWindow
* for us, the max zoom size is only as big as able to show the full paper size at 72dpi
* plus a tiny amount of grey margin, not entire screen.
*/
Boolean 
PhotoWindow::CalcStandardBounds(Rect& ioDest) const {
	MRect		outDest (ioDest);  // convert to MRect for convenience funcs

	// this is biggest possible window on current display
	LWindow::CalcStandardBounds(outDest);
	
	// !!! This is a quick kludge. We take something out of the bottom to avoid the tool palette,
	// or the OS X dock. We should probably use GetWindowAvailablePositioningBounds, once it's
	// available
	outDest.bottom -= 32;
	
	// and if we are associated with a document, then we can calc based on print record
	if (mDoc != nil) {
		// base our size on the current page's size
		MRect	pageBounds;
		PhotoPrinter::CalculatePrintableRect(mDoc->GetPrintRec(), &(mDoc->GetPrintProperties()), 
											 pageBounds, kDPI);		//all computations at 72dpi

		// now, size the window down based on the printable area
		// ideally, we can show both dimensions at 100%
		if (pageBounds.Height() <= outDest.Height()) {
			if (outDest.Height() - pageBounds.Height() > PhotoPrintDoc::kFeelGoodMargin)
				outDest.SetHeight(pageBounds.Height() + PhotoPrintDoc::kFeelGoodMargin);
			}//endif enough room to show height at 100%

		if (pageBounds.Width() <= outDest.Width()) {
			if (outDest.Width() - pageBounds.Width() > PhotoPrintDoc::kFeelGoodMargin)
				outDest.SetWidth(pageBounds.Width() + PhotoPrintDoc::kFeelGoodMargin);
			}//endif enough room to show width at 100%
		}//endif we are associated with a doc

	ioDest = outDest; // bounce out to Rect

	Rect		contentRect = UWindows::GetWindowContentRect(mMacWindowP);
	return ::MacEqualRect(&ioDest, &contentRect);
}//end CalcStandardBounds