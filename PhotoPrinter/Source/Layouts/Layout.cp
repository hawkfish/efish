/*
	File:		Layout.cp

	Contains:	Implementation of Layout object, which manages positioning of images.

	Written by:	David Dunham and Dav Lion

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		28 jun 2001		dml		20 don't set print properties binder holes if that element is disabled!
		28 Jun 2001		rmgw	Clear both headers and footers in CommitOptionsDialog.  Bug #100.
		14 May 2001		drd		65 CountOrientation no longer returns true int, but is biased
								so that empties don't count if there are any non-empties
		21 mar 2001		dml		add initialization of custom margins fields to SetupOptionsDialog
		12 mar 2001		dml		add ignoreEmpty option to CountOrientation
		12 mar 2001		dml		add more discrimination logic to CountOrientation to support templates
		15 feb 2001		dml		Doh!  CommitOptionsDialog must sanity check for buttons + panels
		23 jan 2001		dml		fix evil kDragPromiseFindFile bug w/ enhanced ExtractFSSpec call
		19 jan 2001		dml		add more margin support to SetupOptionsDialog
		18 Jan 2001		drd		CommitOptionsDialog does layout if necessary
		11 Dec 2000		drd		13 DragIsAcceptable checks for kDragFlavor; re-alphabetized
		07 Dec 2000		drd		CommitOptionsDialog forces redraw (to see header/footer change)
		06 dec 2000		dml		add header fudge (for whitespace immed below header)
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
#include "PhotoPrintConstants.h"
#include "PhotoPrintPrefs.h"
#include "PhotoUtility.h"
#include "PhotoPrintApp.h"

#include "MDragItemIterator.h"
#include "MFolderIterator.h"
#include "Registration.h"
#include "PhotoPrinter.h"
#include <stdio.h>
#include "MP2CStr.h"

static	UInt16		FileIsAcceptable(const CInfoPBRec&);

SInt16	Layout::gBinderMargin = k3HoleWidth;	// Might later set from a resource or something

static const double kHeaderSpacing = 0.0625;  // and eigth of an inch
static const double kFooterSpacing = 0.0625;  // and eigth of an inch

static const PaneIDT	RadioGroupView_Position = 'posi';
static const PaneIDT	RadioGroupView_Margins = 'marg';

static const PaneIDT	Pane_MinimalMargins = 'minm';
static const PaneIDT	Pane_SymmetricMargins = 'symm';
static const PaneIDT	Pane_CustomMargins = 'cust';

static const PaneIDT	Pane_Top = 'top ';
static const PaneIDT	Pane_Left= 'left';
static const PaneIDT	Pane_Bottom = 'bot ';
static const PaneIDT	Pane_Right = 'righ';


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
bool
Layout::CommitOptionsDialog(EDialog& inDialog, const bool inDoLayout)
{
	bool					needsLayout = false;
	DocumentProperties&		props = mDocument->GetProperties();

	LRadioGroupView*		titlePos = inDialog.FindRadioGroupView('posi');
	PaneIDT					titleButton = titlePos->GetCurrentRadioID();
	if (titleButton != props.GetTitlePosition())
		needsLayout = true;
	props.SetTitlePosition(static_cast<TitlePositionT>(titleButton));

	Str255					title;
	inDialog.FindEditText('titl')->GetDescriptor(title);
	
	double	lineHeight = PhotoUtility::GetLineHeight(mDocument->GetProperties().GetFontNumber(),
													mDocument->GetProperties().GetFontSize()) / 72.0;
	
	switch (titleButton) {
		case 'head' :
			mDocument->GetPrintProperties().SetHeader(lineHeight + kHeaderSpacing);
			props.SetHeader(title);
			props.SetFooter("\p");
			SetAnnoyingwareNotice(!PhotoPrintApp::gIsRegistered, annoy_footer);
			break;

		case 'foot' :
			mDocument->GetPrintProperties().SetFooter(lineHeight + kFooterSpacing);
			props.SetHeader("\p");
			props.SetFooter(title);
			SetAnnoyingwareNotice(!PhotoPrintApp::gIsRegistered, annoy_header);
			break;

		case 'none' :
			mDocument->GetPrintProperties().SetHeader(0.0);
			props.SetHeader("\p");
			props.SetFooter("\p");
			SetAnnoyingwareNotice(!PhotoPrintApp::gIsRegistered, annoy_header);
			break;
	}//end switch

	LPopupButton*	sizePopup = inDialog.FindPopupButton('fSiz');
	SInt16			size = EUtil::SizeFromMenu(sizePopup->GetValue(), sizePopup->GetMacMenuH());
	props.SetFontSize(size);

	LPopupButton*	fontPopup = inDialog.FindPopupButton('font');
	Str255			fontName;
	props.SetFontName(fontPopup->GetMenuItemText(fontPopup->GetCurrentMenuItem(), fontName));


//Margin
	PrintProperties&		printProps = mDocument->GetPrintProperties();
	SetupMarginPropsFromDialog(inDialog, printProps);
	StuffCustomMarginsIfNecessary(inDialog, printProps);

	LGAColorSwatchControl*	color = dynamic_cast<LGAColorSwatchControl*>(inDialog.FindPaneByID('bCol'));
	if (color != nil) {
		RGBColor		theColor;
		color->GetSwatchColor(theColor);
		// !!! set something
	}

	// Since header/footer might have changed, force redraw
	mDocument->GetView()->Refresh();

	// If header/footer has changed position, we need to do layout
	if (needsLayout) {
		// Our subclass may be doing layout for its own reasons, so don't blindly do it here
		if (inDoLayout == kDoLayoutIfNeeded) {
			this->LayoutImages();
			mDocument->GetView()->Refresh();
		}
	}

	return needsLayout;
} // CommitOptionsDialog

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


void		
Layout::ConvertMarginsToDisplayUnits(double& top, double& left, double& bottom, double& right) {

	// currently handling inches only

	top /= kDPI;
	left /= kDPI;
	bottom /= kDPI;
	right /= kDPI;
	
	}//end ConvertMarginsToDisplayUnits




void		
Layout::ConvertMarginsFromDisplayUnits(double& /*top*/, double& /*left*/, double& /*bottom*/, double& /*right*/) {

	// currently handling inches only
	// and since stored in inches, all is well!
		
	}//end ConvertMarginsFromDisplayUnits




