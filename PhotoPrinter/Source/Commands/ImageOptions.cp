/*
	File:		ImageOptions.cp

	Contains:	Implementation of the Image Options dialog.

	Written by:	David Dunham and Dav Lion

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

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
#include <LGAColorSwatchControl.h>
#include "MPString.h"
#include "PhotoPrintDoc.h"

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
			Layout*		layout = mDoc->GetView()->GetLayout();
			layout->CommitOptionsDialog(theDialog);
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
	*ioStatus->enabled = mDoc->GetModel()->IsAnythingSelected();
} // FindCommandStatus

#pragma mark -

/*
ImageOptionsDialog
*/
ImageOptionsDialog::ImageOptionsDialog(LCommander* inSuper)
	: EDialog(PPob_ImageOptions, inSuper)
{
	int		i;
	for (i = 1; i <= panel_COUNT; i++)
		mInitialized[i - 1] = false;

	this->SetupRotation();							// Initialize the first panel
	this->Initialized(panel_Rotation);				// Mark it as initialized
} // ImageOptionsDialog

/*
~ImageOptionsDialog
*/
ImageOptionsDialog::~ImageOptionsDialog()
{
} // ~ImageOptionsDialog

/*
Initialized
	Returns whether a particular panel has been initialized.
	Side effect: sets that it has.
*/
bool
ImageOptionsDialog::Initialized(const SInt32 inIndex)
{
	if (mInitialized[inIndex - 1])
		return true;

	mInitialized[inIndex - 1] = true;
	return false;
} // Initialized

/*
ListenToMessage {OVERRIDE}
*/
void
ImageOptionsDialog::ListenToMessage(
	MessageT	inMessage,
	void*		ioParam)
{
	if (inMessage == 'tabs') {
		// This message means that a tab has been switched. We can now initialize things for the
		// new panel (this is both lazy instantiation, and we couldn't pre-initialize in any case
		// since the LPanes didn't exist until now).
		SInt32		panel = *(SInt32*)ioParam;
		if (this->Initialized(panel))
			return;
	
		switch (panel) {
			case panel_Rotation:
				this->SetupRotation();
				break;

			case panel_Size:
				this->SetupSize();
				break;

			case panel_Text:
				this->SetupText();
				break;

			case panel_Shape:
				this->SetupShape();
				break;

			case panel_Frame:
				this->SetupFrame();
				break;
		}

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
	PhotoItemRef		theItem = theDoc->GetModel()->GetSelection();

	LGAColorSwatchControl*	color = dynamic_cast<LGAColorSwatchControl*>(this->FindPaneByID('fCol'));
	if (color != nil) {
		color->SetSwatchColor(theItem->GetProperties().GetFrameColor());
	}
} // SetupFrame

/*
SetupRotation
*/
void
ImageOptionsDialog::SetupRotation()
{
	StCursor			watch;
	MRect				thumbBounds(0, 0, 64, 64);
	MRect				bounds;
	PhotoPrintDoc*		theDoc = dynamic_cast<PhotoPrintDoc*>(this->GetSuperCommander());
	PhotoItemRef		theItem = theDoc->GetModel()->GetSelection();
	PicHandle			pict;
	ControlButtonContentInfo	ci;

	// Set up rotation thumbnails
	LBevelButton*		rotate0 = dynamic_cast<LBevelButton*>(this->FindPaneByID('000¡'));
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

	LBevelButton*		rotate90 = dynamic_cast<LBevelButton*>(this->FindPaneByID('090¡'));
	if (rotate90 != nil) {
		mImage90.SetFile(*theItem);
		AlignmentGizmo::FitAndAlignRectInside(mImage90.GetNaturalBounds(), thumbBounds,
			kAlignAbsoluteCenter, bounds);
		mImage90.SetRotation(90);
		mImage90.SetDest(bounds);
		mImage90.MakeProxy(nil);
		pict = mImage90.GetProxy();
		ci.contentType = kControlContentPictHandle;
		ci.u.picture = pict;
		rotate90->SetContentInfo(ci);
		if (mImage90.GetRotation() == theItem->GetRotation())
			rotate90->SetValue(Button_On);
	}

	LBevelButton*		rotate180 = dynamic_cast<LBevelButton*>(this->FindPaneByID('180¡'));
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

	LBevelButton*		rotate270 = dynamic_cast<LBevelButton*>(this->FindPaneByID('270¡'));
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
} // SetupRotation

/*
SetupShape
*/
void
ImageOptionsDialog::SetupShape()
{
	LRadioGroupView*	shapeButtons = dynamic_cast<LRadioGroupView*>(this->FindPaneByID('shap'));
	if (shapeButtons != nil) {
		shapeButtons->SetCurrentRadioID('squa');
	}
} // SetupShape

/*
SetupSize
*/
void
ImageOptionsDialog::SetupSize()
{
} // SetupSize

/*
SetupText
	Initialize the Text panel
*/
void
ImageOptionsDialog::SetupText()
{
	PhotoPrintDoc*		theDoc = dynamic_cast<PhotoPrintDoc*>(this->GetSuperCommander());
	PhotoItemRef		theItem = theDoc->GetModel()->GetSelection();

	LPane*				dateCheck = this->FindPaneByID('fdat');
	if (dateCheck != nil) {
		dateCheck->SetValue(theItem->GetProperties().GetShowDate());
	}
	LPane*				fileName = this->FindPaneByID('fnam');
	if (fileName != nil) {
		MPString		text;
		fileName->GetDescriptor(text);
		// !!! assumes there is a name
		text.Replace(theItem->GetFile()->Name(), "\p#");
		fileName->SetDescriptor(text);

		fileName->SetValue(theItem->GetProperties().GetShowName());
	}
} // SetupText
