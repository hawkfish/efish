/*
	File:		ArrowController.h

	Contains:	arrow controller for kilt

	Written by:	dav lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		30 Aug 2000		drd		Moved DoClickEmpty to PhotoController
		29 Aug 2000		drd		Override AddFlavors, DoDragSendData, RemoveDragItem
		24 Aug 2000		drd		Also inherit from CDragAndDrop; removed Select
		21 aug 2000		dml		AdjustCursorSelf replaces AdjustCursor
		07 Aug 2000		dml		Created
*/

#pragma once

#include "PhotoController.h"
#include "CDragAndDrop.h"

class ArrowController : public PhotoController, public CDragAndDrop {
protected:

	// PhotoController
	virtual void	DoClickBoundingLine(ClickEventT& inEvent); 
	virtual void	DoClickItem(ClickEventT& inEvent);
	virtual void	DoClickHandle(ClickEventT& inEvent);

public:
					ArrowController(PhotoPrintView* inView);
	virtual			~ArrowController();

	// PhotoController
	virtual	void	AdjustCursorSelf(const Point& inViewPt);
	virtual void	HandleClick(const SMouseDownEvent &inMouseDown, const MRect& inBounds);

	// CDragItem
	virtual	void	AddFlavors(DragReference inDragRef);
	virtual void	DoDragSendData(FlavorType inFlavor,
								ItemReference inItemRef,
								DragReference inDragRef);

	// CDragAndDrop
	virtual void	RemoveDragItem(const SMouseDownEvent &inMouseDown);
};//end ArrowController