/*
	File:		PhotoPrintDoc.cp

	Contains:	Implementation of the document (which manages a window and file).

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		24 Jul 2001		rmgw	Remove bogus OnModelDirtied.
		23 Jul 2001		drd		206 Initialize imageCount in Read as a sanity-check for missing element
		23 jul 2001		dml		206 save/restore layout more correctly
		23 Jul 2001		rmgw	Listen for new model messages.
		20 jul 2001		dml		make broadcaster.  broadcast msg on SetDirty
		20 Jul 2001		rmgw	Add min/max/orientation undo.
		20 Jul 2001		rmgw	Remove DeleteAction.
		18 Jul 2001		drd		153 185 186 Added init arg to SetLayoutType
		18 Jul 2001		rmgw	Split up ImageActions.
		16 Jul 2001		rmgw	Report errors from drag.  Bug #162.
		16 Jul 2001		drd		169 SetController disables layout controls when we have name badges up
		13 Jul 2001		drd		168 Put up watch in HandlePrint, where we can
		12 jul 2001		dml		PageSetup must update PhotoPrinter's curPrinterCreator (for about-box)
		12 Jul 2001		rmgw	Fix HandleCreateImportEvent location interpreter.  Bug #154.
		12 Jul 2001		rmgw	Beef up HandleCreatePhotoItemEvent to accept data for copying.
		12 jul 2001		dml		GetPrintRec calls PhotoPrinter::SetupPrintRecordToMatchProperties when creating
		12 Jul 2001		rmgw	Convert the import event to make new import.
		11 jul 2001		dml		98.  AskSaveChanges active, and respects preference
		11 Jul 2001		drd		143 Added mOrientationPopup
		10 Jul 2001		rmgw	Change HandleCreateElementEvent to handle errors.
		10 Jul 2001		rmgw	Change HandleCreateElementEvent to handle lists and update the view.
		10 Jul 2001		drd		91 No need to make ImportCommand; minor optimizations listening to popups
		10 Jul 2001		drd		91 Initialize calls SetDefaultSubModel
		09 Jul 2001		rmgw	AdoptNewItem now returns a PhotoIterator. Bug #142.
		09 Jul 2001		rmgw	Change HandleCreateElementEvent to call SetupDraggedItem.
		06 Jul 2001		drd		72 DoRevert sends UpdateZoom; 128 DoRevert calls SetWatch
		06 Jul 2001		rmgw	Implement HandleCreateElementEvent.
		06 jul 2001		dml		use gNeedDoubleOrientationSetting in Read
		05 Jul 2001		drd		125 SetResolution calls SetUpdateCommandStatus
		02 Jul 2001		drd		Background doesn't need LColorEraseAttachment under OS X
		02 Jul 2001		rmgw	AdoptNewItem now takes a PhotoIterator.
		02 Jul 2001		drd		MatchPopupsToPrintRec; ListenToMessage handles orientation popup
		02 Jul 2001		rmgw	Add PhotoItem AEOM handlers.
		28 Jun 2001		rmgw	Clear dirty flag after save.  Bug #108.
		28 Jun 2001		rmgw	Zoom on center point.  Bug #102.
		28 jun 2001		dml		70 add WarnAboutAlternate
		28 Jun 2001		drd		106 ListenToMessage sets dirtiness (also factored out some code)
		28 Jun 2001		drd		95 DoRevert gets rid of old data and refreshes
		25 Jun 2001		drd		85 Read sets orientation (kLandscape/kPortrait)
		20 Jun 2001		drd		79 DoRevert calls HandleKnownExceptions; alrt_XMLError now has labels, we don't need
								to compose text in DoRevert
		15 Jun 2001		rmgw	Pass in window to SetController.  Bug #66.
		01 Jun 2001		drd		73 ObeyCommand (handles tool button commands)
		23 May 2001		drd		74 Removed GetDescriptor
		23 May 2001		drd		CreateWindow sets kWindowInWindowMenuAttribute; 24 DoOpen titles window and sets
								mIsSpecified, DoRevert clears dirt
		22 May 2001		drd		69 PasteCommand; 24 give document a name before saving it, so name can be written
		21 May 2001		drd		69 CopyCommand, CutCommand
		25 Apr 2001		drd		Hooked up RedrawCommand; FixPopups
		25 Apr 2001		drd		ListenToMessage handles min & max popups
		23 Apr 2001		drd		UpdatePreferences
		27 mar 2001		dml		removed print sheets.  really this time
		26 Mar 2001		drd		Added mMinMaxGroup, mMinPopup, mMaxPopup
		21 Mar 2001		drd		Minor cleanup; added bevel buttons for new UI
		13 Mar 2001		drd		Make mSheetDoneUPP in Initialize, and call this earlier
		09 mar 2001		dml		add FinishHandlePrint, split HandlePrint to deal w/ PrintSheets Async
		28 feb 2001		dml		bug 53.  ensure a fresh print session for page setup
		22 Feb 2001		drd		Try sheets again
		15 feb 2001		dml		Since Carbon pre OSX only allows a single PrintSession (GRR) use the stupid singleton
		06 feb 2001		dml		alphabetize
		05 feb 2001		dml		change UPP instantiation in DoPrint, since Carbon might be <1.2 (iff non-session)
		25 jan 2001		dml		add print sheets to DoPrint
		17 jan 2001		dml		work on AskSaveAs, add save-as-template functionality
		16 jan 2001		dml		added isTemplate to Write()
		14 dec 2000		dml		fix handling of body/header/footer
		07 dec 2000		dml		clamp mPageHeight in MatchViewToPrintRec as others
		05 Dec 2000		drd		Added gCount, use it to give windows unique names
		30 nov 2000		dml		fix bug 22, dt must clear global session if owner
		05 Oct 2000		drd		Use std:: with map, less
		21 Sep 2000		drd		DoPrint reports error using ExceptionHandler
		20 Sep 2000		drd		Stagger our windows
		19 Sep 2000		drd		We do open & save again, so set gWindowProxies to true
		18 sep 2000		dml		fixed crash concerning gFlatPageFormat (hand off copy)
		18 sep 2000		dml		add GetPageHeight, UpdatePageHeight, mPageHeight
		15 Sep 2000		drd		HandleKeyPress checks for pretzel-=
		14 sep 2000		dml		clamp mWidth, mHeight to 100ths in MatchViewRect
		13 Sep 2000		drd		Added gWindowProxies, and conditioned our use of them
		13 sep 2000		dml		removed CalcInitialWindowRect.  using PhotoWindow subclass of LWindow
		12 Sep 2000		drd		MatchViewToPrintRec updates page via new UpdatePageNumber method
		12 Sep 2000		drd		Made test for existence of gFlatPageFormat explicit (fixes crash)
		12 sep 2000		dml		maintain a FlatPageFormat so docs can share MRU page type
		12 sep 2000		dml		added CalcInitialWindowRect
		11 sep 2000		dml		fixes to SetResolution; handle fractional scalars, don't change mWidth, mHeight
		07 Sep 2000		drd		Update zoom
		07 Sep 2000		dml		added mPageCount, mZoomDisplay
		06 Sep 2000		dml		SetResolution works.  mPhotoPrintView replaced by mScreenView
		23 Aug 2000		drd		Hook up ImportCommand, LayoutCommand; HandleAppleEvent
		21 Aug 2000		drd		Removed ParseLayout, renamed sParseLayout
		21 Aug 2000		drd		Read, Write mProperties
		21 aug 2000		dml		add RemoveRotation command
		18 Aug 2000		drd		MakeIconCommand
		15 Aug 2000		drd		RemoveCropCommand
		14 Aug 2000		drd		RevealCommand
		11 Aug 2000		drd		SelectAllCommand
		11 aug 2000		dml		add SetController
		04 Aug 2000		drd		Removed ObeyCommand; added HandleKeyPress (for backspace & clear)
		04 Aug 2000		drd		Use ClearCommand; create LUndoer
		27 Jul 2000		drd		Added proxy stuff (and IsModified) and update window title on
								Save; removed SpendTime; switched to overriding HandlePageSetup,
								added HandlePrint; no more PageSetupCommand
		24 Jul 2000		drd		DoPrint hides both windoids
		24 jul 2000		dml		stopped forcing doc's print props alternate to true
		18 jul 2000		dml		using PhotoPrintApp::gCurPrintSession and gPrintSessionOwner
		17 jul 2000		dml		PageSetup resets the session (fix choose new printer crash)
		14 jul 2000		dml		PageSetup attentive to return value of dialog
		14 jul 2000		dml		more removal of StPrintSessions
		14 jul 2000		dml		use App's PrintSpec, not our own (eventually will reclaim)
		14 jul 2000		dml		use PhotoPrinter::CalculatePrintableRect not GetPageRect
		13 jul 2000		dml		set mNumPages in MatchViewToPrintRec
		11 jul 2000		dml		add numPages to MatchViewToPrintRec
		06 Jul 2000		drd		Use StDesktopDeactivator in DoPrint, and manipulate palette
		30 Jun 2000		drd		DoPageSetup dirties window
		28 jun 2000		dml		add serialization of Layout type
		27 jun 2000		dml		fill-in DoPageSetup 
		27 jun 2000		dml		add DoPageSetup
		26 Jun 2000		dml		Factor ct stuff init Initialize, PrintAlternate by default
		26 Jun 2000		drd		Fixed truncation in CreateWindow
		20 Jun 2000		drd		Added gCurDocument, so others know who we are at constructor time
		20 june 2000 	dml		fixed FSSpec ct to set mDPI
		15 june 2000	drd		add ResizeFrameTo (since scrn no longer bound)
		15 Jun 2000		drd		We now have a background view
		14 Jun 2000		drd		ImageOptionsCommand
		14 Jun 2000		drd		BackgroundOptionsCommand
		1 June 2000		dml		force print record to coincide w/ settings at start of DoPrint
*/

