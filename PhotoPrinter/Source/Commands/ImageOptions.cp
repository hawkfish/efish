/*
	File:		ImageOptions.cp

	Contains:	Implementation of the Image Options dialog.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		29 Jun 2000		drd		Set up dialog (assuming single selection)
		14 Jun 2000		drd		Created
*/

#include "ImageOptions.h"
#include <LBevelButton.h>
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
	: EDialog(PPob_ImageOptions, inSuper)
{
} // ImageOptionsDialog

/*
~ImageOptionsDialog
*/
ImageOptionsDialog::~ImageOptionsDialog()
{
} // ~ImageOptionsDialog

/*
ListenToMessage {OVERRIDE}
*/
void
ImageOptionsDialog::ListenToMessage(
	MessageT	inMessage,
	void*		ioParam)
{
	if (inMessage == 'tabs') {
		// This message means that a tab has been switched. The radio group (and any other views) is
		// not created ahead of time, so this is a plausible place to initialize. (??? This should
		// probably be improved for real.)
		LRadioGroupView*	shapeButtons = dynamic_cast<LRadioGroupView*>(this->FindPaneByID('shap'));
		if (shapeButtons != nil) {
			shapeButtons->SetCurrentRadioID('squa');
		}

		// !!! Temporary code, just to illustrate how to set the thumbnails
		LBevelButton*		rotate0 = dynamic_cast<LBevelButton*>(this->FindPaneByID('000°'));
		if (rotate0 != nil) {
			PicHandle		pict = ::GetPicture(208);
			ControlButtonContentInfo	ci;
			ci.contentType = kControlContentPictHandle;
			ci.u.picture = pict;
			rotate0->SetContentInfo(ci);
		}
	} else {
		EDialog::ListenToMessage(inMessage, ioParam);
	}
} // ListenToMessage
