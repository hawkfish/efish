/*
	File:		ImageActions.cp

	Contains:	Implementation of various undoable actions on images.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

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
