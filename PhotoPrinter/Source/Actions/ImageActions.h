/*
	File:		ImageActions.h

	Contains:	Definition of various undoable actions on images.

	Written by:	David Dunham and Dav Lion

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		15 sep 2000		dml		fix multiple crop zooms
		15 Sep 2000		drd		Added LayoutImages, mAllImages
		23 aug 2000		dml		crop stored as doubles (percentages).  Offset used in both crop + cropzoom
		18 aug 2000		dml		make crop relative
		16 aug 2000		dml		cleanup CropZoomAction
		15 aug 2000		dml		add RotateAction
		15 Aug 2000		drd		CropZoomAction
		14 Aug 2000		drd		CropAction, ImageAction
		04 Aug 2000		drd		Created
*/

#pragma once

#include <LAction.h>

#include "PhotoPrintItem.h"
#include "ERect32.h"

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
	virtual	void		LayoutImages();

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


//-----------------------------------------------------------
// Cropping (and CropZooming)
// even though cropping is now relative, and specified by independent values
// we'll still maintain the dang values glommed inside rects because it's convenient
// this will break horribly if SInt16's aren't what's needed upstream
//-----------------------------------------------------------

class	CropAction : public ImageAction
{
public:
						CropAction(
									PhotoPrintDoc*	inDoc,
									const SInt16	inStringIndex,
									const MRect&	inNewCrop,
									const double	topOffset = 0.0,
									const double	leftOffset = 0.0);
						~CropAction();

protected:
	// LAction
	virtual	void		RedoSelf();
	virtual	void		UndoSelf();

	virtual void		CalcCropValuesAsPercentages(const ERect32& inCrop, const ERect32& inBounds, 
													double& outTopCrop, double& outLeftCrop, 
													double& outBottomCrop, double& outRightCrop);

	double			mOldTopCrop;
	double			mOldLeftCrop;
	double			mOldBottomCrop;
	double			mOldRightCrop;
	
	double			mNewTopCrop;
	double			mNewLeftCrop;
	double			mNewBottomCrop;
	double			mNewRightCrop;
	
	double			mOldTopOffset;
	double			mOldLeftOffset;

	double			mNewTopOffset;
	double			mNewLeftOffset;
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

	double			mOldXScale;
	double			mOldYScale;
		
	double			mNewXScale;
	double			mNewYScale;
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

	PhotoItemList	mAllImages;				// Holds list of original image pointers
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
