/*
	File:		PhotoPrintApp.cp

	Contains:	Implementation of the application class.

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		31 Aug 2000		drd		OpenDocument handles arbitrary files
		25 aug 2000		dml		add UseProxyCommand
		23 Aug 2000		drd		We now pass files from palette as keyAEData
		17 aug 2000		dml		ObeyCommand maps cmd_new to NewCommand("grid") (default layout)
		11 aug 2000		dml		hook up tools messages to SetDocumentControllers, add parm to that func
		11 Aug 2000		drd		Added tool_Rotate
		11 aug 2000		dml		add SetDocumentControllers;
		03 aug 2000		dml		RefreshDocuments takes "forceSort" parm
		02 aug 2000		dml		added gSingleton, RefreshOpenDocuments()
		01 aug 2000		dml		changed printer check again (using classic since more correct until sessions)
		28 Jul 2000		drd		Changed printer check (PMGetDriverCreator may return kPMNotImplemented)
		28 jul 2000		dml		add check for Printer (die if no current printer)
		24 Jul 2000		drd		AllowSubRemoval keeps windoid globals in sync; cmd_LayoutPalette,
								cmd_ToolsPalette
		20 Jul 2000		drd		Added gCurTool
		19 Jul 2000		drd		Align floating windows; manage tools windoid
		18 Jul 2000		drd		Restore debug menu
		18 Jul 2000		drd		Added gTools; MakeMenuBar checks for Aqua
		18 jul 2000		dml		changed gPrintSession to gCurPrintSession
		14 jul 2000		dml		add gPrintSession
		11 Jul 2000		drd		Use PhotoPrintPrefs object
		10 Jul 2000		drd		Split registration into PhotoPrintApp_Register.cp; CheckPlatformSpec
								now a class function
		06 Jun 2000		drd		mPalette is now gPalette
		29 Jun 2000		drd		Override EventSuspend; don't call Initialize in HCEE
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

#include "EUtil.h"
#include "ECurrentPrinter.h"
#include "Layout.h"
#include "NewCommand.h"
#include "OpenCommand.h"
#include "PhotoPrintCommands.h"
#include "PhotoPrintDoc.h"
#include "PhotoPrintPrefs.h"
#include "PhotoPrintView.h"
#include "PrefsCommand.h"
#include "AboutBox.h"
#include "UseProxyCommand.h"

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
const ResIDT	PPob_Tools					= 1005;

const ResIDT	alrt_QuicktimeRequirements = 129;
const ResIDT 	alrt_NavServicesRequirements = 130;
const ResIDT	alrt_NoPrinterSelected = 133;

// Globals
StPrintSession*	PhotoPrintApp::gCurPrintSession = nil;
OSType			PhotoPrintApp::gCurTool = tool_Arrow;
CFStringRef		PhotoPrintApp::gName = CFSTR("electricfish.photoprint");	// Leave out com. for Mac OS 9
LWindow*		PhotoPrintApp::gPalette = nil;
PhotoPrintDoc*	PhotoPrintApp::gPrintSessionOwner = nil;
LWindow*		PhotoPrintApp::gTools = nil;
PhotoPrintApp*	PhotoPrintApp::gSingleton = nil;
MCurResFile	PhotoPrintApp::gAppResFile;

// ===========================================================================
//	¥ main
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
//	¥ PhotoPrintApp								[public]
// ---------------------------------------------------------------------------
//	Application object constructor

PhotoPrintApp::PhotoPrintApp()
{
	Assert_(gSingleton == nil);
	gSingleton = this;

	// Register ourselves with the Appearance Manager
	if (UEnvironment::HasFeature(env_HasAppearance)) {
		::RegisterAppearanceClient();
	}

	this->RegisterClasses();
	this->AddEvents();
	this->AddCommands();
} // PhotoPrintApp

// ---------------------------------------------------------------------------
//	¥ ~PhotoPrintApp								[public, virtual]
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
	
	new AboutCommand(cmd_About, this);
	
	new UseProxyCommand(cmd_UseProxies, this);
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
AllowSubRemoval {OVERRIDE}
	Override to keep track of our floating windows visibility
*/
Boolean
PhotoPrintApp::AllowSubRemoval(
	LCommander* inSub)
{
	if (inSub == gPalette) {
		gPalette = nil;
	} else if (inSub == gTools) {
		gTools = nil;
	}

	return true;
} // AllowSubRemoval

