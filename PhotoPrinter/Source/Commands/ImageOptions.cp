/*
	File:		ImageOptions.cp

	Contains:	Implementation of the Image Options dialog.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

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
	MRect				thumbBounds(0, 0, 64, 64);
	MRect				bounds, rotatedBounds;
	PhotoPrintDoc*		theDoc = dynamic_cast<PhotoPrintDoc*>(this->GetSuperCommander());
	PhotoItemRef		theItem = theDoc->GetModel()->GetSelection();
	PicHandle			pict;
	ControlButtonContentInfo	ci;

	// Set up rotation thumbnails
	LBevelButton*		rotate0 = dynamic_cast<LBevelButton*>(this->FindPaneByID('000°'));
	if (rotate0 != nil) {
		// !!! there is a big fat leak here
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

	LBevelButton*		rotate90 = dynamic_cast<LBevelButton*>(this->FindPaneByID('090°'));
	if (rotate90 != nil) {
		mImage90.SetFile(*theItem);
		AlignmentGizmo::FitAndAlignRectInside(mImage90.GetNaturalBounds(), thumbBounds,
			kAlignAbsoluteCenter, bounds);
		rotatedBounds.top = bounds.left;
		rotatedBounds.left = bounds.top;
		rotatedBounds.bottom = bounds.right;
		rotatedBounds.right = bounds.bottom;
		mImage90.SetRotation(90);
		mImage90.SetScreenDest(rotatedBounds);
		mImage90.MakeProxy(nil);
		pict = mImage90.GetProxy();
		ci.contentType = kControlContentPictHandle;
		ci.u.picture = pict;
		rotate90->SetContentInfo(ci);
		if (mImage90.GetRotation() == theItem->GetRotation())
			rotate90->SetValue(Button_On);
	}

	LBevelButton*		rotate180 = dynamic_cast<LBevelButton*>(this->FindPaneByID('180°'));
	if (rotate180 != nil) {
		mImage180.SetFile(*theItem);
		AlignmentGizmo::FitAndAlignRectInside(mImage180.GetNaturalBounds(), thumbBounds,
			kAlignAbsoluteCenter, bounds);
		mImage180.SetRotation(180);
		mImage180.SetScreenDest(bounds);
		mImage180.MakeProxy(nil);
		pict = mImage180.GetProxy();
		ci.contentType = kControlContentPictHandle;
		ci.u.picture = pict;
		rotate180->SetContentInfo(ci);
		if (mImage180.GetRotation() == theItem->GetRotation())
			rotate180->SetValue(Button_On);
	}

	LBevelButton*		rotate270 = dynamic_cast<LBevelButton*>(this->FindPaneByID('270°'));
	if (rotate270 != nil) {
		mImage270.SetFile(*theItem);
		AlignmentGizmo::FitAndAlignRectInside(mImage270.GetNaturalBounds(), thumbBounds,
			kAlignAbsoluteCenter, bounds);
		rotatedBounds.top = bounds.left;
		rotatedBounds.left = bounds.top;
		rotatedBounds.bottom = bounds.right;
		rotatedBounds.right = bounds.bottom;
		mImage270.SetRotation(270);
		mImage270.SetScreenDest(rotatedBounds);
		mImage270.MakeProxy(nil);
		pict = mImage270.GetProxy();
		ci.contentType = kControlContentPictHandle;
		ci.u.picture = pict;
		rotate270->SetContentInfo(ci);
		if (mImage270.GetRotation() == theItem->GetRotation())
			rotate270->SetValue(Button_On);
	}
} // ImageOptionsDialog

/*
~ImageOptionsDialog
*/
ImageOptionsDialog::~ImageOptionsDialog()
{
} // ~ImageOptionsDialog

/*
ListenToMessage {OVERRIDE}
*/
void
ImageOptionsDialog::ListenToMessage(
	MessageT	inMessage,
	void*		ioParam)
{
	if (inMessage == 'tabs') {
		// This message means that a tab has been switched. The radio group (and any other views) is
		// not created ahead of time, so this is a plausible place to initialize. (??? This should
		// probably be improved for real.)
		LRadioGroupView*	shapeButtons = dynamic_cast<LRadioGroupView*>(this->FindPaneByID('shap'));
		if (shapeButtons != nil) {
			shapeButtons->SetCurrentRadioID('squa');
		}
	} else {
		EDialog::ListenToMessage(inMessage, ioParam);
	}
} // ListenToMessage
