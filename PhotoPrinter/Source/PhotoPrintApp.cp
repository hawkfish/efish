/*
	File:		PhotoPrintApp.cp

	Contains:	Implementation of the application class.

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		26 Jun 2000		drd		Register LPlacard
		23 Jun 2000		drd		Commands for New
		22 Jun 2000		drd		Temporarily disable debug menu for Carbon
		22 Jun 2000		drd		Carbon version of InitializeToolbox
		21 Jun 2000		drd		HandleCreateElementEvent sends ReceiveDragEvent
		21 Jun 2000		drd		Register PaletteButton; override HandleCreateElementEvent
		15 Jun 2000		drd		Use LDebugMenuAttachment, register LColorEraseAttachment
		14 Jun 2000		drd		Create palette
		14 Jun 2000		drd		Register more classes
		14 Jun 2000		drd		RegisterClasses only registers what we need

	To do:
		LDocApplication methods labelled "should override"
*/

#include "NewCommand.h"
#include "OpenCommand.h"
#include "PaletteButton.h"
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
#include <LDebugMenuAttachment.h>
#include <LEditText.h>
#include <LGAColorSwatchControl.h>
#include <LGADialog.h>
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
#include <LAMPlacardImp.h>
#include <LAMPopupButtonImp.h>
#include <LAMPushButtonImp.h>
#include <LAMStaticTextImp.h>
#include <LAMTabsControlImp.h>
#include <LAMTrackActionImp.h>

#include <Appearance.h>
#include "MAppleEvent.h"
#include "MFileSpec.h"
#include <UDebugging.h>
#include <Types.h>

	// Constant declarations
const ResIDT	PPob_SampleWindow			= 128;
const ResIDT	PPob_Palette				= 1003;

#include "PhotoPrintDoc.h"
const ResIDT	alrt_QuicktimeRequirements = 129;
const ResIDT 	alrt_NavServicesRequirements = 130;

Boolean	CheckPlatformSpec();

/*
CheckPlatformSpec
	Returns whether or not our minimum requirements are present, displaying an alert if not
*/
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
	UQDGlobals::InitializeToolbox();
	UQuickTime::Initialize();

	// Install a GrowZone to catch low-memory situations	
	new LGrowZone(20000);

	if (!CheckPlatformSpec())
		return 0;

		// Create the application object and run
	PhotoPrintApp	theApp;
	theApp.Run();
	
	return 0;
} // main

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

	this->RegisterClasses();
	this->AddEvents();
	this->AddCommands();
} // PhotoPrintApp

// ---------------------------------------------------------------------------
//	€ ~PhotoPrintApp								[public, virtual]
// ---------------------------------------------------------------------------
//	Application object destructor

PhotoPrintApp::~PhotoPrintApp()
{
	// Nothing
}

//-----------------------------------------------------------------
// AddCommands
//-----------------------------------------------------------------
//	Creates command attachments
void					
PhotoPrintApp::AddCommands			(void)
{
	new NewCommand('grid', this);
	new NewCommand('sing', this);
	new NewCommand('2fix', this);
	new NewCommand('2dup', this);
	new NewCommand('mult', this);
	new NewCommand('coll', this);
	new NewCommand('cust', this);

	new OpenCommand(cmd_Open, this);
} // AddCommands

//-----------------------------------------------------------------
// AddEvents
//-----------------------------------------------------------------
void					
PhotoPrintApp::AddEvents			(void) {
}//end AddEvents

// ---------------------------------------------------------------------------
//	€ RegisterClasses								[protected]
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

	// Register app-specific classes
	RegisterClass_(PaletteButton);
	RegisterClass_(PhotoPrintView);
} // RegisterClasses

#pragma mark -

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
//	€ HandleCreateElementEvent										  [public]
// ---------------------------------------------------------------------------
//	Respond to an AppleEvent to create a new item
LModelObject*
PhotoPrintApp::HandleCreateElementEvent(
	DescType			inElemClass,
	DescType			inInsertPosition,
	LModelObject*		inTargetObject,
	const AppleEvent&	inAppleEvent,
	AppleEvent&			outAEReply)
{
	switch (inElemClass) {
		case cDocument:
		case cWindow:
		{
			PhotoPrintDoc* doc = new PhotoPrintDoc(this);
			MAppleEvent	aevt(inAppleEvent);
			DescType	theType;
			Size		theSize;

			// See what type of document we should be creating
			OSType		tmplType;
			aevt.GetParamPtr(theType, theSize, &tmplType, sizeof(tmplType), typeType, keyAERequestedType);
			doc->GetView()->SetLayoutType(tmplType);

			// If there are any documents specifid, import them
			if (aevt.HasKey(keyDirectObject)) {
				doc->GetView()->ReceiveDragEvent(aevt);
			}
			return doc;
			break;
		}

		default:
			return LDocApplication::HandleCreateElementEvent(inElemClass,
						inInsertPosition, inTargetObject, inAppleEvent,
						outAEReply);
			break;
	}
}

/*
Initialize {OVERRIDE}
*/
void
PhotoPrintApp::Initialize()
{
#if PP_DEBUG && !TARGET_CARBON
	//	Debug menu
	LDebugMenuAttachment::InstallDebugMenu(this);
#endif
} // Initialize

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
// €OpenDocument
// ---------------------------------------------------------------------------
void
PhotoPrintApp::OpenDocument(FSSpec*				inMacFSSpec) {
	PhotoPrintDoc* doc = new PhotoPrintDoc(this, *inMacFSSpec);
}//end OpenDocument 

// ---------------------------------------------------------------------------
//	€ StartUp										[protected, virtual]
// ---------------------------------------------------------------------------
//	Perform an action in response to the Open Application AppleEvent.
//	Here, issue the New command to open a window.

void
PhotoPrintApp::StartUp()
{
	LWindow*	palette = LWindow::CreateWindow(PPob_Palette, this);

	NewCommand	command('grid', this);
	command.Execute('grid', nil);
} // StartUp