#include "PhotoPrintDoc.h"

#include "BackgroundOptions.h"
#include "ClearCommand.h"
#include "CopyCommand.h"
#include "CutCommand.h"
#include "DocumentAction.h"
#include "ImageOptions.h"
#include "Layout.h"
#include "LayoutCommand.h"
#include "MakeIconCommand.h"
#include "PasteCommand.h"
#include "PhotoExceptionHandler.h"
#include "PhotoItemModelObject.h"
#include "PhotoPrintCommands.h"
#include "PhotoPrintEvents.h"
#include "PhotoPrintResources.h"
#include "PrintCommand.h"
#include "PhotoPrintApp.h"
#include "PhotoPrinter.h"
#include "PhotoPrintPrefs.h"
#include "PhotoPrintView.h"
#include "PhotoUtility.h"
#include "PhotoWindow.h"
#include "RedrawCommand.h"
#include "RevealCommand.h"
#include "RemoveCropCommand.h"
#include "RemoveRotationCommand.h"
#include "SaveCommand.h"
#include "FontCommand.h"
#include "SelectAllCommand.h"
#include "UWindowStagger.h"
#include "ZoomCommands.h"
#include "PhotoPrintConstants.h"

// Epp
#include "EPostAction.h"
#include "EUserMessage.h"
#include "StDisableBroadcaster.h"

// Toolbox++
#include "MAEList.h"
#include "MAEDesc.h"
#include "MAEDescExtractors.h"
#include "MAEDescIterator.h"
#include "MAlert.h"
#include "MAppleEvent.h"
#include "MDialogItem.h"
#include "MFolderIterator.h"
#include "MNavDialogOptions.h"
#include "MNavPutFile.h"
#include "MNavReplyRecord.h"
#include "MNavAskSaveChanges.h"
#include "MNewDialog.h"
#include "MNumberParts.h"
#include "MPString.h"

#include "xmloutput.h"
#include "xmlinput.h"
#include "xmlfile.h"

#include <iostream>
#include <map.h>


// Globals
SInt32			PhotoPrintDoc::gCount = 0;
PhotoPrintDoc*	PhotoPrintDoc::gCurDocument = nil;
bool			PhotoPrintDoc::gWindowProxies = true;

const ResIDT	alrt_XMLError = 	131;
const ResIDT	TEXT_ImportFailure = 1139;
const ResIDT 	PPob_PhotoPrintDocWindow = 1000;
const ResIDT 	prto_PhotoPrintPrintout = 100;
const ResIDT	dlog_WarnAboutAlternate = 2010;
const PaneIDT 	pane_ScreenView = 	'scrn';
const PaneIDT 	pane_Scroller = 	'scrl';
const PaneIDT	pane_ZoomDisplay = 	'zoom';
const PaneIDT	pane_PageCount = 	'page';
const PaneIDT	pane_Background = 	'back';

SInt16 PhotoPrintDoc::kFeelGoodMargin = 32;		// The grey area at the right


//---------------------------------------------------------------
// support for the map between alignment type and text
typedef	std::map<OSType, char*, std::less<OSType> > LayoutMap;

class LayoutMapper {
	protected:
		static	bool sInitialized;
		static	LayoutMap	sMap;
		static	void Initialize();
		
	public :	
		static const char*			Find(OSType key);
		static OSType		Lookup(const char* text);
	};//end class LayoutMapper
	
bool LayoutMapper::sInitialized = false;
LayoutMap	LayoutMapper::sMap;

void
LayoutMapper::Initialize() {
	sMap[Layout::kUnspecified] = "Unspecified";
	sMap[Layout::kCollage] = "Collage";
	sMap[Layout::kFixed] = "Fixed";
	sMap[Layout::kGrid] = "Grid";
	sMap[Layout::kMultiple] = "Multiple";
	sMap[Layout::kSchool] = "School";
	sMap[Layout::kSingle] = "Single";
	sInitialized = true;
	}//end
	
const char*
LayoutMapper::Find(OSType key) {
	if (!sInitialized) {
		Initialize();
		}//endif need to construct

		LayoutMap::const_iterator	i (sMap.find (key));
		if (i != sMap.end ()) 
			return (*i).second;
		else
			return 0;
	}//end Find
	
OSType
LayoutMapper::Lookup(const char* text) {
	if (!sInitialized) {
		Initialize();
		}//endif need to construct

	for (LayoutMap::const_iterator	i = sMap.begin(); i != sMap.end(); ++i) {
		if (strcmp((*i).second, text) == 0) {
			return (*i).first;
			}//endif
		}//end

	return Layout::kUnspecified;
}//end Lookup



#pragma mark -

//-----------------------------------------------------------------
//PhotoPrintDoc
//-----------------------------------------------------------------
PhotoPrintDoc::PhotoPrintDoc		(LCommander*		inSuper, 
									Boolean inVisible)
	:LSingleDoc (inSuper)
	, mFileType ('foto')
	, mDPI (72)
{
	this->Initialize();
	this->CreateWindow(PPob_PhotoPrintDocWindow, inVisible);

	SetUseSubModelList (true);

}//end ct

//-----------------------------------------------------------------
//PhotoPrintDoc
//-----------------------------------------------------------------
PhotoPrintDoc::PhotoPrintDoc		(LCommander*		inSuper,
									 const	FSSpec&		inSpec,
									 Boolean inVisible) 
	: LSingleDoc (inSuper)
	, mFileType ('foto')
	, mDPI (72)
 {
	this->Initialize();
	this->CreateWindow(PPob_PhotoPrintDocWindow, inVisible);

	this->DoOpen(inSpec);
	
	SetUseSubModelList (true);
	
 }//end ct

//-----------------------------------------------------------------
//~PhotoPrintDoc
//-----------------------------------------------------------------
PhotoPrintDoc::~PhotoPrintDoc	(void)
{
	// see if a session is active and we are the owner
	if ((PhotoPrintApp::gPrintSessionOwner != nil) &&
		(PhotoPrintApp::gPrintSessionOwner == this)) {
		// if there is a session, and it _is_ ours, close it
		delete (PhotoPrintApp::gCurPrintSession);
		PhotoPrintApp::gCurPrintSession = nil; 
		PhotoPrintApp::gPrintSessionOwner = nil;
		}//endif there is a session open

	if (mSheetDoneUPP != nil)
		::DisposePMSheetDoneUPP(mSheetDoneUPP);
}//end dt


//-----------------------------------------------------------------
//AddCommands
//-----------------------------------------------------------------
void					
PhotoPrintDoc::AddCommands			(void)
{
	// File menu
	new PrintCommand(cmd_Print, this);
	new SaveCommand(cmd_Save, this);
	new SaveCommand(cmd_SaveAs, this);

	// Edit menu
	new CutCommand(cmd_Cut, this);
	new CopyCommand(cmd_Copy, this);
	new PasteCommand(cmd_Paste,this);
	new ClearCommand(cmd_Clear, this);
	new SelectAllCommand(cmd_SelectAll, this);

	// View Menu
	new BackgroundOptionsCommand(cmd_BackgroundOptions, this);
	new FitInWindowCommand(cmd_FitInWindow, this);
	new ViewFullSizeCommand(cmd_ViewFullSize, this);
	new ZoomInCommand(cmd_ZoomIn, this);
	new ZoomOutCommand(cmd_ZoomOut, this);

	// Image menu
	new ImageOptionsCommand(cmd_ImageOptions, this);
	new MakeIconCommand(cmd_MakeIcon, this);
	new RemoveCropCommand(cmd_RemoveCrop, this);
	new RemoveRotationCommand(cmd_RemoveRotation, this);
	new RevealCommand(cmd_Reveal, this);

	// Font Stuff
	new FontCommand(msg_AnyMessage, this, MENU_FontCopy);

	// Debug menu
	new LayoutCommand(cmd_ReLayout, this);
	new RedrawCommand(cmd_Redraw, this);
}//end AddCommands

//-----------------------------------------------------------------
//AddEvents
//-----------------------------------------------------------------
void					
PhotoPrintDoc::AddEvents			(void) {
}//end AddEvents


//-----------------------------------------------------------------
//AskSaveAs
//-----------------------------------------------------------------
Boolean			
PhotoPrintDoc::AskSaveAs			(FSSpec&			outFSSpec,
									Boolean				/*inRecordIt*/)
{
	Boolean bHappy (false);
	do {
		MNavDialogOptions		options;
		::GetIndString (options.clientName, STRx_Standards, str_ProgramName);
		::GetIndString (options.message, STRx_Standards, str_SaveAs);
		this->GetDescriptor(options.savedFileName);
	
		StDesktopDeactivator	desktopDeactivator;
		MNavPutFile				d ;
		MNavReplyRecord			navReply;
		d.DoPutFile (navReply, &options, GetFileType());
		if (!navReply.validRecord) continue;

		AEKeyword	theAEKeyword;
		DescType	actualType;
		Size		actualSize;
		ThrowIfOSErr_(::AEGetNthPtr (&navReply.selection, 1, typeFSS, &theAEKeyword, &actualType, 
										&outFSSpec, sizeof (outFSSpec), &actualSize));
		
		if (navReply.replacing) 	// Delete existing file
			ThrowIfOSErr_(::FSpDelete (&outFSSpec));
		
		if (navReply.isStationery) 
			DoSaveWithProperties(outFSSpec, 'stat', nil);
		else
			DoSaveToSpec (outFSSpec);

		bHappy = true;
		} while (false);
					
		return bHappy;
}//end AskSaveAs

