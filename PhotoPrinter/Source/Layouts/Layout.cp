/*
	File:		Layout.cp

	Contains:	Implementation of Layout object, which manages positioning of images.

	Written by:	David Dunham and Dav Lion

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

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

/*
Layout
*/
Layout::Layout(HORef<PhotoPrintModel>& inModel)
	: mType(kUnspecified)
	, mModel(inModel)
	, mRows(1)
	, mColumns(1)
	, mGutter(PhotoPrintPrefs::Singleton()->GetGutter())
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
	mDocument->GetView()->RefreshItem(inItem);
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
	props.SetTitlePosition(static_cast<TitlePositionT>(titlePos->GetCurrentRadioID()));

	LPopupButton*	sizePopup = inDialog.FindPopupButton('fSiz');
	props.SetFontSize(sizePopup->GetValue());

	LPopupButton*	fontPopup = inDialog.FindPopupButton('font');
	Str255			fontName;
	props.SetFontName(fontPopup->GetMenuItemText(fontPopup->GetCurrentMenuItem(), fontName));

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
ItemIsAcceptable
*/
bool
Layout::ItemIsAcceptable(
	DragReference	inDragRef,
	ItemReference	inItemRef,
	FlavorType&		outFlavor)
{
	// Get actual count
	UInt16		count;
	::CountDragItems(inDragRef, &count);
	// We may not want multiple items
	if (!this->CanAddToBackground(count))
		return false;

	FlavorFlags	theFlags;

	Boolean		happy = true;
	do {
		if (::GetFlavorFlags(inDragRef, inItemRef, kDragFlavorTypeHFS, &theFlags) == noErr) {
			outFlavor = kDragFlavorTypeHFS;

			// ??? we really should look at the file type here (i.e. let QuickTime determine if it
			// can be imported), so we can give a proper drag hilite instead of failing later

			// Our layout may not want multiple items -- we consider a folder to be multiple items
			break;
			}//endif hfs flavor
		if (::GetFlavorFlags(inDragRef, inItemRef, kDragFlavorTypePromiseHFS, &theFlags) == noErr) {
			outFlavor = kDragFlavorTypePromiseHFS;
			break;
			}//endif hfs promise
		happy = false;
	} while (false);
			
	return happy;
} // ItemIsAcceptable

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
		title->SetDescriptor(props.GetTitle());
		LCommander::SwitchTarget(title);
		title->SelectAll();
	}

	// We no longer have a background color in the dialogs, but this is harmless
	LGAColorSwatchControl*	color = dynamic_cast<LGAColorSwatchControl*>(inDialog.FindPaneByID('bCol'));
	if (color != nil) {
		color->SetSwatchColor(Color_White);
	}
} // SetupOptionsDialog
