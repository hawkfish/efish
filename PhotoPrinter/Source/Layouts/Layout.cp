/*
	File:		Layout.cp

	Contains:	Implementation of Layout object, which manages positioning of images.

	Written by:	David Dunham and Dav Lion

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		06 dec 2000		dml		set header/footer based on actual line size, not useless hardwired constant
		05 dec 2000		dml		factored out SetAnnoyingwareNotice
		01 Dec 2000		drd		26 Added mBinderMargin, gBinderMargin
		27 Sep 2000		rmgw	Change ItemIsAcceptable to DragIsAcceptable.
		21 sep 2000		dml		changed annoyingware sizes, strategry in CommitOptions
		14 Sep 2000		drd		CommitOptionsDialog actually makes space for header/footer
		14 sep 2000		dml		header/footer support
		07 sep 2000		dml		AddItem calls back to view to select 
		31 aug 2000		dml		pass kCalcWithXforms to CountOrientation
		21 Aug 2000		drd		SetupOptionsDialog handles page title
		15 aug 2000		dml		have AddItem call Doc->View->RefreshItem 
		14 aug 2000		dml		add GetDistinctImages
		13 Jul 2000		drd		Initialize gutter from prefs
		13 jul 2000		dml		store orientation locally
		11 jul 2000		dml		add numPages to AdjustDocumentOrientation
		07 Jul 2000		drd		CommitOptionsDialog is for Background, not item!
		05 Jul 2000		drd		CommitOptionsDialog
		30 Jun 2000		drd		SetupOptionsDialog
		28 jun 2000		dml		fix AdjustDocumentOrientation (EPrintSPec*!!)
		27 Jun 2000		drd		AdjustDocumentOrientation, CountOrientation
		27 jun 2000		dml		add HFSPromise Drag Receiving
		26 Jun 2000		drd		AddItem
		23 Jun 2000		drd		Use HORef<PhotoPrintModel> in constructor
		21 Jun 2000		drd		ItemIsAcceptable; allow nil model
		19 Jun 2000		drd		Added mRows, mColumns, mGutter
		19 Jun 2000		drd		Created
*/

#include "Layout.h"

#include "EDialog.h"
#include "EUtil.h"
#include <LGAColorSwatchControl.h>
#include "PhotoPrintPrefs.h"
#include "PhotoUtility.h"
#include "PhotoPrintApp.h"

#include "MDragItemIterator.h"
#include "MFolderIterator.h"
#include "Registration.h"

SInt16	Layout::gBinderMargin = k3HoleWidth;	// Might later set from a resource or something

/*
Layout
*/
Layout::Layout(HORef<PhotoPrintModel>& inModel)
	: mType(kUnspecified)
	, mModel(inModel)
	, mRows(1)
	, mColumns(1)
	, mGutter(PhotoPrintPrefs::Singleton()->GetGutter())
	, mBinderMargin(PhotoPrintPrefs::Singleton()->GetBinderMargin())
{
	if (mModel == nil)
		mDocument = nil;
	else
		mDocument = mModel->GetDocument();
				
} // Layout

/*
~Layout
*/
Layout::~Layout()
{
} // ~Layout

/*
AddItem
	Add an item to the model, handling multiples properly. Default is to just add
	to the model.
*/
void
Layout::AddItem(PhotoItemRef inItem)
{
	mModel->AdoptNewItem(inItem);
	mDocument->GetView()->AddToSelection(inItem);
} // AddItem


/*
AdjustDocumentOrientation
	Set the paper to landscape or portrait orientation to fit the most items
*/
void
Layout::AdjustDocumentOrientation(SInt16 numPages)
{
	UInt32		l = this->CountOrientation(kLandscape);
	UInt32		p = this->CountOrientation(kPortrait);

	EPrintSpec* spec = (EPrintSpec*)mDocument->GetPrintRec();
	// Note that we have a slight bias for landscape (since most pictures are done that way)
	if (p > l) {
		mOrientation = kPortrait;
	} else {
		mOrientation = kLandscape;
	}
	spec->SetOrientation(mOrientation);

	mDocument->MatchViewToPrintRec(numPages);
} // AdjustDocumentOrientation


