/*
	File:		ImageOptions.cp

	Contains:	Implementation of the Image Options dialog.

	Written by:	David Dunham and Dav Lion

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		03 Jul 2001		drd		Use GetValue() to read font popup; font must be in valid range
		03 Jul 2001		drd		38 Font size popup is now a text field
		26 Jun 2001		drd		86 Don't enable command for placeholder
		21 Jun 2001		drd		80 Refresh before and after sending Layout
		20 Jun 2001		drd		76 Commit tries even harder to avoid rotating
		17 Jan 2001		drd		SetupImage sets rotation to dummy value first; SetupInfo stub
		19 Sep 2000		drd		Commit changes size of image (and layout)
		18 Sep 2000		drd		Disable image size/orientation change for school layout
		14 Sep 2000		drd		Display natural bounds
		14 Sep 2000		drd		Popup displays actual size
		14 Sep 2000		drd		Started work on displaying actual size in dialog
		04 aug 2000		dml		PhotoPrintItem::GetFile replaced with GetFileSpec
		04 Aug 2000		drd		We now call CreateAllPanels, so be sure to initialize each panel
		03 aug 2000		dml		selection moved to view.  Note:  we are using PrimarySelection only, not multiple
		12 Jul 2000		drd		Set font and size earlier (so AdjustRectangles can use them)
		12 Jul 2000		drd		Hooked up font, size
		11 Jul 2000		drd		Be sure images used as thumbnails have no captions
		07 Jul 2000		drd		Call SwitchTarget; fewer panels; Commit
		06 Jul 2000		drd		Split panel setup into separate methods
		05 jul 2000		dml		call SetDest, not SetScreenDest
		05 Jul 2000		drd		Send CommitOptionsDialog; use instance data to avoid leaks
		05 Jul 2000		drd		More rotation thumbnails
		03 Jul 2000		drd		Started creating rotation thumbnails
		29 Jun 2000		drd		Set up dialog (assuming single selection)
		14 Jun 2000		drd		Created
*/

#include "ImageOptions.h"
#include "AlignmentGizmo.h"
#include "Layout.h"
#include <LBevelButton.h>
#include <LEditText.h>
#include <LGAColorSwatchControl.h>
#include <LPopupButton.h>
#include "MPString.h"
#include "PhotoPrintConstants.h"
#include "PhotoPrintDoc.h"
#include <LMultiPanelView.h>
#include <LTabsControl.h>
#include "PhotoUtility.h"

/*
ImageOptionsCommand
*/
ImageOptionsCommand::ImageOptionsCommand(const CommandT inCommand, PhotoPrintDoc* inDoc)
	: PhotoDocCommandAttachment(inCommand, inDoc)
{
} // ImageOptionsCommand

/*
~ImageOptionsCommand
*/
ImageOptionsCommand::~ImageOptionsCommand()
{
} // ~ImageOptionsCommand

