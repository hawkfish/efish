/*
	File:		RotateController.cp

	Contains:	rotation controller for kilt

	Written by:	dav lion

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

	15 aug 2000		dml		created
*/


#include "RotateController.h"
#include "PhotoPrintView.h"
#include "PhotoPrintResources.h"
#include "PhotoUtility.h"
#include "PhotoPrintDoc.h"
#include "ImageActions.h"	
/*
RotateController
*/
RotateController::RotateController(PhotoPrintView* inView)
	: ArrowController(inView)
{
}//end ct

/*
~RotateController
*/
RotateController::~RotateController()
{
}//end dt

/*
AdjustCursor
*/
void	
RotateController::AdjustCursorSelf(const Point& inViewPt)
{
	// See if we're over an image
	ClickEventT		clickEvent;
	clickEvent.where = inViewPt;

	this->InterpretClick(clickEvent);
	if (clickEvent.type == kClickInsideItem)
		UCursor::SetTheCursor(curs_Hand);
	else if (clickEvent.type == kClickOnHandle)
		UCursor::SetTheCursor(curs_Rot);
	else
		::InitCursor();
}//end AdjustCursor



void
RotateController::DoRotate(ClickEventT& inEvent) {
	StColorPenState	penState;
	penState.Normalize ();
	UMarchingAnts::UseAntsPattern ();
	::PenMode (srcXor);
	
	MatrixRecord	mat;
	double	startingRot (inEvent.target.item->GetRotation());
	double	skew (inEvent.target.item->GetSkew());
	Point	oldMid (inEvent.target.item->GetDestRect().MidPoint());
	MRect	dest (inEvent.target.item->GetDestRect());
	Point	start (inEvent.where);
	Point	last (start);
	BoundingLineType	whichLine (inEvent.target.boundingLine);
	// if we're inside the midline (either one) then movement towards interior of bounds
	// is unchanged (negative == ccw rotation).  if however, we're on RHand portion of boundingline
	// (as determined by "outside" midline, then movement towards interior is positive == CW rot
	HandlesT handles;
	CalculateHandlesForItem(inEvent.target.item, handles);
	double rotationMultiplier (PointInsideMidline(start, handles, whichLine) ? 1.0 : -1.0);

	Point startPoint;
	Point endPoint;
	bool inside (true);
	GetRotationSegment(whichLine, handles, startPoint, endPoint);

	double	rot (0);
	while (::StillDown ()) {
		Point	dragged;
		::GetMouse (&dragged);
		if (::EqualPt(last, dragged))
			continue;

		double howFar = PointLineDistance(dragged, startPoint, endPoint, inside);

		SetupDestMatrix(&mat, rot + startingRot, skew, oldMid, true);
		RecalcHandlesForDestMatrix(handles, dest, &mat);
		DrawHandles(handles);

		rot = RotFromPointLine(dragged, startPoint, endPoint);
		rot *= rotationMultiplier;

		mView->AdjustTransforms(rot, skew, dest, inEvent.target.item);

		SetupDestMatrix(&mat, rot + startingRot, skew, oldMid, true);
		RecalcHandlesForDestMatrix(handles, dest, &mat);
		DrawHandles(handles);
		last = dragged;
		}//end while stilldown

	if (!PhotoUtility::DoubleEqual(rot,startingRot)) {
		PhotoPrintDoc*	doc = mView->GetModel()->GetDocument();
		doc->PostAction(this->MakeRotateAction(rot + startingRot));
		}//endif rotation changed

	}//end DoRotate


	
/*
DoClickBoundingLine
*/
void 
RotateController::DoClickBoundingLine(ClickEventT& inEvent)
{
	DoRotate(inEvent);
}//end DoClickBoundingLine


/*
DoClickHandle
*/
void 
RotateController::DoClickHandle(ClickEventT& inEvent) {
	DoRotate(inEvent);
}//end DoClickHandle


/*
HandleClick {OVERRIDE}
	Main dispatch of clicks
*/
void 
RotateController::HandleClick(const SMouseDownEvent &inMouseDown, const MRect& inBounds) {
	mBounds = inBounds;
	ClickEventT clickEvent;
	clickEvent.where = inMouseDown.whereLocal;
	InterpretClick(clickEvent);
	
	switch (clickEvent.type) {
		case kClickOnHandle:
			DoClickHandle(clickEvent);
			break;

		case kClickBoundingLine:
			DoClickBoundingLine(clickEvent);
			break;

		case kClickEmpty:
			DoClickEmpty(clickEvent);
			break;

		case kClickInsideItem:
			DoClickItem(clickEvent);
			break;

		default:
			break;
	}//end switch
}//end HandleClick



LAction*
RotateController::MakeRotateAction(double inRot) {
	PhotoPrintDoc*	doc = mView->GetModel()->GetDocument();
	return new RotateAction(doc, si_Rotate, inRot);
	}//end MakeRotateAction


/*
Select {OVERRIDE}
*/
void	
RotateController::Select(PhotoItemList newSelection, bool inRefresh) {
	// at the moment, inherited handles everything
	PhotoController::Select(newSelection, inRefresh);
}//end Select



