/*
	File:		LayoutCommand.cp

	Contains:	Implementation of the Layout menu command

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		23 Aug 2000		drd		Created
*/

#include "LayoutCommand.h"

#include "Layout.h"
#include "PhotoPrintDoc.h"

LayoutCommand::LayoutCommand(CommandT inCommand, PhotoPrintDoc* inDoc)
	: PhotoDocCommandAttachment(inCommand, inDoc)
{
}//end ct

/*
~LayoutCommand
*/
LayoutCommand::~LayoutCommand(void)
{
}//end dt

/*
FindCommandStatus {OVERRIDE}
*/
void		
LayoutCommand::FindCommandStatus		(SCommandStatus*	ioStatus)
{
	PhotoPrintView*		view = mDoc->GetView();
	PhotoPrintModel*	model = view->GetModel();
	*(ioStatus->enabled) = model->GetCount() > 0;
} // FindCommandStatus

/*
ExecuteCommand {OVERRIDE}
*/
void		
LayoutCommand::ExecuteCommand(void*				/*inCommandData*/)
{
	// This is not an undo-able action
	mDoc->PostAction(nil);

	PhotoPrintView*		view = mDoc->GetView();

	view->Refresh();		// In case orientation changes
	view->GetLayout()->LayoutImages();
	view->Refresh();
} // ExecuteCommand										 
