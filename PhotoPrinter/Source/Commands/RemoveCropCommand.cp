/*
	File:		RemoveCropCommand.cp

	Contains:	Implementation of the Remove Cropping command.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		21 aug 2000		dml		FindCommandStatus incorrectly checking, differentiate Execute crop vs. cropzoom
		18 aug 2000		dml		HasCrop replaces empty test on GetCrop (since no longer rect)
		15 Aug 2000		drd		Created
*/

#include "RemoveCropCommand.h"

#include "ImageActions.h"
#include "PhotoPrintDoc.h"
#include "PhotoPrintItem.h"
#include "PhotoPrintResources.h"

/*
RemoveCropCommand
*/
RemoveCropCommand::RemoveCropCommand(const CommandT inCommand, PhotoPrintDoc* inDoc)
	: PhotoDocCommandAttachment(inCommand, inDoc)
{
} // RemoveCropCommand

/*
~RemoveCropCommand
*/
RemoveCropCommand::~RemoveCropCommand()
{
} // ~RemoveCropCommand

/*
ExecuteCommand
*/
void		
RemoveCropCommand::ExecuteCommand(void* inCommandData)
{
#pragma unused(inCommandData)

// !!! We will want to make this command work on multiple selection
// !!! it will also have to remove crop-zoom

	PhotoPrintItem*		image = mDoc->GetView()->GetPrimarySelection();
	MRect				empty;
	if (image->HasZoom()) {
		mDoc->PostAction(new CropZoomAction(mDoc, si_RemoveCrop, empty));
		}//endif cropzoom
	else
		mDoc->PostAction(new CropAction(mDoc, si_RemoveCrop, empty));
} // ExecuteCommand
									 
/*
FindCommandStatus {OVERRIDE}
*/
void		
RemoveCropCommand::FindCommandStatus		(SCommandStatus*	ioStatus)
{
// !!! We will want to make this command work on multiple selection

	*ioStatus->enabled = false;
	if (mDoc->GetView()->IsAnythingSelected()) {
		PhotoPrintItem*		image = mDoc->GetView()->GetPrimarySelection();

		*ioStatus->enabled = image->HasCrop();
	}
} // FindCommandStatus
