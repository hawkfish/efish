/*
	File:		CropZoomAction.h

	Contains:	Implementation of the crop/zoom action.

	Written by:	David Dunham and Dav Lion

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		04 sep 2001		dml		344.  calling superclass ct no longer sufficient.  must get CropZoom, not Crop for old values
		31 aug 2001		dml		275, 282.  refactor and fix
		24 Jul 2001		rmgw	Refresh the image.  Bug #220.
		24 Jul 2001		rmgw	Undo dirty state correctly.
		18 Jul 2001		rmgw	Provide accessors for MVC values.
		18 Jul 2001		rmgw	Split up ImageActions.
*/

#include "CropZoomAction.h"

#include "AlignmentGizmo.h"
#include "PhotoPrintDoc.h"
#include "PhotoPrintModel.h"

#include "ERect32.h"

/*
CropZoomAction
*/
CropZoomAction::CropZoomAction(
	PhotoPrintDoc*	inDoc,
	const SInt16	inStringIndex,
	const MRect&	inNewCrop)
	: CropAction(inDoc, inStringIndex, inNewCrop)
{

	mImage->GetCropZoomScales(mOldXScale, mOldYScale);
	mImage->GetCropZoomOffset(mOldTopOffset, mOldLeftOffset);

	if (!inNewCrop) {
		mNewXScale = mNewYScale = 1.0;
		mNewTopOffset = mNewLeftOffset = 0.0;
		// superclass ct takes care of zeroing out mTopCrop... if emptyRect sent in
		}//endif empty crop rect means turn cropzoom off
	else {
		mImage->GetCropZoom(mOldTopCrop, mOldLeftCrop, mOldBottomCrop, mOldRightCrop);

		ERect32 cropZoomRect;
		ERect32 expandedOffsetImage;
		mImage->DeriveCropZoomRect(cropZoomRect, expandedOffsetImage); // existing scale and offset
		
		// 4 percentages which represent crop-zoom-interest-rectangle as %ages of image-space
		// since calculated against existing expanded + offset cropZoomRect
		PhotoUtility::CalcCropValuesAsPercentages(inNewCrop, expandedOffsetImage, 
									mNewTopCrop, mNewLeftCrop, mNewBottomCrop, mNewRightCrop, kClampToBounds);
		}//else actual crop, need to calculate

} // CropZoomAction

CropZoomAction::~CropZoomAction()
{
} // ~CropZoomAction

/*
RedoSelf {OVERRIDE}
*/
void
CropZoomAction::RedoSelf()
{
		//	Get the new undo state
	bool				mRedoDirty (GetCurrentDirty ());

		//	Swap the values
	mImage->SetCropZoom(mNewTopCrop, mNewLeftCrop, mNewBottomCrop, mNewRightCrop);
	mImage->SetCropZoomOffset(mNewTopOffset, mNewLeftOffset);
	mImage->DeleteProxy();
	
	//	Redraw it
	RefreshImage ();
	
	//	Restore the dirty flag
	GetDocument ()->SetDirty (mUndoDirty);
	
	//	Swap the state
	mUndoDirty = mRedoDirty;

} // RedoSelf

/*
UndoSelf {OVERRIDE}
*/
void
CropZoomAction::UndoSelf()
{
		//	Get the new undo state
	bool				mRedoDirty (GetCurrentDirty ());

		//	Swap the values
	mImage->SetCropZoom(mOldTopCrop, mOldLeftCrop, mOldBottomCrop, mOldRightCrop);
	mImage->SetCropZoomOffset(mOldTopOffset, mOldLeftOffset);
	mImage->DeleteProxy();
	
	//	Redraw it
	RefreshImage ();

	//	Restore the dirty flag
	GetDocument ()->SetDirty (mUndoDirty);
	
	//	Swap the state
	mUndoDirty = mRedoDirty;

} // UndoSelf

