/*
	File:		ImageAction.cp

	Contains:	Implementation of a single image action.

	Written by:	David Dunham and Dav Lion

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

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
	mImage = GetView ()->GetPrimarySelection();
} // ImageAction

/*
~ImageAction
*/
ImageAction::~ImageAction()
{
} // ~ImageAction

