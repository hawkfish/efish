/*
	File:		BackgroundOptions.cp

	Contains:	Implementation of the Background Options dialog.

	Written by:	David Dunham

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		12 jul 2001		dml		148.  add support for multiple units, setting Prefs
		29 jun 2001		dml		3Hole checkbox calls UpdateMargins, pass cleanPrintProps to CommitOptionsDialog
		29 jun 2001		dml		code cleanup from yesterday
		28 jun 2001		dml		26 enable 3hol for symm, min, disable for cust
		27 Jun 2001		drd		103 Handle msg_TextChanged and switch from None to Header
		06 Apr 2001		drd		EnableMarginFields sanity-checks
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
#include "PhotoPrintPrefs.h"


// these should live only here.  unfortunately they are also in Layout.cp
static const PaneIDT	Pane_Top = 'top ';
static const PaneIDT	Pane_Left= 'left';
static const PaneIDT	Pane_Bottom = 'bot ';
static const PaneIDT	Pane_Right = 'righ';
static const PaneIDT	Pane_3Hole = '3hol';
static const PaneIDT	Pane_Units = 'unit';


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
				theLayout->CommitOptionsDialog(theDialog, cleanProps, Layout::kDoLayoutIfNeeded);
				done = true;
				break;
				}//end case
			case msg_MinimalMargins:
			case msg_SymmetricMargins:
				theDialog.EnableMarginFields(false, true);
				theLayout->UpdateMargins(theDialog);
				break;
			case msg_CustomMargins: {
				theDialog.EnableMarginFields(true, false);
				theLayout->UpdateMargins(theDialog);
				break;				
				}//end margin case

			case msg_3Hole:
				LPane*	pane = theDialog.FindPaneByID(Pane_3Hole);
				mDoc->GetPrintProperties().SetBinderHoles(pane->GetValue());
				theLayout->UpdateMargins(theDialog);//force update of icon to match checkbox
				break;

			case msg_TextChanged: {
				LRadioGroupView*		headerGroup = theDialog.FindRadioGroupView('posi');
				if (headerGroup->GetCurrentRadioID() == 'none') {
					headerGroup->SetCurrentRadioID('head');
				}
				break;
			}
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
	
	if (mDoc->GetPrintProperties().GetMarginType() == PrintProperties::kCustom)
		EnableMarginFields(true, false);
	else
		EnableMarginFields(false, true);
} // BackgroundOptionsDialog

/*
~BackgroundOptionsDialog
*/
BackgroundOptionsDialog::~BackgroundOptionsDialog()
{
} // ~BackgroundOptionsDialog


void
BackgroundOptionsDialog::EnableMarginFields(bool inSides, bool inHoles) {
	LPane* pane = FindPaneByID(Pane_Top);
	if (pane != nil) {
		if (inSides)
			pane->Enable();
		else
			pane->Disable();
	}
	pane = FindPaneByID(Pane_Left);
	if (pane != nil) {
		if (inSides)
			pane->Enable();
		else
			pane->Disable();
	}
	pane = FindPaneByID(Pane_Bottom);
	if (pane != nil) {
		if (inSides)
			pane->Enable();
		else
			pane->Disable();
	}
	pane = FindPaneByID(Pane_Right);
	if (pane != nil) {
		if (inSides)
			pane->Enable();
		else
			pane->Disable();
	}
	
	// units always enabled
	pane = FindPaneByID(Pane_Units);
	pane->Enable();

	
	pane = FindPaneByID(Pane_3Hole);
	if (pane != nil) {
		if (inHoles) {
			pane->SetValue(mDoc->GetPrintProperties().GetBinderHoles());
			pane->Enable();
			}//endif showing and enabling holes
		else {
			pane->SetValue(0);
			pane->Disable();
			}
		}//endif sane
}// end EnableMarginFields
	

void	
BackgroundOptionsDialog::ListenToMessage(MessageT	inMessage, void*		ioParam ) {
	Layout*		theLayout = mDoc->GetView()->GetLayout();
	switch (inMessage) {
		case 'Ætxt':
			theLayout->StuffCustomMarginsIfNecessary(*this, mDoc->GetPrintProperties());
			break;
		case msg_Units:
			theLayout->UpdateMargins(*this);
			PhotoPrintPrefs::Singleton()->SetDisplayUnits((UnitsT)(*(SInt32*)ioParam));
			break;
		default:
			EDialog::ListenToMessage(inMessage, ioParam);
			break;
		}//end switch
}//end ListenToMessage
	
	