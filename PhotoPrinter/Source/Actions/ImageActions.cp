/*
	File:		ImageActions.cp

	Contains:	Implementation of various undoable actions on images.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

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
	, mOldCrop(mImage->GetCrop())
	, mNewCrop(inNewCrop)
{
	mOldCrop = mImage->GetCrop();
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
	mImage->SetCrop(mNewCrop);
	mModel->SetDirty();		// !!! need to be more precise
} // RedoSelf

/*
UndoSelf {OVERRIDE}
*/
void
CropAction::UndoSelf()
{
	mImage->SetCrop(mOldCrop);
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
	// We don't want to actually use the new crop, it will determine our zoom instead
	
	// the image rect already has the caption rect removed
	MRect	image = mImage->GetImageRect();
	mNewCrop *= image; // intersect crop rect with imageRect (controller doesn't know about caption/image separation)
	
	// derive various measurements of the crop and image rects
	SInt16	zw = mNewCrop.Width();
	SInt16	zh = mNewCrop.Height();
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
	croppedExpanded.Offset(mNewCrop.left * ratio, mNewCrop.top * ratio);
	// set the width and height
	croppedExpanded.SetWidth(mNewCrop.Width() * ratio);
	croppedExpanded.SetHeight(mNewCrop.Height() * ratio);

	MRect croppedExpandedCentered;
	//center the cropped + expanded rect inside the original bounds
	AlignmentGizmo::AlignRectInside(croppedExpanded, mOldBounds, kAlignAbsoluteCenter, croppedExpandedCentered);

	// offset the full expanded image so that the cropped expanded portion lines up with previous calculation
	image.Offset(-((mNewCrop.left * ratio) - croppedExpandedCentered.left),
				-((mNewCrop.top * ratio) - croppedExpandedCentered.top));

	
	mNewImage = image;
	mNewCrop = croppedExpandedCentered;

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
	mImage->SetImageRect(mNewImage);
	mImage->SetCrop(mNewCrop);
	mImage->DeleteProxy();
	mModel->SetDirty();		// !!! need to be more precise
} // RedoSelf

/*
UndoSelf {OVERRIDE}
*/
void
CropZoomAction::UndoSelf()
{
	mImage->SetImageRect(mOldBounds);
	mImage->SetCrop(mOldCrop);
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
	