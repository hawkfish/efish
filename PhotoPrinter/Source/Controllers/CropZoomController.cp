/*
	File:		CropZoomController.cp

	Contains:	Crop tool controller for Kilt

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		18 Jul 2001		rmgw	Split up ImageActions.
		02 jul 2001		dml		114	clip crop to ImageRect, not DestRect
		29 jun 2001		dml		112  add BodyToScreenCorrection in DoClickItem
		02 mar 2001		dml		cropzoom of rotated images works
		02 mar 2001		dml		stop checking for kClickBoundingLine, bug 21
		30 Aug 2000		drd		Inheritance has changed, so fixed DoClickItem
		25 Aug 2000		drd		ClickEventT now derived from SMouseDownEvent
		23 aug 2000		dml		change signature of MakeCropAction to match super
		22 aug 2000		dml		don't make a copy item in DoClickItem, use ref
		21 aug 2000		dml		whoops, add marquee back in!
		15 Aug 2000		drd		Clicking does marquee (override DoClickItem)
		15 Aug 2000		drd		Created
*/

#include "CropZoomController.h"

#include "CropZoomAction.h"
#include "PhotoPrintDoc.h"
#include "PhotoPrintResources.h"
#include "PhotoPrintView.h"
#include "PhotoPrintCommands.h"

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
	clickEvent.whereLocal = inViewPt;
	this->InterpretClick(clickEvent);
	switch (clickEvent.type) {
		case kClickInsideItem:
		case kClickOnHandle: 
		case kClickBoundingLine: {
			if (clickEvent.target.item == mView->GetPrimarySelection())
				UCursor::SetTheCursor(crossCursor);
			break;
			}//end case item
		default:
			::InitCursor();
			break;
		}//end switch
		
}//end AdjustCursor




/*
HandleClick {OVERRIDE}
*/
void 
CropZoomController::HandleClick(const SMouseDownEvent &inMouseDown, const MRect& inBounds,
								SInt16 inClickCount)
{
	mBounds = inBounds;

	// Build our parameter block -- the first part is just the SMouseDownEvent
	ClickEventT		clickEvent;
	::BlockMoveData(&inMouseDown, &clickEvent, sizeof(SMouseDownEvent));
	// And fill in the rest (analyze what the click represents)
	this->InterpretClick(clickEvent);
	
	switch (clickEvent.type) {
		case kClickInsideItem:
		case kClickOnHandle:
		case kClickBoundingLine: 
			if (inClickCount == 1)
				this->DoClickItem(clickEvent);
			else {
				PhotoPrintDoc*		doc = mView->GetModel()->GetDocument();
				doc->ProcessCommand(cmd_ImageOptions, nil);
				}//else it's a multi-click, bring up the image options dialog
			break;

		case kClickEmpty:
			DoClickEmpty(clickEvent);
			break;

		default:
			break;
	}//end switch
}//end HandleClick



/*
DoClickItem {OVERRIDE}
*/
void 
CropZoomController::DoClickItem(ClickEventT& inEvent)
{
	if (inEvent.target.item == mView->GetPrimarySelection()) {
		Point			last = inEvent.whereLocal;
		PhotoItemRef	image (inEvent.target.item);

		MRect			bounds = image->GetImageRect();
		MatrixRecord bodyToScreenCorrection;
		mView->GetBodyToScreenMatrix(bodyToScreenCorrection);
		::TransformRect(&bodyToScreenCorrection, &bounds, nil);

		double			rot (image->GetRotation());
		double 			skew (image->GetSkew());
		Point			oldMid	= bounds.MidPoint();
		MatrixRecord	mat;

		
		SetupDestMatrix(&mat, rot, skew, oldMid, true);	
		MatrixRecord 	inverse;
		Boolean happy (::InverseMatrix (&mat, &inverse));
		MRect			ants;
		StColorPenState	penState;
		penState.Normalize();
		UMarchingAnts::UseAntsPattern();
		::PenMode (srcXor);
		
		while (::StillDown()) {
			Point		dragged;
			::GetMouse(&dragged);
			if (::EqualPt(last, dragged))
				continue;
			last = dragged;
			
			MRect		rDragged (inEvent.whereLocal, dragged);
			rDragged *= bounds; // clamp to image
			if (rDragged == ants) continue;
			
			ants.Frame();					// Get rid of old one
			ants = rDragged;
			ants.Frame();
		} // while stilldown
		if (!ants.IsEmpty()) {
			PhotoPrintDoc*	doc = mView->GetModel()->GetDocument();

// xform ants by the inverse matrix to bring it to normalized space
			::TransformRect(&inverse, &ants, nil);

			doc->PostAction(this->MakeCropAction(ants));
			}//endif ants isn't empty
		}//endif clicked on the primary selection
	else
		PhotoController::DoClickItem(inEvent);		// call AROUND inherited
}//end DoClickItem


/*
MakeCropAction {OVERRIDE}
*/
LAction*
CropZoomController::MakeCropAction(const MRect&	inNewCrop, const double /*ignored*/, const double /*ignored*/)
{
	PhotoPrintDoc*	doc = mView->GetModel()->GetDocument();
	return new CropZoomAction(doc, si_CropZoom, inNewCrop);
} // MakeCropAction