/*
CommitOptionsDialog
	Handles setting things from the Background Properties dialog
*/
void
Layout::CommitOptionsDialog(EDialog& inDialog)
{
	DocumentProperties&		props = mDocument->GetProperties();

	LRadioGroupView*		titlePos = inDialog.FindRadioGroupView('posi');
	PaneIDT titleButton = titlePos->GetCurrentRadioID();
	props.SetTitlePosition(static_cast<TitlePositionT>(titlePos->GetCurrentRadioID()));


	Str255					title;
	inDialog.FindEditText('titl')->GetDescriptor(title);
	
	double lineHeight = PhotoUtility::GetLineHeight(mDocument->GetProperties().GetFontNumber(),
													mDocument->GetProperties().GetFontSize()) / 72.0;
	
	switch (titleButton) {
		case 'head' :
			mDocument->GetPrintProperties().SetHeader(lineHeight);
			props.SetHeader(title);
			SetAnnoyingwareNotice(!PhotoPrintApp::gIsRegistered, annoy_footer);
			break;
		case 'foot' :
			mDocument->GetPrintProperties().SetFooter(lineHeight);
			props.SetFooter(title);
			SetAnnoyingwareNotice(!PhotoPrintApp::gIsRegistered, annoy_header);
			break;
		case 'none' :
			mDocument->GetPrintProperties().SetHeader(0.0);
			props.SetHeader("\p");
			SetAnnoyingwareNotice(!PhotoPrintApp::gIsRegistered, annoy_header);
			break;
		}//end switch

	LPopupButton*	sizePopup = inDialog.FindPopupButton('fSiz');
	SInt16			size = EUtil::SizeFromMenu(sizePopup->GetValue(), sizePopup->GetMacMenuH());
	props.SetFontSize(size);

	LPopupButton*	fontPopup = inDialog.FindPopupButton('font');
	Str255			fontName;
	props.SetFontName(fontPopup->GetMenuItemText(fontPopup->GetCurrentMenuItem(), fontName));

	LPane*		binderMargin = inDialog.FindPaneByID('3hol');
	if (binderMargin != nil) {
		if (binderMargin->GetValue()) {
			mBinderMargin = gBinderMargin;
		} else {
			mBinderMargin = 0;
		}
	}

	LGAColorSwatchControl*	color = dynamic_cast<LGAColorSwatchControl*>(inDialog.FindPaneByID('bCol'));
	if (color != nil) {
		RGBColor		theColor;
		color->GetSwatchColor(theColor);
		// !!! set something
	}
} // CommitOptionsDialog

/*
CountOrientation
*/
UInt32
Layout::CountOrientation(const OSType inType) const
{
	UInt32		c = 0;

	PhotoIterator	i;
	for (i = mModel->begin(); i != mModel->end(); i++) {
		PhotoItemRef	item = *i;
		if (inType == kLandscape && item->IsLandscape(kCalcWithXforms) || inType == kPortrait &&
			item->IsPortrait(kCalcWithXforms))
			c++;
	}

	return c;
} // CountOrientation

/*
GetDistinctImages
*/
SInt16
Layout::GetDistinctImages() {
	if (mModel)
		return mModel->GetCount();
	else
		return 0;
}//end GetDistinctImages


/*
FileIsAcceptable
*/

static UInt16
FileIsAcceptable (

	const CInfoPBRec&)

{
	
	return true;
	
} // FileIsAcceptable

/*
CountFiles
*/

static UInt16
CountAcceptableFiles (

	const	FSSpec&	inSpec)

{
	
	MFileSpec	spec (inSpec);
	
	CInfoPBRec	pb;
	spec.GetCatInfo (pb);
	if (!(pb.hFileInfo.ioFlAttrib & ioDirMask)) 
		return FileIsAcceptable (pb) ? 1 : 0;
		
	UInt16		count = 0;
	MFolderIterator	end (spec.Volume (), pb.dirInfo.ioDrDirID);
	for (MFolderIterator fi (end); ++fi != end; ) {
		if (fi.IsFolder ())
			count += CountAcceptableFiles (fi.FileSpec ());	//	Recurse
		
		else if (FileIsAcceptable (*fi))
			++count;
		} // if
	
	return count;
	
} // CountAcceptableFiles

