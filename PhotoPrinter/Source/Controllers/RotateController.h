/*
	File:		RotateController.h

	Contains:	rotation controller for kilt

	Written by:	dav lion

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		30 Aug 2000		drd		Changed superclass to PhotoController
		15 Aug 2000		dml		Created
*/

#pragma once

#include "PhotoController.h"

class RotateController : public PhotoController {

	protected:
		virtual void DoClickHandle(ClickEventT& inEvent);
		virtual void DoClickBoundingLine(ClickEventT& inEvent); 
		virtual void DoRotate(ClickEventT& inEvent);
		virtual LAction*	MakeRotateAction(double inRot, const Rect* inDest);
	
	public:
		
				RotateController(PhotoPrintView* inView);
		virtual ~RotateController();
			
		virtual void HandleClick(const SMouseDownEvent &inMouseDown, const MRect& inBounds,
									SInt16 inClickCount);
		virtual	void	AdjustCursorSelf(const Point& inViewPt);
		virtual void	Select(PhotoItemList newSelection, bool inRefresh = true);
		
		};//end RotateController