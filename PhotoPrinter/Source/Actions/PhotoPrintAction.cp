/*
	File:		PhotoPrintAction.cp

	Contains:	Implementation of the basic document action.

	Written by:	David Dunham and Dav Lion

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

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
{
	mDoc = inDoc;
	mView = inDoc->GetView();
	mModel = mView->GetModel();
} // PhotoPrintAction

/*
~PhotoPrintAction
*/
PhotoPrintAction::~PhotoPrintAction()
{
} // ~PhotoPrintAction

/*
LayoutImages
*/
void
PhotoPrintAction::LayoutImages()
{
	mView->Refresh();								// Doc orientation may change, so refresh before AND after
	mView->GetLayout()->LayoutImages();
	mView->Refresh();
} // LayoutImages

/*
Redo {OVERRIDE}
*/
void
PhotoPrintAction::Redo()
{
	try {
		LAction::Redo ();
		}//end try

	catch (LException& e) {
		// Mark the model as dirty
		mModel->SetDirty();
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
		mModel->SetDirty();
		if (!ExceptionHandler::HandleKnownExceptions(e))
			throw;		
		}//end catch
} // Undo

