/*
	File:		CropController.cp

	Contains:	Crop tool controller for Kilt

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		21 aug 2000		dml		make work with rotation (based on old PhotoPrintController)
		21 aug 2000		dml		handle scrolled view
		15 Aug 2000		drd		Added factory method MakeCropAction; clicking selects
		14 Aug 2000		drd		First cut at cropping
		11 Aug 2000		drd		Created
*/

#include "CropController.h"
#include "ImageActions.h"
#include "PhotoPrintDoc.h"
#include "PhotoPrintResources.h"
#include "PhotoPrintView.h"

/*
CropController
*/
CropController::CropController(PhotoPrintView* inView)
	: ArrowController(inView)
{
}//end ct

/*
~CropController
*/
CropController::~CropController()
{
}//end dt

/*
AdjustCursor
*/
void	
CropController::AdjustCursorSelf(const Point& inViewPt)
{
	// See if we're over an image
	ClickEventT		clickEvent;
	clickEvent.where = inViewPt;

	InterpretClick(clickEvent);
	this->InterpretClick(clickEvent);
	if (clickEvent.type == kClickInsideItem)
		UCursor::SetTheCursor(curs_Hand);
	else if (clickEvent.type == kClickOnHandle)
		UCursor::SetTheCursor(curs_Crop);
	else
		::InitCursor();
}//end AdjustCursor


/*
DoClickHandle
	Dragging a handle means crop in that direction
	Assumes that the view is focused
*/
void 
CropController::DoClickHandle(ClickEventT& inEvent)
{
	Point			last = inEvent.where;
	PhotoItemRef	image (inEvent.target.item);
	MRect			bounds = image->GetDestRect();
	double			rot (image->GetRotation());
	double 			skew (image->GetSkew());
	Point			oldMid	= bounds.MidPoint();
	MatrixRecord	mat;
	HandlesT		handles;

	SetupDestMatrix(&mat, rot, skew, oldMid, true);	
	StColorPenState		savePen;
	::PenMode(patXor);

	while (::StillDown()) {
		Point		dragged;
		::GetMouse(&dragged);
		if (::EqualPt(last, dragged))
			continue;
		last = dragged;
		
		// compute and draw the handles
		RecalcHandlesForDestMatrix(handles, bounds, &mat);		
		this->DrawHandles(handles);

		// xform the point by the inverse of the current matrix
		MatrixRecord 	inverse;
		Boolean happy (::InverseMatrix (&mat, &inverse));
		if (happy) {
			::TransformPoints(&inverse, &dragged, 1);
			}//endif able to transform
				
		// based on which handle, and the xformed point, update the rect
		UpdateDraggedRect(inEvent.target.handle,
						bounds,
						dragged);
	
		//compute and draw the handles
		RecalcHandlesForDestMatrix(handles, bounds, &mat);
		this->DrawHandles(handles);
	}

	if (!bounds.IsEmpty()) {
		PhotoPrintDoc*	doc = mView->GetModel()->GetDocument();
		doc->PostAction(this->MakeCropAction(bounds));
	}
}//end DoClickHandle

/*
HandleClick {OVERRIDE}
*/
void 
CropController::HandleClick(const SMouseDownEvent &inMouseDown, const MRect& inBounds)
{
	mBounds = inBounds;
	ClickEventT clickEvent;
	clickEvent.where = inMouseDown.whereLocal;
	InterpretClick(clickEvent);
	
	switch (clickEvent.type) {
		case kClickInsideItem:
			this->DoClickItem(clickEvent);
			break;

		case kClickOnHandle:
			this->DoClickHandle(clickEvent);
			break;

		case kClickEmpty:
			DoClickEmpty(clickEvent);
			break;

		default:
			break;
	}//end switch
}//end HandleClick

/*
MakeCropAction
*/
LAction*
CropController::MakeCropAction(const MRect&	inNewCrop)
{
	PhotoPrintDoc*	doc = mView->GetModel()->GetDocument();
	return new CropAction(doc, si_Crop, inNewCrop);
} // MakeCropAction


//---------------------------------------
// UpdateDraggedRect
// one of the handles has been moved.  figure out what the bounding rect is
//---------------------------------------

void
CropController::UpdateDraggedRect(const HandleType& handle,
								MRect& bounds,
								const Point& dragged) {
	
	switch (handle) {
		case kTopLeft:
			bounds.top = dragged.v;
			bounds.left = dragged.h;
			break;

		case kTopMid:
			bounds.top = dragged.v;
			break;

		case kTopRight:
			bounds.top = dragged.v;
			bounds.right = dragged.h;
			break;

		case kMidLeft:
			bounds.left = dragged.h;
			break;

		case kMidRight:
			bounds.right = dragged.h;
			break;

		case kBotLeft:
			bounds.bottom = dragged.v;
			bounds.left = dragged.h;
			break;

		case kBotMid:
			bounds.bottom = dragged.v;
			break;

		case kBotRight:
			bounds.bottom = dragged.v;
			bounds.right = dragged.h;
			break;
	}//end switch
}//end UpdateDraggedRect
