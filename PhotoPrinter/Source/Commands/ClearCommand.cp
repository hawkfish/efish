/*
	File:		ClearCommand.cp

	Contains:	Implementation of the Clear command (in Edit menu).

	Written by:	David Dunham

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		03 Aug 2001		rmgw	Disable for ImagesAreDuplicated layouts.  Bug #276.
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
	//	Default is disabled
	*ioStatus->enabled = false;
	
	//	No doc - disabled
	if (!mDoc) return;
	
	//	No view - disabled
	PhotoPrintView*		view (mDoc->GetView ());
	if (!view) return;
	
	//	No layout - disabled
	Layout*				layout (view->GetLayout ());
	if (!layout) return;
	
	//	Duplication layout - disabled
	if (layout->ImagesAreDuplicated ()) return;
	
	//	Empty selection - disabled
	if (view->Selection().empty()) return;
	
	//	Must be enabled
	*ioStatus->enabled = true;

} // FindCommandStatus
