/*
	File:		PrefsCommand.cp

	Contains:	Implementation of the Preferences dialog.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		09 Nov 2000		drd		Don't need gShowing (EDialog how does this)
		08 Nov 2000		drd		Don't show dialog when it's already up; Aqua
		21 Sep 2000		drd		Apply more caption stuff; flush Undo
		21 Sep 2000		drd		Apply caption to all documents
		04 Aug 2000		drd		More things force layout, date/time format do redraw
		03 Aug 2000		drd		We do have date_None in the menu; disable sort order for sort_None
		03 Aug 2000		drd		Handle time & date format
		03 aug 2000		dml		add NeedsSort, change parms to RefreshDocuments
		02 aug 2000		dml		dirtying prefs may drive relayout (if apply is set)
		28 jul 2000		dml		add sorting gui
		21 Jul 2000		drd		Added printing options, caption styles
		13 Jul 2000		drd		Handle gutter
		12 Jul 2000		drd		Handle font, size
		11 Jul 2000		drd		Commit sends SetCaptionStyle for now; min, max size
		11 Jul 2000		drd		Handle some prefs, and use PhotoPrintPrefs object
		28 Jun 2000		drd		Created
*/

#include "PrefsCommand.h"

#include "EUtil.h"
#include <LPopupButton.h>
#include "PhotoPrintApp.h"
#include "PhotoPrintDoc.h"
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
	*ioStatus->enabled = !EDialog::IsDialogShowing();

	if (PhotoPrintApp::gAqua) {
		if (!EDialog::IsDialogShowing()) {
			::EnableMenuCommand(0, 'pref');
		} else {
			::DisableMenuCommand(0, 'pref');
		}
	}
} // FindCommandStatus

#pragma mark -

/*
PrefsDialog
*/
PrefsDialog::PrefsDialog(LCommander* inSuper)
	: EDialog(PPob_Prefs, inSuper)
{
	SInt16			i;

	PhotoPrintPrefs*	prefs = PhotoPrintPrefs::Singleton();

	LPopupButton*	dateFormat = this->FindPopupButton('dfor');
	dateFormat->SetValue(prefs->GetDateFormat());
	// Make the menu read according to the userÕs settings
	MenuHandle		menu = dateFormat->GetMacMenuH();
	LStr255			formatString;
	UInt32			sampleTime;		// !!! use a constant?
	::GetDateTime(&sampleTime);
	for (i = date_Numeric; i <= date_LongDay; i++) {
		EChrono::GetDateTime(formatString, sampleTime, static_cast<DateFormatT>(i), time_None);
		::SetMenuItemText(menu, i, formatString);
	}

	LPopupButton*	timeFormat = this->FindPopupButton('tfor');
	timeFormat->SetValue(prefs->GetTimeFormat());

	LPopupButton*	sizePopup = this->FindPopupButton('size');
	SInt16			nItems = ::CountMenuItems(sizePopup->GetMacMenuH());
	for (i = 1; i <= nItems; i++) {
		if (EUtil::SizeFromMenu(i, sizePopup->GetMacMenuH()) == prefs->GetFontSize()) {
			sizePopup->SetCurrentMenuItem(i);
			break;
		}
	}
	LPopupButton*	fontPopup = this->FindPopupButton('font');
	nItems = ::CountMenuItems(fontPopup->GetMacMenuH());
	LStr255			defaultFont, fontName;
	::GetFontName(prefs->GetFontNumber(), defaultFont);
	for (i = 1; i <= nItems; i++) {
		fontPopup->GetMenuItemText(i, fontName);
		if (fontName == defaultFont) {
			fontPopup->SetValue(i);
			break;
		}
	}

	LPane*			showDate = this->FindPaneByID('fdat');
	showDate->SetValue(prefs->GetShowFileDates());
	LPane*			showName = this->FindPaneByID('fnam');
	showName->SetValue(prefs->GetShowFileNames());

	LRadioGroupView*	captionStyle = this->FindRadioGroupView('cSty');
	captionStyle->SetCurrentRadioID(prefs->GetCaptionStyle());

	// Image size
	LPane*			minSize = this->FindPaneByID('mini');
	minSize->SetValue(prefs->GetMinimumSize());
	LPane*			maxSize = this->FindPaneByID('maxi');
	maxSize->SetValue(prefs->GetMaximumSize());

	LEditText*		gutter = this->FindEditText('gutt');
	if (gutter != nil) {
		gutter->SetValue(prefs->GetGutter());
		LCommander::SwitchTarget(gutter);
		gutter->SelectAll();
	}

	// Printing
	LPane*			altPrint = this->FindPaneByID('altP');
	altPrint->SetValue(prefs->GetAlternatePrinting());
	LPane*			bandPrint = this->FindPaneByID('band');
	bandPrint->SetValue(prefs->GetBandedPrinting());
	if (!prefs->GetAlternatePrinting())
		bandPrint->Disable();
		
	// Sorting
	LPane*	sorting = this->FindPaneByID('sort');
	sorting->SetValue(prefs->GetSorting());
	LPane*	sortOrder = this->FindPaneByID('ordr');
	sortOrder->SetValue(prefs->GetSortAscending() ? 1 : 2);	
	if (prefs->GetSorting() == sort_None)
			sortOrder->Disable();
	
	//Application
	LPane* applyToOpen = this->FindPaneByID('aply');
	applyToOpen->SetValue(prefs->GetApplyToOpenDocs());
} // PrefsDialog