/*
ExecuteCommand
*/
void		
ImageOptionsCommand::ExecuteCommand(void* inCommandData)
{
#pragma unused(inCommandData)

	ImageOptionsDialog		theDialog(mDoc);

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
ImageOptionsCommand::FindCommandStatus		(SCommandStatus*	ioStatus)
{
	PhotoItemRef		theItem = mDoc->GetView()->GetPrimarySelection();
	if (theItem != nil && !theItem->IsEmpty())
		*ioStatus->enabled = true;
	else
		*ioStatus->enabled = false;
} // FindCommandStatus

#pragma mark -

/*
ImageOptionsDialog
*/
ImageOptionsDialog::ImageOptionsDialog(LCommander* inSuper)
	: EDialog(PPob_ImageOptions, inSuper)
{
	LTabsControl*		theTabs = FindPaneType<LTabsControl>('tabs', this);
	LMultiPanelView*	tabView = FindPaneType<LMultiPanelView>('tabv', this);
	tabView->CreateAllPanels();						// Be sure they are instantiated so they are
													// there for when we Commit

	PhotoPrintDoc*		theDoc = dynamic_cast<PhotoPrintDoc*>(this->GetSuperCommander());
	Layout*				layout = theDoc->GetView()->GetLayout();

	if (layout->CanEditImages())
		this->SetupImage();							// Initialize the first panel
	else {
		// Image panel is not relevant, so disable it and skip to the next one 
		theTabs->SetValue(panel_Text);
		theTabs->EnableTab(panel_Image, false);
	}
	this->SetupText();
	this->SetupFrame();
	this->SetupInfo();
} // ImageOptionsDialog

/*
~ImageOptionsDialog
*/
ImageOptionsDialog::~ImageOptionsDialog()
{
} // ~ImageOptionsDialog

/*
Commit
	User clicked OK, so commit all changes to the item. Note that this needs to handle
	cases where the user never instantiates a panel.
	!!! This is not undo-able
*/
void
ImageOptionsDialog::Commit()
{
	PhotoPrintDoc*		theDoc = dynamic_cast<PhotoPrintDoc*>(this->GetSuperCommander());
	PhotoItemRef		theItem (theDoc->GetView()->GetPrimarySelection());
	PhotoItemProperties&	props(theItem->GetProperties());
	Layout*				layout = theDoc->GetView()->GetLayout();
	bool				needsLayout = false;

	// Frame
	LGAColorSwatchControl*	color = dynamic_cast<LGAColorSwatchControl*>(this->FindPaneByID('fCol'));
	if (color != nil) {
		RGBColor		theColor;
		color->GetSwatchColor(theColor);
		props.SetFrameColor(theColor);
	}

	// Rotation
	LRadioGroupView*	group = this->FindRadioGroupView('rota');
	if (group != nil) {
		PaneIDT			orientation = group->GetCurrentRadioID();
		double			newRotation = -90;

		switch (orientation) {
			case '000¡':
				newRotation = 0;
				break;

			case '090¡':
				newRotation = 90;
				break;

			case '180¡':
				newRotation = 180;
				break;

			case '270¡':
				newRotation = 270;
				break;
		};
		if (newRotation >= 0 && theItem->GetRotation() != newRotation) {
			theItem->SetRotation(newRotation);
			theDoc->GetModel()->SetDirty();
			needsLayout = true;
		}
	}

	// Shape
	LRadioGroupView*	shapeButtons = this->FindRadioGroupView('shap');
	if (shapeButtons != nil) {
		PaneIDT			theShape = shapeButtons->GetCurrentRadioID();
		// !!! map back to enum, or change the enum
	}

	// Size
	LRadioGroupView*	radio = this->FindRadioGroupView('matc');
	FitT				fit = kFit;
	switch (radio->GetCurrentRadioID()) {
		case 'crop':
			fit = kCrop;
			break;

		case 'fit ':
			fit = kFit;
			break;

		case 'stre':
			fit = kStretch;
			break;
	}
	LPopupButton*		sizePopup = this->FindPopupButton('iSiz');
	if (sizePopup != nil) {
		ResIDT		menuID = sizePopup->GetMenuID();
		LMenu		shadowMenu(menuID);
		CommandT	theCommand = shadowMenu.CommandFromIndex(sizePopup->GetValue());
		needsLayout |= layout->ResizeImage(theCommand, fit, theItem);
	}
	// !!!

	// Text
	LEditText*		sizeField = this->FindEditText('fSiz');
	SInt16			size = sizeField->GetValue();
	if (size >= kMinFontSize && size <= kMaxFontSize) {
		theItem->GetProperties().SetFontSize(size);
	}

	Str255			fontName;
	LPopupButton*	fontPopup = this->FindPopupButton('font');
	fontPopup->GetMenuItemText(fontPopup->GetCurrentMenuItem(), fontName);
	SInt16			fontID;
	::GetFNum(fontName, &fontID);
	theItem->GetProperties().SetFontNumber(fontID);

	LPane*				caption = this->FindPaneByID('capt');
	if (caption != nil) {
		MPString		theString;
		caption->GetDescriptor(theString);
		props.SetCaption(theString);
	}
	LPane*				dateCheck = this->FindPaneByID('fdat');
	if (dateCheck != nil) {
		props.SetShowDate(dateCheck->GetValue());
	}
	LPane*				fileName = this->FindPaneByID('fnam');
	if (fileName != nil) {
		props.SetShowName(fileName->GetValue());

		// !!! A bit of a kludge
		if (theItem->GetProperties().GetCaptionStyle() == caption_None)
			theItem->GetProperties().SetCaptionStyle(caption_Bottom);
		theItem->AdjustRectangles();
	}

	// We could be smarter about checking for actual changes
	theDoc->GetModel()->SetDirty();

	if (needsLayout) {
		theDoc->GetView()->Refresh();		// Doc orientation may change, so refresh before AND after
		layout->LayoutImages();
		theDoc->GetView()->Refresh();
	}
} // Commit

/*
ListenToMessage {OVERRIDE}
*/
void
ImageOptionsDialog::ListenToMessage(
	MessageT	inMessage,
	void*		ioParam)
{
	if (inMessage == 'shad') {
		LPane*		shadowColor = this->FindPaneByID('sCol');
		if (*(SInt32*)ioParam)
			shadowColor->Enable();
		else
			shadowColor->Disable();
	} else if (inMessage == 'tabs') {
		// This message means that a tab has been switched. Most of the initialization has
		// already happened.
		SInt32		panel = *(SInt32*)ioParam;

		if (panel == panel_Text) {
			LEditText*	field = this->FindEditText('capt');
			if (field != nil) {
				LCommander::SwitchTarget(field);
				field->SelectAll();
			}
		}

		// We may have a bunch of new views from the new panel; listen to them
		UReanimator::LinkListenerToBroadcasters(this, this->GetDialog(), PPob_ImageOptions + panel);
	} else {
		EDialog::ListenToMessage(inMessage, ioParam);
	}
} // ListenToMessage

/*
SetupFrame
*/
void
ImageOptionsDialog::SetupFrame()
{
	PhotoPrintDoc*		theDoc = dynamic_cast<PhotoPrintDoc*>(this->GetSuperCommander());
	PhotoItemRef		theItem = theDoc->GetView()->GetPrimarySelection();

	LGAColorSwatchControl*	color = dynamic_cast<LGAColorSwatchControl*>(this->FindPaneByID('fCol'));
	if (color != nil) {
		color->SetSwatchColor(theItem->GetProperties().GetFrameColor());
	}

	LRadioGroupView*	shapeButtons = this->FindRadioGroupView('shap');
	if (shapeButtons != nil) {
		shapeButtons->SetCurrentRadioID('squa');
	}

	LPane*		shadow = this->FindPaneByID('shad');
	LPane*		shadowColor = this->FindPaneByID('sCol');
	if (shadowColor != nil) {
		if (theItem->GetProperties().GetShadow()) {
			shadow->SetValue(Button_On);
			shadowColor->Enable();
		} else {
			shadow->SetValue(Button_Off);
			shadowColor->Disable();
		}
	}
} // SetupFrame

/*
SetupImage
*/
void
ImageOptionsDialog::SetupImage()
{
	StCursor			watch;
	MRect				thumbBounds(0, 0, 64, 64);
	MRect				bounds;
	PhotoPrintDoc*		theDoc = dynamic_cast<PhotoPrintDoc*>(this->GetSuperCommander());
	PhotoItemRef		theItem = theDoc->GetView()->GetPrimarySelection();
	ControlButtonContentInfo	ci;

	theItem->MakeRotatedThumbnails(mImage0, mImage90, mImage180, mImage270, thumbBounds);
	
	// Set up rotation thumbnails
	LRadioGroupView*	group = this->FindRadioGroupView('rota');
	if (group != nil) {
		group->SetCurrentRadioID('othR');
	}

	LBevelButton*		rotate0 = this->FindBevelButton('000¡');
	if (rotate0 != nil) {
		ci.contentType = kControlContentPictHandle;
		ci.u.picture = mImage0;
		rotate0->SetContentInfo(ci);
		if (PhotoUtility::DoubleEqual(theItem->GetRotation(), 0.0))
			rotate0->SetValue(Button_On);
	}

	LBevelButton*		rotate90 = this->FindBevelButton('090¡');
	if (rotate90 != nil) {
		ci.contentType = kControlContentPictHandle;
		ci.u.picture = mImage90;
		rotate90->SetContentInfo(ci);
		if (PhotoUtility::DoubleEqual(theItem->GetRotation(), 90.0))
			rotate90->SetValue(Button_On);
	}

	LBevelButton*		rotate180 = this->FindBevelButton('180¡');
	if (rotate180 != nil) {
		ci.contentType = kControlContentPictHandle;
		ci.u.picture = mImage180;
		rotate180->SetContentInfo(ci);
		if (PhotoUtility::DoubleEqual(theItem->GetRotation(), 180.0))
			rotate180->SetValue(Button_On);
	}

	LBevelButton*		rotate270 = this->FindBevelButton('270¡');
	if (rotate270 != nil) {
		ci.contentType = kControlContentPictHandle;
		ci.u.picture = mImage270;
		rotate270->SetContentInfo(ci);
		if (PhotoUtility::DoubleEqual(theItem->GetRotation(), 270.0))
			rotate270->SetValue(Button_On);
	}

	// Size
	LPopupButton*		sizePopup = this->FindPopupButton('iSiz');
	if (sizePopup != nil) {
		OSType		dimCode;
		LStr255		dimensions;	
		dimCode = theItem->GetDimensions(dimensions, theDoc->GetResolution(), PhotoPrintItem::si_NaturalBounds);
		
		LPane*		dataPane = this->FindPaneByID('data');
		dataPane->SetDescriptor(dimensions);

		dimCode = theItem->GetDimensions(dimensions, theDoc->GetResolution(), PhotoPrintItem::si_OtherDimensions);
		ResIDT	menuID = sizePopup->GetMenuID();
		LMenu	shadowMenu(menuID);
		SInt16	nItems = ::CountMenuItems(shadowMenu.GetMacMenuH());
		SInt16	i;
		for (i = 1; i <= nItems; i++) {
			CommandT	theCommand = shadowMenu.CommandFromIndex(i);
			if (theCommand == dimCode) {
				// We found it
				if (dimCode == 'cust')
					sizePopup->SetMenuItemText(i, dimensions);
				sizePopup->SetValue(i);
				break;
			}
		} // end for
	}
} // SetupImage

/*
SetupInfo
*/
void
ImageOptionsDialog::SetupInfo()
{
	PhotoPrintDoc*		theDoc = dynamic_cast<PhotoPrintDoc*>(this->GetSuperCommander());
	PhotoItemRef		theItem = theDoc->GetView()->GetPrimarySelection();

} // SetupInfo

/*
SetupText
	Initialize the Text panel
*/
void
ImageOptionsDialog::SetupText()
{
	PhotoPrintDoc*		theDoc = dynamic_cast<PhotoPrintDoc*>(this->GetSuperCommander());
	PhotoItemRef		theItem = theDoc->GetView()->GetPrimarySelection();
	PhotoItemProperties&	props(theItem->GetProperties());

	LPane*				caption = this->FindPaneByID('capt');
	if (caption != nil) {
		caption->SetDescriptor(props.GetCaption());
	}

	LPane*				dateCheck = this->FindPaneByID('fdat');
	if (dateCheck != nil) {
		dateCheck->SetValue(props.GetShowDate());
	}
	LPane*				fileName = this->FindPaneByID('fnam');
	if (fileName != nil) {
		MPString		text;
		fileName->GetDescriptor(text);
		// !!! assumes there is a name
		Str255 itemName;
		theItem->GetName(itemName);
		text.Replace(itemName, "\p#");
		fileName->SetDescriptor(text);

		fileName->SetValue(props.GetShowName());
	}

	SInt16			i;
	LEditText*		sizeField = this->FindEditText('fSiz');
	LStr255			sizeText(props.GetFontSize());
	sizeField->SetDescriptor(sizeText);
	LPopupButton*	fontPopup = this->FindPopupButton('font');
	SInt16			nItems = ::CountMenuItems(fontPopup->GetMacMenuH());
	LStr255			itemFont, fontName;
	::GetFontName(props.GetFontNumber(), itemFont);
	for (i = 1; i <= nItems; i++) {
		fontPopup->GetMenuItemText(i, fontName);
		if (fontName == itemFont) {
			fontPopup->SetValue(i);
			break;
		}
	}
} // SetupText
