/*
	File:		RotateAction.h

	Contains:	Implementation of the rotate item action.

	Written by:	David Dunham and Dav Lion

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

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
	mImage->SetRotation(mNewRot);
	PhotoDrawingProperties	drawProps (false, false, false, GetDocument ()->GetResolution());
	mImage->SetDest(mNewDest, drawProps);
	RefreshItemOrLayout();
	}//end RedoSelf


void
RotateAction::RefreshItemOrLayout() {
	OSType	newOrientation (GetView()->GetLayout()->CalcOrientation());
	
	HORef<StDisableBroadcaster> prettyPixels;
	
	if (mOldOrientation == newOrientation) {
		prettyPixels = new StDisableBroadcaster(GetModel()); //EVIL dml 23 july ??????
		GetView()->RefreshItem(mImage);
		}//endif optimizing for prettiness
	else {
		GetView()->Refresh(); //inval cur page cause we will likely switch
		GetView()->GetLayout()->LayoutImages();
		}//end
		
	GetModel ()->SetDirty();		// heavyweight.  will cause refresh if not silenced above
	}//end RefreshItemOrLayout

	
void
RotateAction::UndoSelf() {
	mImage->SetRotation(mOldRot);
	PhotoDrawingProperties	drawProps (false, false, false, GetDocument ()->GetResolution());
	mImage->SetDest(mOldDest, drawProps);
	RefreshItemOrLayout();
	}//end UndoSelf	
	