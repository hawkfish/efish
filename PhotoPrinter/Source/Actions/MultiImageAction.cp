/*
	File:		MultiImageAction.cp

	Contains:	Implementation of a selection based action.

	Written by:	David Dunham and Dav Lion

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		24 Jul 2001		rmgw	Add RefreshImages.  Bug #220.
		18 Jul 2001		rmgw	Provide accessors for MVC values.
		18 Jul 2001		rmgw	Split up ImageActions.
*/

#include "MultiImageAction.h"

#include "PhotoPrintConstants.h"
#include "PhotoPrintView.h"

/*
MultiImageAction
*/
MultiImageAction::MultiImageAction(
	PhotoPrintDoc*	inDoc,
	const SInt16	inStringIndex)
	: PhotoPrintAction(inDoc, inStringIndex, kNotAlreadyDone)
{
	mUndoDirty = true;	//	Needed because of kNotAlreadyDone

	// Copy the selection
	mImages = GetView ()->Selection();

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

/*
RefreshImages
*/
void
MultiImageAction::RefreshImages(void)
{
	PhotoPrintModel::MessageRange		range = {mImages.begin(), mImages.end()};
	GetModel ()->BroadcastMessage (PhotoPrintModel::msg_ModelItemsChanged, &range);
	
} // RefreshImages

