/*
	File:		PhotoPrintApp.cp

	Contains:	Implementation of the application class.

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		14 jul 2000		dml		add gPrintSession
		11 Jul 2000		drd		Use PhotoPrintPrefs object
		10 Jul 2000		drd		Split registration into PhotoPrintApp_Register.cp; CheckPlatformSpec
								now a class function
		06 Jun 2000		drd		mPalette is now gPalette
		29 Jun 2000		drd		Override EventSuspend; donÍt call Initialize in HCEE
		28 Jun 2000		drd		Prefs command
		27 jun 2000		dml		setting MFileSpec.sDefaultCreator in main()
		26 Jun 2000		drd		Register LPlacard; initialize layout in HandleCreateElementEvent
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

#include "PhotoPrintApp.h"

#include "Layout.h"
#include "NewCommand.h"
#include "OpenCommand.h"
#include "PhotoPrintDoc.h"
#include "PhotoPrintPrefs.h"
#include "PhotoPrintView.h"
#include "PrefsCommand.h"

#include <LDebugMenuAttachment.h>
#include <LGrowZone.h>
#include <PP_Messages.h>
#include <PP_Resources.h>
#include <UDebugging.h>
#include <UDrawingState.h>
#include <UEnvironment.h>
#include <UMemoryMgr.h>

#include <Appearance.h>
#include <CFString.h>
#include "MAppleEvent.h"
#include "MFileSpec.h"
#include "MProcesses.h"
#include <Types.h>

	// Constant declarations
const ResIDT	PPob_Palette				= 1003;

const ResIDT	alrt_QuicktimeRequirements = 129;
const ResIDT 	alrt_NavServicesRequirements = 130;

// Globals
CFStringRef	PhotoPrintApp::gName = CFSTR("electricfish.photoprint");	// Leave out com. for Mac OS 9
LWindow*	PhotoPrintApp::gPalette = nil;
HORef<StPrintSession>	PhotoPrintApp::gPrintSession;
HORef<EPrintSpec>		PhotoPrintApp::gPrintSpec;

// ===========================================================================
//	´ main
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

	if (!PhotoPrintApp::CheckPlatformSpec())
		return 0;

	//	Get the process information
	MProcessInfo	process (kCurrentProcess);
	MFileSpec::sDefaultCreator = process.Signature ();

		// Create the application object and run
	PhotoPrintApp	theApp;
	theApp.Run();
	
	return 0;
} // main

// ---------------------------------------------------------------------------
//	´ PhotoPrintApp								[public]
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
//	´ ~PhotoPrintApp								[public, virtual]
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
	new PrefsCommand(cmd_Preferences, this);

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
PhotoPrintApp::AddEvents			(void)
{
}//end AddEvents

#pragma mark -

/*
CheckPlatformSpec
	Returns whether or not our minimum requirements are present, displaying an alert if not
*/
bool
PhotoPrintApp::CheckPlatformSpec()
{
	bool		bHappy (false); // pessimism
	
	do {
		if (!UEnvironment::HasFeature(env_HasQuickTime)) {
			::Alert(alrt_QuicktimeRequirements, 0);
			continue;
			}//endif QT not installed
	
		OSErr	err;
		long	response;
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
} // CheckPlatformSpec

// ---------------------------------------------------------------------------
//	´ EventSuspend
// ---------------------------------------------------------------------------
//	Respond to a Suspend event

void
PhotoPrintApp::EventSuspend(
	const EventRecord&	inMacEvent)
{
	LDocApplication::EventSuspend(inMacEvent);

	// Carbon hids them, we want the palette back
	::ShowFloatingWindows();
	::HiliteWindow(gPalette->GetMacWindow(), false);
}

// ---------------------------------------------------------------------------
//	´ FindCommandStatus								[public, virtual]
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
//	´ HandleCreateElementEvent										  [public]
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

			// If there are any documents specified, import them
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

	// Create the preferences object
	new PhotoPrintPrefs(this->Name());
	
	// create StPrintSession, PrintSpec object
	gPrintSpec = new EPrintSpec ();
	gPrintSession = new StPrintSession(*gPrintSpec);
	gPrintSpec->SetToSysDefault();
} // Initialize

// ---------------------------------------------------------------------------
//	´ ObeyCommand									[public, virtual]
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
// ´OpenDocument
// ---------------------------------------------------------------------------
void
PhotoPrintApp::OpenDocument(FSSpec*				inMacFSSpec) {
	PhotoPrintDoc* doc = new PhotoPrintDoc(this, *inMacFSSpec);
}//end OpenDocument 

// ---------------------------------------------------------------------------
//	´ StartUp										[protected, virtual]
// ---------------------------------------------------------------------------
//	Perform an action in response to the Open Application AppleEvent.
//	Here, issue the New command to open a window.

void
PhotoPrintApp::StartUp()
{
	gPalette = LWindow::CreateWindow(PPob_Palette, this);

	NewCommand	command('grid', this);
	command.Execute('grid', nil);
} // StartUp
