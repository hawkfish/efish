/*
	File:		NameController.h

	Contains:	nametag controller for kilt

	Written by:	dav lion 

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):


	22 feb 2001		dml		created
	
	
*/
#pragma once
#include "PhotoController.h"


class NameController : public PhotoController {

	public:
					NameController(PhotoPrintView* inView);
	virtual			~NameController();

	// PhotoController
	virtual	void	AdjustCursorSelf(const Point& inViewPt);
	virtual void	DoClickItem(ClickEventT& inEvent);
	virtual void	HandleClick(const SMouseDownEvent &inMouseDown, const MRect& inBounds,
								SInt16 inClickCount);
	};//end NameController	