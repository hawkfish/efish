/*
	File:		RotateController.cp

	Contains:	rotation controller for kilt

	Written by:	dav lion and David Dunhm

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		31 aug 2001		dml		275, 282 CalcHandles handles crop-zoom expansion
		20 Aug 2001		rmgw	Constrain with shift key.  Bug #338.
		03 aug 2001		dml		better handles.  not perfect (prob w/ crop), but better
		01 aug 2001		dml		262, 225.  fix problems with handles in DoRotate
		01 Aug 2001		drd		216 Erase with original rotation, and restore original handle state when done
		24 Jul 2001		drd		216 Be sure to erase cell handles before recalculating, and moved
								marching ants into DrawHandles (via new arg)
		23 Jul 2001		rmgw	Get document from view.
		18 Jul 2001		rmgw	Split up ImageActions.
		03 jul 2001		dml		25 fix caption rotating issues.  
		09 mar 2001		dml		add DoClickItem, check for accidental clicks in DoRotate
		02 mar 2001		dml		no longer interpret kClickBoundingLine, item clicks cause rotate!, bug 21
		21 Feb 2001		rmgw	20 Rotate by tracking mouse angle
		30 Aug 2000		drd		Changed superclass back to PhotoController
		25 Aug 2000		drd		ClickEventT now derived from SMouseDownEvent
		23 aug 2000		dml		pass rotation to DrawHandles
		15 aug 2000		dml		created
*/

#include "RotateController.h"

#include "PhotoPrintView.h"
#include "PhotoPrintResources.h"
#include "PhotoUtility.h"
#include "PhotoPrintDoc.h"
#include "PhotoPrintCommands.h"
#include "RotateAction.h"
#include "AlignmentGizmo.h"

/*
RotateController
*/
RotateController::RotateController(PhotoPrintView* inView)
	: PhotoController(inView)
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
	clickEvent.whereLocal = inViewPt;

	this->InterpretClick(clickEvent);
	switch (clickEvent.type) {
		case kClickInsideItem:
		case kClickOnHandle: {
			UCursor::SetTheCursor(curs_Rot);
			break;			
			}//end 
		default:
			::InitCursor();
			break;
		}//end switch
}//end AdjustCursor


void
RotateController::ApplyCrop(MRect& ioRect, const PhotoItemRef item) {
	double topCrop;
	double leftCrop;
	double bottomCrop;
	double rightCrop;

	item->GetCrop(topCrop, leftCrop, bottomCrop, rightCrop);

	SInt16 width (ioRect.Width());
	SInt16 height (ioRect.Height());

	ioRect.top += height * (topCrop / 100.0);
	ioRect.left += width * (leftCrop / 100.0);
	ioRect.bottom -= height * (bottomCrop / 100.0);
	ioRect.right -= width * (rightCrop / 100.0);
	}//end ApplyCrop


/*
PlaneDotProduct

Calculate u ¥ v
*/
static double
PlaneDotProduct (
	
	Point u,
	Point v)
	
	{ // begin PlaneDotProduct
	
		double	dot = u.h * v.h;
		dot += u.v * v.v;
		
		return dot;
	
	} // end PlaneDotProduct
	
/*
PlaneCrossProduct

Calculate u x v
*/
static double
PlaneCrossProduct (
	
	Point u,
	Point v)
	
	{ // begin PlaneCrossProduct
	
		double	cross = u.h * v.v;
		cross -= u.v * v.h;
		
		return cross;
	
	} // end PlaneCrossProduct
	
/*
PlaneAngle

Calculate the angle between two vectors
*/
static double 
PlaneAngle (

	Point	u,
	Point	v,
	Point	origin)
	
	{ // begin PlaneAngle
		
		//	Normalize
		u.h -= origin.h; u.v -= origin.v;
		v.h -= origin.h; v.v -= origin.v;

		//	theta = arccos ((u ¥ v) / (||u|| ||v||))
		double	dot = PlaneDotProduct (u, v);
		
		double	normU = std::sqrt (PlaneDotProduct (u, u));
		if (PhotoUtility::DoubleEqual(normU, 0.0)) return 0.0;
		
		double	normV = std::sqrt (PlaneDotProduct (v, v));
		if (PhotoUtility::DoubleEqual(normV, 0.0)) return 0.0;
		
		double	theta = std::acos (dot / (normU * normV));
		
		//	Determine the sign
		if (PlaneCrossProduct (u, v) < 0.0) theta = -theta;
		
		return theta;
} // end PlaneAngle


/*
* CalculateHandlesForItem OVERRIDE
* override to make handles around image rect only
*/
void 
RotateController::CalculateHandlesForItem(const PhotoItemRef item, HandlesT& outHandles) const {
	MRect	rDest;
	MRect	transformedDestNoCaptionReduction;
	MatrixRecord mat;
	double rot (item->GetRotation());
	double skew (item->GetSkew());
	
	GetSelectionRectangle(item, rDest);	

	mView->AdjustTransforms(rot, skew, rDest, item, &transformedDestNoCaptionReduction);
	SetupDestMatrix(&mat, rot, skew, rDest.MidPoint(), true);
	RecalcHandlesForDestMatrix(outHandles, rDest, &mat);

	}//end CalculateHandlesForItem




/*
DoRotate
*/
const	int	kConstrainAngle = 15;

