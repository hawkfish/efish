/*
	File:		PhotoPrintController.h

	Contains:	controller for items

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

	19 june 2000	dml		added DoCrop method, kCropOperation
*/

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
			kCropOperation,		// crop 'dat rect!
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
		virtual void DoCrop		(const Point& start);
		virtual void DoMove		(const Point& start);
		virtual void DoPlace	(const Point& start);
		virtual void DoResize	(const Point& start, HandleType whichHandle);
		virtual void DoRotate	(const Point& start, BoundingLineType whichLine);
		virtual void DoSelect	(PhotoItemRef selection);
		virtual void DoSkew		(const Point& start);

		// utility functions
		virtual void 	DeconstructDestIntoComponents(MRect& dest, float rot, float skew);
		virtual double 	DistanceFromBoundary(const Point& point, BoundingLineType whichLine, bool& inside);
		virtual double	FindClosestLine(const Point& starting, BoundingLineType& outLine);
		virtual void	GetRotationSegment(const BoundingLineType& whichLine, 
									Point& startPoint, Point& endPoint);
		virtual bool	PointInsideItem(const Point& p, PhotoItemRef item);
		virtual bool 	PointInsideMidline(const Point&p, BoundingLineType whichLine);
		virtual double 	PointLineDistance(const Point p, const Point l1, const Point l2, bool& inside);
		virtual double	RotFromPointLine(const Point& start, const Point& startPoint, const Point& endPoint);


		
		// drawing
		virtual void 	DrawHandles();
		virtual void 	RecalcHandles(const MRect& rDest, const MatrixRecord* pMatrix = 0);
		virtual void	SetupHandlesForNewSelection(const PhotoItemRef selection);
		virtual void 	SetupDestMatrix(MatrixRecord* pMatrix, float inRot, float skew,
											const Point& center, bool bInitialize = false);
		
		// updating
		virtual void	UpdateModelSelection(float rot, float skew, const MRect& r);
	
	public:
						PhotoPrintController(PhotoPrintView* inView, PhotoPrintModel* inModel = 0);
		virtual			~PhotoPrintController();

		virtual void	HandleClick(const SMouseDownEvent &inMouseDown, const MRect& bounds);
		virtual void	Select(PhotoItemRef newSelection, bool inRefresh = true);
		virtual void	SetModel(PhotoPrintModel* inModel) {mModel = inModel;};



	}; // end PhotoPrintController