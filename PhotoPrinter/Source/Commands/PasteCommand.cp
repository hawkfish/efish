/*
	File:		PasteCommand.cp

	Contains:	Implementation of the Paste command (in Edit menu).

	Written by:	David Dunham

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		22 May 2001		drd		69 Created
*/

#include "PasteCommand.h"

#include "ImageActions.h"
#include "PhotoPrintDoc.h"
#include "PhotoPrintConstants.h"
#include "PhotoPrintResources.h"

/*
PasteCommand
*/
PasteCommand::PasteCommand(
	const CommandT	inCommand, 
	PhotoPrintDoc*	inDoc)
	: PhotoDocCommandAttachment(inCommand, inDoc)
{
} // PasteCommand

/*
~PasteCommand
*/
PasteCommand::~PasteCommand()
{
} // ~PasteCommand

/*
ExecuteCommand {OVERRIDE}
	Put up print dialog
*/
void		
PasteCommand::ExecuteCommand(void*				/*inCommandData*/)
{
} // ExecuteCommand										 

/*
FindCommandStatus {OVERRIDE}
	Sets its argument to TRUE if the command should be enabled
*/
void		
PasteCommand::FindCommandStatus(SCommandStatus* ioStatus)
{
	*ioStatus->enabled = UScrap::HasData(kDragFlavor) || UScrap::HasData('PICT');
} // FindCommandStatus
