/*
	File:		PhotoPrintApp_Register.cp

	Contains:	PowerPlant registration for the application class.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		21 aug 2000		dml		add imageWell
		10 Jul 2000		drd		Split from PhotoPrintApp.cp
*/

#include "PhotoPrintApp.h"

#include "PaletteButton.h"
#include "PhotoPrintView.h"
#include "PhotoWindow.h"

#include <UControlRegistry.h>
#include <URegistrar.h>

#include <LBevelButton.h>
#include <LCheckBox.h>
#include <LCmdBevelButton.h>
#include <LDebugMenuAttachment.h>
#include <LEditText.h>
#include <LGAColorSwatchControl.h>
#include <LGADialog.h>
#include <LImageWell.h>
#include <LMultiPanelView.h>
#include <LPictureControl.h>
#include <LPlacard.h>
#include <LPopupButton.h>
#include <LProgressBar.h>
#include <LPushButton.h>
#include <LRadioButton.h>
#include <LScrollBar.h>
#include <LScrollerView.h>
#include <LSeparatorLine.h>
#include <LSlider.h>
#include <LStaticText.h>
#include <LTabsControl.h>
#include <LTextGroupBox.h>
#include <LWindow.h>
#include <UAttachments.h>

// Appearance Manager Implementation (for registration)
#include <LAMBevelButtonImp.h>
#include <LAMControlImp.h>
#include <LAMControlViewImp.h>
#include <LAMEditTextImp.h>
#include <LAMImageWellImp.h>
#include <LAMPlacardImp.h>
#include <LAMPopupButtonImp.h>
#include <LAMPushButtonImp.h>
#include <LAMStaticTextImp.h>
#include <LAMTabsControlImp.h>
#include <LAMTrackActionImp.h>

// ---------------------------------------------------------------------------
//	´ RegisterClasses								[protected]
// ---------------------------------------------------------------------------
//	To reduce clutter within the Application object's constructor, class
//	registrations appear here in this seperate function for ease of use.

void
PhotoPrintApp::RegisterClasses()
{
	// Register core PowerPlant classes.
	RegisterClass_(LColorEraseAttachment);
	RegisterClass_(LMultiPanelView);
	RegisterClass_(LPlaceHolder);
	RegisterClass_(LPrintout);
	RegisterClass_(LRadioGroupView);
	RegisterClass_(LScrollerView);
	RegisterClass_(LTabGroup);
	RegisterClass_(LView);
	RegisterClass_(LWindow);
	RegisterClass_(LWindowThemeAttachment);

	// Register the Appearance Manager/GA classes we actually use, rather than just
	// registering all of them via UControlRegistryRegisterClasses().
	RegisterClass_(LBevelButton);
	RegisterClass_(LCheckBox);
	RegisterClass_(LCmdBevelButton);
	RegisterClass_(LEditText);
	RegisterClass_(LGAColorSwatchControl);
	RegisterClass_(LGADialog);
	RegisterClass_(LImageWell);
	RegisterClass_(LPictureControl);
	RegisterClass_(LPlacard);
	RegisterClass_(LPopupButton);
	RegisterClass_(LProgressBar);
	RegisterClass_(LPushButton);
	RegisterClass_(LRadioButton);
	RegisterClass_(LScrollBar);
	RegisterClass_(LSeparatorLine);
	
	RegisterClass_(LSlider);
	RegisterClass_(LStaticText);
	RegisterClass_(LTabsControl);
	RegisterClass_(LTextGroupBox);

	RegisterClassID_(LAMBevelButtonImp,		LBevelButton::imp_class_ID);
	RegisterClassID_(LAMControlImp,			LCheckBox::imp_class_ID);
	RegisterClassID_(LAMEditTextImp,		LEditText::imp_class_ID);
	RegisterClassID_(LAMControlImp,			LPictureControl::imp_class_ID);
	RegisterClassID_(LAMPlacardImp,		 	LPlacard::imp_class_ID);
	RegisterClassID_(LAMPopupButtonImp,	 	LPopupButton::imp_class_ID);
	RegisterClassID_(LAMTrackActionImp,		LProgressBar::imp_class_ID);
	RegisterClassID_(LAMPushButtonImp,		LPushButton::imp_class_ID);
	RegisterClassID_(LAMControlImp,			LRadioButton::imp_class_ID);
	RegisterClassID_(LAMControlImp,			LSeparatorLine::imp_class_ID);
	RegisterClassID_(LAMTrackActionImp,		LScrollBar::imp_class_ID);
	RegisterClassID_(LAMTrackActionImp,		LSlider::imp_class_ID);
	RegisterClassID_(LAMStaticTextImp,		LStaticText::imp_class_ID);
	RegisterClassID_(LAMTabsControlImp,		LTabsControl::imp_class_ID);
	RegisterClassID_(LAMControlViewImp,		LTextGroupBox::imp_class_ID);
	RegisterClassID_(LAMImageWellImp,		LImageWell::imp_class_ID);
	

	// Register app-specific classes
	RegisterClass_(PaletteButton);
	RegisterClass_(PhotoPrintView);
	RegisterClass_(PhotoWindow);
} // RegisterClasses
