/*
	File:		ImageActions.cp

	Contains:	Implementation of various undoable actions on images.

	Written by:	David Dunham and Dav Lion

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		21 Sep 2000		drd		Exception handling class renamed to ExceptionHandler
		20 sep 2000		dml		use PhotoExceptionHandler
		15 sep 2000		dml		fix multiple-crop-zoom 
		15 Sep 2000		drd		LayoutImages, called by DeleteAction
		23 aug 2000		dml		crops are now stored as doubles (percentages)
		22 aug 2000		dml		add offset to CropAction
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
#include "Layout.h"
#include "PhotoPrintConstants.h"
#include "PhotoPrintDoc.h"
#include "PhotoPrintResources.h"
#include "PhotoPrintView.h"
#include "AlignmentGizmo.h"
#include "PhotoExceptionHandler.h"

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
LayoutImages
*/
void
PhotoPrintAction::LayoutImages()
{
	mView->Refresh();								// Doc orientation may change, so refresh before AND after
	mView->GetLayout()->LayoutImages();
	mView->Refresh();
} // LayoutImages

/*
Redo {OVERRIDE}
*/
void
PhotoPrintAction::Redo()
{
	try {
		if (this->CanRedo()) {
			this->RedoSelf();

			// Mark the model as dirty
			mModel->SetDirty();
			}
		mIsDone = true;
		}//end try

	catch (LException e) {
		// Mark the model as dirty
		mModel->SetDirty();
		if (!ExceptionHandler::HandleKnownExceptions(e))
			throw;
		}//end catch
} // Redo

/*
Undo {OVERRIDE}
*/
void
PhotoPrintAction::Undo()
{
	try {
		if (this->CanUndo()) {
			this->UndoSelf();

			// Mark the model as dirty
			mModel->SetDirty();
		}

		mIsDone = false;
		}//end try

	catch (LException e) {
		// Mark the model as dirty
		mModel->SetDirty();
		if (!ExceptionHandler::HandleKnownExceptions(e))
			throw;		
		}//end catch
} // Undo

#pragma mark -

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
	mModel->SetDirty();		// !!! need to be more precise
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

#pragma mark -

/*
DeleteAction
*/
DeleteAction::DeleteAction(
	PhotoPrintDoc*	inDoc,
	const SInt16	inStringIndex)
	: MultiImageAction(inDoc, inStringIndex)
{
	mAllImages.assign(mModel->begin(), mModel->end());
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

	this->LayoutImages();

	mOwnsImages = true;
} // RedoSelf

/*
UndoSelf {OVERRIDE}
*/
void
DeleteAction::UndoSelf()
{
	PhotoIterator	i;

	// There's no API to insert stuff at a particular point in the list, so start with
	// a clean slate and add all the ones that used to be there back
	mModel->DeleteAll();					// Gets rid of its items, but not the image data
	for (i = mAllImages.begin(); i != mAllImages.end(); i++) {
		mModel->AdoptNewItem(*i);
	}
	mView->AddToSelection(mImages);

	this->LayoutImages();

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
	