/*
	File:		ImageAction.cp

	Contains:	Implementation of a single image action.

	Written by:	David Dunham and Dav Lion

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		24 Jul 2001		rmgw	Add RefreshImage.  Bug #220.
		18 Jul 2001		rmgw	Provide accessors for MVC values.
		18 Jul 2001		rmgw	Split up ImageActions.
*/

#include "ImageAction.h"

#include "PhotoPrintConstants.h"
#include "PhotoPrintView.h"

/*
ImageAction
*/
ImageAction::ImageAction(
	PhotoPrintDoc*	inDoc,
	const SInt16	inStringIndex)
	: PhotoPrintAction(inDoc, inStringIndex, kNotAlreadyDone)
{
	mUndoDirty = true;	//	Needed because of kNotAlreadyDone
	
	mImage = GetView ()->GetPrimarySelection();
} // ImageAction

/*
~ImageAction
*/
ImageAction::~ImageAction()
{
} // ~ImageAction

/*
RefreshImage
*/
void
ImageAction::RefreshImage(void)
{
	PhotoPrintModel*	model (GetModel ());
	ConstPhotoIterator	i = std::find (model->begin (), model->end (), mImage);
	Assert_(i != model->end ());
	
	PhotoPrintModel::MessageRange		range = {i, i + 1};
	GetModel ()->BroadcastMessage (PhotoPrintModel::msg_ModelItemsChanged, &range);
	
} // RefreshImage

