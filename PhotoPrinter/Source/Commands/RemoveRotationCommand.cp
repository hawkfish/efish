/*
	File:		RemoveRotationCommand.cp

	Contains:	Implementation of the Remove Cropping command.

	Written by:	David Dunham

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		18 Jul 2001		rmgw	Split up ImageActions.
		21 aug 2000		dml		created
*/

#include "RemoveRotationCommand.h"

#include "RotateAction.h"
#include "PhotoPrintDoc.h"
#include "PhotoPrintItem.h"
#include "PhotoPrintResources.h"
#include "PhotoUtility.h"

/*
RemoveRotationCommand
*/
RemoveRotationCommand::RemoveRotationCommand(const CommandT inCommand, PhotoPrintDoc* inDoc)
	: PhotoDocCommandAttachment(inCommand, inDoc)
{
} // RemoveRotationCommand

/*
~RemoveRotationCommand
*/
RemoveRotationCommand::~RemoveRotationCommand()
{
} // ~RemoveRotationCommand

/*
ExecuteCommand
*/
void		
RemoveRotationCommand::ExecuteCommand(void* inCommandData)
{
#pragma unused(inCommandData)

// !!! We will want to make this command work on multiple selection
// !!! it will also have to remove crop-zoom

	PhotoPrintItem*		image = mDoc->GetView()->GetPrimarySelection();
	mDoc->PostAction(new RotateAction(mDoc, si_RemoveRotation, 0.0));
	} // ExecuteCommand
									 
/*
FindCommandStatus {OVERRIDE}
*/
void		
RemoveRotationCommand::FindCommandStatus		(SCommandStatus*	ioStatus)
{
// !!! We will want to make this command work on multiple selection

	*ioStatus->enabled = false;
	if (mDoc->GetView()->IsAnythingSelected()) {
		PhotoPrintItem*		image = mDoc->GetView()->GetPrimarySelection();
		*ioStatus->enabled = !(PhotoUtility::DoubleEqual(image->GetRotation(), 0.0));
	}
} // FindCommandStatus
