/*
	File:		CropController.h

	Contains:	Crop tool controller for Kilt

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		03 aug 2001		dml		add CalculateHandlesForItem, CalculateNewOffsets
		30 Aug 2000		drd		Changed superclass back to PhotoController
		23 aug 2000		dml		move DrawXFormedRect to PhotoController.h
		23 aug 2000		dml		crop-drag-hand working
		22 aug 2000		dml		work w/ rotation
		15 Aug 2000		drd		Added factory method MakeCropAction
		11 Aug 2000		drd		Created
*/

#pragma once

#include "PhotoController.h"

class CropController : public PhotoController {
	public:
						CropController(PhotoPrintView* inView);
		virtual			~CropController();
			
		virtual	void	AdjustCursorSelf(const Point& inViewPt);
		virtual void	HandleClick(const SMouseDownEvent &inMouseDown, const MRect& inBounds,
								SInt16 inClickCount);

	protected:
		static void 		ClampPointBetween(Point& ioClamp, const Point& minPoint, const Point& maxPoint);
		virtual void		DoClickHandle(ClickEventT& inEvent);
		virtual	LAction*	MakeCropAction(const MRect&	inNewCrop, const double inNewTopOffset = 0.0, const double inNewLeftOffset = 0.0);
		virtual void		UpdateDraggedRect(const HandleType& handle,
											MRect& ioRect,
											const Point& inverseXformedPoint);
		virtual void 		DoClickItem(ClickEventT& inEvent);
		virtual void 		CalculateHandlesForItem(PhotoItemRef item, HandlesT& outHandles) const;
		virtual void 		CalculateNewOffsets(const MRect& oldCrop, const MRect& newCrop, 
												const ClickEventT& inEvent, 
												double& newTopOffset, double& newLeftOffset);

};//end CropController
