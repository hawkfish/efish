/*
	File:		CropAction.cp

	Contains:	Implementation of the crop action.

	Written by:	David Dunham and Dav Lion

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		18 Jul 2001		rmgw	Provide accessors for MVC values.
		18 Jul 2001		rmgw	Split up ImageActions.
*/

#include "CropAction.h"

#include "PhotoPrintModel.h"

#include "ERect32.h"

/*
CropAction
*/
CropAction::CropAction(
	PhotoPrintDoc*	inDoc,
	const SInt16	inStringIndex,
	const MRect&	inNewCrop,
	const double	inTopOffset,
	const double	inLeftOffset)
	: ImageAction(inDoc, inStringIndex)
	, mNewTopOffset (inTopOffset)
	, mNewLeftOffset (inLeftOffset)
{
	mImage->GetCrop(mOldTopCrop, mOldLeftCrop, mOldBottomCrop, mOldRightCrop);
	mImage->GetCropZoomOffset(mOldTopOffset, mOldLeftOffset);
	ERect32	newCrop32		(inNewCrop);
	ERect32 image			(mImage->GetImageRect());
	
	CalcCropValuesAsPercentages(newCrop32, image, mNewTopCrop, mNewLeftCrop, mNewBottomCrop, mNewRightCrop);	
} // CropAction


CropAction::~CropAction()
{
} // ~CropAction


/*
RedoSelf {OVERRIDE}
*/
void
CropAction::RedoSelf()
{
	mImage->SetCrop(mNewTopCrop, mNewLeftCrop, mNewBottomCrop, mNewRightCrop);
	mImage->SetCropZoomOffset(mNewTopOffset, mNewLeftOffset);
	GetModel ()->SetDirty();		// !!! need to be more precise
} // RedoSelf


void		
CropAction::CalcCropValuesAsPercentages(const ERect32& inCrop, const ERect32& inBounds, 
										double& outTopCrop, double& outLeftCrop, 
										double& outBottomCrop, double& outRightCrop)
{	
	if (inCrop.IsEmpty()) {// if incoming crop is empty rect
		outTopCrop = outLeftCrop = outBottomCrop = outRightCrop = 0.0;
		}//endif
	else {		
		double height	(inBounds.Height() / 100.);
		double width 	(inBounds.Width() / 100.);
		
		ERect32	clampedCrop (inCrop);
		clampedCrop *= inBounds;
		
		outTopCrop = (clampedCrop.top - inBounds.top) / height;
		outLeftCrop = (clampedCrop.left - inBounds.left) / width;
		outBottomCrop = (inBounds.bottom - clampedCrop.bottom) / height;
		outRightCrop = (inBounds.right - clampedCrop.right) / width;
	}//else must calculate
}//end CalcCropValuesAsPercentages


/*
UndoSelf {OVERRIDE}
*/
void
CropAction::UndoSelf()
{
	mImage->SetCrop(mOldTopCrop, mOldLeftCrop, mOldBottomCrop, mOldRightCrop);
	mImage->SetCropZoomOffset(mOldTopOffset, mOldLeftOffset);
	GetModel ()->SetDirty();		// !!! need to be more precise
} // UndoSelf

