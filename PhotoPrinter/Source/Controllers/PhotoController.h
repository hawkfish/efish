/*
	File:		PhotoController.h

	Contains:	base controller for kilt

	Written by:	dav lion

	Copyright:	Copyright �2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		04 Aug 2000		dml		Created
*/

#pragma once

class PhotoPrintView;
#include "PhotoPrintItem.h"

class PhotoController {
public:
		static const UInt32	kNoModifiers = 	0x00000000;
		static const UInt32	kShiftKey 	= 	0x00000001;
		static const UInt32	kControlKey = 	0x00000010;
		static const UInt32	kOptionKey 	= 	0x00000100;
		static const UInt32	kCommandKey = 	0x00001000;

		enum OperationType {
			kNullOperation = 0,	// nothing doing
			kSelectOperation,	// toggle selection state
			kPlaceOperation,	// dragging out a new rect
			kMoveOperation,		// moving an existing rect (click inside)
			kResizeOperation,	// resize a selection
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
			
		typedef	Point HandlesT[kFnordHandle];

		enum ClickType {
			kClickEmpty = 0,
			kClickInsideItem,
			kClickOnHandle,
			kClickBoundingLine,
			kFnordClick
			};//end ClickType
		
		enum BoundingLineType { // a total of 6 boundary lines are defined
			kTopLine = 0,		// though only the first 4 are visible
			kRightLine,
			kBotLine,
			kLeftLine,
			kVerticalMidline,
			kHorizontalMidline,
			kFnordLine};

		// all the information about the click target (which item, which line, which handle)
		typedef union {
			PhotoItemRef		item;
			HandleType			handle; // kFnordHandle if not applicable
			BoundingLineType 	boundingLine; // kFnordLine if not applicable
			} ClickedTarget;

		// all the information about a click:  where, what-kind, the click-target info, and any modifier keys down
		typedef struct {
			Point		where;
			ClickType	type;
			ClickedTarget target;
			UInt32		modifierKeys; 
			} ClickEventT;


	protected:
		// a controller operates inside of a view
		MRect			mBounds; // set each time HandleClick is called.  bounds of current view
		PhotoPrintView*	mView;
		virtual PhotoPrintView*	GetView() {return mView;};	

		// operates on a selection
		virtual void HighlightSelection(PhotoItemList& selection);
		virtual void FrameItem(PhotoItemRef item);
		virtual void DrawHandles(HandlesT& handles);
		virtual void CalculateHandlesForItem(PhotoItemRef item, HandlesT& outHandles);

		
		// utility routines
		// matrix mung
		virtual void SetupDestMatrix(MatrixRecord* pMatrix, double inRot, double skew,
								const Point& center, bool bInitialize = false);
		virtual void 	DeconstructDestIntoComponents(MRect& dest, double rot, double skew, HandlesT& handles);

		// line stuff
		virtual double 	DistanceFromBoundary(const Point& point, HandlesT& handles, BoundingLineType whichLine, bool& inside);
		virtual double	FindClosestLine(const Point& starting, HandlesT& handles, BoundingLineType& outLine);
		virtual void	GetRotationSegment(const BoundingLineType& whichLine, HandlesT& handles,
									Point& startPoint, Point& endPoint);
		// test with points and lines
		virtual bool	PointInsideItem(const Point& p, PhotoItemRef item);
		virtual bool 	PointInsideMidline(const Point&p, HandlesT& handles, BoundingLineType whichLine);
		virtual double 	PointLineDistance(const Point p, const Point l1, const Point l2, bool& inside);
		virtual double	RotFromPointLine(const Point& start, const Point& startPoint, const Point& endPoint);

		// top level internal entry point
		virtual void InterpretClick(ClickEventT& ioEvent);
		

	public:
				PhotoController(PhotoPrintView* inView);
		virtual ~PhotoController();
		
		// your subclass should implement these!
		virtual void 	HandleClick(const SMouseDownEvent &inMouseDown, const MRect& inBounds) = 0;
		virtual	void	AdjustCursor(const Point& inPortPt) = 0;
		virtual void	Select(PhotoItemList newSelection, bool inRefresh = true) = 0;

	};//end PhotoController