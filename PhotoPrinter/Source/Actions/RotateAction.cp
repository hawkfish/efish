/*
	File:		RotateAction.cp

	Contains:	Implementation of the rotate item action.

	Written by:	David Dunham and Dav Lion

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		20 Aug 2001		rmgw	Fix Undo orientation changes.  Bug #339.
		31 Jul 2001		drd		260 RefreshItemOrLayout self-sends LayoutImages
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
	
	, mUndoRot (inRot)

{

	if (inDest != nil)
		mUndoDest = *inDest;
	else
		mUndoDest = mImage->GetDestRect();

	}//end ct


RotateAction::~RotateAction() {
	}//end dt


void
RotateAction::RedoSelf (void) 

{ // begin RedoSelf
	
	UndoSelf ();
	
} // end RedoSelf


void
RotateAction::UndoSelf (void) 

{ // begin UndoSelf

	//	Get the new undo state
	bool				mRedoDirty (GetCurrentDirty ());
	OSType				mRedoOrientation (GetView()->GetLayout()->GetOrientation());
	double				mRedoRot (mImage->GetRotation());
	MRect				mRedoDest (mImage->GetDestRect());

	//	Swap the values
	mImage->SetRotation (mUndoRot);
	PhotoDrawingProperties	drawProps (false, false, false, GetDocument ()->GetResolution());
	mImage->SetDest (mUndoDest, drawProps);
		
	//	Refresh intelligently
	if (GetView()->GetLayout()->CalcOrientation() == mRedoOrientation)
		RefreshImage();
	else this->LayoutImages ();
	
	//	Restore the dirty flag
	GetDocument ()->SetDirty (mUndoDirty);
	
	//	Swap the state
	mUndoDirty = mRedoDirty;
	mUndoRot = mRedoRot;
	mUndoDest = mRedoDest;

} // end UndoSelf	
	