//-----------------------------------------------------------------
//AskSaveChanges
//-----------------------------------------------------------------
SInt16			
PhotoPrintDoc::AskSaveChanges		(bool				/*inQuitting*/)
{	
	
	MNavDialogOptions		options;
	::GetIndString (options.clientName, STRx_Standards, str_ProgramName);
	this->GetDescriptor(options.savedFileName);

	NavAskSaveChangesResult res (kNavAskSaveChangesDontSave);
	PhotoPrintPrefs*	prefs = PhotoPrintPrefs::Singleton();
	if (prefs->GetWarnDirty()) {
		MNavAskSaveChanges	askSaveChanges;
		askSaveChanges.DoAskSaveChanges(res, kNavSaveChangesOther, &options);
		}//endif want warning
	
	return res;
}//end AskSaveChanges


// ---------------------------------------------------------------------------
//	¥ CountSubModels
// ---------------------------------------------------------------------------
//	Return number of SubModels of the specified type
//
//	Must be overridden by subclasses which have SubModels that aren't
//	implemented using the submodel list (ie lazy instantiated submodels).
//
//	When overriding, you should add the inherited result to your overridden
//	result.

SInt32
PhotoPrintDoc::CountSubModels(
	DescType	inModelID) const
{
	switch (inModelID) {
		case PhotoItemModelObject::cClass:
			return GetModel ()->GetCount ();
			
		case cImportClass:
			SignalString_("CountSubModels for import class!");
			return 0;
		} // switch

	return LModelObject::CountSubModels (inModelID);
}

//-----------------------------------------------------------------
//CreateWindow
//-----------------------------------------------------------------
void					
PhotoPrintDoc::CreateWindow		(ResIDT				inWindowID, 
								 Boolean 			inVisible)
{
	gCurDocument = this;						// So the model (and layout) can link to us

	mWindow = LWindow::CreateWindow(inWindowID, this);
	ThrowIfNil_(mWindow);

	PhotoWindow* photoWindow = dynamic_cast<PhotoWindow*>(mWindow);
	if (photoWindow)
		photoWindow->SetDoc(this);

	if (gWindowProxies) {
		// Give it the little icon in the window title
		StGrafPortSaver		port;					// Mac OS 8.5 needs this
		::SetWindowProxyCreatorAndType(mWindow->GetMacWindow(), MFileSpec::sDefaultCreator,
			'TEXT' /* this->GetFileType() */, 0L);
	}

	// Be sure it shows up in Window menu
	::ChangeWindowAttributes(mWindow->GetMacWindow(), kWindowInWindowMenuAttribute, 0L);

	// Give the window a unique name by adding a numeric suffix
	gCount++;
	LStr255		theName;
	mWindow->GetDescriptor(theName);
	theName += '-';							// Dash not space in case of Unix
	theName += gCount;
	mWindow->SetDescriptor(theName);

	// Stagger the window (the system can't, it gets confused about floaters)
//	UWindowStagger::Stagger(mWindow);

	LView*	background = dynamic_cast<LView*>(mWindow->FindPaneByID(pane_Background));
	if (background != nil && UEnvironment::HasFeature(env_HasAquaTheme)) {
		// Under OSX, having the attachment meant the pinstripes didn't align
		background->RemoveAllAttachments();
	}

	mScreenView = dynamic_cast<PhotoPrintView*>(mWindow->FindPaneByID(pane_ScreenView));	
	ThrowIfNil_(mScreenView);

	mScroller = dynamic_cast<LScrollerView*>(mWindow->FindPaneByID(pane_Scroller));
	ThrowIfNil_(mScroller);

	mPageCount = mWindow->FindPaneByID(pane_PageCount);
	ThrowIfNil_(mPageCount);
	
	mZoomDisplay = mWindow->FindPaneByID(pane_ZoomDisplay);
	ThrowIfNil_(mZoomDisplay);

	mDupPopup = dynamic_cast<LBevelButton*>(mWindow->FindPaneByID('dupl'));
	ThrowIfNil_(mDupPopup);
	mDupPopup->SetCurrentMenuItem(1);		// Set this before we start listening
	mDupPopup->AddListener(mScreenView);

	mLayoutPopup = dynamic_cast<LBevelButton*>(mWindow->FindPaneByID('layo'));
	ThrowIfNil_(mLayoutPopup);
	mLayoutPopup->SetCurrentMenuItem(1);	// Set this before we start listening
	mLayoutPopup->AddListener(mScreenView);

	// Min and Max
	mMinMaxGroup = mWindow->FindPaneByID('mmgr');
	mMinPopup = dynamic_cast<LPopupButton*>(mWindow->FindPaneByID('mini'));
	mMinPopup->AddListener(this);
	mMaxPopup = dynamic_cast<LPopupButton*>(mWindow->FindPaneByID('maxi'));
	mMaxPopup->AddListener(this);

	mOrientationPopup = dynamic_cast<LBevelButton*>(mWindow->FindPaneByID('orie'));
	if (mOrientationPopup != nil)
		mOrientationPopup->AddListener(this);

	this->UpdatePreferences();							// Initialize our own, and the popups

	// !!! by zooming, we ruin the staggering, and any offset from the left
	// !!! also, it would be nice to avoid the dock
	mWindow->DoSetZoom(true); // set to "standard" (zoom, though overridden) state
	// Try again to stagger
	UWindowStagger::Stagger(mWindow);

	if (inVisible)
		mWindow->Show();

	this->MatchViewToPrintRec();
	
	mScreenView->GetModel ()->AddListener (this);
	
}// end CreateWindow								 


//-----------------------------------------------------------------
// DoOpen
//-----------------------------------------------------------------
void
PhotoPrintDoc::DoOpen(const FSSpec& inSpec) {
	mFileSpec = new MFileSpec(inSpec);
	mIsSpecified = true;

	// Set the window's title and proxy icon
	mWindow->SetDescriptor(inSpec.name);
	::SetWindowProxyFSSpec(mWindow->GetMacWindow(), &inSpec);

	this->DoRevert();
}//end DoOpen


/*
DoPrint {OVERRIDE}
	Print a Document
*/
void
PhotoPrintDoc::DoPrint()
{
		
	HORef<LPrintout>		thePrintout (LPrintout::CreatePrintout (prto_PhotoPrintPrintout));
	thePrintout->SetPrintSpec(*this->GetPrintRec());
	LPlaceHolder			*placeHolder = (LPlaceHolder*) thePrintout->FindPaneByID ('TBox');
	HORef<PhotoPrinter>		pPrinter = new PhotoPrinter(this, mScreenView, this->GetPrintRec(), 
														&mPrintProperties, thePrintout->GetMacPort());
	
	placeHolder->InstallOccupant (&*pPrinter, atNone);
	try {
		StDisableDebugThrow_ ();
		thePrintout->DoPrintJob();
	}//end try

	catch (LException e) {
		ExceptionHandler::HandleKnownExceptions(e);
	}//end catch
	catch (...) {
		//we should never get here, but just in case, silently eat the error
	}//end last chance catch
	placeHolder->RemoveOccupant ();			//	Always RemoveOccupant!
} // DoPrint

//-----------------------------------------------------------------
//DoSave
//-----------------------------------------------------------------
void			
PhotoPrintDoc::DoSave()
{
	this->DoSaveToSpec(*mFileSpec);
}//end DoSave


//-----------------------------------------------------------------
//DoSaveToSpec
//-----------------------------------------------------------------
void			
PhotoPrintDoc::DoSaveToSpec	(const FSSpec& inSpec, bool isTemplate)
{
	MFileSpec				theSpec(inSpec, Throw_No);
	HORef<char>				path(theSpec.MakePath());
	
	XML::FileOutputStream	file(path);
	XML::Output				out(file);

	// XML is making the file as if it were CodeWarrior
	FInfo					info;
	theSpec.GetFinderInfo(info);
	info.fdCreator = MFileSpec::sDefaultCreator;
	theSpec.SetFinderInfo(info);

	// and write the data
	this->Write(out, isTemplate);

	if (mFileSpec == nil || *mFileSpec != theSpec)
		mFileSpec = new MFileSpec(theSpec);
	
	// Now that we have a file, update title & icon
	mWindow->SetDescriptor(theSpec.Name());
	if (gWindowProxies) {
		StGrafPortSaver		port;				// Mac OS 8.5 needs this
		::SetWindowProxyFSSpec(mWindow->GetMacWindow(), &inSpec);

		// Be sure the little icon in the window title shows that we're saved
		::SetWindowModified(mWindow->GetMacWindow(), false);
	}

	this->SetDirty (false);
	
	mIsSpecified = true;
}//end DoSaveToSpec

/*
DoSaveWithProperties
*/
void			
PhotoPrintDoc::DoSaveWithProperties (const FSSpec& ioSpec, OSType inType, const AEDesc* /*inProps*/){
	//requires valid FSSpec
	
	if (MFileSpec::IsReadOnly (&ioSpec)) 
		ThrowOSErr_(opWrErr);

	switch (inType) {
		case 'stat' : {
			DoSaveToSpec(ioSpec, true);
			
			MFileSpec spec (ioSpec);
			FInfo info;
			spec.GetFinderInfo(info);
			info.fdFlags |= kIsStationery;
			spec.SetFinderInfo(info);
			}//end case stationery
			break;
		}//end switch
	}//end DoSaveWithProperties


