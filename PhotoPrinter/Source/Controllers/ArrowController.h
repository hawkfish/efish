/*
	File:		ArrowController.h

	Contains:	arrow controller for kilt

	Written by:	dav lion

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

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
	virtual void DoClickEmpty(ClickEventT& inEvent);
	virtual void DoClickItem(ClickEventT& inEvent);
	virtual void DoClickHandle(ClickEventT& inEvent);
	virtual void DoClickBoundingLine(ClickEventT& inEvent); 

public:
	
					ArrowController(PhotoPrintView* inView);
	virtual			~ArrowController();
		
	// PhotoController
	virtual	void	AdjustCursorSelf(const Point& inViewPt);
	virtual void	HandleClick(const SMouseDownEvent &inMouseDown, const MRect& inBounds);
};//end ArrowController