/*
	File:		ClearCommand.cp

	Contains:	Implementation of the Clear command (in Edit menu).

	Written by:	David Dunham

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		18 Jul 2001		rmgw	Split up ImageActions.
		04 Aug 2000		drd		Created
*/

#include "ClearCommand.h"

#include "DeleteAction.h"
#include "PhotoPrintDoc.h"
#include "PhotoPrintResources.h"

/*
ClearCommand
*/
ClearCommand::ClearCommand(
	const CommandT	inCommand, 
	PhotoPrintDoc*	inDoc)
	: PhotoDocCommandAttachment(inCommand, inDoc)
{
} // ClearCommand

/*
~ClearCommand
*/
ClearCommand::~ClearCommand()
{
} // ~ClearCommand

/*
ExecuteCommand {OVERRIDE}
	Put up print dialog
*/
void		
ClearCommand::ExecuteCommand(void*				/*inCommandData*/)
{
	mDoc->PostAction(new DeleteAction(mDoc, si_DeleteImage));
} // ExecuteCommand										 

/*
FindCommandStatus {OVERRIDE}
	Sets its argument to TRUE if the command should be enabled (if anything is selected)
*/
void		
ClearCommand::FindCommandStatus(SCommandStatus* ioStatus)
{
	*ioStatus->enabled = !mDoc->GetView()->Selection().empty();
} // FindCommandStatus