//-----------------------------------------------------------------
//DoRevert
//-----------------------------------------------------------------
void			
PhotoPrintDoc::DoRevert(void)
{
	UCursor::SetWatch();

	// 95 Be sure to redraw the old data
	this->GetView()->Refresh();

	HORef<char> path (mFileSpec->MakePath());
	XML::FileInputStream file (path);
	XML::Input input(file);

	XML::Handler handlers[] = {
		XML::Handler("Document", sDocHandler),
		XML::Handler::END
		};
		
	try {
		// 95 Get rid of current data before reading from the file
		this->GetView()->GetModel()->RemoveAllItems();

		input.Process(handlers, (void*)this);
		}//end try
	catch (const XML::ParseException& e) {
		// !!! should have an XML exception handler (we could get filename or something)
		LStr255 sWhat (e.What());
		LStr255 sLineNumber ((short)e.GetLine());
		LStr255 sColumnNumber ((short)e.GetColumn());
		
		::ParamText(sWhat, sLineNumber, sColumnNumber, nil);
		::InitCursor();			// Be sure we have arrow cursor
		::StopAlert(alrt_XMLError, nil);
		throw;					// Rethrow -- hopefully app won't put up another alert
	} catch (LException e) {
		// 79 OpenCommand calls us via an Apple Event, so the catch in ECommandAttachment::ExecuteSelf
		// will never see the exception (it's not passed through the Toolbox). So we need to do our
		// error handling here.
		if (!ExceptionHandler::HandleKnownExceptions(e))
			throw;
	}//catch

	// Be sure proxy icon shows that there are no unsaved changes
	::SetWindowModified(mWindow->GetMacWindow(), false);

	// We've just read the data, it should not be considered dirty (apparently the fact of adding each
	// item is making it so)
	this->SetDirty(false);

	// 95 Be sure to redraw the reverted data
	this->GetView()->Refresh();

	// 72 Be sure placard is updated
	this->UpdateZoom();
}//end DoRevert

void
PhotoPrintDoc::FixPopups(void)
{
	// ??? to do: move somewhere, probably properties
	MenuHandle		menu;
	MenuItemIndex	item;

	// Min can't go higher than the value of Max
	menu = mMinPopup->GetMacMenuH();
	for (item = limit_Index; item <= ::CountMenuItems(menu); item++) {
		if (mMaximumSize == limit_None || item <= mMaximumSize)
			::EnableMenuItem(menu, item);
		else
			::DisableMenuItem(menu, item);
	}

	// Max can't go lower than the value of Min
	menu = mMaxPopup->GetMacMenuH();
	for (item = limit_Index; item <= ::CountMenuItems(menu); item++) {
		if (mMinimumSize == limit_None || item >= mMinimumSize)
			::EnableMenuItem(menu, item);
		else
			::DisableMenuItem(menu, item);
	}
} // FixPopups

/*
ForceNewPrintSession
*/
void
PhotoPrintDoc::ForceNewPrintSession()
{
	if	(PhotoPrintApp::gPrintSessionOwner != nil) {
		// if there is a session open, close it
		delete (PhotoPrintApp::gCurPrintSession);
		PhotoPrintApp::gCurPrintSession = nil; 
		PhotoPrintApp::gPrintSessionOwner = nil;
	}//endif there is a session open 
	
	PhotoPrintApp::gCurPrintSession = new StPrintSession(*mPrintSpec);
	PhotoPrintApp::gPrintSessionOwner = this;
}//end ForceNewPrintSession

//-----------------------------------------------------------------
//GetDisplayCenter
//-----------------------------------------------------------------
void			
PhotoPrintDoc::GetDisplayCenter			(

	double&	h,
	double&	v) const
{
	
	LView*	background = dynamic_cast<LView*>(mWindow->FindPaneByID(pane_Background));
	Assert_(background);
	
	SDimension16	extents;
	background->GetFrameSize (extents);
	
	SPoint32		midPos;
	background->GetScrollPosition (midPos);
	
	midPos.h += extents.width / 2;
	midPos.v += extents.height / 2;
	
	h = midPos.h; h /= mDPI;
	v = midPos.v; v /= mDPI;
	
}//end GetDisplayCenter

//-----------------------------------------------------------------
//GetFileType
//-----------------------------------------------------------------
OSType			
PhotoPrintDoc::GetFileType			(void) const
{
	return mFileType;
}//end GetFileType

/*
GetOrientation
*/
PhotoPrintDoc::Orientation 
PhotoPrintDoc::GetOrientation(void) const

{
	const	PrintProperties&	props = this->GetPrintProperties();

	switch (props.GetRotationBehavior ()) {
		case PrintProperties::kPickBestRotation:
			return kFlexibleIndex;
			
		case PrintProperties::kForceLandscape:
			return kLandscapeIndex;
			
		case PrintProperties::kForcePortrait:
			return kPortraitIndex;
		} // switch
	
	Assert_(false);
	
	return kPortraitIndex;
	
} // GetOrientation

// ---------------------------------------------------------------------------
//		 GetPageHeight
// returns our floating point body height * resolution.  used to determine curPage
// ---------------------------------------------------------------------------
SInt32
PhotoPrintDoc::GetPageHeight(void) const {
	return mPageHeight * GetResolution();
	}//end


// ---------------------------------------------------------------------------
//		 GetPrintRec
// Will construct if necessary.  Attentive to existing session
// ---------------------------------------------------------------------------

HORef<EPrintSpec>&
PhotoPrintDoc::GetPrintRec (void)

{ // begin PrintRec

	// until we are only in OSX, stupidly, Carbon allows a single
	// PrintSession open at a time (for the entire app)
	// Sooooo, each Doc maintains its own PrintSession, and should we happen
	// to need ours, we, ahem, close any open one
	// there is a global in PhotoPrintApp::gCurPrintSession for this process
	if ((PhotoPrintApp::gPrintSessionOwner != nil) &&
		(PhotoPrintApp::gPrintSessionOwner != this)) {
		// if there is a session, and it is not ours, close it
		delete (PhotoPrintApp::gCurPrintSession);
		PhotoPrintApp::gCurPrintSession = nil; 
		PhotoPrintApp::gPrintSessionOwner = nil;
		}//endif there is a session open

	bool needToInitialize (false);
	// have we even made an EPrintSpec yet?!
	if (mPrintSpec == nil) {
		mPrintSpec = new EPrintSpec();
		if (PhotoPrintApp::gFlatPageFormat != nil) {
			// make a copy of the flat page format to give to the print spec
			Handle copyToOrphan = *PhotoPrintApp::gFlatPageFormat;
			::HandToHand(&copyToOrphan);
			mPrintSpec->SetFlatPageFormat(copyToOrphan);
			}//endif
		else
			needToInitialize = true;
		}//endif need to make print spec

	// if we are here, and a session is open, it must be ours
	// otherwise we need to make and install a session
	if (PhotoPrintApp::gCurPrintSession == nil) {
		PhotoPrintApp::gCurPrintSession = new StPrintSession(*mPrintSpec);
		PhotoPrintApp::gPrintSessionOwner = this;

// support for print sheets is problematic.  note code must also be enabled/disabled
// in HandlePrint

//		if (PhotoPrintApp::gOSX) {
//			OSStatus s = ::PMSessionUseSheets(mPrintSpec->GetPrintSession(), 
//												mWindow->GetMacWindow(), 
//												mSheetDoneUPP);
//			if (s == 0) s++;	// Try to fool optimizer
//		}
	}//endif no session open
		
	// we couldn't initialize w/o a session open, deferred until here.
	if (needToInitialize) {
		mPrintSpec->SetToSysDefault();
		PhotoPrinter::SetupPrintRecordToMatchProperties(mPrintSpec, &GetPrintProperties());
		}//endif
	else
		mPrintSpec->GetPrintSettings();

	return mPrintSpec;
} // GetPrintRec


// ---------------------------------------------------------------------------
//	¥ GetPhotoItemModel
// ---------------------------------------------------------------------------
//	PhotoItems are lazy, so we need to find them specially

void
PhotoPrintDoc::GetPhotoItemModel (

	const	PhotoItemRef&		inItem,
	AEDesc						&outToken) const
	
	{ // begin GetPhotoItemModel
		
		//	Find it
		if (mSubModels != nil) {
			StAEDescriptor		reqType;
			TArrayIterator<LModelObject*>	iterator(*mSubModels);
			for (LModelObject *p; iterator.Next (p); ) {
				if (p->GetModelKind () != PhotoItemModelObject::cClass) continue;
				
				PhotoItemModelObject*	pi = reinterpret_cast<PhotoItemModelObject*> (p);
				Assert_(pi);
				
				if (pi->GetPhotoItem () != inItem) continue;
				
				PutInToken (p, outToken);
				return;
				} // while
			} // if
		
		//	Make a new model
		LModelObject*	newPhotoItem = new PhotoItemModelObject (const_cast<PhotoPrintDoc*> (this), inItem);
		newPhotoItem->SetLaziness (true);
		PutInToken (newPhotoItem, outToken);

	} // end GetPhotoItemModel

// ---------------------------------------------------------------------------
//	¥ GetPositionOfSubModel
// ---------------------------------------------------------------------------
//	Fields are lazy, so we need to find them specially

SInt32
PhotoPrintDoc::GetPositionOfSubModel (

	DescType				inModelID,
	const	LModelObject*	inModel) const
	
	{ // begin GetPositionOfSubModel
		
		switch (inModelID) {
			case PhotoItemModelObject::cClass:
				{
				const	PhotoItemModelObject*	inPhotoItemModel = reinterpret_cast<const PhotoItemModelObject*> (inModel);
				Assert_(inPhotoItemModel);
				
				PhotoItemRef			inPhotoItem = inPhotoItemModel->GetPhotoItem ();
				Assert_(inPhotoItem);
				
				const	PhotoPrintModel*		model = GetModel ();
				ConstPhotoIterator				pi = std::find (model->begin (), model->end (), inPhotoItem);
				if (pi == model->end ()) ThrowOSErr_(errAENoSuchObject);
				
				return 1 + (pi - model->begin ());
				} // case
				
			case cImportClass:
				SignalString_("GetPositionOfSubModel for import class!");
				return 0;

			default:
				return LModelObject::GetPositionOfSubModel (inModelID, inModel);
			} // switch

	} // end GetPositionOfSubModel

// ---------------------------------------------------------------------------
//	¥ GetSubModelByPosition
// ---------------------------------------------------------------------------
//	Fields are lazy, so we need to find them specially

