/*
	File:		PhotoPrintAction.h

	Contains:	Interface to the basic document action.

	Written by:	David Dunham and Dav Lion

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		24 Jul 2001		rmgw	Save dirty state.
		18 Jul 2001		rmgw	Provide accessors for MVC values.
		18 Jul 2001		rmgw	Split up ImageActions.
*/

#pragma once

#include <LAction.h>

class	PhotoPrintDoc;
class	PhotoPrintModel;
class	PhotoPrintView;

class	PhotoPrintAction : public LAction
{
public:
						PhotoPrintAction(
									PhotoPrintDoc*	inDoc,
									const SInt16	inStringIndex,
									const Boolean	inAlreadyDone);
						~PhotoPrintAction();

		// LAction
	virtual	void		Redo();
	virtual	void		Undo();

protected:

	bool				mUndoDirty;

		//	Accessors
	PhotoPrintDoc*		GetDocument	(void) const;
	PhotoPrintView*		GetView		(void) const;
	PhotoPrintModel*	GetModel	(void) const;
	
		//	Construction/Destruction Utilities
	bool				GetCurrentDirty		(void) const;
		
		//	General utilities
	virtual	void		LayoutImages();
		
private:
	PhotoPrintDoc*		mDoc;
};

