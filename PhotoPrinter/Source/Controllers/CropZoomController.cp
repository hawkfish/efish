/*
	File:		CropZoomController.cp

	Contains:	Crop tool controller for Kilt

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

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
CropZoomController::AdjustCursor(const Point& inPortPt)
{
	// See if we're over an image
	ClickEventT		clickEvent;
	clickEvent.where = inPortPt;
	this->InterpretClick(clickEvent);
	if (clickEvent.type == kClickInsideItem)
		UCursor::SetTheCursor(curs_Hand);
	else if (clickEvent.type == kClickOnHandle)
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