void
PhotoPrintDoc::GetSubModelByPosition (

	DescType		inModelID,
	SInt32			inPosition,
	AEDesc			&outToken) const
	
	{ // begin GetSubModelByPosition
		
		switch (inModelID) {
			case PhotoItemModelObject::cClass:
				Assert_(GetModel ()->GetCount () >= inPosition);
				GetPhotoItemModel (*(GetModel ()->begin () + (inPosition - 1)), outToken);
				break;
			
			case cImportClass:
				SignalString_("GetSubModelByPosition for import class!");
				break;
				
			default:
				LModelObject::GetSubModelByPosition (inModelID, inPosition, outToken);
				break;
			} // switch

	} // end GetSubModelByPosition

// ---------------------------------------------------------------------------
//	¥ GetSubModelByName
// ---------------------------------------------------------------------------
//	Fields are lazy, so we need to find them specially

void
PhotoPrintDoc::GetSubModelByName (

	DescType		inModelID,
	Str255			inName,
	AEDesc			&outToken) const
	
	{ // begin GetSubModelByName
		
		switch (inModelID) {
			case PhotoItemModelObject::cClass:
				for (ConstPhotoIterator i = GetModel ()->begin (); i != GetModel ()->end (); ++i) {
					Str255		name;
					(*i)->GetName (name);
					if (::RelString (name, inName, false, true)) continue;
					
					GetPhotoItemModel (*i, outToken);
					} // for
				
				ThrowOSErr_(errAENoSuchObject);				
				break;
				
			case cImportClass:
				SignalString_("GetSubModelByName for import class!");
				break;
				
			default:
				LModelObject::GetSubModelByName (inModelID, inName, outToken);
				break;
			} // switch

	} // end GetSubModelByName

// ---------------------------------------------------------------------------
//	¥ HandleCreateElementEvent
// ---------------------------------------------------------------------------
//	Respond to a Create Element AppleEvent ("make new" in AppleScript).
//
//	The parameters specify the Class ID for the new element, and where to
//	insert the new element in relation to a target object. Also, the
//	AppleEvent record may contain additional parameters that specify
//	initial values for the new element.
//
//	Subclasses which have SubModels which can be dynamically created should
//	override this function. Return a pointer to the newly created SubModel.
//	The calling function takes care of putting an object specifier for
//	this new SubModel in the AppleEvent reply.

LModelObject*
PhotoPrintDoc::HandleCreateElementEvent (

	DescType			inElemClass,
	DescType			inInsertPosition,
	LModelObject*		inTargetObject,
	const AppleEvent&	inAppleEvent,
	AppleEvent&			outAEReply)

	{ // begin HandleCreateElementEvent
	
		switch (inElemClass) {
			case PhotoItemModelObject::cClass:
				return HandleCreatePhotoItemEvent (inElemClass, inInsertPosition, inTargetObject, inAppleEvent, outAEReply);
			
			case cImportClass:
				return HandleCreateImportEvent (inElemClass, inInsertPosition, inTargetObject, inAppleEvent, outAEReply);
			
			default:
				return LSingleDoc::HandleCreateElementEvent (inElemClass, inInsertPosition, inTargetObject, inAppleEvent, outAEReply);
			} // switch
		
	} // end HandleCreateElementEvent

// ---------------------------------------------------------------------------
//	¥ HandleCreateImportEvent
// ---------------------------------------------------------------------------
//	Weird kludge to use PP's unfactored location parsing:
//		make new import at <location> with data <file list>

LModelObject*
PhotoPrintDoc::HandleCreateImportEvent (

	DescType			/*inElemClass*/,
	DescType			inInsertPosition,
	LModelObject*		inTargetObject,
	const AppleEvent&	inAppleEvent,
	AppleEvent&			outAEReply)

	{ // begin HandleCreateImportEvent
		
		//	What we will return
		LModelObject*			result = this;
		
		//	Message set
		EUserMessage::TextRef	resultText = new MNewHandle (Size (0));

		// find position in list
		PhotoPrintModel*		model = this->GetModel ();
		PhotoPrintView*			view (this->GetView());
		Layout*					layout (view->GetLayout());

		// Deselect, so we can select new ones
		view->ClearSelection();							
		
		//	Expand the file list
		MAEList					props;
		{
			StAEDescriptor		inList;
			inList.GetParamDesc (inAppleEvent, keyAEData, typeAEList);
			
			MAEDescIterator		end (inList);
			for (MAEDescIterator i (end); ++i != end; ) {
				FSSpec			fss;
				*i >> fss;
				MakeNewAEFileItem (props, fss);
				} // for
		}

		//	Walk the expanded list, adding new items
		MAEDescIterator		end (props);
		for (MAEDescIterator i (end); ++i != end; ) {
			FSSpec				fss;
			*i >> fss;

			//	Where does it go?
			PhotoIterator		targetIterator = model->end ();
			switch (inInsertPosition) {
				case kAEBeginning:
					targetIterator = model->begin ();
					break;

				case kAEEnd:
					targetIterator = model->end ();
					break;

				case kAEBefore:
				case kAEReplace:
					targetIterator = model->begin () + (this->GetPositionOfSubModel(inTargetObject->GetModelKind (), inTargetObject) - 1);
					break;

				case kAEAfter:
					targetIterator = model->begin () + this->GetPositionOfSubModel(inTargetObject->GetModelKind (), inTargetObject);
					break;
				} // switch
			
			PhotoItemRef		newItem = 0;
			
			try {
				//	Make the new item
				newItem = new PhotoPrintItem (fss);
				} // try
				
			catch (const LException& e) {
				delete newItem;
				newItem = 0;
				
				LStr255		errCode;
				LStr255		errText;
				LStr255		errFile (fss.name);
				ExceptionHandler::GetErrorAndDescription(e, errCode, errText);
				if (0 == errFile.Length ()) errFile = e.GetErrorString();
				
				*resultText += *EUserMessage::SetParamText (TEXT_ImportFailure, errFile, errText, errCode);
				continue;
				} // catch

			view->SetupDraggedItem (newItem);
			targetIterator = layout->AddItem (newItem, targetIterator);
						
			//	Delete the next item if we were replacing
			if (inInsertPosition == kAEReplace)	{
				view->RemoveFromSelection (targetIterator + 1, targetIterator + 2);
				model->RemoveItems (targetIterator + 1, targetIterator + 2);
				} // if
			}

		// Now that we have all the files imported, we can do layout
		if (layout->GetDistinctImages() > 1)
			model->Sort();
		
		// Doc orientation may change, so refresh before AND after
		view->Refresh();								
		layout->LayoutImages();
		view->Refresh();
		
		// Menu may change due to drag
		LCommander::SetUpdateCommandStatus(true);		
		
		//	Return any messages
		if (resultText->GetSize ()) {
			resultText->Lock ();
			::AEPutKeyPtr (&outAEReply, keyAEResultInfo, typeText, **resultText, resultText->GetSize ());
			} // if

		return this;	//	!

	} // end HandleCreateImportEvent

// ---------------------------------------------------------------------------
//	¥ HandleCreatePhotoItemEvent
// ---------------------------------------------------------------------------

LModelObject*
PhotoPrintDoc::HandleCreatePhotoItemEvent (

	DescType			inElemClass,
	DescType			inInsertPosition,
	LModelObject*		inTargetObject,
	const AppleEvent&	inAppleEvent,
	AppleEvent&			/*outAEReply*/)

	{ // begin HandleCreatePhotoItemEvent
	
		LModelObject	*result = nil;
		
		// find position in list
		PhotoPrintModel*	model = this->GetModel ();
		PhotoPrintView*		view (this->GetView());
		Layout*				layout (view->GetLayout());

		//	Where does it go?
		PhotoIterator		targetIterator = model->end ();
		switch (inInsertPosition) {
			case kAEBeginning:
				targetIterator = model->begin ();
				break;

			case kAEEnd:
				targetIterator = model->end ();
				break;

			case kAEBefore:
			case kAEReplace:
				targetIterator = model->begin () + (this->GetPositionOfSubModel(inTargetObject->GetModelKind (), inTargetObject) - 1);
				break;

			case kAEAfter:
				targetIterator = model->begin () + this->GetPositionOfSubModel(inTargetObject->GetModelKind (), inTargetObject);
				break;
			} // switch
			
		//	Get the data
		StAEDescriptor	inData;
		inData.GetOptionalParamDesc (inAppleEvent, keyAEData, typeUInt32);
		
		//	Make the new item
		PhotoItemRef		newItem = 0;
		try {
			if (inData.IsNotNull ()) {
				//	We use the data parameter to hold a pointer to the object being cloned
				//	This is a speed enhancement
				UInt32		dataItem;
				inData >> dataItem;
				newItem	= new PhotoPrintItem (*reinterpret_cast<PhotoPrintItem*> (dataItem));
				} // if
				
			else newItem = new PhotoPrintItem;
			
			//	Get the properties
			StAEDescriptor	inProps;
			inProps.GetOptionalParamDesc (inAppleEvent, keyAEPropData, typeAERecord);
		
			// add properties 
			if (inProps.IsNotNull ()) {
				StAEDescriptor			ignore;
				PhotoItemModelObject	pimo (0, newItem);
				pimo.SetAEProperty (pProperties, inProps, ignore);
				} // if

			view->SetupDraggedItem (newItem);
			targetIterator = layout->AddItem (newItem, targetIterator);
			this->SetDirty (true);
						
			//	Delete the next item if we were replacing
			if (inInsertPosition == kAEReplace)	{
				view->RemoveFromSelection (targetIterator + 1, targetIterator + 2);
				model->RemoveItems (targetIterator + 1, targetIterator + 2);
				} // if
			
			//	Return the last model
			SInt32				targetPosition = 1 + (targetIterator - model->begin ());
			StAEDescriptor		token;
			this->GetSubModelByPosition (inElemClass, targetPosition, token);
			
			result = GetModelFromToken (token);
			} // try
			
		catch (const LException& e) {
			delete newItem;
			throw;
			} // catch

		// Now that we have the, we can do layout
		if (layout->GetDistinctImages() > 1)
			model->Sort();
		
		// Doc orientation may change, so refresh before AND after
		view->Refresh();								
		layout->LayoutImages();
		view->Refresh();
		
		// Menu may change due to drag
		LCommander::SetUpdateCommandStatus(true);		

		return result;

	} // end HandleCreatePhotoItemEvent


