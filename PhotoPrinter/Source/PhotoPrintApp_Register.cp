/*
	File:		PhotoPrintApp_Register.cp

	Contains:	PowerPlant registration for the application class.

	Written by:	David Dunham

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		23 May 2001		drd		38 We don't use PaletteButton any more, so don't register it
		23 May 2001		drd		Removed ALL of LImageWell
		22 May 2001		drd		Removed LGAColorSwatchControl, LImageWell
		21 Mar 2001		drd		PopupBevelButton
		22 feb 2001		dml		add PhotoBadge 
		08 Feb 2001		drd		LGADialog is obsolete (in PP 2.1.1), so don't register it
		15 Sep 2000		drd		Register E2ClickAttachment
		21 aug 2000		dml		add imageWell
		10 Jul 2000		drd		Split from PhotoPrintApp.cp
*/

#include "PhotoPrintApp.h"

#include "FileEditText.h"
#include "FPEditText.h"
#include "PhotoPrintView.h"
#include "PhotoWindow.h"
#include "PhotoBadge.h"
#include "PopupBevelButton.h"

#include "CURLPushButton.h"

#include "E2ClickAttachment.h"

#include <URegistrar.h>

#include <LBevelButton.h>
#include <LCheckBox.h>
#include <LCmdBevelButton.h>
#include <LDebugMenuAttachment.h>
#include <LDialogBox.h>
#include <LEditText.h>
#include <LIconControl.h>
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
//	¥ RegisterClasses								[protected]
// ---------------------------------------------------------------------------
//	To reduce clutter within the Application object's constructor, class
//	registrations appear here in this seperate function for ease of use.

void
PhotoPrintApp::RegisterClasses()
{
	// Register core PowerPlant classes.
	RegisterClass_(LColorEraseAttachment);
	RegisterClass_(LDialogBox);
	RegisterClass_(LMultiPanelView);
	RegisterClass_(LPlaceHolder);
	RegisterClass_(LPrintout);
	RegisterClass_(LRadioGroupView);
	RegisterClass_(LScrollerView);
	RegisterClass_(LTabGroup);
	RegisterClass_(LView);
	RegisterClass_(LWindow);
	RegisterClass_(LWindowThemeAttachment);
	RegisterClass_(LBorderAttachment);

	// Register the Appearance Manager/GA classes we actually use, rather than just
	// registering all of them via UControlRegistryRegisterClasses().
	RegisterClass_(LBevelButton);
	RegisterClass_(LCheckBox);
	RegisterClass_(LCmdBevelButton);
	RegisterClass_(LEditText);
	RegisterClass_(LIconControl);
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
	RegisterClassID_(LAMControlImp, 		LIconControl::imp_class_ID);
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
	
	// E++
	RegisterClass_(E2ClickAttachment);
	
	// third party
	RegisterClass_(CURLPushButton);

	// Register app-specific classes
	RegisterClass_(FileEditText);
	RegisterClass_(FPEditText);
	RegisterClass_(PhotoPrintView);
	RegisterClass_(PhotoWindow);
	RegisterClass_(PhotoBadge);
	RegisterClass_(PopupBevelButton);
	
} // RegisterClasses
