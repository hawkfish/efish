/*
	File:		CropZoomAction.h

	Contains:	Implementation of the crop/zoom action.

	Written by:	David Dunham and Dav Lion

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		18 Jul 2001		rmgw	Split up ImageActions.
*/

#include "CropZoomAction.h"

#include "AlignmentGizmo.h"
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
		}//endif empty crop rect means turn cropzoom off
	else {
		MRect tempImage;
		mImage->GetExpandedOffsetImageRect(tempImage);
		ERect32 image (tempImage); // image will become too big to fit in a normal Rect

		// pretend the image is at origin for all intermediate calculations
		SInt32	offsetLeft (image.left);
		SInt32 	offsetTop	(image.top);
		image.Offset(-offsetLeft, -offsetTop);
		// which means we must adjust the crop rect also
		ERect32 newCrop (inNewCrop);
		newCrop.Offset(-offsetLeft, -offsetTop);
		// and work with an offset copy of the original bounds
		ERect32 oldBoundsAtOrigin 	(tempImage);
		oldBoundsAtOrigin.Offset(-offsetLeft, -offsetTop);

		newCrop *= image; // intersect crop rect with imageRect (controller doesn't know about caption/image separation)
		
		// derive various measurements of the crop and image rects
		SInt32	zw = newCrop.Width();
		SInt32	zh = newCrop.Height();
		SInt32	cw = image.Width();
		SInt32	ch = image.Height();
		// we get to expand by the smaller of the ratios
		double	ratio = std::min((double)cw / zw, (double)ch / zh);

		// this is the full size of the expanded image
		image.SetWidth(cw * ratio);
		image.SetHeight(ch * ratio);

		// now make just the expanded cropped rect
		// start with the full expanded rect
		ERect32 croppedExpanded (image);
		// offset over to the start of the cropped section
		croppedExpanded.Offset(newCrop.left * ratio, newCrop.top * ratio);
		// set the width and height
		croppedExpanded.SetWidth(newCrop.Width() * ratio);
		croppedExpanded.SetHeight(newCrop.Height() * ratio);

		ERect32 croppedExpandedCentered;
		//center the cropped + expanded rect inside the original bounds
		AlignmentGizmo::AlignRectInside(croppedExpanded, oldBoundsAtOrigin, kAlignAbsoluteCenter, croppedExpandedCentered);

		// offset the full expanded image so that the cropped expanded portion lines up with previous calculation
		image.Offset(-((newCrop.left * ratio) - croppedExpandedCentered.left),
					-((newCrop.top * ratio) - croppedExpandedCentered.top));

		
		// return from origin to previous location
		image.Offset(offsetLeft, offsetTop);
		
		
		// support for the new relative cropping
		mNewTopOffset = (-((newCrop.top * ratio) - croppedExpandedCentered.top)) / (double)image.Height();
		mNewLeftOffset = -((newCrop.left * ratio) - croppedExpandedCentered.left) / (double)image.Width();
		mNewXScale = ratio;
		mNewYScale = ratio;

		CalcCropValuesAsPercentages(croppedExpandedCentered, oldBoundsAtOrigin, 
									mNewTopCrop, mNewLeftCrop, mNewBottomCrop, mNewRightCrop);
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
//	mImage->SetImageRect(mNewImage);
	mImage->SetCrop(mNewTopCrop, mNewLeftCrop, mNewBottomCrop, mNewRightCrop);
	mImage->SetCropZoomScales(mNewXScale, mNewYScale);
	mImage->SetCropZoomOffset(mNewTopOffset, mNewLeftOffset);
	mImage->DeleteProxy();
	mModel->SetDirty();		// !!! need to be more precise
} // RedoSelf

/*
UndoSelf {OVERRIDE}
*/
void
CropZoomAction::UndoSelf()
{
	mImage->SetCrop(mOldTopCrop, mOldLeftCrop, mOldBottomCrop, mOldRightCrop);
	mImage->SetCropZoomScales(mOldXScale, mOldYScale);
	mImage->SetCropZoomOffset(mOldTopOffset, mOldLeftOffset);
	mImage->DeleteProxy();
	mModel->SetDirty();		// !!! need to be more precise
} // UndoSelf

