//PhotoPrintController.h
// Copyright © 2000 Electric Fish, Inc.  All Rights Reserved

#pragma once
#include "MRect.h"
#include "PhotoPrintItem.h"
#include "PhotoPrintModel.h"

class PhotoPrintView;

class PhotoPrintController {
	public:
		enum OperationType {
			kNullOperation = 0,		// nothing doing
			kSelectOperation,
			kPlaceOperation,	// dragging out a new rect
			kMoveOperation,		// moving an existing rect (click inside)
			kResizeOperation,
			kRotateOperation,	// rotating an existing rect
			kSkewOperation,		// skewing an existing rect
			kFnordOperation};	// illegal op

		
		enum HandleType { // there are 10 handles around an object
			kTopLeft = 0,
			kTopMid,
			kTopRight,
			kMidLeft,
			kMidMid,
			kMidRight,
			kBotLeft,
			kBotMid,
			kBotRight,
			kFnordHandle};


		enum BoundingLineType { // a total of 6 boundary lines are defined
			kTopLine = 0,		// though only the first 4 are visible
			kRightLine,
			kBotLine,
			kLeftLine,
			kVerticalMidline,
			kHorizontalMidline,
			kFnordLine};

	protected:
		Point				handles[kFnordHandle];
		PhotoPrintView*		mView;
		PhotoPrintModel*	mModel;
		MRect				mBounds;
		
		// maps click to operation
		virtual OperationType OperationFromClick(const SMouseDownEvent& inMouseDown,
										HandleType& outHandle, BoundingLineType& outLine, PhotoItemRef& outSelect);
		PhotoItemRef	Selection();

		// operations we can perform
		virtual void DoPlace	(const Point& start);
		virtual void DoRotate	(const Point& start, BoundingLineType whichLine);
		virtual void DoSkew		(const Point& start);
		virtual void DoResize	(const Point& start, HandleType whichHandle);
		virtual void DoMove		(const Point& start);
		virtual void DoSelect	(PhotoItemRef selection);

		// utility functions
		virtual double 	PointLineDistance(const Point p, const Point l1, const Point l2, Boolean& inside);
		virtual double 	DistanceFromBoundary(const Point& point, BoundingLineType whichLine, Boolean& inside);
		virtual void 	DeconstructDestIntoComponents(MRect& dest, float rot, float skew);
		virtual double	FindClosestLine(const Point& starting, BoundingLineType& outLine);
		virtual double	RotFromPointLine(const Point& start, const Point& startPoint, const Point& endPoint);

		virtual void	GetRotationSegment(const BoundingLineType& whichLine, 
									Point& startPoint, Point& endPoint);
		virtual Boolean	PointInsideItem(const Point& p, PhotoItemRef item);
		virtual Boolean PointInsideMidline(const Point&p, BoundingLineType whichLine);

		
		// drawing
		virtual void	SetupHandlesForNewSelection(const PhotoItemRef selection);
		virtual void 	SetupDestMatrix(MatrixRecord* pMatrix, float inRot, float skew,
											const Point& center, Boolean bInitialize = false);
		virtual void 	RecalcHandles(const MRect& rDest, const MatrixRecord* pMatrix = 0);
		virtual void 	DrawHandles();
		
		// updating
		virtual void	UpdateModelSelection(float rot, float skew, const MRect& r);
	
	public:
						PhotoPrintController(PhotoPrintView* inView, PhotoPrintModel* inModel = 0);
		virtual			~PhotoPrintController();

		virtual void	SetModel(PhotoPrintModel* inModel) {mModel = inModel;};
		virtual void	HandleClick(const SMouseDownEvent &inMouseDown, const MRect& bounds);
		virtual void	Select(PhotoItemRef newSelection);



	}; // end PhotoPrintController