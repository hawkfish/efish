/*
	File:		PhotoController.cp

	Contains:	base controller (in MVC) for kilt

	Written by:	dav lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		15 aug 2000 	dml		add RecalcHandlesForDestMatrix
		15 aug 2000		dml		InterpretClick sets BoundingLine when over a Handle
		15 aug 2000		dml		handles (and frames) drawn Xor
		11 Aug 2000		drd		Fixed calculation of handles in InterpretClick
		11 Aug 2000		drd		Select can go here (not ArrowController)
		07 Aug 2000		dml		Created
*/

#include "PhotoController.h"
#include "PhotoUtility.h"
#include "PhotoPrintModel.h"
#include "MKeyMap.h"
#include "PhotoPrintView.h"

static const SInt16	kHandleSize = 5;

//----------------------------------------------
//
//----------------------------------------------
PhotoController::PhotoController(PhotoPrintView* inView)
	: mView (inView)
{
}//end ct

 
//----------------------------------------------
//
//----------------------------------------------
PhotoController::~PhotoController()
{
}//end dt


//----------------------------------------------
//
//----------------------------------------------
void PhotoController::CalculateHandlesForItem(PhotoItemRef item, HandlesT& outHandles){
	MatrixRecord mat;
	MRect	rDest;
	
	item->GetMatrix(&mat, false);
	rDest = item->GetDestRect();

	// MRect built-ins
	outHandles[kTopLeft] = rDest.TopLeft();
	outHandles[kBotRight] = rDest.BotRight();
	outHandles[kMidMid] = rDest.MidPoint();

	// derived
	outHandles[kTopMid].h = outHandles[kMidMid].h;
	outHandles[kTopMid].v = outHandles[kTopLeft].v;	
	
	outHandles[kTopRight].h = outHandles[kBotRight].h;
	outHandles[kTopRight].v = outHandles[kTopLeft].v;	
	
	outHandles[kMidLeft].h = outHandles[kTopLeft].h;
	outHandles[kMidLeft].v = outHandles[kMidMid].v;	

	outHandles[kMidRight].h = outHandles[kTopRight].h;
	outHandles[kMidRight].v = outHandles[kMidMid].v;	

	outHandles[kBotLeft].h = outHandles[kTopLeft].h;
	outHandles[kBotLeft].v = outHandles[kBotRight].v;
	
	outHandles[kBotMid].h = outHandles[kMidMid].h;
	outHandles[kBotMid].v = outHandles[kBotRight].v;

	TransformPoints (&mat, outHandles, kFnordHandle); 
}//end CalculateHandlesForItem


//----------------------------------------------
// we've just finished a resize operation
// the handles are correct, but dest isn't quite right
// because the rotation (in order to rubberband/draw the handles correctly)
// has ocurred around some point other than dest.MidPoint()
// So, based on the handles and dest, we reconstruct dest + associated placement values
// SKEW NOT YET IMPLEMENTED
//----------------------------------------------
void 	 
PhotoController::DeconstructDestIntoComponents(MRect& dest, double rot, double /*skew*/, HandlesT& handles){
	long height;
	long width;

	height = dest.Height();
	width = dest.Width();


	Point newMiddle = dest.MidPoint();
	Point newTopLeft = handles[kTopLeft];
	MatrixRecord unRotate;
	::SetIdentityMatrix(&unRotate);
	::RotateMatrix(&unRotate, ::Long2Fix(-rot), 
					::Long2Fix(handles[kMidMid].h), ::Long2Fix(handles[kMidMid].v));
	::TransformPoints(&unRotate, &newTopLeft, 1);

	dest.Offset(newTopLeft.h - dest.left, newTopLeft.v - dest.top);
	
}//end DeconstructDestIntoComponents


