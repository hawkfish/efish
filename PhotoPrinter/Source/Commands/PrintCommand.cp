/*
	File:		PrintCommand.cp

	Contains:	Implementation of the PageSetup interface

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		11 Aug 2000		drd		DocumentProperties no longer has Empty, check model instead
		27 Jul 2000		drd		Use HandlePrint instead of DoPrint; use ExecuteCommand
								instead of ExecuteCommandNumber
*/

#include "PrintCommand.h"
#include "PhotoPrintDoc.h"

PrintCommand::PrintCommand(CommandT inCommand, PhotoPrintDoc* inDoc)
	: PhotoDocCommandAttachment (inCommand, inDoc)
{
}//end ct

PrintCommand::~PrintCommand(void)
{
}//end dt

/*
FindCommandStatus {OVERRIDE}
	Don't print empty documents
*/
void		
PrintCommand::FindCommandStatus		(SCommandStatus*	ioStatus)
{
	PhotoPrintView*		view = mDoc->GetView();
	PhotoPrintModel*	model = view->GetModel();
	*(ioStatus->enabled) = model->GetCount() > 0;
}//end FindCommandStatus

/*
ExecuteCommand {OVERRIDE}
	Put up print dialog
*/
void		
PrintCommand::ExecuteCommand(void*				/*inCommandData*/)
{
	mDoc->HandlePrint();
} // ExecuteCommand										 
