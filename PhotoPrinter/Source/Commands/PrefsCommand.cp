/*
	File:		PrefsCommand.cp

	Contains:	Implementation of the Preferences dialog.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		28 Jun 2000		drd		Created
*/

#include "PrefsCommand.h"

#include <LPopupButton.h>
#include "PhotoPrintApp.h"

/*
PrefsCommand
*/
PrefsCommand::PrefsCommand(const CommandT inCommand, PhotoPrintApp* inApp)
	: PhotoAppCommandAttachment(inCommand, inApp)
{
} // PrefsCommand

/*
~PrefsCommand
*/
PrefsCommand::~PrefsCommand()
{
} // ~PrefsCommand

/*
ExecuteCommand
*/
void		
PrefsCommand::ExecuteCommand(void* inCommandData)
{
#pragma unused(inCommandData)

	PrefsDialog		theDialog(mApp);

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
PrefsCommand::FindCommandStatus(SCommandStatus*	ioStatus)
{
	*ioStatus->enabled = true;
} // FindCommandStatus

#pragma mark -

/*
PrefsDialog
*/
PrefsDialog::PrefsDialog(LCommander* inSuper)
	: StDialogHandler(PPob_Prefs, inSuper)
{
	LPopupButton*	dateFormat = dynamic_cast<LPopupButton*>(this->GetDialog()->FindPaneByID('dfor'));

#ifdef SAMPLE_FIXUP
	dateFormat->SetValue(fDateFormat);
	// Make it international
	SInt16			i;
	MenuHandle		menu = dateFormat->GetMacMenuH();
	LStr255			formatString;
	for (i = date_Numeric; i <= dateFormat->GetMaxValue(); i++) {
		PDate::GetDateTime(formatString, kSampleTime, (DateFormatT)i, time_None);
		::SetMenuItemText(menu, i - 1, formatString);
	}
#endif
} // PrefsDialog

/*
~PrefsDialog
*/
PrefsDialog::~PrefsDialog()
{
} // ~PrefsDialog
