/*
	File:		CropZoomController.h

	Contains:	Crop-Zoom tool controller for Kilt

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		15 Aug 2000		drd		Created
*/

#pragma once

#include "CropController.h"

class CropZoomController : public CropController {
	public:
							CropZoomController(PhotoPrintView* inView);
		virtual				~CropZoomController();
			
		virtual	void		AdjustCursor(const Point& inPortPt);

	protected:
		virtual	LAction*	MakeCropAction(const MRect&	inNewCrop);

};//end CropZoomController
