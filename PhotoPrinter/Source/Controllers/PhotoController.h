/*
	File:		PhotoController.h

	Contains:	Base controller (in MVC) for kilt

	Written by:	dav lion and David Dunham

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		24 Jul 2001		drd		216 DrawHandles takes antsy arg
		13 Jul 2001		drd		75 kHandleSize now 4, not 5
		11 Jul 2001		rmgw	InterpretClick is public and const.
		22 Sep 2000		drd		Moved DrawXformedRect to PhotoUtility
		19 sep 2000		dml		add multiclick support
		30 aug 2000		dml		add parm to DrawXformedRect to optionally use region
		30 Aug 2000		drd		Moved DoClickEmpty here (from ArrowController), DoClickItem
								now starts here
		25 Aug 2000		drd		ClickEventT now derived from SMouseDownEvent
		21 Aug 2000		drd		Moved kHandleSize here
		14 Aug 2000		drd		ClickedTarget is now a struct, not a union
		11 Aug 2000		drd		Select no longer pure virtual
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

		static const SInt16	kHandleSize = 4;

		enum OperationType {
			kNullOperation = 0,	// nothing doing
			kSelectOperation,	// toggle selection state
			kPlaceOperation,	// dragging out a new rect
			kMoveOperation,		// moving an existing rect (click inside)
			kResizeOperation,	// resize a selection
			kRotateOperation,	// rotating an existing rect
			kSkewOperation,		// skewing an existing rect
			kCropOperation,		// crop 'dat rect!
			kFnordOperation		// illegal op
		};
		
		enum HandleType { // there are 10 handles around an object (including center)
			kTopLeft = 0,
			kTopMid,
			kTopRight,
			kMidLeft,
			kMidMid,
			kMidRight,
			kBotLeft,
			kBotMid,
			kBotRight,
			kFnordHandle
		};
			
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
			kFnordLine
		};

		enum {
			kSolid = false,
			kMarchingAnts = true
		};

		// all the information about the click target (which item, which line, which handle)
		typedef struct {
			PhotoItemRef		item;
			HandleType			handle; // kFnordHandle if not applicable
			BoundingLineType 	boundingLine; // kFnordLine if not applicable
		} ClickedTarget;

		// all the information about a click:  where, what-kind, the click-target info, and any modifier keys down
		class ClickEventT : public SMouseDownEvent {
		public:
			ClickedTarget	target;
			ClickType		type;
		};


	protected:
		// a controller operates inside of a view
		MRect			mBounds; // set each time HandleClick is called.  bounds of current view
		SPoint32		mScrollPosition; // also initialized at each HandleClick call
		PhotoPrintView*	mView;
		virtual PhotoPrintView*	GetView()		{ return mView; }	

		// operates on a selection
		virtual void HighlightSelection(PhotoItemList& selection);
		virtual void FrameItem(PhotoItemRef item);
		virtual void DrawHandles(HandlesT& handles, const double inRot, const bool inAntsy = kSolid);
		virtual void CalculateHandlesForItem(PhotoItemRef item, HandlesT& outHandles) const;
		virtual void RecalcHandlesForDestMatrix(HandlesT& outHandles, const MRect& dest, const MatrixRecord* inMat = 0);

		
		// utility routines
		// matrix mung
		virtual void SetupDestMatrix(MatrixRecord* pMatrix, double inRot, double skew,
								const Point& center, bool bInitialize = false);
		virtual void 	DeconstructDestIntoComponents(MRect& dest, double rot, double skew, HandlesT& handles);

		// line stuff
		virtual double 	DistanceFromBoundary(const Point& point, HandlesT& handles, BoundingLineType whichLine, bool& inside) const;
		virtual double	FindClosestLine(const Point& starting, HandlesT& handles, BoundingLineType& outLine) const;
		virtual void	GetRotationSegment(const BoundingLineType& whichLine, HandlesT& handles,
									Point& startPoint, Point& endPoint) const;
		// test with points and lines
		virtual bool	PointInsideItem(const Point& p, PhotoItemRef item) const;
		virtual bool 	PointInsideMidline(const Point&p, HandlesT& handles, BoundingLineType whichLine);
		virtual double 	PointLineDistance(const Point p, const Point l1, const Point l2, bool& inside) const;
		virtual double	RotFromPointLine(const Point& start, const Point& startPoint, const Point& endPoint);

		// top level internal entry point
		virtual void	DoClickEmpty(ClickEventT& inEvent);
		virtual void	DoClickItem(ClickEventT& inEvent);

	public:
						PhotoController(PhotoPrintView* inView);
		virtual			~PhotoController();
		
		virtual	void	AdjustCursor(const Point& inPortPt);

		// your subclass should implement these!
		virtual void	AdjustCursorSelf(const Point& inViewPt) = 0;
		virtual void 	HandleClick(const SMouseDownEvent &inMouseDown, const MRect& inBounds, SInt16 inClickCount) = 0;
		virtual void	InterpretClick(ClickEventT& ioEvent) const;

		virtual void	Select(PhotoItemList newSelection, bool inRefresh = true);
};//end PhotoController