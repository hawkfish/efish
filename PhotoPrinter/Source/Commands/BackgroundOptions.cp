/*
	File:		BackgroundOptions.cp

	Contains:	Implementation of the Background Options dialog.

	Written by:	David Dunham

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		18 Jan 2001		drd		Added arg to CommitOptionsDialog call
		21 Aug 2000		drd		All layouts now have options, so removed HasOptions test
		15 Aug 2000		drd		Only for layouts with HasOptions true
		30 Jun 2000		drd		Descend from EDialog; let layout do the work
		14 Jun 2000		drd		BackgroundOptionsDialog
		14 Jun 2000		drd		Created
*/

#include "BackgroundOptions.h"
#include "Layout.h"
#include "PhotoPrintDoc.h"
#include "PhotoPrintCommands.h"

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
	Layout*		theLayout (mDoc->GetView()->GetLayout());

	PrintProperties cleanProps (mDoc->GetPrintProperties());
	bool	done (false);
	while (!done) {
		MessageT	hitMessage = theDialog.DoDialog();

		switch (hitMessage) {
			case msg_Cancel:
				done = true;
				mDoc->GetPrintProperties() = cleanProps;
				break;
			case msg_OK: {
				theLayout->CommitOptionsDialog(theDialog, Layout::kDoLayoutIfNeeded);
				done = true;
				break;
				}//end case
			case msg_MinimalMargins:
			case msg_SymmetricMargins:
			case msg_CustomMargins: {
				theLayout->UpdateMargins(theDialog);
				break;				
				}//end margin case
		}//switch
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
