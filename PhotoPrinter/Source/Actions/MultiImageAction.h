/*
	File:		MultiImageAction.h

	Contains:	Interface to a selection based action.

	Written by:	David Dunham and Dav Lion

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		24 Jul 2001		rmgw	Add RefreshImages.  Bug #220.
		18 Jul 2001		rmgw	Split up ImageActions.
*/

#pragma once

#include "PhotoPrintAction.h"

#include "PhotoPrintItem.h"

class	MultiImageAction : public PhotoPrintAction
{
public:
						MultiImageAction(
									PhotoPrintDoc*	inDoc,
									const SInt16	inStringIndex);
	virtual				~MultiImageAction();
	
	virtual	void		RefreshImages	(void);
	
protected:
	PhotoItemList	mImages;				// Holds list of originally selected image pointers
	bool			mOwnsImages;
};

