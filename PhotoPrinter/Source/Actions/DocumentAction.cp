/*
	File:		DocumentAction.cp

	Contains:	Interface to a model swapping action.

	Written by:	Richard Wesley

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		20 jul 2001		dml		use doc's SetDirty
		20 Jul 2001		rmgw	Created.
*/

#include "DocumentAction.h"

#include "PhotoPrintCommands.h"
#include "PhotoPrintConstants.h"
#include "PhotoPrintDoc.h"

// ---------------------------------------------------------------------------
//	¥ DocumentAction													  [public]
// ---------------------------------------------------------------------------

DocumentAction::DocumentAction (

	PhotoPrintDoc*	inDoc,
	const SInt16	inStringIndex)

	: PhotoPrintAction (inDoc, inStringIndex, kAlreadyDone)
	
	, mUndoDirty (GetCurrentDirty ())
	, mUndoMin (GetCurrentMin ())
	, mUndoMax (GetCurrentMax ())
	, mUndoPrintProps (GetCurrentPrintProps ())
	
	{ // begin DocumentAction
	
	} // end DocumentAction

// ---------------------------------------------------------------------------
//	¥ ~DocumentAction													  [public]
// ---------------------------------------------------------------------------

DocumentAction::~DocumentAction (void)

	{ // begin ~DocumentAction
	
	} // end ~DocumentAction

#pragma mark -

// ---------------------------------------------------------------------------
//	¥ GetCurrentDirty											   [protected]
// ---------------------------------------------------------------------------
//	Not virtual because constructor uses it.

bool
DocumentAction::GetCurrentDirty (void) const

	{ // begin GetCurrentDirty
		
		return GetDocument ()->GetProperties().GetDirty ();
		
	} // end GetCurrentDirty

// ---------------------------------------------------------------------------
//	¥ GetCurrentMax												   [protected]
// ---------------------------------------------------------------------------
//	Not virtual because constructor uses it.

SizeLimitT
DocumentAction::GetCurrentMax (void) const

	{ // begin GetCurrentMin
		
		return GetDocument ()->GetMaximumSize ();
		
	} // end GetCurrentMax

// ---------------------------------------------------------------------------
//	¥ GetCurrentMin												   [protected]
// ---------------------------------------------------------------------------
//	Not virtual because constructor uses it.

SizeLimitT
DocumentAction::GetCurrentMin (void) const

	{ // begin GetCurrentMin
		
		return GetDocument ()->GetMinimumSize ();
		
	} // end GetCurrentMin

// ---------------------------------------------------------------------------
//	¥ GetCurrentPrintProps										   [protected]
// ---------------------------------------------------------------------------
//	Not virtual because constructor uses it.

const PrintProperties&
DocumentAction::GetCurrentPrintProps (void) const

	{ // begin GetCurrentPrintProps
		
		return GetDocument ()->GetPrintProperties ();
		
	} // end GetCurrentPrintProps

// ---------------------------------------------------------------------------
//	¥ RedoSelf														  [public]
// ---------------------------------------------------------------------------

void
DocumentAction::RedoSelf (void)

	{ // begin RedoSelf
		
		UndoSelf ();
		
	} // end RedoSelf

// ---------------------------------------------------------------------------
//	¥ UndoSelf														  [public]
// ---------------------------------------------------------------------------

void
DocumentAction::UndoSelf (void)

	{ // begin UndoSelf
		
		//	Useful locals
		PhotoPrintDoc*		doc (GetDocument ());
		PhotoPrintView*		view (GetView ());
		
		//	Get the new undo state
		bool				mRedoDirty (GetCurrentDirty ());
		SizeLimitT			mRedoMin (GetCurrentMin ());
		SizeLimitT			mRedoMax (GetCurrentMax ());
		PrintProperties		mRedoPrintProps (GetCurrentPrintProps ());
		
		//	Restore the minimum
		if (mRedoMin != mUndoMin)
			doc->SetMinimumSize (mUndoMin);
			
		//	Restore the maximum
		if (mRedoMax != mUndoMax) 
			doc->SetMaximumSize (mUndoMax);
			
		//	Restore the print properties
		if (mRedoPrintProps != mUndoPrintProps) 
			doc->SetPrintProperties (mUndoPrintProps);
			
		//	Restore the dirty flag
		doc->SetDirty (mUndoDirty);
		
		//	Swap the state
		mUndoDirty = mRedoDirty;
		mUndoMin = mRedoMin;
		mUndoMax = mRedoMax;
		mUndoPrintProps = mRedoPrintProps;
		
	} // end UndoSelf

