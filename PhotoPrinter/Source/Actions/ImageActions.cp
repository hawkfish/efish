/*
	File:		ImageActions.cp

	Contains:	Implementation of various undoable actions on images.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		15 Aug 2000		drd		CropZoomAction
		14 Aug 2000		drd		CropAction, ImageAction
		04 Aug 2000		drd		Created
*/

#include "ImageActions.h"
#include "PhotoPrintConstants.h"
#include "PhotoPrintDoc.h"
#include "PhotoPrintResources.h"
#include "PhotoPrintView.h"

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
	, mOldBounds(mImage->GetDestRect())
{
	// We don't want to actually use the new crop, it will determine our zoom instead
	SInt16	zw = mNewCrop.Width();
	SInt16	zh = mNewCrop.Height();
	MRect	image = mImage->GetImageRect();
	SInt16	cw = image.Width();
	SInt16	ch = image.Height();
	double	ratio = std::min((double)cw / zw, (double)ch / zh);
	image.SetWidth(cw * ratio);
	image.SetHeight(ch * ratio);
	mNewCrop = image;
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
	mImage->SetImageRect(mNewCrop);
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
