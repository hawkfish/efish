/*
	File:		CropController.h

	Contains:	Crop tool controller for Kilt

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		11 Aug 2000		drd		Created
*/

#pragma once

#include "PhotoController.h"

class CropController : public PhotoController {
	public:
						CropController(PhotoPrintView* inView);
		virtual			~CropController();
			
		virtual	void	AdjustCursor(const Point& inPortPt);
		virtual void	HandleClick(const SMouseDownEvent &inMouseDown, const MRect& inBounds);

	protected:
		virtual void	DoClickHandle(ClickEventT& inEvent);
		virtual void	DoClickItem(ClickEventT& inEvent);
};//end CropController
