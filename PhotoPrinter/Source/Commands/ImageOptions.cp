/*
	File:		ImageOptions.cp

	Contains:	Implementation of the Image Options dialog.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		14 Jun 2000		drd		Created
*/

#include "ImageOptions.h"
#include "PhotoPrintDoc.h"

/*
ImageOptionsCommand
*/
ImageOptionsCommand::ImageOptionsCommand(const CommandT inCommand, PhotoPrintDoc* inDoc)
	: PhotoDocCommandAttachment(inCommand, inDoc)
{
} // ImageOptionsCommand

/*
~ImageOptionsCommand
*/
ImageOptionsCommand::~ImageOptionsCommand()
{
} // ~ImageOptionsCommand

/*
ExecuteCommand
*/
void		
ImageOptionsCommand::ExecuteCommand(void* inCommandData)
{
#pragma unused(inCommandData)

	ImageOptionsDialog		theDialog(mDoc);

	while (true) {
		MessageT	hitMessage = theDialog.DoDialog();

		if (hitMessage == msg_Cancel) {
			break;
		} else if (hitMessage == msg_OK) {
			break;
		}
	}
} // ExecuteCommand
									 
/*
FindCommandStatus {OVERRIDE}
*/
void		
ImageOptionsCommand::FindCommandStatus		(SCommandStatus*	ioStatus)
{
	*ioStatus->enabled = mDoc->GetModel()->IsAnythingSelected();
} // FindCommandStatus

#pragma mark -

/*
ImageOptionsDialog
*/
ImageOptionsDialog::ImageOptionsDialog(LCommander* inSuper)
	: StDialogHandler(PPob_ImageOptions, inSuper)
{
} // ImageOptionsDialog

/*
~ImageOptionsDialog
*/
ImageOptionsDialog::~ImageOptionsDialog()
{
} // ~ImageOptionsDialog