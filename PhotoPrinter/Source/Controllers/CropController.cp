/*
	File:		CropController.cp

	Contains:	Crop tool controller for Kilt

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		30 Aug 2000		drd		Changed superclass to PhotoController
		25 Aug 2000		drd		ClickEventT now derived from SMouseDownEvent
		23 aug 2000		dml		move DrawXFormedRect to PhotoController
		23 aug 2000		dml		remember to crop the draw when handing
		23 aug 2000		dml		crop-hand-drag works
		23 Aug 2000		drd		AdjustCursorSelf: removed extra InterpretClick, only show hand
								if we are cropped
		22 aug 2000		dml		allow multiple crops to work
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
#include "PhotoUtility.h"
#include "MNewRegion.h"

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
CropController::~CropController()
{
}//end dt

/*
AdjustCursorSelf {OVERRIDE}
*/
void	
CropController::AdjustCursorSelf(const Point& inViewPt)
{
	// See if we're over an image
	ClickEventT		clickEvent;
	clickEvent.whereLocal = inViewPt;
	// Analyze what the mouse position represents
	this->InterpretClick(clickEvent);
	if (clickEvent.type == kClickInsideItem) {
		if (clickEvent.target.item->HasCrop())
			UCursor::SetTheCursor(curs_Hand);
		else
			::InitCursor();
	} else if (clickEvent.type == kClickOnHandle)
		UCursor::SetTheCursor(curs_Crop);
	else
		::InitCursor();
} // AdjustCursorSelf


//---------------------------------------------------------
// ClampPointBetween
//---------------------------------------------------------
void
CropController::ClampPointBetween(Point& ioClamp, const Point& minPoint, const Point& maxPoint) {
	ioClamp.h = max(ioClamp.h, minPoint.h);
	ioClamp.v = max(ioClamp.v, minPoint.v);
	ioClamp.h = min(ioClamp.h, maxPoint.h);
	ioClamp.v = min(ioClamp.v, maxPoint.v);
	}//end ClampPointBetween



/*
DoClickHandle
	Dragging a handle means crop in that direction
	Assumes that the view is focused
*/
void 
CropController::DoClickHandle(ClickEventT& inEvent)
{
	Point			last = inEvent.whereLocal;
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
		this->DrawHandles(handles, inEvent.target.item->GetRotation());

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
		this->DrawHandles(handles, inEvent.target.item->GetRotation());
	}

	if (!bounds.IsEmpty()) {
	
		// intersect with existing crop rect
		MRect existingCrop;
		image->DeriveCropRect(existingCrop);
		bounds *= existingCrop;
	
		PhotoPrintDoc*	doc = mView->GetModel()->GetDocument();
		doc->PostAction(this->MakeCropAction(bounds));
	}
}//end DoClickHandle



