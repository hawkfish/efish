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
CropZoomController::AdjustCursor(const Point& inPortPt)
{
	// See if we're over an image
	ClickEventT		clickEvent;
	clickEvent.where = inPortPt;
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
DoClickItem {OVERRIDE}
*/
void 
CropZoomController::DoClickItem(ClickEventT& inEvent)
{
	bool				handled = false;
	if (inEvent.target.item == mView->GetPrimarySelection()) {
		PhotoPrintItem	image(*inEvent.target.item);
		MRect			bounds = image.GetDestRect();


		MRect			ants;
		StColorPenState	penState;
		penState.Normalize();
		UMarchingAnts::UseAntsPattern();
		::PenMode (srcXor);
		
		while (::StillDown()) {
			Point		dragged;
			::GetMouse(&dragged);
			
			MRect		rDragged (inEvent.where, dragged);
			rDragged *= bounds; // clamp to image
			if (rDragged == ants) continue;
			
			ants.Frame();					// Get rid of old one
			ants = rDragged;
			ants.Frame();
		} // while stilldown
		if (!ants.IsEmpty()) {
			PhotoPrintDoc*	doc = mView->GetModel()->GetDocument();
			doc->PostAction(this->MakeCropAction(ants));
		}
	}

	if (!handled)
		CropController::DoClickItem(inEvent);
}//end DoClickItem


/*
MakeCropAction {OVERRIDE}
*/
LAction*
CropZoomController::MakeCropAction(const MRect&	inNewCrop)
{
	PhotoPrintDoc*	doc = mView->GetModel()->GetDocument();
	return new CropZoomAction(doc, si_CropZoom, inNewCrop);
} // MakeCropAction
