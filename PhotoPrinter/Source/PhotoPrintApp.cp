/*
	File:		PhotoPrintApp.cp

	Contains:	Implementation of the application class.

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		21 May 2001		drd		#include PhotoPrintResources.h
		03 May 2001		drd		Make a Window menu
		30 Apr 2001		drd		If not registered, throw 'quit' instead of a naked throw
		26 Apr 2001		drd		Use CreateStandardFontMenu to populate font menu
		23 Apr 2001		drd		RefreshDocuments calls UpdatePreferences
		23 Apr 2001		drd		Tweaked CheckPlatformSpec braces so CodeWarrior function popup works
		28 Mar 2001		drd		#ifdef out palette stuff
		22 Mar 2001		drd		Don't make layout palette, either
		14 Mar 2001		drd		Don't do tool palette
		13 mar 2001		dml		add font menu
		13 Mar 2001		drd		Fixed test for gOSX
		09 mar 2001		dml		add gOSX
		02 mar 2001		dml		add cmd_DrawMaxBounds command
		26 feb 2001		dml		change platform spec to carbon >= 1.2
		23 feb 2001		dml		add tool_Name to controller handling
		21 Feb 2001		drd		PP 2.1.1: no need for LDropArea::RemoveHandlers
		02 feb 2001		dml		add gCarbonVersion
		25 jan 2001		dml		split check for carbon into non-session (>1.0.4) and session (> 1.1.0)
		24 jan 2001		dml		add check for Carbon > 1.0.4
		19 Jan 2001		drd		Clear gPalette's kWindowHideOnSuspendAttribute when it's re-created;
								we don't need EventResume
		28 dec 2000		dml		remove annoyingware tinker for holiday printing, mwsf demo
		07 dec 2000		dml		annoyingware tinkering for debugging
		05 dec 2000		dml		changes to annoy text; time-limited version treated as non-registered for annoy ware at moment
		01 dec 2000		dml		throw from Initialize if Registration fails
		28 Nov 2000		drd		Clear gPalette's kWindowHideOnSuspendAttribute
		09 Nov 2000		drd		Override UpdateMenus
		08 Nov 2000		drd		HandleAppleEvent for prefs
		21 sep 2000		dml		using PhotoUtility::kHardwiredHeaderSize for annoyingware
		21 sep 2000		dml		install memory exception handler (now using chain of handlers)
		20 sep 2000		dml 	install default exception handler
		20 Sep 2000		drd		Stack-based grow zone, other cleanup of PowerPlant objects (for Spotlight)
		15 Sep 2000		drd		Manually add stuff to top of debug menu; move creation of palettes
								to Initialize
		13 sep 2000		dml		add gIsRegistered, annoyingware support
		12 sep 2000		dml		add gFlatPageFormat
		11 sep 2000		dml		make new documents FitToPage!
		31 Aug 2000		drd		OpenDocument checks for already-open document; override OpenOrPrintDocList
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

#include "AboutBox.h"
#include "EUtil.h"
#include "ECurrentPrinter.h"
#include "Layout.h"
#include "NewCommand.h"
#include "OpenCommand.h"
#include "PhotoExceptionHandler.h"
#include "PhotoPrintCommands.h"
#include "PhotoPrintDoc.h"
#include "PhotoPrintEvents.h"
#include "PhotoPrintPrefs.h"
#include "PhotoPrintView.h"
#include "PrefsCommand.h"
#include "Registration.h"
#include "UseProxyCommand.h"
#include "DrawMaxBounds.h"
#include "PhotoPrintConstants.h"
#include "PhotoPrintResources.h"
#include <LDebugMenuAttachment.h>
#include <LGrowZone.h>
#ifdef PP_DEBUG
#include <PP_DebugConstants.h>
#endif
#include <PP_Messages.h>
#include <PP_Resources.h>
#include <UDebugging.h>
#include <UDrawingState.h>
#include <UEnvironment.h>
#include <UMemoryMgr.h>

#include <Appearance.h>
#include <CFString.h>
#include "MAEDescExtractors.h"
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
const ResIDT	alrt_NonSessionCarbonRequirements = 134;
const ResIDT	alrt_SessionCarbonRequirements = 135;

// Globals
MPString		PhotoPrintApp::gAnnoyanceText = "\pUnregistered Copy - Please Register Your Copy Today";
MCurResFile	PhotoPrintApp::gAppResFile;
bool			PhotoPrintApp::gAqua = false;				// Do we have Aqua layout?
StPrintSession*	PhotoPrintApp::gCurPrintSession = nil;
OSType			PhotoPrintApp::gCurTool = tool_Arrow;
HORef<MNewHandle>	PhotoPrintApp::gFlatPageFormat = nil;
bool			PhotoPrintApp::gIsRegistered = false;
CFStringRef		PhotoPrintApp::gName = CFSTR("electricfish.photoprint");	// Leave out com. for Mac OS 9
LWindow*		PhotoPrintApp::gPalette = nil;
PhotoPrintDoc*	PhotoPrintApp::gPrintSessionOwner = nil;
LWindow*		PhotoPrintApp::gTools = nil;
PhotoPrintApp*	PhotoPrintApp::gSingleton = nil;
long			PhotoPrintApp::gCarbonVersion = 0;
bool			PhotoPrintApp::gOSX = false;

// ===========================================================================
//	¥ main
// ===========================================================================

int main()
{							
	// Set Debugging options
	SetDebugThrow_(debugAction_Alert);
	SetDebugSignal_(debugAction_Alert);

	DefaultExceptionHandler	defaultExceptionHandler;
	MemoryExceptionHandler	memoryExceptionHandler;
	
	// Initialize Memory Manager. Parameter is the number of
	// master pointer blocks to allocate
	InitializeHeap(3);
	
	// Initialize standard Toolbox managers
	UQDGlobals::InitializeToolbox();
	UQuickTime::Initialize();

	// Install a GrowZone to catch low-memory situations	
	LGrowZone	theZone(20000);

	if (!PhotoPrintApp::CheckPlatformSpec())
		return 0;

	//	Get the process information
	MProcessInfo	process (kCurrentProcess);
	MFileSpec::sDefaultCreator = process.Signature ();
	
	// Create the application object and run it
	{
		PhotoPrintApp	theApp;
		theApp.Run();
	}

	// Cleanup PowerPlant (so Spotlight doesn't complain about these being leaks)
	delete LMenuBar::GetCurrentMenuBar();
#if ( __PowerPlant__ < 0x02114003 )				// Version 2.1.1a3 no longer has this function
	LDropArea::RemoveHandlers();
#endif
	LPeriodical::DeleteIdlerAndRepeaterQueues();
	LModelObject::DestroyLazyList();
	URegistrar::DisposeClassTable();
	delete LComparator::GetComparator();		// It's there, no need to worry about instantiating it
	delete LLongComparator::GetComparator();	// It exists, no need to worry about instantiating it

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
	// Cleanup
	delete gCurPrintSession;
	delete PhotoPrintPrefs::Singleton();
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
	new DrawMaxBoundsCommand(cmd_DrawMaxBounds, this);
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
	long		response;
	OSErr		err;

	// Are we running under Mac OS X?
	err = ::Gestalt(gestaltSystemVersion, &response);
	if (err == noErr && (response >= 0x00001000)) {
		gOSX = true;
		}//endif

	do {
		// Check for CarbonLib >= 1.0.4
		err = ::Gestalt(gestaltCarbonVersion, &response);
#if PM_USE_SESSION_APIS
		if ((err != noErr) || (response < 0x00000120)) {
			::StopAlert(alrt_SessionCarbonRequirements, nil);
			continue;
		}//endif
#else
		if ((err != noErr) || (response < 0x00000104)) {
			::StopAlert(alrt_NonSessionCarbonRequirements, nil);
			continue;
		}//endif
#endif
		gCarbonVersion = response;

#if PM_USE_SESSION_APIS
		EPrintSpec	tempSpec;
		StPrintSession briefSession (tempSpec);
		PMPrinter	thePrinter;
		OSStatus status = ::PMSessionGetCurrentPrinter(tempSpec.GetPrintSession(), &thePrinter);
		if (status == kPMNoDefaultPrinter)
#else
		if (!ValidPrinter())
#endif
		{	// Normally I hate braces on a line by themselves; this makes the function popup work
			::StopAlert(alrt_NoPrinterSelected, nil);
			continue;
		}//die if no printer selected


		// We require QuickTime 4.0 or later
		if (!UEnvironment::HasFeature(env_HasQuickTime)) {
			::StopAlert(alrt_QuicktimeRequirements, 0);
			continue;
		}//endif QT not installed

		err = ::Gestalt(gestaltQuickTimeVersion, &response);
		if ((err != noErr) || (response < 0x04000000)) {
			::StopAlert(alrt_QuicktimeRequirements, 0);
			continue;
		}//endif
			
		if (!::NavServicesAvailable()) {
			::StopAlert(alrt_NavServicesRequirements, 0);
			continue;
		}//endif

		bHappy = true;
	} while (false);
		
	return bHappy;
} // CheckPlatformSpec

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
/*
	if (gTools != nil)
		gTools->Hide();
*/
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
		case tool_Name:
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