/*
CountOrientation
	Doesn't return an actual count (bug 65), but rather a relative number.
*/
UInt32
Layout::CountOrientation(const OSType inType, bool ignoreEmpty) const
{
	UInt32		c = 0;

	PhotoIterator	i;
	for (i = mModel->begin(); i != mModel->end(); i++) {
		PhotoItemRef	item = *i;
	
		if (ignoreEmpty && item->IsEmpty()) continue;

		// if there is no rotation, then the natural bounds suffice for determining orientation.
		// this helps clarify items which have just replaced empty templates
		// since the templates may have had opposite but maleable aspect ratio bounds
		bool useNaturalBounds (PhotoUtility::DoubleEqual(item->GetRotation(), 0.0));
			
		if ((inType == kLandscape && item->IsLandscape(useNaturalBounds)) || 
			(inType == kPortrait &&	item->IsPortrait(useNaturalBounds))) {
			if (item->IsEmpty())
				c++;
			else
				c += 0x400;						// A picture is worth 1K empties
		}
	}

	return c;
} // CountOrientation

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
	for (MDragItemIterator i = end; ++i != end; ) {
		if (i.HasFlavor(kDragFlavor)) {
			count++;		// ??? Is there really only one item?
		} else {
			FSSpec		fsSpec;
			Boolean		ioAllowEvilPromise (true);
			PromiseHFSFlavor	promise;
			if (!i.ExtractFSSpec (fsSpec, ioAllowEvilPromise, promise)) return false;	//	Unknown item type
			
			if (!ioAllowEvilPromise)
				count += CountAcceptableFiles (fsSpec);
		}
	} // for
		
	// We may not want multiple items
	if (!this->CanAddToBackground (count))
		return false;

	return true;
} // DragIsAcceptable

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
GetDistinctImages
*/
SInt16
Layout::GetDistinctImages() {
	if (mModel)
		return mModel->GetCount();
	else
		return 0;
}//end GetDistinctImages



