/*
	File:		DeleteAction.cp

	Contains:	Impelentation the selection deletion action.

	Written by:	David Dunham and Dav Lion

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		18 Jul 2001		rmgw	Split up ImageActions.
*/

#include "DeleteAction.h"

#include "FixedLayout.h"
#include "PhotoPrintModel.h"

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
	// if we're operating on a FixedLayout, we have to replace deleted items with empties
	FixedLayout* fixed (dynamic_cast<FixedLayout*>(mView->GetLayout()));
	bool patchFixedLayout (fixed != nil);
	long howMany (0);
	if (patchFixedLayout)		
		howMany = fixed->GetImageCount();

	// take them all away
	mView->RemoveFromSelection(mImages);
	mModel->RemoveItems(mImages, PhotoPrintModel::kRemove);

	if (patchFixedLayout)
		fixed->SetImageCount(howMany);
		
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
	mModel->RemoveAllItems();					// Gets rid of its items, but not the image data
	for (i = mAllImages.begin(); i != mAllImages.end(); ++i) {
		mModel->AdoptNewItem(*i, mModel->end ());
	}
	mView->AddToSelection(mImages);

	this->LayoutImages();

	mOwnsImages = false;
} // UndoSelf

