/*
	File:		ImageActions.cp

	Contains:	Implementation of various undoable actions on images.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		21 aug 2000		dml		added a pragma mark - near rotationaction
		21 aug 2000		dml		CalcCropValuesAsRect must respect EmptyRect flag == noCrop, CropZoom ct also
		18 aug 2000		dml		rewrite cropping as relative
		17 aug 2000		dml		perform cropZoom at origin, offset afterwords (!)
		16 aug 2000		dml		tweaks on CropZoomAction
		15 aug 2000		dml		add RotateAction
		15 Aug 2000		drd		CropZoomAction
		14 Aug 2000		drd		CropAction, ImageAction
		04 Aug 2000		drd		Created
*/

#include "ImageActions.h"
#include "PhotoPrintConstants.h"
#include "PhotoPrintDoc.h"
#include "PhotoPrintResources.h"
#include "PhotoPrintView.h"
#include "AlignmentGizmo.h"

/*
PhotoPrintAction
*/
PhotoPrintAction::PhotoPrintAction(
	PhotoPrintDoc*	inDoc,
	const SInt16	inStringIndex,
	const Boolean	inAlreadyDone)
	: LAction(str_Redo, inStringIndex, inAlreadyDone)
{
	mDoc = inDoc;
	mView = inDoc->GetView();
	mModel = mView->GetModel();
} // PhotoPrintAction

/*
~PhotoPrintAction
*/
PhotoPrintAction::~PhotoPrintAction()
{
} // ~PhotoPrintAction

/*
CanRedo {OVERRIDE}
*/
Boolean
PhotoPrintAction::CanRedo() const
{
	return !this->IsDone() && mDoc->IsOnDuty();
} // CanRedo

/*
CanUndo {OVERRIDE}
*/
Boolean
PhotoPrintAction::CanUndo() const
{
	return this->IsDone() && mDoc->IsOnDuty();
} // CanUndo

/*
Redo {OVERRIDE}
*/
void
PhotoPrintAction::Redo()
{
	if (this->CanRedo()) {
		this->RedoSelf();

		// Mark the model as dirty
		mModel->SetDirty();
	}

	mIsDone = true;
} // Redo

/*
Undo {OVERRIDE}
*/
void
PhotoPrintAction::Undo()
{
	if (this->CanUndo()) {
		this->UndoSelf();

		// Mark the model as dirty
		mModel->SetDirty();
	}

	mIsDone = false;
} // Undo

#pragma mark -

/*
CropAction
*/
CropAction::CropAction(
	PhotoPrintDoc*	inDoc,
	const SInt16	inStringIndex,
	const MRect&	inNewCrop)
	: ImageAction(inDoc, inStringIndex)
{
	mImage->GetCrop(mOldCrop.top, mOldCrop.left, mOldCrop.bottom, mOldCrop.right);
	MRect image			(mImage->GetImageRect());
	
	CalcCropValuesAsRect(inNewCrop, image, mNewCrop);	
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
	mImage->SetCrop(mNewCrop.top, mNewCrop.left, mNewCrop.bottom, mNewCrop.right);
	mModel->SetDirty();		// !!! need to be more precise
} // RedoSelf



void
CropAction::CalcCropValuesAsRect(const MRect& inCrop, const MRect& inBounds, MRect& outStuffedRect) {
	
	if (!inCrop) {// if incoming crop is empty rect
		outStuffedRect = inCrop; // then pass through as empty rect (== no crop)
		}//endif
	else {		
		double height	(inBounds.Height() / 100.);
		double width 	(inBounds.Width() / 100.);
		
		MRect	clampedCrop (inCrop);
		clampedCrop *= inBounds;
		
		outStuffedRect.top = (clampedCrop.top - inBounds.top) / height;
		outStuffedRect.left = (clampedCrop.left - inBounds.left) / width;
		outStuffedRect.bottom = (inBounds.bottom - clampedCrop.bottom) / height;
		outStuffedRect.right = (inBounds.right - clampedCrop.right) / width;
		}//else must calculate
		
	}//end CalcCropValuesAsRect


/*
UndoSelf {OVERRIDE}
*/
void
CropAction::UndoSelf()
{
	mImage->SetCrop(mOldCrop.top, mOldCrop.left, mOldCrop.bottom, mOldCrop.right);
	mModel->SetDirty();		// !!! need to be more precise
} // UndoSelf

#pragma mark -