// line stuff
//----------------------------------------------
//Note all lines are Right handed (CW traversal define the bounding box)
//----------------------------------------------
double 	 
PhotoController::DistanceFromBoundary(const Point& point, HandlesT& handles, BoundingLineType whichLine, bool& inside){
	switch (whichLine) {
		case kTopLine:
			return (PointLineDistance(point, handles[kTopLeft], handles[kTopRight], inside));
			break;
		case kRightLine:
			return (PointLineDistance(point, handles[kTopRight], handles[kBotRight], inside));
			break;
		case kBotLine:
			return (PointLineDistance(point, handles[kBotRight], handles[kBotLeft], inside));
			break;
		case kLeftLine:
			return (PointLineDistance(point, handles[kBotLeft], handles[kTopLeft], inside));
			break;
		case kVerticalMidline:
			return (PointLineDistance(point, handles[kTopMid], handles[kBotMid], inside));
			break;
		case kHorizontalMidline:
			return (PointLineDistance(point, handles[kMidLeft], handles[kMidRight], inside));
			break;
		default:
			DebugStr("\pBogus Argument to DistanceToBoundary");
		}//end switch	

	return (0.0);
}//end DistanceFromBoundary

 
//----------------------------------------------
//
//----------------------------------------------
void  
PhotoController::DrawHandles(HandlesT& handles){
	StColorPenState	existingState;
	::PenMode(patXor);

	bool realData (false);
	Point	emptyPoint;
	emptyPoint.v = emptyPoint.h = 0;

	for (int i = 0; i < kFnordHandle; ++i) {
		MRect rHandle(handles[i], handles[i]);
		if (!::EqualPt(handles[i], emptyPoint)) { // make sure some handle is real
			realData = true;
			rHandle.Inset(-kHandleSize, -kHandleSize);
			rHandle.Frame();
			}//endif sane
		}//for all handles

	if (realData) {		
		// draw the lines between the handles
		::MoveTo(handles[kTopLeft].h, handles[kTopLeft].v);
		::LineTo(handles[kTopRight].h, handles[kTopRight].v);

		::MoveTo(handles[kTopRight].h, handles[kTopRight].v);
		::LineTo(handles[kBotRight].h, handles[kBotRight].v);

		::MoveTo(handles[kBotRight].h, handles[kBotRight].v);
		::LineTo(handles[kBotLeft].h, handles[kBotLeft].v);

		::MoveTo(handles[kBotLeft].h, handles[kBotLeft].v);
		::LineTo(handles[kTopLeft].h, handles[kTopLeft].v);
		}//endif there is something to draw

}//end DrawHandles

 
//----------------------------------------------
//FindClosestLine
//----------------------------------------------
double	 
PhotoController::FindClosestLine(const Point& starting, HandlesT& handles, BoundingLineType& outLine){
	double shortest (0.0);
	double temp;
	bool inside;
	
	shortest = DistanceFromBoundary(starting, handles, kTopLine, inside);
	outLine = kTopLine;
	
	temp = DistanceFromBoundary(starting, handles, kBotLine, inside);
	if (temp < shortest) {
		outLine = kBotLine;
		shortest = temp;
		}//endif 
		
	temp = DistanceFromBoundary(starting, handles, kLeftLine, inside);
	if (temp < shortest) {
		outLine = kLeftLine;
		shortest = temp;
		}//endif 
	
	temp = DistanceFromBoundary(starting, handles, kRightLine, inside);
	if (temp < shortest) {
		outLine = kRightLine;
		shortest = temp;
		}//endif 

	return shortest;
}//end FindClosestLine

 
//----------------------------------------------
//FrameItem
//----------------------------------------------
void  
PhotoController::FrameItem(PhotoItemRef item){
	StColorPenState	existingState;
	::PenMode(patXor);
	::PenSize(2,2);

	MRect rDest = item->GetDestRect();
	::FrameRect(&rDest);

}//end FrameItem


