/*
	File:		PhotoPrintApp.cp

	Contains:	Implementation of the application class.

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		14 Jun 2000		drd		Register more classes
		14 Jun 2000		drd		RegisterClasses only registers what we need
*/

#include "PhotoPrintApp.h"
#include "PhotoPrintView.h"

#include <LGrowZone.h>
#include <PP_Messages.h>
#include <PP_Resources.h>
#include <UDrawingState.h>
#include <UMemoryMgr.h>
#include <URegistrar.h>
#include <UEnvironment.h>

#include <UControlRegistry.h>

#include <LBevelButton.h>
#include <LCheckBox.h>
#include <LCmdBevelButton.h>
#include <LEditText.h>
#include <LGAColorSwatchControl.h>
#include <LGADialog.h>
#include <LMultiPanelView.h>
#include <LPictureControl.h>
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

// Appearance Manager Implementation (for registration)
#include <LAMBevelButtonImp.h>
#include <LAMControlImp.h>
#include <LAMControlViewImp.h>
#include <LAMEditTextImp.h>
#include <LAMPopupButtonImp.h>
#include <LAMPushButtonImp.h>
#include <LAMStaticTextImp.h>
#include <LAMTabsControlImp.h>
#include <LAMTrackActionImp.h>

#include <Appearance.h>
#include "MFileSpec.h"
#include <UDebugging.h>
#include <Types.h>

	// Constant declarations
const ResIDT	PPob_SampleWindow			= 128;

#include "PhotoPrintDoc.h"
const ResIDT	alrt_QuicktimeRequirements = 129;
const ResIDT 	alrt_NavServicesRequirements = 130;

Boolean	CheckPlatformSpec();

Boolean CheckPlatformSpec() {
	Boolean bHappy (false); // pessimism
	
	do {
		if (!UEnvironment::HasFeature(env_HasQuickTime)) {
			::Alert(alrt_QuicktimeRequirements, 0);
			continue;
			}//endif QT not installed
	
		OSErr err;
		long response;
		err = ::Gestalt(gestaltQuickTimeVersion, &response);
		if ((err != noErr) ||
		(response < 0x04000000)) {
			::Alert(alrt_QuicktimeRequirements, 0);
			continue;
			}//end
			
		if (!NavServicesAvailable()) {
			::Alert(alrt_NavServicesRequirements, 0);
			continue;
			}//endif

		bHappy = true;
		} while (false);
		
	return bHappy;
	}//end

// ===========================================================================
//	€ main
// ===========================================================================

int main()
{							
		// Set Debugging options
	SetDebugThrow_(debugAction_Alert);
	SetDebugSignal_(debugAction_Alert);

		// Initialize Memory Manager. Parameter is the number of
		// master pointer blocks to allocate
	InitializeHeap(3);
	
		// Initialize standard Toolbox managers
	UQDGlobals::InitializeToolbox(&qd);
	UQuickTime::Initialize();
		// Install a GrowZone to catch low-memory situations	
	new LGrowZone(20000);


	if (!CheckPlatformSpec())
		return 0;

		// Create the application object and run
	PhotoPrintApp	theApp;
	theApp.Run();
	
	return 0;
}


// ---------------------------------------------------------------------------
//	€ PhotoPrintApp								[public]
// ---------------------------------------------------------------------------
//	Application object constructor

PhotoPrintApp::PhotoPrintApp()
{
		// Register ourselves with the Appearance Manager
	if (UEnvironment::HasFeature(env_HasAppearance)) {
		::RegisterAppearanceClient();
	}

	RegisterClasses();
	AddEvents();
	AddCommands();
}


// ---------------------------------------------------------------------------
//	€ ~PhotoPrintApp								[public, virtual]
// ---------------------------------------------------------------------------
//	Application object destructor

PhotoPrintApp::~PhotoPrintApp()
{
	// Nothing
}


// ---------------------------------------------------------------------------
//	€ StartUp										[protected, virtual]
// ---------------------------------------------------------------------------
//	Perform an action in response to the Open Application AppleEvent.
//	Here, issue the New command to open a window.

void
PhotoPrintApp::StartUp()
{
	this->ObeyCommand(cmd_New, nil);
}


// ---------------------------------------------------------------------------
//	€ ObeyCommand									[public, virtual]
// ---------------------------------------------------------------------------
//	Respond to Commands. Returns true if the Command was handled, false if not.

Boolean
PhotoPrintApp::ObeyCommand(
	CommandT	inCommand,
	void*		ioParam)
{
	Boolean		cmdHandled = true;	// Assume we'll handle the command

	switch (inCommand) {

		case cmd_New: {
			PhotoPrintDoc* doc = new PhotoPrintDoc(this);
		}

		default: {
			cmdHandled = LApplication::ObeyCommand(inCommand, ioParam);
			break;
		}
	}
	
	return cmdHandled;
}


// ---------------------------------------------------------------------------
//	€ FindCommandStatus								[public, virtual]
// ---------------------------------------------------------------------------
//	Determine the status of a Command for the purposes of menu updating.

void
PhotoPrintApp::FindCommandStatus(
	CommandT	inCommand,
	Boolean&	outEnabled,
	Boolean&	outUsesMark,
	UInt16&		outMark,
	Str255		outName)
{
	switch (inCommand) {

		case cmd_New: {
			outEnabled = true;
			break;
		}

		default: {
			LApplication::FindCommandStatus(inCommand, outEnabled,
											outUsesMark, outMark, outName);
			break;
		}
	}
} // FindCommandStatus


// ---------------------------------------------------------------------------
// €DoOpen
// ---------------------------------------------------------------------------
void
PhotoPrintApp::OpenDocument(FSSpec*				inMacFSSpec) {
	PhotoPrintDoc* doc = new PhotoPrintDoc(this, *inMacFSSpec);
	}//end OpenDocument 



// ---------------------------------------------------------------------------
//	€ RegisterClasses								[protected]
// ---------------------------------------------------------------------------
//	To reduce clutter within the Application object's constructor, class
//	registrations appear here in this seperate function for ease of use.

void
PhotoPrintApp::RegisterClasses()
{
	// Register core PowerPlant classes.
	RegisterClass_(LMultiPanelView);
	RegisterClass_(LPlaceHolder);
	RegisterClass_(LPrintout);
	RegisterClass_(LRadioGroupView);
	RegisterClass_(LScrollerView);
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
	RegisterClass_(LPictureControl);
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

	// Register app-specific classes
	RegisterClass_(PhotoPrintView);
}


//-----------------------------------------------------------------
// AddEvents
//-----------------------------------------------------------------
void					
PhotoPrintApp::AddEvents			(void) {
}//end AddEvents

#pragma mark -
#include "OpenCommand.h"
//-----------------------------------------------------------------
// AddCommands
//-----------------------------------------------------------------
//	Creates command attachments
void					
PhotoPrintApp::AddCommands			(void)
{
	new OpenCommand(cmd_Open, this);
}//end AddCommands

