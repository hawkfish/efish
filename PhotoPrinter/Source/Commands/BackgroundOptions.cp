/*
	File:		BackgroundOptions.cp

	Contains:	Implementation of the Background Options dialog.

	Written by:	David Dunham

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		22 mar 2001		dml		fixed handling of custom margins
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
				theDialog.EnableMarginFields(false);
				theLayout->UpdateMargins(theDialog);
				break;
			case msg_CustomMargins: {
				theDialog.EnableMarginFields(true);
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

// these should live only here.  unfortunately they are also in Layout.cp
static const PaneIDT	Pane_Top = 'top ';
static const PaneIDT	Pane_Left= 'left';
static const PaneIDT	Pane_Bottom = 'bot ';
static const PaneIDT	Pane_Right = 'righ';



/*
BackgroundOptionsDialog
*/
BackgroundOptionsDialog::BackgroundOptionsDialog(PhotoPrintDoc* inSuper)
	: EDialog(inSuper->GetView()->GetLayout()->GetDialogID(), inSuper)
	, mDoc(inSuper)
{
	Layout*		theLayout = mDoc->GetView()->GetLayout();

	theLayout->SetupOptionsDialog(*this);
	
	if (mDoc->GetPrintProperties().GetMarginType() == PrintProperties::kCustom)
		EnableMarginFields(true);
	else
		EnableMarginFields(false);
} // BackgroundOptionsDialog

/*
~BackgroundOptionsDialog
*/
BackgroundOptionsDialog::~BackgroundOptionsDialog()
{
} // ~BackgroundOptionsDialog


void
BackgroundOptionsDialog::EnableMarginFields(bool inState) {
	LPane* pane = FindPaneByID(Pane_Top);
	if (inState)
		pane->Enable();
	else
		pane->Disable();
	 pane = FindPaneByID(Pane_Left);
	if (inState)
		pane->Enable();
	else
		pane->Disable();
	 pane = FindPaneByID(Pane_Bottom);
	if (inState)
		pane->Enable();
	else
		pane->Disable();
	 pane = FindPaneByID(Pane_Right);
	if (inState)
		pane->Enable();
	else
		pane->Disable();
	}// end EnableMarginFields
	
	