/*
CropZoomAction
*/
CropZoomAction::CropZoomAction(
	PhotoPrintDoc*	inDoc,
	const SInt16	inStringIndex,
	const MRect&	inNewCrop)
	: CropAction(inDoc, inStringIndex, inNewCrop)
	, mOldBounds(mImage->GetImageRect())
{
	mImage->GetCropZoomScales(mOldXScale, mOldYScale);
	mImage->GetCropZoomOffset(mOldTopOffset, mOldLeftOffset);

	if (!inNewCrop) {
		mNewXScale = mNewYScale = 1.0;
		mNewTopOffset = mNewLeftOffset = 0.0;
		}//endif empty crop rect means turn cropzoom off
	else {
		// the image rect already has the caption rect removed
		MRect	image = mImage->GetImageRect();

		// pretend the image is at origin for all intermediate calculations
		SInt16	offsetLeft (image.left);
		SInt16 	offsetTop	(image.top);
		image.Offset(-offsetLeft, -offsetTop);
		// which means we must adjust the crop rect also
		MRect newCrop (inNewCrop);
		newCrop.Offset(-offsetLeft, -offsetTop);
		// and work with an offset copy of the original bounds
		MRect oldBoundsAtOrigin 	(mOldBounds);
		oldBoundsAtOrigin.Offset(-offsetLeft, -offsetTop);

		newCrop *= image; // intersect crop rect with imageRect (controller doesn't know about caption/image separation)
		
		// derive various measurements of the crop and image rects
		SInt16	zw = newCrop.Width();
		SInt16	zh = newCrop.Height();
		SInt16	cw = image.Width();
		SInt16	ch = image.Height();
		// we get to expand by the smaller of the ratios
		double	ratio = std::min((double)cw / zw, (double)ch / zh);

		// this is the full size of the expanded image
		image.SetWidth(cw * ratio);
		image.SetHeight(ch * ratio);

		// now make just the expanded cropped rect
		// start with the full expanded rect
		MRect croppedExpanded (image);
		// offset over to the start of the cropped section
		croppedExpanded.Offset(newCrop.left * ratio, newCrop.top * ratio);
		// set the width and height
		croppedExpanded.SetWidth(newCrop.Width() * ratio);
		croppedExpanded.SetHeight(newCrop.Height() * ratio);

		MRect croppedExpandedCentered;
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

		CalcCropValuesAsRect(croppedExpandedCentered, oldBoundsAtOrigin, mNewCrop);
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
	mImage->SetCrop(mNewCrop.top, mNewCrop.left, mNewCrop.bottom, mNewCrop.right);
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
//	mImage->SetImageRect(mOldBounds);
	mImage->SetCrop(mOldCrop.top, mOldCrop.left, mOldCrop.bottom, mOldCrop.right);
	mImage->SetCropZoomScales(mOldXScale, mOldYScale);
	mImage->SetCropZoomOffset(mOldTopOffset, mOldLeftOffset);
	mImage->DeleteProxy();
	mModel->SetDirty();		// !!! need to be more precise
} // UndoSelf

#pragma mark -

/*
DeleteAction
*/
DeleteAction::DeleteAction(
	PhotoPrintDoc*	inDoc,
	const SInt16	inStringIndex)
	: MultiImageAction(inDoc, inStringIndex)
{
} // DeleteAction

/*
~DeleteAction
*/
DeleteAction::~DeleteAction()
{
} // ~DeleteAction

/*
RedoSelf {OVERRIDE}
*/
void
DeleteAction::RedoSelf()
{
	// take them all away
	mView->RemoveFromSelection(mImages);
	mModel->DeleteItems(mImages, PhotoPrintModel::kRemove);

	mOwnsImages = true;
} // RedoSelf

/*
UndoSelf {OVERRIDE}
*/
void
DeleteAction::UndoSelf()
{
	PhotoIterator	i;

	for (i = mImages.begin(); i != mImages.end(); i++) {
		mModel->AdoptNewItem(*i);
	}
	mView->AddToSelection(mImages);

	mOwnsImages = false;
} // UndoSelf

#pragma mark -

/*
ImageAction
*/
ImageAction::ImageAction(
	PhotoPrintDoc*	inDoc,
	const SInt16	inStringIndex)
	: PhotoPrintAction(inDoc, inStringIndex, kNotAlreadyDone)
{
	mImage = mView->GetPrimarySelection();
} // ImageAction

/*
~ImageAction
*/
ImageAction::~ImageAction()
{
} // ~ImageAction

#pragma mark -

/*
MultiImageAction
*/
MultiImageAction::MultiImageAction(
	PhotoPrintDoc*	inDoc,
	const SInt16	inStringIndex)
	: PhotoPrintAction(inDoc, inStringIndex, kNotAlreadyDone)
{
	// Copy the selection
	mImages = mView->Selection();

	mOwnsImages = false;
} // MultiImageAction

/*
~MultiImageAction
*/
MultiImageAction::~MultiImageAction()
{
	// Delete images if they have been deleted
	if (mOwnsImages) {
		PhotoIterator	i;
		for (i = mImages.begin(); i != mImages.end(); i++) {
			delete (*i);
		}
	}
} // ~MultiImageAction


#pragma mark -


RotateAction::RotateAction(PhotoPrintDoc*	inDoc,
							const SInt16	inStringIndex,
							double inRot) 
	: ImageAction(inDoc, inStringIndex)
	, mOldRot(mImage->GetRotation())
	, mNewRot(inRot)
{
	}//end ct


RotateAction::~RotateAction() {
	}//end dt


void
RotateAction::RedoSelf() {
	mImage->SetRotation(mNewRot);
	mModel->SetDirty();		// !!! need to be more precise
	}//end RedoSelf
	
void
RotateAction::UndoSelf() {
	mImage->SetRotation(mOldRot);
	mModel->SetDirty();		// !!! need to be more precise
	}//end UndoSelf	
	