/*
	File:		PrefsCommand.cp

	Contains:	Implementation of the Preferences dialog.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		11 Jul 2000		drd		Commit sends SetCaptionStyle for now; min, max size
		11 Jul 2000		drd		Handle some prefs, and use PhotoPrintPrefs object
		28 Jun 2000		drd		Created
*/

#include "PrefsCommand.h"

#include <LPopupButton.h>
#include "PhotoPrintApp.h"
#include "PhotoPrintPrefs.h"

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
			theDialog.Commit();
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
	: EDialog(PPob_Prefs, inSuper)
{
	PhotoPrintPrefs*	prefs = PhotoPrintPrefs::Singleton();

	LPopupButton*	dateFormat = dynamic_cast<LPopupButton*>(this->FindPaneByID('dfor'));

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

	LPane*			fontSize = this->FindPaneByID('size');
	// PhotoPrintApp::FontSize();
	LPane*			showDate = this->FindPaneByID('fdat');
	showDate->SetValue(prefs->GetShowFileDates());
	LPane*			showName = this->FindPaneByID('fnam');
	showName->SetValue(prefs->GetShowFileNames());

	// Image size
	LPane*			minSize = this->FindPaneByID('mini');
	minSize->SetValue(prefs->GetMinimumSize());
	LPane*			maxSize = this->FindPaneByID('maxi');
	maxSize->SetValue(prefs->GetMaximumSize());
} // PrefsDialog

/*
~PrefsDialog
*/
PrefsDialog::~PrefsDialog()
{
} // ~PrefsDialog

void
PrefsDialog::Commit()
{
	// Set the application's preferences
	PhotoPrintPrefs*	prefs = PhotoPrintPrefs::Singleton();
	prefs->SetFontSize(12);	// !!!
	LPane*			showDate = this->FindPaneByID('fdat');
	prefs->SetShowFileDates(showDate->GetValue());
	LPane*			showName = this->FindPaneByID('fnam');
	prefs->SetShowFileNames(showName->GetValue());

	LPane*			minSize = this->FindPaneByID('mini');
	prefs->SetMinimumSize((SizeLimitT)minSize->GetValue());
	LPane*			maxSize = this->FindPaneByID('maxi');
	prefs->SetMaximumSize((SizeLimitT)maxSize->GetValue());

	prefs->SetCaptionStyle(caption_Bottom);		// !!! kludge, we need a choice

	// Write all changes in all sources of application defaults. Returns success or failure.
	prefs->Write();
} // Commit

