/*
	File:		ImageOptions.cp

	Contains:	Implementation of the Image Options dialog.

	Written by:	David Dunham and Dav Lion

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

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
#include "PhotoPrintDoc.h"
#include <LMultiPanelView.h>

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
	*ioStatus->enabled = mDoc->GetView()->IsAnythingSelected();
} // FindCommandStatus

#pragma mark -

/*
ImageOptionsDialog
*/
ImageOptionsDialog::ImageOptionsDialog(LCommander* inSuper)
	: EDialog(PPob_ImageOptions, inSuper)
{
	// Be sure our thumbnail images donÕt have captions (no matter what the current
	// preference is)
	mImage0.GetProperties().SetCaptionStyle(caption_None);
	mImage90.GetProperties().SetCaptionStyle(caption_None);
	mImage180.GetProperties().SetCaptionStyle(caption_None);
	mImage270.GetProperties().SetCaptionStyle(caption_None);

	LMultiPanelView* tabView = (LMultiPanelView*)this->FindPaneByID('tabv');
	tabView->CreateAllPanels();						// Be sure they are instantiated so they are
													// there for when we Commit
	
	this->SetupImage();								// Initialize the first panel
	this->SetupText();
	this->SetupFrame();
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
		double			newRotation;

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
		if (theItem->GetRotation() != newRotation) {
			theItem->SetRotation(newRotation);
			theItem->MakeProxy(nil);
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

	// Text
	LPopupButton*	sizePopup = this->FindPopupButton('fSiz');
	SInt16			size = EUtil::SizeFromMenu(sizePopup->GetValue(), sizePopup->GetMacMenuH());
	theItem->GetProperties().SetFontSize(size);

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
		theItem->GetProperties().SetCaptionStyle(caption_Bottom);
		theItem->AdjustRectangles();
	}

	// We could be smarter about checking for actual changes
	theDoc->GetModel()->SetDirty();

	if (needsLayout) {
		Layout*		layout = theDoc->GetView()->GetLayout();
		layout->LayoutImages();
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
	if (theItem->GetProperties().GetShadow()) {
		shadow->SetValue(Button_On);
		shadowColor->Enable();
	} else {
		shadow->SetValue(Button_Off);
		shadowColor->Disable();
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
	PicHandle			pict;
	ControlButtonContentInfo	ci;

	// Set up rotation thumbnails
	LBevelButton*		rotate0 = this->FindBevelButton('000¡');
	if (rotate0 != nil) {
		mImage0.SetFile(*theItem);
		AlignmentGizmo::FitAndAlignRectInside(mImage0.GetNaturalBounds(), thumbBounds,
			kAlignAbsoluteCenter, bounds);
		
		mImage0.SetDest(bounds);
		mImage0.MakeProxy(nil);
		pict = mImage0.GetProxy();
		ci.contentType = kControlContentPictHandle;
		ci.u.picture = pict;
		rotate0->SetContentInfo(ci);
		if (mImage0.GetRotation() == theItem->GetRotation())
			rotate0->SetValue(Button_On);
	}

	LBevelButton*		rotate90 = this->FindBevelButton('090¡');
	if (rotate90 != nil) {
		mImage90.SetFile(*theItem);
		AlignmentGizmo::FitAndAlignRectInside(mImage90.GetNaturalBounds(), thumbBounds,
			kAlignAbsoluteCenter, bounds);
		mImage90.SetRotation(90);
		mImage90.SetDest(bounds);
		mImage90.MakeProxy(nil);
		pict = mImage90.GetProxy();		// ??? just rotate the first proxy
		ci.contentType = kControlContentPictHandle;
		ci.u.picture = pict;
		rotate90->SetContentInfo(ci);
		if (mImage90.GetRotation() == theItem->GetRotation())
			rotate90->SetValue(Button_On);
	}

	LBevelButton*		rotate180 = this->FindBevelButton('180¡');
	if (rotate180 != nil) {
		mImage180.SetFile(*theItem);
		AlignmentGizmo::FitAndAlignRectInside(mImage180.GetNaturalBounds(), thumbBounds,
			kAlignAbsoluteCenter, bounds);
		mImage180.SetRotation(180);
		mImage180.SetDest(bounds);
		mImage180.MakeProxy(nil);
		pict = mImage180.GetProxy();
		ci.contentType = kControlContentPictHandle;
		ci.u.picture = pict;
		rotate180->SetContentInfo(ci);
		if (mImage180.GetRotation() == theItem->GetRotation())
			rotate180->SetValue(Button_On);
	}

	LBevelButton*		rotate270 = this->FindBevelButton('270¡');
	if (rotate270 != nil) {
		mImage270.SetFile(*theItem);
		AlignmentGizmo::FitAndAlignRectInside(mImage270.GetNaturalBounds(), thumbBounds,
			kAlignAbsoluteCenter, bounds);
		mImage270.SetRotation(270);
		mImage270.SetDest(bounds);
		mImage270.MakeProxy(nil);
		pict = mImage270.GetProxy();
		ci.contentType = kControlContentPictHandle;
		ci.u.picture = pict;
		rotate270->SetContentInfo(ci);
		if (mImage270.GetRotation() == theItem->GetRotation())
			rotate270->SetValue(Button_On);
	}

	// Size
	// ??? kludge so far
	LPopupButton*		sizePopup = this->FindPopupButton('iSiz');
	if (sizePopup != nil) {
		OSType		dimCode;
		LStr255		dimensions;	
		dimCode = theItem->GetDimensions(dimensions, PhotoPrintItem::si_OtherDimensions);
		
		sizePopup->SetMenuItemText(13, dimensions);
		sizePopup->SetCurrentMenuItem(13);
	}
} // SetupImage

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
		text.Replace(theItem->GetFile()->Name(), "\p#");
		fileName->SetDescriptor(text);

		fileName->SetValue(props.GetShowName());
	}

	SInt16			i;
	LPopupButton*	sizePopup = this->FindPopupButton('fSiz');
	SInt16			nItems = ::CountMenuItems(sizePopup->GetMacMenuH());
	for (i = 1; i <= nItems; i++) {
		if (EUtil::SizeFromMenu(i, sizePopup->GetMacMenuH()) == props.GetFontSize()) {
			sizePopup->SetCurrentMenuItem(i);
			break;
		}
	}
	LPopupButton*	fontPopup = this->FindPopupButton('font');
	nItems = ::CountMenuItems(fontPopup->GetMacMenuH());
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
