/*
	File:		SelectAllCommand.cp

	Contains:	Implementation of the Select All command (in Edit menu).

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		11 Aug 2000		drd		Created
*/

#include "SelectAllCommand.h"

#include "PhotoPrintDoc.h"

/*
SelectAllCommand
*/
SelectAllCommand::SelectAllCommand(
	const CommandT	inCommand, 
	PhotoPrintDoc*	inDoc)
	: PhotoDocCommandAttachment(inCommand, inDoc)
{
} // SelectAllCommand

/*
~SelectAllCommand
*/
SelectAllCommand::~SelectAllCommand()
{
} // ~SelectAllCommand

/*
ExecuteCommand {OVERRIDE}
*/
void		
SelectAllCommand::ExecuteCommand(void*				/*inCommandData*/)
{
	PhotoPrintView*		view = mDoc->GetView();
	PhotoPrintModel*	model = view->GetModel();
	PhotoItemList		allItems(model->begin(), model->end());
	view->AddToSelection(allItems);
} // ExecuteCommand										 

/*
FindCommandStatus {OVERRIDE}
	Sets its argument to TRUE if the command should be enabled (if anything can be selected)
*/
void		
SelectAllCommand::FindCommandStatus(SCommandStatus* ioStatus)
{
	PhotoPrintView*		view = mDoc->GetView();
	PhotoPrintModel*	model = view->GetModel();
	*(ioStatus->enabled) = model->GetCount() > 0;
} // FindCommandStatus
