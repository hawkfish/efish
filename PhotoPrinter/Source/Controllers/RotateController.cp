/*
	File:		RotateController.cp

	Contains:	rotation controller for kilt

	Written by:	dav lion and David Dunhm

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

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
#include "ImageActions.h"
#include "PhotoPrintCommands.h"

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
DoRotate
*/

void
RotateController::DoRotate(

	ClickEventT& inEvent) 
	
	{
	
	double				startingRot (inEvent.target.item->GetRotation());
	double				skew (inEvent.target.item->GetSkew());
	Point				oldMid (inEvent.target.item->GetDestRect().MidPoint());
	MRect				dest (inEvent.target.item->GetDestRect());
	
	Point				startMouse (inEvent.whereLocal);
	Point				prevMouse (startMouse);
	
	HandlesT 			handles;
	CalculateHandlesForItem (inEvent.target.item, handles);
	
	double				rot (0);
	MatrixRecord		mat;
	SetupDestMatrix (&mat, rot + startingRot, skew, oldMid, true);
	RecalcHandlesForDestMatrix (handles, dest, &mat);
	
	StColorPenState	penState;
	penState.Normalize ();
	UMarchingAnts::UseAntsPattern ();
	::PenMode (srcXor);
	
	bool likelyToBeAccident (true);
	
	MRect transformedDestNoCaptionReduction;
	while (::StillDown ()) {
		Point	curMouse;
		::GetMouse (&curMouse);
		if (::EqualPt(prevMouse, curMouse)) continue;

		DrawHandles(handles, rot);
		
		rot = PlaneAngle (startMouse, curMouse, oldMid);
		rot *= PhotoUtility::kRad2Degrees;
		rot += startingRot;

		dest = inEvent.target.item->GetDestRect();
		mView->AdjustTransforms(rot, skew, dest, inEvent.target.item, &transformedDestNoCaptionReduction);

		SetupDestMatrix(&mat, rot , skew, oldMid, true);
		RecalcHandlesForDestMatrix(handles, transformedDestNoCaptionReduction, &mat);
		DrawHandles(handles, rot );
		
		prevMouse = curMouse;
		likelyToBeAccident = false; // there was a stilldown, and points not equal, so prob was a drag
		}//end while stilldown

	// if no change, return
	if (PhotoUtility::DoubleEqual(rot, startingRot)) return;
	// if it looks like an accidental tap, return
	if (likelyToBeAccident) return;
	
	PhotoPrintDoc*	doc = mView->GetModel()->GetDocument();
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
				PhotoPrintDoc*		doc = mView->GetModel()->GetDocument();
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
	PhotoPrintDoc*	doc = mView->GetModel()->GetDocument();
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



