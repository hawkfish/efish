// ===========================================================================
//	PhotoPrintApp.cp 		Copyright © 2000 Electric Fish, Inc. All rights reserved.

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

#include <LWindow.h>
#include <LScrollerView.h>
#include <Appearance.h>

#include "MFileSpec.h"
#include <UDebugging.h>
#include <Types.h>

	// Constant declarations
const ResIDT	PPob_SampleWindow			= 128;

#include "PhotoPrintDoc.h"
const ResIDT	alrt_QuicktimeRequirements = 129;

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
	ObeyCommand(cmd_New, nil);
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
}




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
	RegisterClass_(LWindow);

		// Register the Appearance Manager/GA classes. You may want
		// to remove this use of UControlRegistry and instead perform
		// a "manual" registration of the classes. This cuts down on
		// extra code being linked in and streamlines your app and
		// project. However, use UControlRegistry as a reference/index
		// for your work, and ensure to check UControlRegistry against
		// your registrations each PowerPlant release in case
		// any mappings might have changed.
		
	UControlRegistry::RegisterClasses();
	RegisterClass_(LScrollerView);
	RegisterClass_(PhotoPrintView);
	RegisterClass_(LPrintout);
	RegisterClass_(LPlaceHolder);
}


//-----------------------------------------------------------------
//AddEvents
//-----------------------------------------------------------------
void					
PhotoPrintApp::AddEvents			(void) {
}//end AddEvents

#pragma mark -
#include "OpenCommand.h"
//-----------------------------------------------------------------
//AddCommands
//-----------------------------------------------------------------
void					
PhotoPrintApp::AddCommands			(void)
{
	new OpenCommand(cmd_Open, this);
	}//end AddCommands

