/*
	File:		PasteCommand.cp

	Contains:	Implementation of the Paste command (in Edit menu).

	Written by:	David Dunham

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		11 Jul 2001		rmgw	kDragFlavor => kClipFlavor.
		23 May 2001		drd		69 Use PasteAction to actually paste
		22 May 2001		drd		69 Created

	Note:
		We can't handle PICT because PhotoPrintItem needs an alias
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
	Undo-ably paste
*/
void		
PasteCommand::ExecuteCommand(void*				/*inCommandData*/)
{
	ScrapFlavorType		flavor = kClipFlavor;
	StHandleBlock		h(0L);					// Empty handle to get scrap data
	UScrap::GetData(flavor, h);					// May throw
	mDoc->PostAction(new PasteAction(mDoc, si_PasteImage, flavor, h.Release()));
} // ExecuteCommand										 

/*
FindCommandStatus {OVERRIDE}
	Sets its argument to TRUE if the command should be enabled
*/
void		
PasteCommand::FindCommandStatus(SCommandStatus* ioStatus)
{
	*ioStatus->enabled = UScrap::HasData(kClipFlavor);
} // FindCommandStatus
