/*
	File:		RotateAction.h

	Contains:	Implementation of the rotate item action.

	Written by:	David Dunham and Dav Lion

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		24 Jul 2001		rmgw	Refresh the image.  Bug #220.
		24 Jul 2001		rmgw	Undo dirty state correctly.
		23 jul 2001		dml		179 add RefreshItemOrLayout
		18 Jul 2001		rmgw	Split up ImageActions.
*/

#include "RotateAction.h"

#include "PhotoPrintDoc.h"
#include "StDisableBroadcaster.h"

RotateAction::RotateAction(PhotoPrintDoc*	inDoc,
							const SInt16	inStringIndex,
							double inRot,
							const Rect* inDest) 
	: ImageAction(inDoc, inStringIndex)
	, mOldRot(mImage->GetRotation())
	, mOldDest(mImage->GetDestRect())
	, mNewRot(inRot)
{
	mOldOrientation = GetView()->GetLayout()->GetOrientation();

	if (inDest != nil)
		mNewDest = *inDest;
	else
		mNewDest = mOldDest;

	}//end ct


RotateAction::~RotateAction() {
	}//end dt


void
RotateAction::RedoSelf() {
		//	Get the new undo state
	bool				mRedoDirty (GetCurrentDirty ());

		//	Swap the values
	mImage->SetRotation(mNewRot);
	PhotoDrawingProperties	drawProps (false, false, false, GetDocument ()->GetResolution());
	mImage->SetDest(mNewDest, drawProps);
	RefreshItemOrLayout();

	//	Restore the dirty flag
	GetDocument ()->SetDirty (mUndoDirty);
	
	//	Swap the state
	mUndoDirty = mRedoDirty;

	}//end RedoSelf


void
RotateAction::RefreshItemOrLayout() {
	OSType	newOrientation (GetView()->GetLayout()->CalcOrientation());
	
	if (mOldOrientation == newOrientation) {
		RefreshImage();
		}//endif optimizing for prettiness
	else {
		GetView()->Refresh(); //inval cur page cause we will likely switch
		GetView()->GetLayout()->LayoutImages();
		}//end
		
	}//end RefreshItemOrLayout

	
void
RotateAction::UndoSelf() {
		//	Get the new undo state
	bool				mRedoDirty (GetCurrentDirty ());

		//	Swap the values
	mImage->SetRotation(mOldRot);
	PhotoDrawingProperties	drawProps (false, false, false, GetDocument ()->GetResolution());
	mImage->SetDest(mOldDest, drawProps);
	RefreshItemOrLayout();

	//	Restore the dirty flag
	GetDocument ()->SetDirty (mUndoDirty);
	
	//	Swap the state
	mUndoDirty = mRedoDirty;

	}//end UndoSelf	
	