void		
Layout::GetMarginsFromDialog(EDialog& inDialog, double& outTop, double& outLeft, 
												double& outBottom, double& outRight) {
	LPane* top (inDialog.FindPaneByID(Pane_Top));
	ThrowIfNil_(top);
	LPane*  left (inDialog.FindPaneByID(Pane_Left));
	ThrowIfNil_(left);
	LPane*  bottom (inDialog.FindPaneByID(Pane_Bottom));
	ThrowIfNil_(bottom);
	LPane*  right (inDialog.FindPaneByID(Pane_Right));
	ThrowIfNil_(right);

	Str255 text;
	{
	top->GetDescriptor(text);
	MP2CStr cText (text);
	sscanf(cText, "%lf", &outTop);
	}
	{
	left->GetDescriptor(text);
	MP2CStr cText (text);
	sscanf(cText, "%lf", &outLeft);
	}
	{
	bottom->GetDescriptor(text);
	MP2CStr cText (text);
	sscanf(cText, "%lf", &outBottom);
	}
	{
	right->GetDescriptor(text);
	MP2CStr cText (text);
	sscanf(cText, "%lf", &outRight);
	}

	ConvertMarginsFromDisplayUnits(outTop, outLeft, outBottom, outRight);
}//end GetMarginsFromDialog







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
				mDocument->GetPrintProperties().SetHeader(lineHeight + kHeaderSpacing);
				props.SetHeader(PhotoPrintApp::gAnnoyanceText);
				}//endif need annoyance
			else
				mDocument->GetPrintProperties().SetHeader(0.0);
			break;
			}//case
		case annoy_footer: {
			if (inState) {
				mDocument->GetPrintProperties().SetFooter(lineHeight + kFooterSpacing);
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


//------------------------------------------------------------------
// SetupMargins
//------------------------------------------------------------------

void
Layout::SetupMargins(EDialog& inDialog) {
	PrintProperties&		printProps = mDocument->GetPrintProperties();

	LRadioGroupView*	marginView = inDialog.FindRadioGroupView(RadioGroupView_Position);
	if (marginView != nil) {
		PrintProperties::MarginType margin = printProps.GetMarginType();
		LPane*	button (nil);

		switch (margin) {
			case PrintProperties::kMinimalMargins :
				button = inDialog.FindPaneByID(Pane_MinimalMargins);
				break;
			case PrintProperties::kFullSymmetric :
				button = inDialog.FindPaneByID(Pane_SymmetricMargins);
				break;
			case PrintProperties::kCustom:
				button = inDialog.FindPaneByID(Pane_CustomMargins);
				break;
			}//end switch
		if (button != nil)
			button->SetValue(1);

		// Binder margin (for 3-hole punching, at least in the USA)
		LPane*		binderMargin = inDialog.FindPaneByID('3hol');
		if (binderMargin != nil) {
			if (mBinderMargin != 0)
				binderMargin->SetValue(Button_On);
			else
				binderMargin->SetValue(Button_Off);
		}//endif binderMargin found
	
	UpdateMargins(inDialog, false);
	}//endif marginView exists
}//end SetupMargins



/*
SetupOptionsDialog
	Subclasses should override (and call inherited)
*/
void
Layout::SetupOptionsDialog(EDialog& inDialog)
{


	// Set up title stuff
	DocumentProperties&		props = mDocument->GetProperties();

	LRadioGroupView*	titlePos = inDialog.FindRadioGroupView(RadioGroupView_Position);
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


//Margin Stuff
	SetupMargins(inDialog);
	
	// We no longer have a background color in the dialogs, but this is harmless
	LGAColorSwatchControl*	color = dynamic_cast<LGAColorSwatchControl*>(inDialog.FindPaneByID('bCol'));
	if (color != nil) {
		color->SetSwatchColor(Color_White);
	}
} // SetupOptionsDialog




void
Layout::SetupMarginPropsFromDialog(EDialog& inDialog, PrintProperties& inProps) 
{
	LRadioGroupView*		marginView = inDialog.FindRadioGroupView(RadioGroupView_Margins);
	if (marginView != nil) {
		PaneIDT marginButton = marginView->GetCurrentRadioID();
		if (marginButton != nil) {
			switch (marginButton) {
				case Pane_MinimalMargins:
					inProps.SetMarginType(PrintProperties::kMinimalMargins);
					break;
				case Pane_SymmetricMargins:
					inProps.SetMarginType(PrintProperties::kFullSymmetric);
					break;
				case Pane_CustomMargins:
					inProps.SetMarginType(PrintProperties::kCustom);
					break;
				}//switch	
			}//endif marginButton exists


		LPane*		binderMargin = inDialog.FindPaneByID('3hol');
		if ((binderMargin != nil)  && (binderMargin->IsEnabled())) {
			inProps.SetBinderHoles(binderMargin->GetValue());
		}//endif binderMargin found
	}//endif there is a margin panel

}//end SetupMarginPropsFromDialog



void
Layout::StuffCustomMarginsIfNecessary(EDialog& inDialog, PrintProperties& inProps) {
	if (inProps.GetMarginType() == PrintProperties::kCustom) {
		double top, left, bottom, right;
		GetMarginsFromDialog(inDialog, top, left, bottom, right);
		inProps.SetMargins(top, left, bottom, right);
		}//endif
	}//


void
Layout::UpdateMargins(EDialog& inDialog, bool inUseDialog) {

	PrintProperties& printProps (mDocument->GetPrintProperties());

	if (inUseDialog) {
		if (inDialog.FindRadioGroupView(RadioGroupView_Margins)->GetCurrentRadioID() != Pane_CustomMargins){
			StuffCustomMarginsIfNecessary(inDialog, printProps);
			}//endif switching from custom, save the custom values in the printprops
		SetupMarginPropsFromDialog(inDialog, printProps);
		}//endif

	// get the values (at 72dpi), convert to inches
	// and stuff those fields!
	MRect paper;
	MRect page;
	PhotoPrinter::CalculatePaperRect(&*mDocument->GetPrintRec(), &printProps, paper, kDPI);
	PhotoPrinter::CalculatePrintableRect(&*mDocument->GetPrintRec(), &printProps, page, kDPI);
	page.Offset(paper.left, paper.top);
	
	double fTop ((page.top - paper.top) );
	double fLeft ((page.left - paper.left) );
	double fRight ((paper.right - page.right) );
	double fBot ((paper.bottom - page.bottom) );


	ConvertMarginsToDisplayUnits(fTop, fLeft, fRight, fBot);


	char	text[256];
	{
	sprintf(text, "%.4g", fTop);
	MC2PStr pText (text);
	LPane* pane (inDialog.FindPaneByID(Pane_Top));
	if (pane != nil)
		pane->SetDescriptor(pText);
	}
	{
	sprintf(text, "%.4g", fLeft);
	MC2PStr pText (text);
	LPane* pane (inDialog.FindPaneByID(Pane_Left));
	if (pane != nil)
		pane->SetDescriptor(pText);
	}
	{
	sprintf(text, "%.4g", fRight);
	MC2PStr pText (text);
	LPane* pane (inDialog.FindPaneByID(Pane_Right));
	if (pane != nil)
		pane->SetDescriptor(pText);
	}
	{
	sprintf(text, "%.4g", fBot);
	MC2PStr pText (text);
	LPane* pane (inDialog.FindPaneByID(Pane_Bottom));
	if (pane != nil)
		pane->SetDescriptor(pText);
	}
	

}//end UpdateMargins