//-----------------------------------------------------------------
// HandleKeyPress {OVERRIDE}
//-----------------------------------------------------------------
 Boolean			
 PhotoPrintDoc::HandleKeyPress			(const EventRecord&	inKeyEvent)
 {
	Boolean		enabled;
	Boolean		usesMark;
	UInt16		mark;
	Str255		name;

	Boolean		keyHandled	 = false;
	UInt16		theChar		 = (UInt16) (inKeyEvent.message & charCodeMask);
	if (theChar == char_Backspace || theChar == char_Clear &&
			(inKeyEvent.message & keyCodeMask) == vkey_Clear) {
		this->ProcessCommandStatus(cmd_Clear, enabled, usesMark, mark, name);
		if (enabled)
			this->ProcessCommand(cmd_Clear);
		keyHandled = true;
	} else if (inKeyEvent.modifiers & cmdKey && theChar == '=') {
		// ??? this should maybe be done as a resource (or even something with an attachment
		// which manages a list of mappings)
		this->ProcessCommandStatus(cmd_ZoomIn, enabled, usesMark, mark, name);
		if (enabled)
			this->ProcessCommand(cmd_ZoomIn);
		keyHandled = true;
	} else
		keyHandled = LSingleDoc::HandleKeyPress(inKeyEvent);
	
	return keyHandled;
 }//end HandleKeyPress


// ---------------------------------------------------------------------------
// HandlePageSetup {OVERRIDE}
// ---------------------------------------------------------------------------
void
PhotoPrintDoc::HandlePageSetup()
{
	StDesktopDeactivator	deactivator;
	
	ForceNewPrintSession();
	if (UPrinting::AskPageSetup(*GetPrintRec())) {
		// force a flattening of the page format so that we can save it
		Handle				orphan;
		::PMFlattenPageFormat(GetPrintRec()->GetPageFormat(), &orphan);
		PhotoPrintApp::gFlatPageFormat = new MNewHandle (orphan);

		// When the user chooses Page Setup, we assume they are trying to force an orientation (even if
		// they may actually be doing something else). It'd be pretty confusing to them if we didn't
		// respect their choice in the dialog. (Yeah, they might not have made a choice...)
		HORef<EPrintSpec>&	spec = this->GetPrintRec();
		PrintProperties&	props = this->GetPrintProperties();
		if (spec->GetOrientation() == kLandscape)
			props.SetRotationBehavior(PrintProperties::kForceLandscape);
		else
			props.SetRotationBehavior(PrintProperties::kForcePortrait);

		this->MatchViewToPrintRec();
		this->GetView()->GetLayout()->LayoutImages();
		this->GetWindow()->Refresh();

		// Fix the popup menu
		this->MatchPopupsToPrintRec();
		
		PhotoPrinter::SetCurPrinterCreator(GetPrintRec()->GetCreator());

	}//endif successful setup (assume something changed)
} // HandlePageSetup

/*
HandlePrint {OVERRIDE}
	Handle the "print" command
*/
void			
PhotoPrintDoc::HandlePrint(void)
{
	// first thing:  warn about alternate printing.  if they want to abort here, let them
	bool goAhead = WarnAboutAlternate(GetPrintRec()->GetCreator());	
	if (!goAhead)
		return;

	StDesktopDeactivator		deactivator;
	UCursor::SetWatch();					// Be sure to do this after calling UDesktop::Deactivate!
	if (PhotoPrintApp::gPalette != nil)
		PhotoPrintApp::gPalette->Hide();	// Deactivating doesn't hide our floater!
	if (PhotoPrintApp::gTools != nil)
		PhotoPrintApp::gTools->Hide();		// Deactivating doesn't hide our floater!

	PhotoPrinter::SetupPrintRecordToMatchProperties(this->GetPrintRec(), &mPrintProperties);
	
	UCursor::SetArrow();					// Since dialog won't switch it back
	bool						printIt = UPrinting::AskPrintJob(*this->GetPrintRec());

	UCursor::SetWatch();					// Be sure to do this after calling UDesktop::Deactivate!

// enable this line only if using print sheets in GetPrintRec()
//	if (!PhotoPrintApp::gOSX)
		this->FinishHandlePrint(printIt);
}//end HandlePrint


void
PhotoPrintDoc::FinishHandlePrint(bool printIt) {

	if (printIt) {
		this->SendSelfAE(kCoreEventClass, kAEPrint, ExecuteAE_No);
		this->DoPrint();
	}

	// ??? Is there a window hiding class?
	if (PhotoPrintApp::gPalette != nil)
		PhotoPrintApp::gPalette->Show();
	if (PhotoPrintApp::gTools != nil)
		PhotoPrintApp::gTools->Show();
} // HandlePrint

//-----------------------------------------------------------------
//	HandlePrintPreview
//-----------------------------------------------------------------
void			
PhotoPrintDoc::HandlePrintPreview(void)
{
} // HandlePrintPreview

//-----------------------------------------------------------------
//Initialize
//-----------------------------------------------------------------
void
PhotoPrintDoc::Initialize()
{
	mSheetDoneUPP = ::NewPMSheetDoneUPP(PMSheetDoneProc);
	this->AddCommands();
	this->AddEvents();

	this->AddAttachment(new LUndoer);

	// We need to keep track of the current document so we can send it an Apple Event.  We also do this in
	// PhotoPrintView::ActivateSelf, but that may not be called in time if there's a brand new document.
	PhotoPrintApp::gSingleton->SetDefaultSubModel(this);
}//end Initialize


/*
IsModified {OVERRIDE}
*/
Boolean	
PhotoPrintDoc::IsModified()
{
	Boolean 		modified = this->GetProperties().GetDirty();

	if (gWindowProxies) {
		// This is a reasonable place to update the proxy icon
		::SetWindowModified(mWindow->GetMacWindow(), modified || !mIsSpecified);
	}

	return modified;
} // IsModified

/*
JamDuplicated
	Sets the value of the duplicated popup without sending any messages
*/
void
PhotoPrintDoc::JamDuplicated(const SInt16 inValue)
{
	StDisableBroadcaster	disable (mDupPopup);
	mDupPopup->SetCurrentMenuItem(inValue);
} // JamDuplicated

/*
ListenToCommand
	We'll be getting messages from the bevel buttons
*/
void
PhotoPrintDoc::ListenToCommand(
	MessageT	inMessage,
	void		*ioParam)
{
	SInt32			theValue;
	SInt32			theUndoIndex;
	
	switch (inMessage) {
		case msg_MaximumSize:
			theUndoIndex = si_ChangeMaxSize;
			break;
			
		case msg_MinimumSize:
			theUndoIndex = si_ChangeMinSize;
			break;
			
		case 'orie':
			theUndoIndex = si_ChangeOrientation;
			break;
		
		default:
			Assert_(false);
	} // switch
	
	EPostAction		theAction (this);
	
	try {theAction = new DocumentAction (this, theUndoIndex);} catch (...) {}
	
	switch (inMessage) {
		case msg_MaximumSize:
			theValue = *(SInt32*)ioParam;
			SetMaximumSize ((SizeLimitT) theValue);
			break;

		case msg_MinimumSize:
			theValue = *(SInt32*)ioParam;
			SetMinimumSize ((SizeLimitT) theValue);
			break;

		case 'orie': 
			theValue = *(SInt32*)ioParam;
			SetOrientation ((Orientation) theValue);
			break;
	} // switch

	this->SetDirty(true);
} // ListenToCommand

/*
ListenToMessage {OVERRIDE}
*/
void
PhotoPrintDoc::ListenToMessage(
	MessageT	inMessage,
	void		*ioParam)
{	
	switch (inMessage) {
		case msg_MaximumSize:
		case msg_MinimumSize:
		case 'orie':
			ListenToCommand (inMessage, ioParam);
			break;
		
		case PhotoPrintModel::msg_ModelItemsAdded: 
			OnModelItemsAdded ((PhotoPrintModel::MessageRange*) ioParam);
			break;	
		
		case PhotoPrintModel::msg_ModelItemsChanged: 
			OnModelItemsChanged ((PhotoPrintModel::MessageRange*) ioParam);
			break;	
		
		case PhotoPrintModel::msg_ModelItemsRemoved: 
			OnModelItemsRemoved ((PhotoPrintModel::MessageRange*) ioParam);
			break;
	} // switch
	
} // ListenToMessage

// ---------------------------------------------------------------------------------
//	¥ MakeNewAEFileItem											[static]
// ---------------------------------------------------------------------------------

void
PhotoPrintDoc::MakeNewAEFileItem (
	
	MAEList&			outList,
	const MFileSpec&	inSpec)

	{ // begin MakeNewAEFileItem
		
		//	First, resolve it
		MFileSpec	resolvedSpec (inSpec);
		Boolean		targetIsFolder;
		Boolean		wasAliased;
		resolvedSpec.ResolveAlias (targetIsFolder, wasAliased);
		
		//	If it is a folder, recurse
		if (inSpec.IsFolder ()) {
			MakeNewAEFolderItem (outList, resolvedSpec);
			return;
			} // if
			
		//	Add file item
		const	FSSpec&	spec (resolvedSpec);
		outList.PutPtr (typeFSS, &spec, sizeof (spec));
	
	} // end MakeNewAEFileItem