/*
CheckPlatformSpec
	Returns whether or not our minimum requirements are present, displaying an alert if not
*/
bool
PhotoPrintApp::CheckPlatformSpec()
{
	bool		bHappy (false); // pessimism
	
	do {
		if (!ValidPrinter()) {
			::StopAlert(alrt_NoPrinterSelected, 0);
			continue;
			}//die if no printer selected


		// We require QuickTime 4.0 or later
		if (!UEnvironment::HasFeature(env_HasQuickTime)) {
			::StopAlert(alrt_QuicktimeRequirements, 0);
			continue;
		}//endif QT not installed

		OSErr	err;
		long	response;
		err = ::Gestalt(gestaltQuickTimeVersion, &response);
		if ((err != noErr) || (response < 0x04000000)) {
			::StopAlert(alrt_QuicktimeRequirements, 0);
			continue;
		}//endif
			
		if (!::NavServicesAvailable()) {
			::StopAlert(alrt_NavServicesRequirements, 0);
			continue;
		}//endif

		// ??? do we need to check for Appearance?

		bHappy = true;
	} while (false);
		
	return bHappy;
} // CheckPlatformSpec

// ---------------------------------------------------------------------------
//	¥ EventResume
// ---------------------------------------------------------------------------
//	Respond to a Resume event

void
PhotoPrintApp::EventResume(
	const EventRecord&	inMacEvent)
{
	LDocApplication::EventResume(inMacEvent);

	if (gTools != nil)
		gTools->Show();							// Since we hid it on Suspend
} // EventResume

// ---------------------------------------------------------------------------
//	¥ EventSuspend
// ---------------------------------------------------------------------------
//	Respond to a Suspend event

void
PhotoPrintApp::EventSuspend(
	const EventRecord&	inMacEvent)
{
	LDocApplication::EventSuspend(inMacEvent);

	// Carbon hides them, we want the palette back (but not the tools)
	::ShowFloatingWindows();
	if (gTools != nil)
		gTools->Hide();
	if (gPalette != nil)
		::HiliteWindow(gPalette->GetMacWindow(), false);
} // EventSuspend

// ---------------------------------------------------------------------------
//	¥ FindCommandStatus								[public, virtual]
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
		case cmd_New:
			outEnabled = true;
			break;

		case tool_Arrow:
		case tool_Crop:
		case tool_Rotate:
		case tool_Zoom:
			outEnabled = true;
			break;

		case cmd_LayoutPalette:
			outUsesMark = true;
			outMark = (gPalette != nil) ? checkMark : noMark;
			outEnabled = true;
			break;

		case cmd_ToolsPalette:
			outUsesMark = true;
			outMark = (gTools != nil) ? checkMark : noMark;
			outEnabled = true;
			break;

		default:
			LApplication::FindCommandStatus(inCommand, outEnabled,
											outUsesMark, outMark, outName);
			break;
	}
} // FindCommandStatus

// ---------------------------------------------------------------------------
//	¥ HandleCreateElementEvent										  [public]
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
			if (aevt.HasKey(keyAEData)) {
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
} // HandleCreateElementEvent

/*
Initialize {OVERRIDE}
*/
void
PhotoPrintApp::Initialize()
{
#if PP_DEBUG	// && !TARGET_CARBON
	//	Debug menu
	LDebugMenuAttachment::InstallDebugMenu(this);
#endif

	// Create the preferences object
	new PhotoPrintPrefs(this->Name());
} // Initialize



/*
MakeMenuBar {OVERRIDE}								[protected]
	Create MenuBar object for this Application
	We override to set up menus differently for Aqua
*/
void
PhotoPrintApp::MakeMenuBar()
{
	OSErr	err;
	long	response;
	err = ::Gestalt(gestaltMenuMgrAttr, &response);
	if ((err == noErr) && (response & gestaltMenuMgrAquaLayoutMask))
		new LMenuBar(mbar_Carbon);
	else
		new LMenuBar(MBAR_Initial);
} // MakeMenuBar

