/*
	File:		CropAction.h

	Contains:	Interface to a crop action.

	Written by:	David Dunham and Dav Lion

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		18 Jul 2001		rmgw	Split up ImageActions.
*/

//-----------------------------------------------------------
// Cropping (and CropZooming)
// even though cropping is now relative, and specified by independent values
// we'll still maintain the dang values glommed inside rects because it's convenient
// this will break horribly if SInt16's aren't what's needed upstream
//-----------------------------------------------------------

#pragma once

#include "ImageAction.h"

class ERect32;

class	CropAction : public ImageAction
{
public:
						CropAction(
									PhotoPrintDoc*	inDoc,
									const SInt16	inStringIndex,
									const MRect&	inNewCrop,
									const double	topOffset = 0.0,
									const double	leftOffset = 0.0);
						~CropAction();

protected:
	// LAction
	virtual	void		RedoSelf();
	virtual	void		UndoSelf();

	virtual void		CalcCropValuesAsPercentages(const ERect32& inCrop, const ERect32& inBounds, 
													double& outTopCrop, double& outLeftCrop, 
													double& outBottomCrop, double& outRightCrop);

	double			mOldTopCrop;
	double			mOldLeftCrop;
	double			mOldBottomCrop;
	double			mOldRightCrop;
	
	double			mNewTopCrop;
	double			mNewLeftCrop;
	double			mNewBottomCrop;
	double			mNewRightCrop;
	
	double			mOldTopOffset;
	double			mOldLeftOffset;

	double			mNewTopOffset;
	double			mNewLeftOffset;
};