// ---------------------------------------------------------------------------------
//	¥ MakeNewAEFolderItem											[static]
// ---------------------------------------------------------------------------------

void
PhotoPrintDoc::MakeNewAEFolderItem (
	
	MAEList&			outList,
	const MFileSpec&	inSpec)

	{ // begin MakeNewAEFolderItem

		MFolderIterator 	end (inSpec.Volume(), inSpec.GetDirID());
		for (MFolderIterator fi (end); ++fi != end;) {
			if (!fi.IsVisible()) continue;
			
			MFileSpec 	spec (fi.Name(), fi.Directory(), fi.Volume());
			MakeNewAEFileItem (outList, spec);
			} // for
	
	} // end MakeNewAEFolderItem

/*
MatchPopupsToPrintRec
*/
void
PhotoPrintDoc::MatchPopupsToPrintRec()
{
	// Fix the popup menu
	if (mOrientationPopup != nil) {
		ResIDT				icon;
		SInt32				index;
		HORef<EPrintSpec>&	spec = this->GetPrintRec();
		if (spec->GetOrientation() == kLandscape) {
			icon = cicn_FlexibleLandscape;
			index = kLandscapeIndex;
		} else {
			icon = cicn_FlexiblePortrait;
			index = kPortraitIndex;
		}
		// Match the Flexible icon to this
		MenuRef				theMenu = mOrientationPopup->GetMacMenuH();
		::SetItemIcon(theMenu, kFlexibleIndex, icon);

		PrintProperties&	props = this->GetPrintProperties();
		if (props.GetRotationBehavior() == PrintProperties::kPickBestRotation) {
			index = kFlexibleIndex;
		}
		StDisableBroadcaster	disable (mOrientationPopup);
		mOrientationPopup->SetCurrentMenuItem(index);
	}
} // MatchPopupsToPrintRec

/*
MatchViewToPrintRec
*/
void
PhotoPrintDoc::MatchViewToPrintRec(SInt16 inPageCount)
{
	this->UpdatePageNumber(inPageCount);

	// base our size on the current page's size
	MRect		pageBounds;
	PhotoPrinter::CalculatePrintableRect(GetPrintRec(), &GetPrintProperties(), 
										 pageBounds, GetResolution());
		
	// multiply by number of pages required (most cases == 1)
	// additional pages always go down!
	pageBounds.SetHeight(pageBounds.Height() * GetPageCount());
	
	mHeight = (double)pageBounds.Height() / this->GetResolution();
	mWidth = (double)pageBounds.Width() / this->GetResolution();

	//clamp to the lower hundredth
	// to be at same resolution as PrintManager (stops xtra empty single-pixel pages)
	mHeight = floor(mHeight * 100.0) / 100.0;
	mWidth = floor(mWidth * 100.0) / 100.0;	

	// we need to show the entire page on screen, including unprintable area
	MRect paperBounds;
	PhotoPrinter::CalculatePaperRect(GetPrintRec(), &GetPrintProperties(), 
										 paperBounds, GetResolution());
	paperBounds.SetHeight(paperBounds.Height() * GetPageCount());									 
	mScreenView->ResizeFrameTo(paperBounds.Width(), paperBounds.Height(), Refresh_No);
	mScreenView->ResizeImageTo(paperBounds.Width(), paperBounds.Height(), Refresh_No);

	// Since the background is what sits inside the LScrollerView, we need to change
	// its size as well
	LView*		background = dynamic_cast<LView*>(mWindow->FindPaneByID(pane_Background));
	background->ResizeImageTo(paperBounds.Width(), paperBounds.Height(), Refresh_Yes);

	MRect		body;
	PhotoPrinter::CalculateBodyRect(GetPrintRec(), &GetPrintProperties(), 
									 body, GetResolution());
	mPageHeight = body.Height() / (double)GetResolution();
	mPageHeight = floor(mPageHeight * 100.0) / 100.0;

	// This is a pretty good place to also make sure the Orientation popup matches
	this->MatchPopupsToPrintRec();
}//end MatchViewToPrintRec

/*
ObeyCommand {OVERRIDE}
*/
Boolean
PhotoPrintDoc::ObeyCommand(
	CommandT	inCommand,
	void*		ioParam)
{
	Boolean		cmdHandled = true;	// Assume we'll handle the command

	switch (inCommand) {
		case tool_Arrow:
		case tool_Crop:
		case tool_Rotate:
		case tool_Zoom:
		case tool_Name:
			this->SetController(inCommand);
			break;

		default:
			cmdHandled = LSingleDoc::ObeyCommand(inCommand, ioParam);
			break;
	}

	return cmdHandled;
}

//------------------------------------
// I/O		based on xmlio library
//------------------------------------
#include "MP2CStr.h"


/*
OnModelItemsAdded
*/
void
PhotoPrintDoc::OnModelItemsAdded(
	PhotoPrintModel::MessageRange*	/*inRange*/)
{
	SetDirty (true);
} // OnModelItemsAdded

/*
OnModelItemsChanged
*/
void
PhotoPrintDoc::OnModelItemsChanged(
	PhotoPrintModel::MessageRange*	/*inRange*/)
{
	SetDirty (true);
} // OnModelItemsChanged

/*
OnModelItemsRemoved
*/
void
PhotoPrintDoc::OnModelItemsRemoved(
	PhotoPrintModel::MessageRange*	/*inRange*/)
{
	SetDirty (true);
} // OnModelItemsRemoved

/*
ParseLayout [static]
*/
void
PhotoPrintDoc::ParseLayout(XML::Element &elem, void *userData)
{
	OSType* pLayout ((OSType*)userData);

	XML::Char tmp[64];
	size_t len = elem.ReadData(tmp, sizeof(tmp));
	tmp[len] = 0;
	
	*pLayout = LayoutMapper::Lookup(tmp);	
} // ParseLayout

/*
Read
*/
void PhotoPrintDoc::Read(XML::Element &elem)
{
	double	minVal (0.0);
	double	maxVal (200000.0);
	Layout::LayoutType	layoutType;
	UInt16	imageCount = 1;

	XML::Handler handlers[] = {
		XML::Handler("Document_Properties", DocumentProperties::ParseProperties, (void*)&mProperties),
		XML::Handler("Print_Properties", PrintProperties::sParseProperties, (void*)&mPrintProperties),
		XML::Handler("Objects", sParseObjects),
		XML::Handler("width", &mWidth, &minVal, &maxVal),
		XML::Handler("height", &mHeight, &minVal, &maxVal),
		XML::Handler("dpi", &mDPI),
		XML::Handler("layout", ParseLayout, &layoutType),
		XML::Handler("image_count", &imageCount),
		XML::Handler::END
	};
		
	elem.Process(handlers, this);

	// Set the orientation to match the implicitly saved one
	EPrintSpec*		spec = this->GetPrintRec();
	OSType			orientation;
	if (mWidth > mHeight)
		orientation = kLandscape;
	else
		orientation = kPortrait;

	spec->SetOrientation(orientation, PhotoUtility::gNeedDoubleOrientationSetting);
	this->MatchViewToPrintRec(mNumPages);

	// 206.  Call SwitchLayout with full info
	this->GetView()->SwitchLayout(layoutType, imageCount);
} // Read

/*
SetController
*/
void 
PhotoPrintDoc::SetController(OSType inNewController) {
	this->GetView()->SetController(inNewController, mWindow);

	LPane*		group = mWindow->FindPaneByID('layG');
	if (inNewController == tool_Name) {
		group->Disable();
	} else {
		group->Enable();
	}
} // SetController


void
PhotoPrintDoc::SetDirty(bool inState) {
	GetProperties().SetDirty(inState);
	
	BroadcastMessage(msg_ModelChanged, NULL);
	}//end SetDirty




//-----------------------------------------------------------------
//SetDisplayCenter
//-----------------------------------------------------------------
void			
PhotoPrintDoc::SetDisplayCenter			(

	double		h,
	double		v,
	Boolean		inRefresh)
{
	
	LView*	background = dynamic_cast<LView*>(mWindow->FindPaneByID(pane_Background));
	Assert_(background);

	SDimension16	extents;
	background->GetFrameSize (extents);
	
	SPoint32		midPos = {h * mDPI, v * mDPI};
	midPos.h -= extents.width / 2;
	midPos.v -= extents.height / 2;
	
	background->ScrollPinnedImageTo (midPos.h, midPos.v, inRefresh);

}//end SetDisplayCenter

/*
SetMaximumSize
*/
void 
PhotoPrintDoc::SetMaximumSize(SizeLimitT inMax) {

	if (inMax == GetMaximumSize ()) return;
	
	StDisableBroadcaster	disable (mMaxPopup);
	mMaxPopup->SetValue (mMaximumSize = inMax);

	mScreenView->Refresh();
	mScreenView->GetLayout()->LayoutImages();
	mScreenView->Refresh();
	this->FixPopups();

	this->SetDirty(true);
	
} // SetMaximumSize

/*
SetMinimumSize
*/
void 
PhotoPrintDoc::SetMinimumSize(SizeLimitT inMin) {

	if (inMin == GetMinimumSize ()) return;
	
	StDisableBroadcaster	disable (mMinPopup);
	mMinPopup->SetValue (mMinimumSize = inMin);

	mScreenView->Refresh();
	mScreenView->GetLayout()->LayoutImages();
	mScreenView->Refresh();
	this->FixPopups();

	this->SetDirty(true);

} // SetMinimum

