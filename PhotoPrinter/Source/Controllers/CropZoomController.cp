/*
	File:		CropZoomController.cp

	Contains:	Crop tool controller for Kilt

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		15 Aug 2000		drd		Clicking does marquee (override DoClickItem)
		15 Aug 2000		drd		Created
*/

#include "CropZoomController.h"
#include "ImageActions.h"
#include "PhotoPrintDoc.h"
#include "PhotoPrintResources.h"
#include "PhotoPrintView.h"

/*
CropZoomController
*/
CropZoomController::CropZoomController(PhotoPrintView* inView)
	: CropController(inView)
{
}//end ct

/*
~CropController
*/
CropZoomController::~CropZoomController()
{
}//end dt

/*
AdjustCursor {OVERRIDE}
*/
void	
CropZoomController::AdjustCursorSelf(const Point& inViewPt)
{
	// See if we're over an image
	ClickEventT		clickEvent;
	clickEvent.where = inViewPt;
	this->InterpretClick(clickEvent);
	if (clickEvent.type == kClickInsideItem) {
		if (clickEvent.target.item == mView->GetPrimarySelection())
			UCursor::SetTheCursor(crossCursor);
		else
			::InitCursor();
	} else if (clickEvent.type == kClickOnHandle)
		UCursor::SetTheCursor(curs_Crop);	// !!! probably want different one
	else
		::InitCursor();
}//end AdjustCursor



/*
MakeCropAction {OVERRIDE}
*/
LAction*
CropZoomController::MakeCropAction(const MRect&	inNewCrop)
{
	PhotoPrintDoc*	doc = mView->GetModel()->GetDocument();
	return new CropZoomAction(doc, si_CropZoom, inNewCrop);
} // MakeCropAction