//----------------------------------------------
//GetRotationSegment
//----------------------------------------------
void	 
PhotoController::GetRotationSegment(const BoundingLineType& whichLine, HandlesT& handles,
									Point& startPoint, Point& endPoint){
	switch (whichLine) {
		case kTopLine:
				startPoint = handles[kTopLeft];
				endPoint = handles[kTopRight];
			break;
		case kRightLine:
				startPoint = handles[kTopRight];
				endPoint = handles[kBotRight];
			break;
		case kBotLine:
				startPoint = handles[kBotRight];
				endPoint = handles[kBotLeft];
			break;
		case kLeftLine:
				startPoint = handles[kBotLeft];
				endPoint = handles[kTopLeft];
			break;
		}//end switch
}//end GetRotationSegment
 

//----------------------------------------------
//HighlightSelection
//----------------------------------------------
void  
PhotoController::HighlightSelection(PhotoItemList& selection){
	PhotoIterator i (selection.begin());
	if (i != selection.end()) {
		HandlesT handles;
		CalculateHandlesForItem(*i, handles);
		DrawHandles(handles);
		}//endif at least one selected
	
	// just frame the rest of the images;
	for (++i; i != selection.end(); ++i) {
		FrameItem(*i);
		}//for all secondary selections
}//end HighlightSelection