/*
HandleAppleEvent {OVERRIDE}
*/
void
PhotoPrintApp::HandleAppleEvent(
	const AppleEvent	&inAppleEvent,
	AppleEvent			&outAEReply,
	AEDesc				&outResult,
	long				inAENumber)
{
	if (inAENumber == ae_Preferences) {
		this->ProcessCommand(cmd_Preferences, nil);
	} else {
		LDocApplication::HandleAppleEvent(inAppleEvent, outAEReply, outResult, inAENumber);
	}
}

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
			
			// annoyingware:  if not registered, place notice in header/footer
			if (!PhotoPrintApp::gIsRegistered){
				doc->GetPrintProperties().SetFooter(PhotoUtility::kHardwiredHeaderSize/*inches*/);
				}//endif not registered



			// If there are any documents specified, import them
			if (aevt.HasKey(keyAEData)) {
				doc->GetView()->ReceiveDragEvent(aevt);
			} 
	
			doc->ProcessCommand(cmd_FitInWindow, nil);

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

	// Add our own stuff to the top of the Debug menu
	LMenu*		debugMenu = LMenuBar::GetCurrentMenuBar()->FetchMenu(MENU_DebugMenu);
	debugMenu->InsertCommand("\p-", cmd_Nothing, -1);
	debugMenu->InsertCommand("\pUse Proxies", cmd_UseProxies, -1);
	debugMenu->InsertCommand("\pForce Layout/L", cmd_ReLayout, -1);
	debugMenu->InsertCommand("\pForce Redraw", cmd_Redraw, -1);
	debugMenu->InsertCommand("\pDraw maxBounds", cmd_DrawMaxBounds, -1);
