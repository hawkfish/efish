/*
	File:		ImageActions.h

	Contains:	Definition of various undoable actions on images.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		16 aug 2000		dml		cleanup CropZoomAction
		15 aug 2000		dml		add RotateAction
		15 Aug 2000		drd		CropZoomAction
		14 Aug 2000		drd		CropAction, ImageAction
		04 Aug 2000		drd		Created
*/

#pragma once

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

class	ImageAction : public PhotoPrintAction
{
public:
						ImageAction(
									PhotoPrintDoc*	inDoc,
									const SInt16	inStringIndex);
						~ImageAction();

protected:
	PhotoItemRef	mImage;					// Holds originally selected image
};

class	CropAction : public ImageAction
{
public:
						CropAction(
									PhotoPrintDoc*	inDoc,
									const SInt16	inStringIndex,
									const MRect&	inNewCrop);
						~CropAction();

protected:
	// LAction
	virtual	void		RedoSelf();
	virtual	void		UndoSelf();

	MRect			mNewCrop;
	MRect			mOldCrop;
};

class	CropZoomAction : public CropAction
{
public:
						CropZoomAction(
									PhotoPrintDoc*	inDoc,
									const SInt16	inStringIndex,
									const MRect&	inNewCrop);
						~CropZoomAction();

protected:
	// LAction
	virtual	void		RedoSelf();
	virtual	void		UndoSelf();

	MRect			mOldBounds;
	MRect			mNewImage;
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


class	RotateAction : public ImageAction
{
public:
						RotateAction(
									PhotoPrintDoc*	inDoc,
									const SInt16	inStringIndex,
									double inRot);
						~RotateAction();

protected:
	double		mOldRot;
	double		mNewRot;

	// LAction
	virtual	void		RedoSelf();
	virtual	void		UndoSelf();
};