/*
SetOrientation
*/
void 
PhotoPrintDoc::SetOrientation(Orientation inOrient) {

	if (inOrient == GetOrientation ()) return;
	
	PrintProperties&	props = this->GetPrintProperties();
	switch (inOrient) {
		case kFlexibleIndex:
			props.SetRotationBehavior(PrintProperties::kPickBestRotation);
			break;
			
		case kLandscapeIndex:
			props.SetRotationBehavior(PrintProperties::kForceLandscape);
			break;
			
		default:
			props.SetRotationBehavior(PrintProperties::kForcePortrait);
			break;
	} // switch

	StDisableBroadcaster	disable (mMinPopup);
	mOrientationPopup->SetCurrentMenuItem (inOrient);

	mScreenView->Refresh();
	mScreenView->GetLayout()->LayoutImages();
	mScreenView->Refresh();

	this->SetDirty(true);

} // SetOrientation


/*
SetPrintProperties
*/
void 
PhotoPrintDoc::SetPrintProperties(const PrintProperties& inProps) {

	PrintProperties&	props = this->GetPrintProperties();

	if (inProps == props) return;
	
	props = inProps;

	mScreenView->Refresh();
	mScreenView->GetLayout()->LayoutImages();
	mScreenView->Refresh();
	this->FixPopups();

	this->SetDirty(true);

} // SetPrintProperties

//-----------------------------------------------------------------
//SetResolution
//-----------------------------------------------------------------
void
PhotoPrintDoc::SetResolution(SInt16 inRes)
{
	if (inRes != mDPI) {
		MRect screenViewFrame;
		double scalar (inRes / (double)mDPI);
		// height and width of document in inches should not change, just change screen view size
		screenViewFrame.SetWidth(mWidth * inRes);
		screenViewFrame.SetHeight(mHeight * inRes);		
		mDPI = inRes;
		mScreenView->Refresh(); // inval the current extents (for shrinking)
		mScreenView->ResizeImageTo(screenViewFrame.Width() , screenViewFrame.Height(), Refresh_Yes);
		LView*	background = dynamic_cast<LView*>(mWindow->FindPaneByID('back'));
		background->ResizeImageTo(screenViewFrame.Width(), screenViewFrame.Height(), Refresh_Yes);
		GetView()->GetLayout()->LayoutImages();
		GetView()->Refresh(); // inval the new extents (for enlarging)
		
		this->UpdateZoom();

		SetUpdateCommandStatus(true);		// 125 Be sure that menus enable correctly
	}//endif need to change
}//end SetResolution


/*
sDocHandler
*/
void
PhotoPrintDoc::sDocHandler(XML::Element &elem, void* userData) {
	
	PhotoPrintDoc* pDoc = (PhotoPrintDoc*)userData;
	pDoc->Read(elem);
}//end sDocHandler


void
PhotoPrintDoc::sParseObject(XML::Element &elem, void *userData)
{
	PhotoPrintDoc *doc = static_cast<PhotoPrintDoc *> (userData);
	PhotoItemRef item (nil);
	if (strcmp(elem.GetName(), "photo") == 0) {
		item = new PhotoPrintItem;
		item->Read(elem);
		
		PhotoPrintModel*	model = doc->GetView()->GetModel();
		model->AdoptNewItem(item, model->end ());	
		}//endif found one
	}//end sParseObject

/*
sParseObjects
	This function handles the "Objects" tag in our XML file, which represents a collection
	of images
*/
void
PhotoPrintDoc::sParseObjects(XML::Element &elem, void *userData)
{
	PhotoPrintDoc *doc = static_cast<PhotoPrintDoc *> (userData);

	XML::Handler handlers[] = {
		XML::Handler(sParseObject),
		XML::Handler::END
		};
		
	elem.Process(handlers, userData);
}//end sParseObjects


/*
UpdatePageNumber
*/
void
PhotoPrintDoc::UpdatePageNumber(const SInt16 inPageCount)
{
	mNumPages = inPageCount;

	// Keep the page count placard up to date
	SInt16		index;
	if (inPageCount == 1)
		index = si_SinglePage;
	else
		index = si_MultiplePages;
	MPString	theText(str_Page, index);
	if (inPageCount != 1) {
		MPString	curNumber(mScreenView->GetCurPage());		// !!! need to get this somehow
		MPString	maxNumber((long)inPageCount);
		theText.Replace(curNumber, "\p^1");
		theText.Replace(maxNumber, "\p^2");
	}
	mPageCount->SetDescriptor(theText);
} // UpdatePageNumber

/*
UpdatePreferences
	Called from constructor, and when user wants pref change to apply to open documents
*/
void PhotoPrintDoc::UpdatePreferences()
{
	PhotoPrintPrefs*	prefs = PhotoPrintPrefs::Singleton();

	mMinimumSize = prefs->GetMinimumSize();
	mMaximumSize = prefs->GetMaximumSize();

	// Be sure the UI matches
	if (mMinPopup != nil) {
		StDisableBroadcaster	disable (mMinPopup);
		mMinPopup->SetValue(mMinimumSize);
	}
	if (mMaxPopup != nil) {
		StDisableBroadcaster	disable (mMaxPopup);
		mMaxPopup->SetValue(mMaximumSize);
	}
	this->FixPopups();
} // UpdatePreferences

/*
UpdateZoom
	Be sure the placard showing % zoom is correct
*/
void
PhotoPrintDoc::UpdateZoom()
{
	// Update the placard
	double		zoom = (double)mDPI / (double)kDPI;
	SInt16		index;
	if (zoom < 0.20) {
		index = si_Precise;		// Use more precision
	} else {
		index = si_Normal;
	}

	MNumberParts		parts;
	MNumFormatString	format(LStr255(str_Zoom, index), parts);
	extended80			x80;

	Str31				zoomText;
	format.ExtendedToString (format.DoubleToExtended(zoom, x80), parts, zoomText);	

	mZoomDisplay->SetDescriptor(zoomText);
} // UpdateZoom

bool
PhotoPrintDoc::WarnAboutAlternate(OSType inPrinterCreator) {
	bool bHappy (true);

	do {
		PhotoPrintPrefs*	prefs = PhotoPrintPrefs::Singleton();
		
		// if alternate is on, we're done
		if (prefs->GetAlternatePrinting())
			continue;

		// if they don't want to be warned, we done
		if (!prefs->GetWarnAlternate())
			continue;
		
		// see if the printer demands a warning
		StResource	alternatePrinterList ('CRE#', 128);
		OSType** creatorList = reinterpret_cast<OSType**> ((Handle)(alternatePrinterList));
		OSType*	firstCreator = *creatorList;
		OSType* lastCreator = firstCreator + ::GetHandleSize((Handle)alternatePrinterList) / sizeof(*firstCreator);
		OSType* found = std::find(firstCreator, lastCreator, inPrinterCreator);

		if (found != lastCreator) {
			MNewDialog dlog (dlog_WarnAboutAlternate);
			WindowRef dlogWindow (dlog.GetWindowRef());
			::ShowWindow(dlogWindow);
			::BringToFront(dlogWindow);
			
			dlog.SetDefaultItem();
			dlog.SetCancelItem();
			bool done (false);
			short item;
			short state;
			do {
				dlog.DoModal(item);
				switch (item) {
					case kWarnOK:
						done = true;
						break;
					case kWarnCancel:
						bHappy = false;
						done = true;
						break;
					case kWarnUseAlternate:
						MDialogItem(dlog, kWarnUseAlternate) >> state;
						MDialogItem(dlog, kWarnUseAlternate) << !state;
						prefs->SetAlternatePrinting(!state);					
						break;
					case kWarnDontShowAgain:
						MDialogItem(dlog, kWarnDontShowAgain) >> state;
						MDialogItem(dlog, kWarnDontShowAgain) << !state;
						prefs->SetWarnAlternate(state);
						break;
						
					}//end switch
				} while (!done);
				
			
			::HideWindow(dlogWindow);
			::SendBehind(dlogWindow, (WindowRef)0);
			}//endif found it in the list
		} while (false);
	return bHappy;
}//end WarnAboutAlternate



void PhotoPrintDoc::Write(XML::Output &out, bool isTemplate) 
{
	out.BeginDocument();
	out.writeLine("");

	out.BeginElementAttrs("Document");
	Str255 bogus;
	MPString title (GetDescriptor(bogus));
	out.WriteAttr("name", MP2CStr (title));
	out.EndAttrs();

	out.WriteElement("layout", 	LayoutMapper::Find(GetView()->GetLayout()->GetType()));
	out.WriteElement("image_count", GetView()->GetLayout()->GetImageCount());

	out.WriteElement("width", mWidth);
	out.WriteElement("height", mHeight);
	out.WriteElement("dpi", mDPI);

	out.BeginElement("Document_Properties");
	mProperties.Write(out);
	out.EndElement();//doc properties
	out.writeLine("");

	out.BeginElement("Print_Properties");
	mPrintProperties.Write(out);
	out.EndElement();//print properties
	out.writeLine("");

	// write objects
	out.BeginElement("Objects");
	PhotoPrintView*	view = GetView();
	PhotoPrintModel* model (view->GetModel());
	for (PhotoIterator it = model->begin(); it != model->end(); ++it)
	{
		const PhotoItemRef item = (*it);
		out.BeginElement("photo");
			(*it)->Write(out, isTemplate);
		out.EndElement();
	}
	out.EndElement();	// Objects

	out.EndElement();	// Document

	out.EndDocument();
}

#pragma mark -

/*
PMSheetDoneProc
	Callback from print manager when sheet is closed
*/
pascal void 
PhotoPrintDoc::PMSheetDoneProc(PMPrintSession /*inSession*/,
								 WindowRef		inDocWindow,
								 Boolean		accepted)
{	
	LWindow* pWindow = LWindow::FetchWindowObject(inDocWindow);
	if (pWindow) {
		LCommander* theSuper = pWindow->GetSuperCommander();		
		PhotoPrintDoc* pDoc = dynamic_cast<PhotoPrintDoc*>(theSuper);
		if (pDoc != nil)
			pDoc->FinishHandlePrint(accepted);
		}//endif found the window
}//end PMSheetDoneProc									 
