/*
	File:		ImageOptions.cp

	Contains:	Implementation of the Image Options dialog.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		03 Jul 2000		drd		Started creating rotation thumbnails
		29 Jun 2000		drd		Set up dialog (assuming single selection)
		14 Jun 2000		drd		Created
*/

#include "ImageOptions.h"
#include "AlignmentGizmo.h"
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
	MRect				bounds;
	PhotoPrintDoc*		theDoc = dynamic_cast<PhotoPrintDoc*>(inSuper);
	PhotoPrintItem*		theItem = theDoc->GetModel()->GetSelection();

	// !!! Set up rotation thumbnails
	LBevelButton*		rotate0 = dynamic_cast<LBevelButton*>(this->FindPaneByID('000°'));
	if (rotate0 != nil) {
		// !!! there is a big fat leak here
		PhotoPrintItem*	item = new PhotoPrintItem(*theItem);
		AlignmentGizmo::FitAndAlignRectInside(item->GetDestRect(), thumbBounds,
			kAlignAbsoluteCenter, bounds);
		
		item->SetDest(bounds);
		item->MakeProxy(nil);
		PicHandle		pict = item->GetProxy();
		ControlButtonContentInfo	ci;
		ci.contentType = kControlContentPictHandle;
		ci.u.picture = pict;
		rotate0->SetContentInfo(ci);
		rotate0->SetValue(Button_On);
	}

	LBevelButton*		rotate90 = dynamic_cast<LBevelButton*>(this->FindPaneByID('090°'));
	if (rotate90 != nil) {
		// !!! there is a big fat leak here
		PhotoPrintItem*	item = new PhotoPrintItem(*theItem);
		AlignmentGizmo::FitAndAlignRectInside(item->GetDestRect(), thumbBounds,
			kAlignAbsoluteCenter, bounds);
		item->SetRotation(90);
		item->SetScreenDest(bounds);
		item->MakeProxy(nil);
		PicHandle		pict = item->GetProxy();
		ControlButtonContentInfo	ci;
		ci.contentType = kControlContentPictHandle;
		ci.u.picture = pict;
		rotate90->SetContentInfo(ci);
	}

	LBevelButton*		rotate180 = dynamic_cast<LBevelButton*>(this->FindPaneByID('180°'));
	if (rotate180 != nil) {
		// !!! there is a big fat leak here
		PhotoPrintItem*	item = new PhotoPrintItem(*theItem);
		AlignmentGizmo::FitAndAlignRectInside(item->GetDestRect(), thumbBounds,
			kAlignAbsoluteCenter, bounds);
		item->SetRotation(180);
		item->SetScreenDest(bounds);
		item->MakeProxy(nil);
		PicHandle		pict = item->GetProxy();
		ControlButtonContentInfo	ci;
		ci.contentType = kControlContentPictHandle;
		ci.u.picture = pict;
		rotate180->SetContentInfo(ci);
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