//----------------------------------------------
//InterpretClick
//----------------------------------------------
void  
PhotoController::InterpretClick(ClickEventT& ioEvent){
	// figure out any modifier keys 
	MKeyMap	keymap;
	if (keymap.ScanPressed(MKeyMap::kCmdScan))
		ioEvent.modifierKeys |= kCommandKey;
	if (keymap.ScanPressed(MKeyMap::kControlScan))
		ioEvent.modifierKeys |= kControlKey;
	if (keymap.ScanPressed(MKeyMap::kOptionScan))
		ioEvent.modifierKeys |= kOptionKey;
	if (keymap.ScanPressed(MKeyMap::kShiftScan))
		ioEvent.modifierKeys |= kShiftKey;
		
	const PhotoItemList&	selection (mView->Selection());
	ConstPhotoIterator		firstItem (selection.begin());
	if (firstItem != selection.end()) {
		HandlesT			handles;
		CalculateHandlesForItem(*firstItem, handles);

		for (int i = 0; i < kFnordHandle; ++i) {
			MRect rHandle(handles[i], handles[i]);
			rHandle.Inset(-kHandleSize, -kHandleSize);
			if (::PtInRect(ioEvent.where, &rHandle)) {
				ioEvent.target.item = *firstItem;
				ioEvent.target.handle = (HandleType)i;
				ioEvent.type = kClickOnHandle;
				// if a handle was hit, find the closest line
				FindClosestLine(ioEvent.where, handles, ioEvent.target.boundingLine);
				return;
			}//endif found it
		}//for
				
		//	check against boundary lines
		if (FindClosestLine(ioEvent.where, handles, ioEvent.target.boundingLine) < kHandleSize) {
			ioEvent.target.item = *firstItem;
			ioEvent.type = kClickBoundingLine;
			return;
			}//endif close enough to a line
		}//endif something is selected

	// only the primary selection has handles + bounding lines, so any other click in item is simple
	PhotoPrintModel* model (mView->GetModel());
	for (PhotoIterator item = model->begin(); item != model->end(); ++item) {
		if (PointInsideItem(ioEvent.where, *item)) {
			ioEvent.target.item = *item;
			ioEvent.type = kClickInsideItem;
			return;
			}//endif
		}//for all items
		
	// nothing yet, must have clicked on empty space:
	ioEvent.type = kClickEmpty;
	return;
}//end InterpretClick
 
 
// test with points and lines
//----------------------------------------------
//PointInsideItem
//----------------------------------------------
bool	 
PhotoController::PointInsideItem(const Point& p, PhotoItemRef item){
	Point startPoint;
	Point endPoint;
	bool inside (true);
	
	if (item) {
		HandlesT	handles;
		CalculateHandlesForItem(item, handles);
		do {
			GetRotationSegment(kTopLine, handles, startPoint, endPoint);
			PointLineDistance(p, startPoint, endPoint, inside);
			if (!inside)
				continue;
			GetRotationSegment(kRightLine, handles, startPoint, endPoint);
			PointLineDistance(p, startPoint, endPoint, inside);
			if (!inside)
				continue;
			GetRotationSegment(kBotLine, handles, startPoint, endPoint);
			PointLineDistance(p, startPoint, endPoint, inside);
			if (!inside)
				continue;
			GetRotationSegment(kLeftLine, handles, startPoint, endPoint);
			PointLineDistance(p, startPoint, endPoint, inside);
			if (!inside)
				continue;
			} while (false);
		}//endif sane		
	else
		inside = false;	

	return inside;
}//end PointInsideItem

 
//----------------------------------------------
// PointInsideMidline
//----------------------------------------------
bool 	 
PhotoController::PointInsideMidline(const Point&p, HandlesT& handles, BoundingLineType whichLine){
	// so, we're rotating.  figure out which side of midline we've grabbed.
	// inside is typically defined as "right side of", so we flip in 2 cases
	bool insideMidline;
	switch (whichLine) {
		case kTopLine:
			DistanceFromBoundary(p, handles, kVerticalMidline, insideMidline);
			insideMidline = !insideMidline;// flip sense since reflected case
			break;
		case kBotLine:
			DistanceFromBoundary(p, handles, kVerticalMidline, insideMidline);
			break;
		case kLeftLine:
			DistanceFromBoundary(p, handles, kHorizontalMidline, insideMidline);
			insideMidline = !insideMidline;// flip sense since reflected case
			break;
		case kRightLine:
			DistanceFromBoundary(p, handles, kHorizontalMidline, insideMidline);
			break;
		}//end switch	

	return insideMidline;
}//end PointInsideMidline

 
//----------------------------------------------
//
//----------------------------------------------
double 	 
PhotoController::PointLineDistance(const Point p, const Point l1, const Point l2, bool& inside){
	double distance (0.0);

	do {
		// degenerate cases first
		
		// vertical line
		if (l2.h == l1.h) {
			if (((l1.v >= p.v) && (p.v >= l2.v)) ||
				((l1.v <= p.v) && (p.v <= l2.v))) {
				distance = fabs((double)(p.h - l2.h));
				}//endif in between endpoints of vertical line, so distance is dh
			else {
				double d1 = (sqrt(((p.v - l1.v) * (p.v - l1.v)) + ((p.h - l1.h) * (p.h - l1.h))));
				double d2 = (sqrt(((p.v - l2.v) * (p.v - l2.v)) + ((p.h - l2.h) * (p.h - l2.h))));
				distance = min(d1,d2);
				}//else distance is min of distances from endpoints
			
			if (l2.v > l1.v)
				inside = p.h < l2.h ? true : false;
			else
				inside = p.h > l2.h ? true : false;
			continue;
			}//endif a vertical line

		if (l2.v == l1.v) {
			if (((l1.h >= p.h) && (p.h >= l2.h)) ||
				((l1.h <= p.h) && (p.h <= l2.h))) {
				distance = fabs((double)(p.v - l2.v));
				}//endif point is between endpoints, so distance is dv
			else {
				double d1 = (sqrt(((p.v - l1.v) * (p.v - l1.v)) + ((p.h - l1.h) * (p.h - l1.h))));
				double d2 = (sqrt(((p.v - l2.v) * (p.v - l2.v)) + ((p.h - l2.h) * (p.h - l2.h))));
				distance = min(d1,d2);				
				}//else distance is min of distances from endpoints
			
			if (l2.h > l1.h)
				inside = p.v > l2.v ? true : false;
			else
				inside = p.v < l2.v ? true : false;
			continue;
			}//endif it's a horizontal line

		//otherwise,  non-orthagonal line.
		
		double dx (l2.h - l1.h);
		double dy (l2.v - l1.v);
		double m = dy / dx;
		double b = (double)l1.v - (double)l1.h * m;

		// find negative recprocal slope of l1-l2
		double mPrime = -1.0 / m;
		// solve y = mPrimeX + b using p:  
		double bPrime;
		bPrime = (double)p.v - (double)p.h * mPrime;
		
		// compute intersection of lines.
		double x = (bPrime - b) / (m - mPrime);
		double y = (m * x) + b;

		distance = (sqrt(((p.v - y) * (p.v - y)) + ((p.h - x) * (p.h - x))));

		// we already have most of the coefficients for this test (though there is a function)
		double F ((dy * p.h) - (dx * p.v) + (b * dx));
		inside = F < 0.0 ? true : false;

		} while (false);
		

	// find distance from p to that intersection	
	return distance;
}//end PointLineDistance



