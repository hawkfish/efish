/*
	File:		ModelAction.cp

	Contains:	Interface to a model swapping action.

	Written by:	Richard Wesley and Dav Lion

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		01 Aug 2001		rmgw	Rename ImageCount property to ItemsPerPage.  Bug #265.
		31 Jul 2001		drd		Don't use instance variable names for non-instance variables
		24 jul 2001		rmgw	Move dirty tracking to PhotoPrintAction.
		20 jul 2001		dml		use doc::SetDirty
		20 Jul 2001		rmgw	Fix layout change logic.  Bug #200.
		18 Jul 2001		rmgw	Created.
*/

#include "ModelAction.h"

#include "FixedLayout.h"
#include "PhotoPrintConstants.h"
#include "PhotoPrintDoc.h"

// ---------------------------------------------------------------------------
//	¥ ModelAction													  [public]
// ---------------------------------------------------------------------------

ModelAction::ModelAction (

	PhotoPrintDoc*	inDoc,
	const SInt16	inStringIndex)

	: PhotoPrintAction (inDoc, inStringIndex, kAlreadyDone)
	
	, mUndoLayoutType (GetCurrentLayoutType ())
	, mUndoItemsPerPage (GetCurrentItemsPerPage ())
	, mUndoModel (MakeCurrentModel ())
	, mUndoSelection (MakeCurrentSelection ())
	
	{ // begin ModelAction
	
	} // end ModelAction

// ---------------------------------------------------------------------------
//	¥ ~ModelAction													  [public]
// ---------------------------------------------------------------------------

ModelAction::~ModelAction (void)

	{ // begin ~ModelAction
	
	} // end ~ModelAction

#pragma mark -

// ---------------------------------------------------------------------------
//	¥ GetCurrentLayoutType										   [protected]
// ---------------------------------------------------------------------------
//	Not virtual because constructor uses it.

ModelAction::LayoutType
ModelAction::GetCurrentLayoutType (void) const

	{ // begin GetCurrentLayoutType
		
		return GetView ()->GetLayout ()->GetType ();
		
	} // end GetCurrentLayoutType

// ---------------------------------------------------------------------------
//	¥ GetCurrentItemsPerPage										   [protected]
// ---------------------------------------------------------------------------
//	Not virtual because constructor uses it.

ModelAction::ImageCount
ModelAction::GetCurrentItemsPerPage (void) const

	{ // begin GetCurrentItemsPerPage
		
		return GetView ()->GetLayout ()->GetItemsPerPage ();
		
	} // end GetCurrentItemsPerPage

// ---------------------------------------------------------------------------
//	¥ MakeCurrentModel											   [protected]
// ---------------------------------------------------------------------------
//	Not virtual because constructor uses it.

ModelAction::ModelRef
ModelAction::MakeCurrentModel (void) const

	{ // begin MakeCurrentModel
		
		return new PhotoPrintModel (*GetModel ());
		
	} // end MakeCurrentModel

// ---------------------------------------------------------------------------
//	¥ MakeCurrentSelection										   [protected]
// ---------------------------------------------------------------------------
//	Not virtual because constructor uses it.

ModelAction::ModelSelection
ModelAction::MakeCurrentSelection (void) const

	{ // begin MakeCurrentSelection
		
		ModelSelection			outSelection;
		
		PhotoPrintView*			view (GetView ());
		const	PhotoItemList&	viewSelection (view->Selection ());
		
		PhotoPrintModel*		model (GetModel ());
		for (ConstPhotoIterator i = viewSelection.begin (); i != viewSelection.end (); ++i)
			outSelection.push_back (std::find (model->begin (), model->end (), *i) - model->begin ());
		
		return outSelection;
		
	} // end MakeCurrentSelection

// ---------------------------------------------------------------------------
//	¥ RedoSelf														  [public]
// ---------------------------------------------------------------------------

void
ModelAction::RedoSelf (void)

	{ // begin RedoSelf
		
		this->UndoSelf ();
		
	} // end RedoSelf

// ---------------------------------------------------------------------------
//	¥ UndoSelf														  [public]
// ---------------------------------------------------------------------------

void
ModelAction::UndoSelf (void)

	{ // begin UndoSelf
		
		//	Useful locals
		PhotoPrintDoc*		doc (GetDocument ());
		PhotoPrintView*		view (GetView ());
		
		//	Get the new undo state
		bool				redoDirty (GetCurrentDirty ());
		LayoutType			redoLayoutType (GetCurrentLayoutType ());
		ImageCount			redoItemsPerPage (GetCurrentItemsPerPage ());
		ModelRef			redoModel (MakeCurrentModel ());
		ModelSelection		redoSelection (MakeCurrentSelection ());
		
		//	Clear the current selection
		PhotoItemList		oldSelection (view->Selection ());
		view->ToggleSelected (oldSelection);
		
		//	Restore the layout type and count
		if ((redoLayoutType != mUndoLayoutType) || (mUndoItemsPerPage != redoItemsPerPage))
			view->SwitchLayout (mUndoLayoutType, mUndoItemsPerPage);
			
		//	Restore the old items
		view->GetLayout()->SetItems (mUndoModel->begin(), mUndoModel->end());
		
		//	Restore the old selection
		PhotoPrintModel*	model (GetModel ());
		for (ModelSelection::iterator s = mUndoSelection.begin (); s != mUndoSelection.end (); ++s) {
			Assert_(*s < model->GetCount ());
			
			view->AddToSelection (*(model->begin () + *s));
			} // for
		
		//	Restore the dirty flag
		doc->SetDirty (mUndoDirty);
		
		//	Swap the state
		mUndoDirty = redoDirty;
		mUndoLayoutType = redoLayoutType;
		mUndoItemsPerPage = redoItemsPerPage;
		mUndoModel = redoModel;
		mUndoSelection = redoSelection;
		
		//	Redo the layout
		this->LayoutImages();
	} // end UndoSelf
