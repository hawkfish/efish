/*
	File:		RotateAction.h

	Contains:	Implementation of the rotate item action.

	Written by:	David Dunham and Dav Lion

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		18 Jul 2001		rmgw	Split up ImageActions.
*/

#include "RotateAction.h"

#include "PhotoPrintDoc.h"

RotateAction::RotateAction(PhotoPrintDoc*	inDoc,
							const SInt16	inStringIndex,
							double inRot,
							const Rect* inDest) 
	: ImageAction(inDoc, inStringIndex)
	, mOldRot(mImage->GetRotation())
	, mOldDest(mImage->GetDestRect())
	, mNewRot(inRot)
{
	if (inDest != nil)
		mNewDest = *inDest;
	else
		mNewDest = mOldDest;
	}//end ct


RotateAction::~RotateAction() {
	}//end dt


void
RotateAction::RedoSelf() {
	mImage->SetRotation(mNewRot);
	PhotoDrawingProperties	drawProps (false, false, false, mDoc->GetResolution());
	mImage->SetDest(mNewDest, drawProps);
	mModel->SetDirty();		// !!! need to be more precise
	}//end RedoSelf
	
void
RotateAction::UndoSelf() {
	mImage->SetRotation(mOldRot);
	PhotoDrawingProperties	drawProps (false, false, false, mDoc->GetResolution());
	mImage->SetDest(mOldDest, drawProps);
	mModel->SetDirty();		// !!! need to be more precise
	}//end UndoSelf	
	