void
RotateController::DoRotate(
	ClickEventT& inEvent) 	
{

	double				startingRot (inEvent.target.item->GetRotation());
	double				skew (inEvent.target.item->GetSkew());
	MRect				dest (inEvent.target.item->GetDestRect());
	Point				oldMid (dest.MidPoint());
	
	Point				startMouse (inEvent.whereLocal);
	Point				prevMouse (startMouse);
	
	HandlesT 			handles;
	CalculateHandlesForItem (inEvent.target.item, handles);
	this->DrawHandles(handles, startingRot);				// Get rid of original 

	double				rot (0);
	MatrixRecord		mat;
	MRect transformedDestNoCaptionReduction;

	GetSelectionRectangle(inEvent.target.item, dest);
	mView->AdjustTransforms(startingRot, skew, dest, inEvent.target.item, &transformedDestNoCaptionReduction);
//	ApplyCrop(dest, inEvent.target.item);
	SetupDestMatrix(&mat, startingRot , skew, dest.MidPoint(), true);
	RecalcHandlesForDestMatrix(handles, dest, &mat);
	this->DrawHandles(handles, startingRot);				//draw first new outside of loop (so loop can erase at top) 

	bool likelyToBeAccident (true);	
	while (::StillDown ()) {
		//	Get the mouse location
		Point	curMouse;
		::GetMouse (&curMouse);

		//	Check for a change
		if (::EqualPt(prevMouse, curMouse)) continue;

		this->DrawHandles(handles, rot, kMarchingAnts);
		
		rot = PlaneAngle (startMouse, curMouse, oldMid);
		rot *= PhotoUtility::kRad2Degrees;
		rot += startingRot;

		//	Constrain it
		if (::GetCurrentKeyModifiers () & shiftKey) 
			rot = kConstrainAngle * std::round (rot / kConstrainAngle);
		
		GetSelectionRectangle(inEvent.target.item, dest);
		mView->AdjustTransforms(rot, skew, dest, inEvent.target.item, &transformedDestNoCaptionReduction);
//		ApplyCrop(dest, inEvent.target.item);
		SetupDestMatrix(&mat, rot , skew, dest.MidPoint(), true);
		RecalcHandlesForDestMatrix(handles, dest, &mat);
		this->DrawHandles(handles, rot, kMarchingAnts);
		
		prevMouse = curMouse;
		likelyToBeAccident = false; // there was a stilldown, and points not equal, so prob was a drag
	}//end while stilldown

	// 216 Be sure to un-draw the temporary handles, and then redraw the selection handles
	this->DrawHandles(handles, rot, kMarchingAnts);
	this->CalculateHandlesForItem(inEvent.target.item, handles);
	this->DrawHandles(handles, startingRot);			// Restore original 

	// if no change, return
	if (PhotoUtility::DoubleEqual(rot, startingRot)) return;
	// if it looks like an accidental tap, return
	if (likelyToBeAccident) return;
	
	PhotoPrintDoc*	doc = mView->GetDocument();
	doc->PostAction(this->MakeRotateAction (rot, &transformedDestNoCaptionReduction));
}//end DoRotate


/*
DoClickBoundingLine
*/
void 
RotateController::DoClickBoundingLine(ClickEventT& inEvent)
{
	if (inEvent.target.item == mView->GetPrimarySelection()) 
		DoRotate(inEvent);
	else
		PhotoController::DoClickItem(inEvent);		// Call inherited
}//end DoClickBoundingLine




/*
DoClickItem
*/
void 
RotateController::DoClickItem(ClickEventT& inEvent) {
	if (inEvent.target.item == mView->GetPrimarySelection()) 
		DoRotate(inEvent);
	else
		PhotoController::DoClickItem(inEvent);		// Call inherited
	}//end



/*
DoClickHandle
*/
void 
RotateController::DoClickHandle(ClickEventT& inEvent) {
	DoRotate(inEvent);
}//end DoClickHandle


/*
GetSelectionRectangle
*/
void
RotateController::GetSelectionRectangle(const PhotoItemRef item, MRect& rDest) const {
	if (item->HasZoom()) {
		ERect32 cropZoom;
		ERect32 expandedOffsetImage;
		item->DeriveCropZoomRect(cropZoom, expandedOffsetImage);
		rDest.top = cropZoom.top;
		rDest.left = cropZoom.left;
		rDest.bottom = cropZoom.bottom;
		rDest.right = cropZoom.right;
		}//endif crop-zoom active
	else
		rDest = item->GetDestRect();

	}//end GetSelectionRectangle




/*
HandleClick {OVERRIDE}
	Main dispatch of clicks
*/
void 
RotateController::HandleClick(const SMouseDownEvent &inMouseDown, const MRect& inBounds,
								SInt16 inClickCount) {
	mBounds = inBounds;

	// Build our parameter block -- the first part is just the SMouseDownEvent
	ClickEventT		clickEvent;
	::BlockMoveData(&inMouseDown, &clickEvent, sizeof(SMouseDownEvent));
	// And fill in the rest (analyze what the click represents)
	this->InterpretClick(clickEvent);
	
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
			if (inClickCount == 1)
				DoClickItem(clickEvent);
			else {
				PhotoPrintDoc*		doc = mView->GetDocument();
				doc->ProcessCommand(cmd_ImageOptions, nil);
				}//else it's a multi-click, bring up the image options dialog
			break;

		default:
			break;
	}//end switch
}//end HandleClick


/*
MakeRotateAction
*/
LAction*
RotateController::MakeRotateAction(double inRot, const Rect* inDest) {
	PhotoPrintDoc*	doc = mView->GetDocument();
	return new RotateAction(doc, si_Rotate, inRot, inDest);
	}//end MakeRotateAction


/*
Select {OVERRIDE}
*/
void	
RotateController::Select(PhotoItemList newSelection, bool inRefresh) {
	// at the moment, inherited handles everything
	PhotoController::Select(newSelection, inRefresh);
}//end Select



