/*
	File:		ImageActions.h

	Contains:	Definition of various undoable actions on images.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		04 Aug 2000		drd		Created
*/

#include <LAction.h>

#include "PhotoPrintItem.h"

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
	virtual	Boolean		CanRedo() const;
	virtual	Boolean		CanUndo() const;
	virtual	void		Redo();
	virtual	void		Undo();

protected:
	PhotoPrintDoc*			mDoc;
	PhotoPrintModel*		mModel;
	PhotoPrintView*			mView;
};

class	MultiImageAction : public PhotoPrintAction
{
public:
						MultiImageAction(
									PhotoPrintDoc*	inDoc,
									const SInt16	inStringIndex);
						~MultiImageAction();

protected:
	PhotoItemList	mImages;				// Holds list of originally selected image pointers
	bool			mOwnsImages;
};

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
};
