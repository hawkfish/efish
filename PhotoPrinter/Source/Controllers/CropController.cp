/*
	File:		CropController.cp

	Contains:	Crop tool controller for Kilt

	Written by:	David Dunham and Dav Lion

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		24 Jul 2001		drd		127 DoClickHandle erases old handles (and uses antsy)
		23 Jul 2001		rmgw	Get document from view.
		18 Jul 2001		rmgw	Split up ImageActions.
		05 jul 2001		dml		68.  fix crop on drag
		05 jul 2001		dml		120.  don't install an empty-crop region even if rect was valid
		02 jul 2001		dml		57.  Set resolution of DrawProperties in DoClickItem loop
		28 mar 2001		dml		use GetBodyToScreenMatrix
		02 mar 2001		dml		move inversematrix out of loop
		26 feb 2001		dml		cleanup for refactoring, make GetMatrix use explicit desires
		05 Oct 2000		drd		Added using for min, max
		22 Sep 2000		drd		DrawXformedRect is now in PhotoUtility
		18 Sep 2000		drd		Avoid warning by including ESpinCursor.h (why?)
		31 aug 2000		dml		cropping via handle must preserve offsets!
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

#include "CropAction.h"
#include "PhotoPrintDoc.h"
#include "PhotoPrintResources.h"
#include "PhotoPrintView.h"
#include "PhotoUtility.h"
#include "MNewRegion.h"
#include "PhotoPrintCommands.h"

using std::max;
using std::min;

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
	MatrixRecord 	inverse;
	Boolean happy (::InverseMatrix (&mat, &inverse));

	// Get rid of original handles
	this->CalculateHandlesForItem(inEvent.target.item, handles);
	this->DrawHandles(handles, inEvent.target.item->GetRotation());

	// Draw new ones
	this->RecalcHandlesForDestMatrix(handles, bounds, &mat);		
	this->DrawHandles(handles, inEvent.target.item->GetRotation(), kMarchingAnts);

	while (::StillDown()) {
		Point		dragged;
		::GetMouse(&dragged);
		if (::EqualPt(last, dragged))
			continue;
		last = dragged;
		
		// Undraw the handles
		this->DrawHandles(handles, inEvent.target.item->GetRotation(), kMarchingAnts);

		// xform the point by the inverse of the current matrix
		if (happy) {
			::TransformPoints(&inverse, &dragged, 1);
			}//endif able to transform
				
		// based on which handle, and the xformed point, update the rect
		UpdateDraggedRect(inEvent.target.handle,
						bounds,
						dragged);
	
		//compute and draw the handles
		RecalcHandlesForDestMatrix(handles, bounds, &mat);
		this->DrawHandles(handles, inEvent.target.item->GetRotation(), kMarchingAnts);
	}

	bool forceRefreshToCleanupDrag (true);
	if (!bounds.IsEmpty()) {
	
		// intersect with existing crop rect
		MRect existingCrop;
		image->DeriveCropRect(existingCrop);
		bounds *= existingCrop;
	
		if (bounds) {
			PhotoPrintDoc*	doc = mView->GetDocument();
			double oldTopOffset;
			double oldLeftOffset;
			image->GetCropZoomOffset(oldTopOffset, oldLeftOffset);
			doc->PostAction(this->MakeCropAction(bounds, oldTopOffset, oldLeftOffset));
			forceRefreshToCleanupDrag = false;
			}//endif bounds aren't empty
	}

	if (forceRefreshToCleanupDrag) 
		this->DrawHandles(handles, inEvent.target.item->GetRotation());

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
		image->GetMatrix(&imageMatrix, kDoScale, kDoRotation);
		Boolean inverseAvail (::InverseMatrix (&imageMatrix, &inverse));
    	if (inverseAvail) {
    		::TransformPoints(&inverse, &start, 1);
    		}//endif inverse matrix possible

		double newTopOffset (oldTopOffset);
		double newLeftOffset (oldLeftOffset);

		//figure out what crop rect
		MRect cropRect;
		image->DeriveCropRect(cropRect);
		
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

		MRect viewRevealed;
		mView->CalcRevealedRect();
		mView->GetRevealedRect(viewRevealed);
		Point viewOrigin;
		mView->GetPortOrigin(viewOrigin);
		viewRevealed.Offset(viewOrigin.h, viewOrigin.v);
		
		MNewRegion			clip;
		clip = viewRevealed;

		PhotoDrawingProperties	props (kNotPrinting, kPreview, kDraft, 
										mView->GetDocument()->GetResolution());

		// inside this loop we will draw to the screen
		// that will require a call to GetBodyToScreenMatrix
		MatrixRecord bodyToScreenCorrection;
		mView->GetBodyToScreenMatrix(bodyToScreenCorrection);

		while (::StillDown()) {
			MRect offsetCrop (cropRect);
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
			image->Draw(props, &bodyToScreenCorrection, 0, 0, clip);						
			} // while stilldown
		
		//RESTORE the image's offsets
		image->SetCropZoomOffset(oldTopOffset,  oldLeftOffset);
		if (!(PhotoUtility::DoubleEqual(oldTopOffset, newTopOffset) &&
			PhotoUtility::DoubleEqual(oldLeftOffset, newLeftOffset))) {
			PhotoPrintDoc*	doc = mView->GetDocument();
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
CropController::HandleClick(const SMouseDownEvent &inMouseDown, const MRect& inBounds,
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
			if (inClickCount == 1)
				this->DoClickItem(clickEvent);
			else {
				PhotoPrintDoc*		doc = mView->GetDocument();
				doc->ProcessCommand(cmd_ImageOptions, nil);
				}//else it's a multi-click, bring up the image options dialog
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
	PhotoPrintDoc*	doc = mView->GetDocument();
	return new CropAction(doc, si_Crop, inNewCrop, inNewTopOffset, inNewLeftOffset);
} // MakeCropAction


//---------------------------------------
// UpdateDraggedRect
// one of the handles has been moved.  figure out what the bounding rect is
//---------------------------------------

void
CropController::UpdateDraggedRect(const HandleType& handle,
								MRect& bounds,
								const Point& dragged)
{	
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