/*
DragIsAcceptable
*/
Boolean
Layout::DragIsAcceptable (

	DragReference	inDragRef)

{
	
	//	Count up the items that we will try to add
	UInt16				count = 0;
	MDragItemIterator	end (inDragRef);
	for (MDragItemIterator i = end; ++i != end;) {
		FSSpec		fsSpec;
		if (!i.ExtractFSSpec (fsSpec)) return false;	//	Unknown item type
		
		count += CountAcceptableFiles (fsSpec);
		} // for
		
	// We may not want multiple items
	if (!CanAddToBackground (count))
		return false;

	return true;

} // DragIsAcceptable





/*
* SetAnnoyingwareNotice
*/
void
Layout::SetAnnoyingwareNotice(bool inState, AnnoyLocationT inWhere) {

	DocumentProperties&		props = mDocument->GetProperties();

	double lineHeight = PhotoUtility::GetLineHeight(mDocument->GetProperties().GetFontNumber(),
													mDocument->GetProperties().GetFontSize()) / 72.0;
	switch (inWhere) {
		case annoy_header: {
			if (inState) {
				mDocument->GetPrintProperties().SetHeader(lineHeight);
				props.SetHeader(PhotoPrintApp::gAnnoyanceText);
				}//endif need annoyance
			else
				mDocument->GetPrintProperties().SetHeader(0.0);
			break;
			}//case
		case annoy_footer: {
			if (inState) {
				mDocument->GetPrintProperties().SetFooter(lineHeight);
				props.SetFooter(PhotoPrintApp::gAnnoyanceText);
				}//endif need annoyance
			else
				mDocument->GetPrintProperties().SetFooter(0.0);
			break;
			}//case
		case annoy_diagonal:
			break;
		case annoy_none:
			break;
		}//end switch
	
	}//end SetAnnoyingwareNotice





/*
SetupOptionsDialog
	Subclasses should override (and call inherited)
*/
void
Layout::SetupOptionsDialog(EDialog& inDialog)
{
	// Set up title stuff
	DocumentProperties&		props = mDocument->GetProperties();

	LRadioGroupView*	titlePos = inDialog.FindRadioGroupView('posi');
	titlePos->SetCurrentRadioID(props.GetTitlePosition());

	LPopupButton*	sizePopup = inDialog.FindPopupButton('fSiz');
	SInt16			nItems = ::CountMenuItems(sizePopup->GetMacMenuH());
	SInt16			i;
	for (i = 1; i <= nItems; i++) {
		if (EUtil::SizeFromMenu(i, sizePopup->GetMacMenuH()) == props.GetFontSize()) {
			sizePopup->SetCurrentMenuItem(i);
			break;
		}
	}
	LPopupButton*	fontPopup = inDialog.FindPopupButton('font');
	nItems = ::CountMenuItems(fontPopup->GetMacMenuH());
	LStr255			defaultFont, fontName;
	::GetFontName(props.GetFontNumber(), defaultFont);
	for (i = 1; i <= nItems; i++) {
		fontPopup->GetMenuItemText(i, fontName);
		if (fontName == defaultFont) {
			fontPopup->SetValue(i);
			break;
		}
	}

	LEditText*		title = inDialog.FindEditText('titl');
	if (title != nil) {
		switch (props.GetTitlePosition()) {
			case kHeader:
				title->SetDescriptor(props.GetHeader());
				break;
			case kFooter:
				title->SetDescriptor(props.GetFooter());
				break;
			case kNoTitle:
				title->SetDescriptor("\p");
				break;
			}//end switch
		LCommander::SwitchTarget(title);
		title->SelectAll();
	}

	// Binder margin (for 3-hole punching, at least in the USA)
	LPane*		binderMargin = inDialog.FindPaneByID('3hol');
	if (binderMargin != nil) {
		if (mBinderMargin != 0)
			binderMargin->SetValue(Button_On);
		else
			binderMargin->SetValue(Button_Off);
	}

	// We no longer have a background color in the dialogs, but this is harmless
	LGAColorSwatchControl*	color = dynamic_cast<LGAColorSwatchControl*>(inDialog.FindPaneByID('bCol'));
	if (color != nil) {
		color->SetSwatchColor(Color_White);
	}
} // SetupOptionsDialog