#endif

	// Create the preferences object
	new PhotoPrintPrefs(this->Name());

	gIsRegistered = Registration::RunDialog(this, 60 * 10, everyEvent & ~(highLevelEventMask));
	if (!gIsRegistered) {
		StDisableDebugThrow_();					// No need to have debug versions mention this throw
		if (Registration::IsTimeLimited()) 
			throw 'quit';						// Nobody need catch this
		}//endif not registered copy
	//else we'll slam in an annoyingware notice when we construct the layout if needed

// for the moment, a time limited version is treated like annoyingware
//	if (Registration::IsTimeLimited())
//		gIsRegistered = false;

#ifdef NEED_LAYOUT_PALETTE
	// Open our floating windows (aka palettes, aka windoids)
	gPalette = LWindow::CreateWindow(PPob_Palette, this);
	EUtil::AlignToScreen(gPalette, kAlignTopRight);
	// Be sure it isn't automatically hidden
	::ChangeWindowAttributes(gPalette->GetMacWindow(), 0, kWindowHideOnSuspendAttribute);
#endif

#ifdef NEED_TOOL_PALETTE
	gTools = LWindow::CreateWindow(PPob_Tools, this);
	EUtil::AlignToScreen(gTools, kAlignBottomLeft);
	// Try to suppress stuff
	::ChangeWindowAttributes(gTools->GetMacWindow(), 0, kWindowCloseBoxAttribute + kWindowCollapseBoxAttribute + (1L << 21));
#endif
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
	if ((err == noErr) && (response & gestaltMenuMgrAquaLayoutMask)) {
		gAqua = true;
		new LMenuBar(mbar_Carbon);
		// Note that in Aqua, the Apple menu is considered to have only one item in it
		// so we can't just jam the command in the second item of the Apple menu
	} else
		new LMenuBar(MBAR_Initial);

	// Fill in Font menu
	LMenu *			fontMenu = LMenuBar::GetCurrentMenuBar()->FetchMenu(MENU_FontCopy);
	::CreateStandardFontMenu(fontMenu->GetMacMenuH(), 0, 23456, kNilOptions, nil);
	fontMenu->SetCommand(::CountMenuItems(fontMenu->GetMacMenuH()), cmd_UseMenuItem);

	// Make Windows menu
	MenuRef			windowMenu;
	ThrowIfOSErr_(::CreateStandardWindowMenu(kNilOptions, &windowMenu));
	::MacInsertMenu(windowMenu, 0);				// Insert at end of window list
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

#ifdef NEED_LAYOUT_PALETTE
		case cmd_LayoutPalette:
			if (gPalette != nil) {
				delete gPalette;
				gPalette = nil;
			} else {
				gPalette = LWindow::CreateWindow(PPob_Palette, this);
				EUtil::AlignToScreen(gPalette, kAlignTopRight);
				// Be sure it isn't automatically hidden
				::ChangeWindowAttributes(gPalette->GetMacWindow(), 0, kWindowHideOnSuspendAttribute);
			}
			SetUpdateCommandStatus(true);
			break;
