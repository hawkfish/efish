/*
	File:		CropZoomAction.h

	Contains:	Interface to the crop/zoom action.

	Written by:	David Dunham and Dav Lion

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		18 Jul 2001		rmgw	Split up ImageActions.
*/

#pragma once

#include "CropAction.h"

class	CropZoomAction : public CropAction
{
public:
						CropZoomAction(
									PhotoPrintDoc*	inDoc,
									const SInt16	inStringIndex,
									const MRect&	inNewCrop);
						~CropZoomAction();

protected:
	// LAction
	virtual	void		RedoSelf();
	virtual	void		UndoSelf();

	MRect			mOldBounds;
	MRect			mNewImage;

	double			mOldXScale;
	double			mOldYScale;
		
	double			mNewXScale;
	double			mNewYScale;
};

