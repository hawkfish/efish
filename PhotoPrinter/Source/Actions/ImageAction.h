/*
	File:		ImageAction.h

	Contains:	Interface to a single image action.

	Written by:	David Dunham and Dav Lion

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		24 Jul 2001		rmgw	Add RefreshImage.  Bug #220.
		18 Jul 2001		rmgw	Split up ImageActions.
*/

#pragma once

#include "PhotoPrintAction.h"

#include "PhotoPrintItem.h"

class	ImageAction : public PhotoPrintAction
{
public:
						ImageAction(
									PhotoPrintDoc*	inDoc,
									const SInt16	inStringIndex);
	virtual				~ImageAction();
	
	virtual	void		RefreshImage	(void);
	
protected:
	PhotoItemRef	mImage;					// Holds originally selected image
};
