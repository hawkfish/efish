/*
	File:		BackgroundOptions.cp

	Contains:	Implementation of the Background Options dialog.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		21 Aug 2000		drd		All layouts now have options, so removed HasOptions test
		15 Aug 2000		drd		Only for layouts with HasOptions true
		30 Jun 2000		drd		Descend from EDialog; let layout do the work
		14 Jun 2000		drd		BackgroundOptionsDialog
		14 Jun 2000		drd		Created
*/

#include "BackgroundOptions.h"
#include "Layout.h"
#include "PhotoPrintDoc.h"

/*
BackgroundOptionsCommand
*/
BackgroundOptionsCommand::BackgroundOptionsCommand(const CommandT inCommand, PhotoPrintDoc* inDoc)
	: PhotoDocCommandAttachment(inCommand, inDoc)
{
} // BackgroundOptionsCommand

/*
~BackgroundOptionsCommand
*/
BackgroundOptionsCommand::~BackgroundOptionsCommand()
{
} // ~BackgroundOptionsCommand

/*
ExecuteCommand
*/
void		
BackgroundOptionsCommand::ExecuteCommand(void* inCommandData)
{
#pragma unused(inCommandData)

	StDesktopDeactivator		deactivator;
	BackgroundOptionsDialog		theDialog(mDoc);

	while (true) {
		MessageT	hitMessage = theDialog.DoDialog();

		if (hitMessage == msg_Cancel) {
			break;
		} else if (hitMessage == msg_OK) {
			Layout*		theLayout = mDoc->GetView()->GetLayout();

			theLayout->CommitOptionsDialog(theDialog);
			break;
		}
	}
} // ExecuteCommand
									 
/*
FindCommandStatus {OVERRIDE}
*/
void		
BackgroundOptionsCommand::FindCommandStatus(SCommandStatus*	ioStatus)
{
	// All layouts have options
	*ioStatus->enabled = true;
} // FindCommandStatus

#pragma mark -

/*
BackgroundOptionsDialog
*/
BackgroundOptionsDialog::BackgroundOptionsDialog(PhotoPrintDoc* inSuper)
	: EDialog(inSuper->GetView()->GetLayout()->GetDialogID(), inSuper)
	, mDoc(inSuper)
{
	Layout*		theLayout = mDoc->GetView()->GetLayout();

	theLayout->SetupOptionsDialog(*this);
} // BackgroundOptionsDialog

/*
~BackgroundOptionsDialog
*/
BackgroundOptionsDialog::~BackgroundOptionsDialog()
{
} // ~BackgroundOptionsDialog