#endif

#ifdef NEED_TOOL_PALETTE
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
#endif

		case tool_Arrow:
		case tool_Crop:
		case tool_Rotate:
		case tool_Zoom:
		case tool_Name:
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
	Open the specified file as a new document. It's assumed to be a text file, i.e. one of ours.
*/
void
PhotoPrintApp::OpenDocument(FSSpec* inMacFSSpec)
{
	// The file may already be open, in which case we bring its document window to the front
	LDocument*	theDoc = LDocument::FindByFileSpec(*inMacFSSpec);
	if (theDoc != nil) {
		theDoc->MakeCurrent();
	} else {
		// ??? quick check for XML doc?
		PhotoPrintDoc* doc = new PhotoPrintDoc(this, *inMacFSSpec);
	}
} // OpenDocument 

/*
OpenOrPrintDocList {OVERRIDE}
	We check the list of files. If they are text files, we assume they are our documents, and open
	them in separate windows. Otherwise, we open a new grid document and import all the non-text files
	(if we didn't override, we'd get a different document for each file)
*/
void
PhotoPrintApp::OpenOrPrintDocList(
	const AEDescList&	inDocList,
	SInt32				inAENumber)
{
	if (inAENumber == ae_PrintDoc) {
		// We don't really handle printing by dragging to icon
		LDocApplication::OpenOrPrintDocList(inDocList, inAENumber);
		return;
	}

	MAEList			docList(inDocList);		// Wrap it
	SInt32			numDocs = docList.GetCount();
	MAEList			theList;				// The files we will import
	for (SInt32 i = 1; i <= numDocs; i++) {
		AEKeyword	theKey;
		DescType	theType;
		FSSpec		theFileSpec;
		Size		theSize;
		docList.GetNthPtr(theSize, theKey, theType, i, typeFSS, &theFileSpec, sizeof(FSSpec));

		FInfo		info;
		::FSpGetFInfo(&theFileSpec, &info);

		if (info.fdType == 'TEXT') {
			// Our documents are text files; just open it (in separate window)
			this->OpenDocument(&theFileSpec);
		} else {
			// Assume we can import it (i.e. that QuickTime will handle it)
			theList.PutPtr(typeFSS, &theFileSpec, sizeof(FSSpec));
		}
	}

	// Import all non-text files together
	if (theList.GetCount() > 0) {
		// Create a "new document" event
		MAEAddressDesc		realAddress (MFileSpec::sDefaultCreator);
		MAppleEvent 		aevt(kAECoreSuite, kAECreateElement, realAddress);
		DescType			docType = cDocument;
		aevt.PutParamPtr(typeType, &docType, sizeof(DescType), keyAEObjectClass);

		// What kind of template
		docType = 'grid';
		aevt.PutParamPtr(typeType, &docType, sizeof(DescType), keyAERequestedType);

		// and the files
		aevt.PutParamDesc(theList, keyAEData);

		// And send it! This will result in a window being opened by HandleCreateElementEvent;
		// PhotoPrintView::ReceiveDragEvent will then handle importing.
		aevt.Send();
	}
} // OpenOrPrintDocList

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
			if (forceLayout) {
				photoDoc->UpdatePreferences();
				photoDoc->GetView()->GetLayout()->LayoutImages();
			}
			photoDoc->GetView()->Refresh();
		}//endif
	}//for
}//end RefreshDocuments


/*
SetDocumentControllers [static]
*/
void
PhotoPrintApp::SetDocumentControllers(OSType inTool) {
	TArray<LDocument*>&		docList (LDocument::GetDocumentList());
	SInt32					count = (SInt32) docList.GetCount();
	
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
	NewCommand	command('grid', this);
	command.Execute('grid', nil);
} // StartUp

// ---------------------------------------------------------------------------
//	¥ UpdateMenus {OVERRIDE}
// ---------------------------------------------------------------------------
//	Update the status of all menu items
void
PhotoPrintApp::UpdateMenus()
{
	// Update the PowerPlant commands
	LDocApplication::UpdateMenus();

	if (gAqua) {
		// Under Aqua, Preferences is not a PowerPlant command
		if (!EDialog::IsDialogShowing()) {
			::EnableMenuCommand(0, 'pref');
		} else {
			::DisableMenuCommand(0, 'pref');
		}

		::EnableMenuCommand(0, 'quit');
	}
} // UpdateMenus