//----------------------------------------------
// RecalcHandlesForDestMatrix
// useful in complicated drag-tracking
//----------------------------------------------
void 
PhotoController::RecalcHandlesForDestMatrix(HandlesT& outHandles, const MRect& dest, const MatrixRecord* pMatrix) {
	// MRect built-ins
	outHandles[kTopLeft] = dest.TopLeft();
	outHandles[kBotRight] = dest.BotRight();
	outHandles[kMidMid] = dest.MidPoint();

	// derived
	outHandles[kTopMid].h = outHandles[kMidMid].h;
	outHandles[kTopMid].v = outHandles[kTopLeft].v;	
	
	outHandles[kTopRight].h = outHandles[kBotRight].h;
	outHandles[kTopRight].v = outHandles[kTopLeft].v;	
	
	outHandles[kMidLeft].h = outHandles[kTopLeft].h;
	outHandles[kMidLeft].v = outHandles[kMidMid].v;	

	outHandles[kMidRight].h = outHandles[kTopRight].h;
	outHandles[kMidRight].v = outHandles[kMidMid].v;	

	outHandles[kBotLeft].h = outHandles[kTopLeft].h;
	outHandles[kBotLeft].v = outHandles[kBotRight].v;
	
	outHandles[kBotMid].h = outHandles[kMidMid].h;
	outHandles[kBotMid].v = outHandles[kBotRight].v;

	if (pMatrix)
		TransformPoints (pMatrix, outHandles, kFnordHandle); 

	}//end RecalcHandlesForDestMatrix


 
//----------------------------------------------
// RotFromPointLine
//----------------------------------------------
double	 
PhotoController::RotFromPointLine(const Point& start, const Point& startPoint, const Point& endPoint){
	// dot product is |a||b|cos theta
	// but we more easily can determine sin, since we have fund to determine perp distance.
	
	bool inside;
	double b = PointLineDistance(start, startPoint, endPoint, inside);

	double dx = (start.h - startPoint.h);
	double dy = (start.v - startPoint.v);
	double a (sqrt((dx * dx) + (dy * dy)));
	
	double sinTheta (b / a);
	double rotation ((asin(sinTheta) * PhotoUtility::kRad2Degrees));
	if (!inside)
		rotation *= -1.0;

	return rotation;
}//end RotFromPointLine

//----------------------------------------------
// Select
//----------------------------------------------
void	
PhotoController::Select(PhotoItemList newSelection, const bool inRefresh) {
	if (inRefresh)
		this->HighlightSelection(newSelection);
}//end Select

//----------------------------------------------
// SetupDestMatrix
//----------------------------------------------
void  
PhotoController::SetupDestMatrix(MatrixRecord* pMatrix, 
								double inRot, double /*skew*/,
								const Point& center, 
								bool bInitialize){
	if (bInitialize)
		::SetIdentityMatrix(pMatrix);
	::RotateMatrix (pMatrix, Long2Fix(inRot), Long2Fix(center.h), Long2Fix(center.v));
	
}//end SetupDestMatrix
 
