/*
	File:		ArrowController.h

	Contains:	arrow controller for kilt

	Written by:	dav lion

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		21 aug 2000		dml		AdjustCursorSelf replaces AdjustCursor
		07 Aug 2000		dml		Created
*/

#pragma once

#include "PhotoController.h"

class ArrowController : public PhotoController {

	protected:
	
		// we are a virtual base class.  subclasses should implement these functions
		virtual void DoClickEmpty(ClickEventT& inEvent);
		virtual void DoClickItem(ClickEventT& inEvent);
		virtual void DoClickHandle(ClickEventT& inEvent);
		virtual void DoClickBoundingLine(ClickEventT& inEvent); 

	public:
		
				ArrowController(PhotoPrintView* inView);
		virtual ~ArrowController();
			
		virtual void HandleClick(const SMouseDownEvent &inMouseDown, const MRect& inBounds);
		virtual	void	AdjustCursorSelf(const Point& inViewPt);
		virtual void	Select(PhotoItemList newSelection, bool inRefresh = true);
		
		};//end ArrowController