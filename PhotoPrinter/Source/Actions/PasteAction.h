/*
	File:		PasteAction.h

	Contains:	Interface to the paste action.

	Written by:	David Dunham and Dav Lion

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		26 Jul 2001		rmgw	Factor XML parsing.  Bug #228.
		18 Jul 2001		rmgw	Split up ImageActions.
*/

#pragma once

#include "PhotoPrintAction.h"

#include "PhotoPrintItem.h"

class	PasteAction : public	PhotoPrintAction
{
public:
						PasteAction(
									PhotoPrintDoc*	inDoc,
									const SInt16	inStringIndex,
									const ScrapFlavorType	inType,
									Handle			inData);
						~PasteAction();

	// XMLItemParser
	virtual	void		OnItemRead	(PhotoItemRef inItem);

protected:
	// LAction
	virtual	void		RedoSelf();
	virtual	void		UndoSelf();

	PhotoItemList	mInsertedImages;
	bool			mOwnsImages;
};

