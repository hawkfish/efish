/*
	File:		DeleteAction.h

	Contains:	Interface to the selection deletion action.

	Written by:	David Dunham and Dav Lion

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		18 Jul 2001		rmgw	Split up ImageActions.
*/

#pragma once

#include "MultiImageAction.h"

class	DeleteAction : public MultiImageAction
{
public:
						DeleteAction(
									PhotoPrintDoc*	inDoc,
									const SInt16	inStringIndex);
						~DeleteAction();

protected:
	// LAction
	virtual	void		RedoSelf();
	virtual	void		UndoSelf();

	PhotoItemList	mAllImages;				// Holds list of original image pointers
};