/*
DoClickItem {OVERRIDE}
*/
void 
CropController::DoClickItem(ClickEventT& inEvent)
{
	if (inEvent.target.item == mView->GetPrimarySelection()) {
		PhotoItemRef	image (inEvent.target.item);
		MRect			bounds = image->GetImageRect();
		MRect			offsetExpanded;
		image->GetExpandedOffsetImageRect(offsetExpanded);
		double oldTopOffset;
		double oldLeftOffset;
		image->GetCropZoomOffset(oldTopOffset, oldLeftOffset);
		Point	start = inEvent.whereLocal;
		
		// convert starting point to normalized coordinate system
		MatrixRecord	inverse;
		MatrixRecord	imageMatrix;
		image->GetMatrix(&imageMatrix);
		Boolean inverseAvail (::InverseMatrix (&imageMatrix, &inverse));
    	if (inverseAvail) {
    		::TransformPoints(&inverse, &start, 1);
    		}//endif inverse matrix possible

		double newTopOffset (oldTopOffset);
		double newLeftOffset (oldLeftOffset);

		//figure out what crop rect
		MRect cropRect;
		image->DeriveCropRect(cropRect);

		SInt16 wiggleWidth (offsetExpanded.Width() - cropRect.Width());
		SInt16 wiggleHeight (offsetExpanded.Height() - cropRect.Height());
		
		// compute the farthest points that we can drag (in normal space!)
		// these are the mouse drags which would place topleft of image at topleft of crop
		// and botright of image at botright of crop
		Point minPoint;
		Point maxPoint;
		minPoint.h =   start.h - (offsetExpanded.right - cropRect.right);
		minPoint.v =   start.v - (offsetExpanded.bottom - cropRect.bottom);
		maxPoint.h =   start.h + (cropRect.left - offsetExpanded.left);
		maxPoint.v =   start.v + (cropRect.top - offsetExpanded.top);
				
		StColorPenState		savePen;
		::PenMode(patXor);
		MNewRegion clip;
		clip.Open();
		DrawXformedRect(cropRect, &imageMatrix);
		clip.Close();
		PhotoDrawingProperties	props (kNotPrinting, kPreview, kDraft);

		while (::StillDown()) {
			MRect offsetCrop (cropRect);
			offsetCrop.Offset(newLeftOffset *  offsetExpanded.Width(),
								newTopOffset * offsetExpanded.Height());

			image->SetCropZoomOffset(newTopOffset, newLeftOffset);
			image->Draw(props, 0, 0, 0, clip);

			Point		dragged;
			::GetMouse(&dragged);
			
			// convert from xformed space (where dragged) to normalized space
			if (inverseAvail)
				::TransformPoints(&inverse, &dragged, 1);
			ClampPointBetween(dragged, minPoint, maxPoint);
			
			Point offset;
			offset.h = dragged.h - start.h;
			offset.v = dragged.v - start.v;
		
			// figure out offset as percentage
			newTopOffset = (offset.v / ((double)offsetExpanded.Height() )) + oldTopOffset;
			newLeftOffset = (offset.h / ((double)offsetExpanded.Width() )) + oldLeftOffset;

			offsetCrop = cropRect;
			offsetCrop.Offset(newLeftOffset  * offsetExpanded.Width(),
								newTopOffset  * offsetExpanded.Height());
			image->SetCropZoomOffset(newTopOffset, newLeftOffset);
			image->Draw(props, 0, 0, 0, clip);						
			} // while stilldown
		
		//RESTORE the image's offsets
		image->SetCropZoomOffset(oldTopOffset,  oldLeftOffset);
		if (!(PhotoUtility::DoubleEqual(oldTopOffset, newTopOffset) &&
			PhotoUtility::DoubleEqual(oldLeftOffset, newLeftOffset))) {
			PhotoPrintDoc*	doc = mView->GetModel()->GetDocument();
			doc->PostAction(this->MakeCropAction(cropRect, newTopOffset, newLeftOffset));
			}//endif ants isn't empty
		}//endif clicked on the primary selection

	else
		PhotoController::DoClickItem(inEvent);		// Call inherited
}//end DoClickItem


/*
HandleClick {OVERRIDE}
*/
void 
CropController::HandleClick(const SMouseDownEvent &inMouseDown, const MRect& inBounds)
{
	mBounds = inBounds;

	// Build our parameter block -- the first part is just the SMouseDownEvent
	ClickEventT		clickEvent;
	::BlockMoveData(&inMouseDown, &clickEvent, sizeof(SMouseDownEvent));
	// And fill in the rest (analyze what the click represents)
	this->InterpretClick(clickEvent);
	
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
CropController::MakeCropAction(const MRect&	inNewCrop,
								const double inNewTopOffset,
								const double inNewLeftOffset)
{
	PhotoPrintDoc*	doc = mView->GetModel()->GetDocument();
	return new CropAction(doc, si_Crop, inNewCrop, inNewTopOffset, inNewLeftOffset);
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