/*
~PrefsDialog
*/
PrefsDialog::~PrefsDialog()
{
} // ~PrefsDialog

/*
Commit
	Apply all the current dialog settings
*/
void
PrefsDialog::Commit()
{
	// Set the application's preferences
	PhotoPrintPrefs*	prefs = PhotoPrintPrefs::Singleton();
	PhotoPrintPrefs		orig (*prefs);

	LPopupButton*	dateFormat = this->FindPopupButton('dfor');
	prefs->SetDateFormat((DateFormatT)dateFormat->GetValue());
	LPopupButton*	timeFormat = this->FindPopupButton('tfor');
	prefs->SetTimeFormat((TimeFormatT)timeFormat->GetValue());

	LPopupButton*	sizePopup = this->FindPopupButton('size');
	SInt16			size = EUtil::SizeFromMenu(sizePopup->GetValue(), sizePopup->GetMacMenuH());
	prefs->SetFontSize(size);

	Str255			fontName;
	LPopupButton*	fontPopup = this->FindPopupButton('font');
	fontPopup->GetMenuItemText(fontPopup->GetCurrentMenuItem(), fontName);
	SInt16			fontID;
	::GetFNum(fontName, &fontID);
	prefs->SetFontNumber(fontID);

	LPane*			showDate = this->FindPaneByID('fdat');
	prefs->SetShowFileDates(showDate->GetValue());
	LPane*			showName = this->FindPaneByID('fnam');
	prefs->SetShowFileNames(showName->GetValue());

	LRadioGroupView*	captionStyle = this->FindRadioGroupView('cSty');
	prefs->SetCaptionStyle((CaptionT)captionStyle->GetCurrentRadioID());
	
	LPane*			minSize = this->FindPaneByID('mini');
	prefs->SetMinimumSize((SizeLimitT)minSize->GetValue());
	LPane*			maxSize = this->FindPaneByID('maxi');
	prefs->SetMaximumSize((SizeLimitT)maxSize->GetValue());

	LPane*			gutter = this->FindPaneByID('gutt');
	prefs->SetGutter(gutter->GetValue());

	// Printing
	LPane*			altPrint = this->FindPaneByID('altP');
	prefs->SetAlternatePrinting(altPrint->GetValue());
	LPane*			bandPrint = this->FindPaneByID('band');
	prefs->SetBandedPrinting(bandPrint->GetValue());

	// Sorting
	LPane*	sorting = this->FindPaneByID('sort');
	prefs->SetSorting((SortingT)sorting->GetValue());
	LPane*	sortOrder = this->FindPaneByID('ordr');
	prefs->SetSortAscending(sortOrder->GetValue() == 1 ? true : false);	
	
	// application
	LPane*	applyToOpen = this->FindPaneByID('aply');
	prefs->SetApplyToOpenDocs(applyToOpen->GetValue());

	// Write all changes in all sources of application defaults. Returns success or failure.
	prefs->Write();
	
	bool							needsSort (this->NeedsSort(*prefs, orig));
	if (applyToOpen->GetValue()) {
		// Apply relevant changes to each document
		TArray<LDocument*>&			docList (LDocument::GetDocumentList());
		ArrayIndexT					count = (ArrayIndexT)docList.GetCount();
		CaptionT					newCaption = (CaptionT)captionStyle->GetCurrentRadioID();
		
		for (ArrayIndexT i = 1; i <= count; ++i) {
			PhotoPrintDoc*			photoDoc = dynamic_cast<PhotoPrintDoc*>(docList[i]);
			if (photoDoc != nil) {
				// This is not undo-able
				photoDoc->PostAction(nil);

				PhotoPrintModel*	model = photoDoc->GetModel();
				PhotoIterator		iter;
				for (iter = model->begin(); iter != model->end(); iter++) {
					PhotoItemRef	theItem = *iter;
					// caption
					PhotoItemProperties&	props(theItem->GetProperties());
					props.SetCaptionStyle(newCaption);
					props.SetFontNumber(fontID);
					props.SetFontSize(size);
					props.SetShowDate(showDate->GetValue());
					props.SetShowName(showName->GetValue());
					theItem->AdjustRectangles();
				}
			} //endif
		} //end for

		if (this->NeedsRefresh(*prefs, orig) || needsSort) {
			PhotoPrintApp::GetSingleton()->RefreshDocuments(needsSort,
				this->NeedsLayout(*prefs, orig));
		}
	} // endif
}//end Commit