// ---------------------------------------------------------------------------
//	¥ ObeyCommand									[public, virtual]
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
			NewCommand	command('grid', this);
			command.Execute('grid', nil);			
		}

		case cmd_LayoutPalette:
			if (gPalette != nil) {
				delete gPalette;
				gPalette = nil;
			} else {
				gPalette = LWindow::CreateWindow(PPob_Palette, this);
				EUtil::AlignToScreen(gPalette, kAlignTopRight);
			}
			SetUpdateCommandStatus(true);
			break;

		case cmd_ToolsPalette:
			if (gTools != nil) {
				delete gTools;
				gTools = nil;
			} else {
				gTools = LWindow::CreateWindow(PPob_Tools, this);
				EUtil::AlignToScreen(gTools, kAlignBottomLeft);
			}
			SetUpdateCommandStatus(true);
			break;

		case tool_Arrow:
		case tool_Crop:
		case tool_Rotate:
		case tool_Zoom:
			if (gCurTool != inCommand) {
				gCurTool = inCommand;
				SetDocumentControllers(gCurTool);
				}//endif something new chosen
			break;

		default: {
			cmdHandled = LApplication::ObeyCommand(inCommand, ioParam);
			break;
		}
	}
	
	return cmdHandled;
} // ObeyCommand

/*
OpenDocument {OVERRIDE}
	Open the specified file as a new document. We allow dragging of arbitrary files and folders
	onto the application icon; this opens a new grid document and imports the file.
*/
void
PhotoPrintApp::OpenDocument(FSSpec* inMacFSSpec)
{
	FInfo		info;
	::FSpGetFInfo(inMacFSSpec, &info);

	if (info.fdType == 'TEXT') {
		// Assume any text file is one we can open
		PhotoPrintDoc* doc = new PhotoPrintDoc(this, *inMacFSSpec);
	} else {
		// Assume we can import it (i.e. that QuickTime will handle it)
		// Create a "new document" event
		MAEAddressDesc		realAddress (MFileSpec::sDefaultCreator);
		MAppleEvent 		aevt(kAECoreSuite, kAECreateElement, realAddress);
		DescType			docType = cDocument;
		aevt.PutParamPtr(typeType, &docType, sizeof(DescType), keyAEObjectClass);

		// What kind of template
		docType = 'grid';
		aevt.PutParamPtr(typeType, &docType, sizeof(DescType), keyAERequestedType);

		// The file dropped on us
		MAEList				theList;
		theList.PutPtr(typeFSS, inMacFSSpec, sizeof(FSSpec));
		aevt.PutParamDesc(theList, keyAEData);

		// And send it! This will result in a window being opened by HandleCreateElementEvent;
		// PhotoPrintView::ReceiveDragEvent will then handle importing.
		aevt.Send();
	}
} // OpenDocument 

//----------------------------------------------------
// RefreshDocuments
//----------------------------------------------------
void
PhotoPrintApp::RefreshDocuments(bool forceSort, bool forceLayout) {
	TArray<LDocument*>& docList (LDocument::GetDocumentList());
	SInt32 count = (SInt32) docList.GetCount();
	
	for (ArrayIndexT i = 1; i <= count; ++i) {
		LDocument* pDoc = docList[i];
		PhotoPrintDoc* photoDoc = dynamic_cast<PhotoPrintDoc*>(pDoc);
		if (photoDoc != nil) {
			if (forceSort)
				photoDoc->GetModel()->Sort();
			if (forceLayout)
				photoDoc->GetView()->GetLayout()->LayoutImages();
			photoDoc->GetView()->Refresh();
			}//endif
		}//end

	}//end RefreshDocuments



void
PhotoPrintApp::SetDocumentControllers(OSType inTool) {
	TArray<LDocument*>& docList (LDocument::GetDocumentList());
	SInt32 count = (SInt32) docList.GetCount();
	
	for (ArrayIndexT i = 1; i <= count; ++i) {
		LDocument* pDoc = docList[i];
		PhotoPrintDoc* photoDoc = dynamic_cast<PhotoPrintDoc*>(pDoc);
		if (photoDoc != nil) {
			photoDoc->SetController(inTool);
			}//endif document
		}//for all documents
	}//end SetDocumentControllers



// ---------------------------------------------------------------------------
//	¥ StartUp										[protected, virtual]
// ---------------------------------------------------------------------------
//	Perform an action in response to the Open Application AppleEvent.
//	Here, issue the New command to open a window.

void
PhotoPrintApp::StartUp()
{
	gPalette = LWindow::CreateWindow(PPob_Palette, this);
	EUtil::AlignToScreen(gPalette, kAlignTopRight);

	gTools = LWindow::CreateWindow(PPob_Tools, this);
	EUtil::AlignToScreen(gTools, kAlignBottomLeft);

	NewCommand	command('grid', this);
	command.Execute('grid', nil);
} // StartUp
