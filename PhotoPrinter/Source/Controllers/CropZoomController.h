/*
	File:		CropZoomController.h

	Contains:	Crop-Zoom tool controller for Kilt

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		22 aug 2000		dml		change signature of MakeCropAction
		21 aug 2000		dml		return to overriding DoClickItem
		15 Aug 2000		drd		Override DoClickItem
		15 Aug 2000		drd		Created
*/

#pragma once

#include "CropController.h"

class CropZoomController : public CropController {
	public:
							CropZoomController(PhotoPrintView* inView);
		virtual				~CropZoomController();
			
		virtual	void		AdjustCursorSelf(const Point& inViewPt);

	protected:
		virtual	LAction*	MakeCropAction(const MRect&	inNewCrop, const double inNewTopOffset = 0.0,
											const double inNewLeftOffset = 0.0);
		virtual void 		DoClickItem(ClickEventT& inEvent);

};//end CropZoomController