/*
ListenToMessage {OVERRIDE}
*/
void
PrefsDialog::ListenToMessage(
	MessageT	inMessage,
	void*		ioParam)
{
	if (inMessage == 'altP') {
		LPane*		bandPrint = this->FindPaneByID('band');
		if (*(SInt32*)ioParam)
			bandPrint->Enable();
		else
			bandPrint->Disable();
	} else if (inMessage == 'sort') {
		LPane*		sortOrder = this->FindPaneByID('ordr');
		if (*(SInt32*)ioParam != sort_None)
			sortOrder->Enable();
		else
			sortOrder->Disable();
	} else {
		EDialog::ListenToMessage(inMessage, ioParam);
	}
} // ListenToMessage


bool
PrefsDialog::NeedsLayout(const PhotoPrintPrefs& orig, const PhotoPrintPrefs& recent)
{
	bool need (true);

	do {
		if (orig.GetMinimumSize() != recent.GetMinimumSize())
			continue;
		if (orig.GetMaximumSize() != recent.GetMinimumSize())
			continue;
		if (orig.GetGutter() != recent.GetGutter())
			continue;

		if (orig.GetCaptionStyle() != recent.GetCaptionStyle())
			continue;
		if (orig.GetFontNumber() != recent.GetFontNumber())
			continue;
		if (orig.GetFontSize() != recent.GetFontSize())
			continue;
		if (orig.GetShowFileDates() != recent.GetShowFileDates())
			continue;
		if (orig.GetShowFileNames() != recent.GetShowFileNames())
			continue;

		need = false;
	} while (false);
		
	return need;	
}//end NeedsLayout

/*
NeedsRefresh
	Will we need to redraw windows?
*/
bool
PrefsDialog::NeedsRefresh(const PhotoPrintPrefs& orig, const PhotoPrintPrefs& recent)
{
	bool need (true);
	
	do {
		if (orig.GetDateFormat() != recent.GetDateFormat())
			continue;
		if (orig.GetTimeFormat() != recent.GetTimeFormat())
			continue;

		if (this->NeedsLayout(orig, recent))
			continue;

		need = false;
	} while (false);

	return need;	
}// end NeedsRefresh

/*
NeedsSort
*/
bool
PrefsDialog::NeedsSort(const PhotoPrintPrefs& orig, const PhotoPrintPrefs& recent)
{
	bool need (true);
	
	do {
		if (orig.GetSorting() != recent.GetSorting())
			continue;		
		if (orig.GetSortAscending() != recent.GetSortAscending())
			continue;
		need = false;
		} while (false);

	return need;	
}// end NeedsSort

