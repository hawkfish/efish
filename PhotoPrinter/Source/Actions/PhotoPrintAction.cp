/*
	File:		PhotoPrintAction.cp

	Contains:	Implementation of the basic document action.

	Written by:	David Dunham and Dav Lion

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		24 Jul 2001		rmgw	Save dirty state.
		18 Jul 2001		rmgw	Provide accessors for MVC values.
		18 Jul 2001		rmgw	Split up ImageActions.
*/

#include "PhotoPrintAction.h"

#include "Layout.h"
#include "PhotoPrintDoc.h"
#include "PhotoExceptionHandler.h"
#include "PhotoPrintResources.h"

/*
PhotoPrintAction
*/
PhotoPrintAction::PhotoPrintAction(
	PhotoPrintDoc*	inDoc,
	const SInt16	inStringIndex,
	const Boolean	inAlreadyDone)
	
	: LAction(str_Redo, inStringIndex, inAlreadyDone)
	
	, mDoc (inDoc)
	, mUndoDirty (GetCurrentDirty ())
	
{

	Assert_(mDoc);

} // PhotoPrintAction

/*
~PhotoPrintAction
*/
PhotoPrintAction::~PhotoPrintAction()
{
} // ~PhotoPrintAction

#pragma mark -

// ---------------------------------------------------------------------------
//	¥ GetCurrentDirty											   [protected]
// ---------------------------------------------------------------------------
//	Not virtual because constructor uses it.

bool
PhotoPrintAction::GetCurrentDirty (void) const

	{ // begin GetCurrentDirty
		
		return GetDocument ()->GetProperties().GetDirty ();
		
	} // end GetCurrentDirty

/*
GetDocument
*/
PhotoPrintDoc*
PhotoPrintAction::GetDocument (void) const
{
	return mDoc;

} // GetDocument

/*
GetModel
*/
PhotoPrintModel*
PhotoPrintAction::GetModel (void) const
{
	return GetView ()->GetModel ();

} // GetView

/*
GetView
*/
PhotoPrintView*
PhotoPrintAction::GetView (void) const
{
	return GetDocument ()->GetView ();

} // GetView

/*
LayoutImages
*/
void
PhotoPrintAction::LayoutImages()
{
	PhotoPrintView*	theView (GetView ());
	
	theView->Refresh();								// Doc orientation may change, so refresh before AND after
	theView->GetLayout()->LayoutImages();
	theView->Refresh();
} // LayoutImages

/*
Redo {OVERRIDE}
*/
void
PhotoPrintAction::Redo()
{

	try {
		//	Redo the action
		LAction::Redo ();
		}//end try

	catch (LException& e) {
		// Mark the model as dirty
		GetDocument ()->SetDirty (true);
		if (!ExceptionHandler::HandleKnownExceptions(e))
			throw;
		}//end catch
} // Redo

/*
Undo {OVERRIDE}
*/
void
PhotoPrintAction::Undo()
{
	
	try {
		LAction::Undo ();
		}//end try

	catch (LException& e) {
		// Mark the model as dirty
		GetDocument ()->SetDirty (true);
		if (!ExceptionHandler::HandleKnownExceptions(e))
			throw;		
		}//end catch
} // Undo

