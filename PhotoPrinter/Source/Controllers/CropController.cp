/*
	File:		CropController.cp

	Contains:	Crop tool controller for Kilt

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		11 Aug 2000		drd		Created
*/

#include "CropController.h"
#include "PhotoPrintResources.h"
#include "PhotoPrintView.h"

/*
CropController
*/
CropController::CropController(PhotoPrintView* inView)
	: PhotoController(inView)
{
}//end ct

/*
~CropController
*/
CropController::~CropController() {
}//end dt

/*
AdjustCursor
*/
void	
CropController::AdjustCursor(const Point& inPortPt) {
	// See if we're over an image
	ClickEventT		clickEvent;
	clickEvent.where = inPortPt;
	this->InterpretClick(clickEvent);
	if (clickEvent.type == kClickInsideItem)
		UCursor::SetTheCursor(curs_Hand);
	else if (clickEvent.type == kClickOnHandle)
		UCursor::SetTheCursor(curs_Crop);
	else
		::InitCursor();
}//end AdjustCursor
	
void 
CropController::DoClickItem(ClickEventT& inEvent)
{
	// Scroll cropped image
	PhotoPrintItem*		image = inEvent.target.item;
}//end DoClickItem


void 
CropController::DoClickHandle(ClickEventT& /*inEvent*/) {
}//end DoClickHandle

/*
HandleClick {OVERRIDE}
*/
void 
CropController::HandleClick(const SMouseDownEvent &inMouseDown, const MRect& inBounds) {
	mBounds = inBounds;
	ClickEventT clickEvent;
	clickEvent.where = inMouseDown.whereLocal;
	this->InterpretClick(clickEvent);
	
	switch (clickEvent.type) {
		case kClickInsideItem:
			this->DoClickItem(clickEvent);
			break;

		case kClickOnHandle:
			this->DoClickHandle(clickEvent);
			break;

		default:
			break;
	}//end switch
	
}//end HandleClick
