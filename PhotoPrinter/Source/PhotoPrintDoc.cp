/*
	File:		PhotoPrintDoc.cp

	Contains:	Implementation of the document (which manages a window and file).

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

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
#include "ImageActions.h"
#include "ImageOptions.h"
#include "ImportCommand.h"
#include "Layout.h"
#include "LayoutCommand.h"
#include "MakeIconCommand.h"
#include "PhotoPrintCommands.h"
#include "PhotoPrintEvents.h"
#include "PhotoPrintResources.h"
#include "PrintCommand.h"
#include "PhotoPrintApp.h"
#include "PhotoPrinter.h"
#include "PhotoPrintView.h"
#include "PhotoUtility.h"
#include "PhotoWindow.h"
#include "RevealCommand.h"
#include "RemoveCropCommand.h"
#include "RemoveRotationCommand.h"
#include "SaveCommand.h"
#include "SelectAllCommand.h"
#include "ZoomCommands.h"

// Toolbox++
#include "MAEList.h"
#include "MAEDesc.h"
#include "MAppleEvent.h"
#include "MNavDialogOptions.h"
#include "MNavPutFile.h"
#include "MNavReplyRecord.h"
#include "MNumberParts.h"
#include "MPString.h"
#include "MAlert.h"

#include "xmloutput.h"
#include "xmlinput.h"
#include "xmlfile.h"

#include <iostream>
#include <map.h>

// Globals
PhotoPrintDoc*	PhotoPrintDoc::gCurDocument = nil;
bool			PhotoPrintDoc::gWindowProxies = false;

const ResIDT	alrt_XMLError = 	131;
const ResIDT 	PPob_PhotoPrintDocWindow = 1000;
const ResIDT 	prto_PhotoPrintPrintout = 1002;
const PaneIDT 	pane_ScreenView = 	'scrn';
const PaneIDT 	pane_Scroller = 	'scrl';
const PaneIDT	pane_ZoomDisplay = 	'zoom';
const PaneIDT	pane_PageCount = 	'page';

SInt16 PhotoPrintDoc::kFeelGoodMargin = 32;

//---------------------------------------------------------------
// support for the map between alignment type and text
typedef	map<OSType, char*, less<OSType> > LayoutMap;

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
	this->CreateWindow(PPob_PhotoPrintDocWindow, inVisible);
	this->Initialize();
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
	this->CreateWindow(PPob_PhotoPrintDocWindow, inVisible);

	this->DoOpen(inSpec);
	this->Initialize();
 }//end ct

//-----------------------------------------------------------------
//~PhotoPrintDoc
//-----------------------------------------------------------------
PhotoPrintDoc::~PhotoPrintDoc	(void)
{
}//end dt


//-----------------------------------------------------------------
//Initialize
//-----------------------------------------------------------------
void
PhotoPrintDoc::Initialize()
{
	this->AddCommands();
	this->AddEvents();

	this->AddAttachment(new LUndoer);
}//end Initialize


//-----------------------------------------------------------------
//AddCommands
//-----------------------------------------------------------------
void					
PhotoPrintDoc::AddCommands			(void)
{
	// File menu
	new ImportCommand(cmd_Import, this);
	new PrintCommand(cmd_Print, this);
	new SaveCommand(cmd_Save, this);
	new SaveCommand(cmd_SaveAs, this);

	// Edit menu
	new ClearCommand(cmd_Clear, this);
	new SelectAllCommand(cmd_SelectAll, this);

	// View Menu
	new BackgroundOptionsCommand(cmd_BackgroundOptions, this);
	new LayoutCommand(cmd_ReLayout, this);
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
}//end AddCommands

//-----------------------------------------------------------------
//AddEvents
//-----------------------------------------------------------------
void					
PhotoPrintDoc::AddEvents			(void) {
}//end AddEvents





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

	mScreenView = dynamic_cast<PhotoPrintView*>(mWindow->FindPaneByID(pane_ScreenView));	
	ThrowIfNil_(mScreenView);

	mScroller = dynamic_cast<LScrollerView*>(mWindow->FindPaneByID(pane_Scroller));
	ThrowIfNil_(mScroller);

	mPageCount = dynamic_cast<LPane*>(mWindow->FindPaneByID(pane_PageCount));
	ThrowIfNil_(mPageCount);
	
	mZoomDisplay = dynamic_cast<LPane*>(mWindow->FindPaneByID(pane_ZoomDisplay));
	ThrowIfNil_(mZoomDisplay);

	MRect bestStart;
	mWindow->DoSetZoom(true); // set to "standard" (zoom, though overridden) state
	
	if (inVisible)
		mWindow->Show();

	this->MatchViewToPrintRec();
}// end CreateWindow								 

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

	
	MRect		screenViewFrame;
	mScreenView->CalcPortFrameRect(screenViewFrame);
	mScreenView->ResizeFrameTo(pageBounds.Width(), pageBounds.Height(), Refresh_No);
	mScreenView->ResizeImageTo(pageBounds.Width(), pageBounds.Height(), Refresh_No);

	// Since the background is what sits inside the LScrollerView, we need to change
	// its size as well
	LView*		background = dynamic_cast<LView*>(mWindow->FindPaneByID('back'));
	background->ResizeImageTo(pageBounds.Width(), pageBounds.Height(), Refresh_Yes);

	MRect body;
	PhotoPrinter::CalculateBodyRect(GetPrintRec(), &GetPrintProperties(), 
									 body, GetResolution());
	mPageHeight = body.Height() / (double)GetResolution();
	

}//end MatchViewToPrintRec

#pragma mark -

//------------------------------------
// I/O		based on xmlio library
//------------------------------------
#include "MP2CStr.h"

void PhotoPrintDoc::Write(XML::Output &out) 
{
	out.BeginDocument();
	out.writeLine("");

	out.BeginElementAttrs("Document");
	Str255 bogus;
	MPString title (GetDescriptor(bogus));
	out.WriteAttr("name", MP2CStr (title));
	out.EndAttrs();

	out.WriteElement("layout", 	LayoutMapper::Find(GetView()->GetLayout()->GetType()));

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
			(*it)->Write(out);
		out.EndElement();
	}
	out.EndElement();	// Objects

	out.EndElement();	// Document

	out.EndDocument();
}

/*
Read
*/
void PhotoPrintDoc::Read(XML::Element &elem)
{
	double	minVal (0.0);
	double	maxVal (200000.0);
	OSType	type;

	XML::Handler handlers[] = {
		XML::Handler("Document_Properties", DocumentProperties::ParseProperties, (void*)&mProperties),
		XML::Handler("Print_Properties", PrintProperties::sParseProperties, (void*)&mPrintProperties),
		XML::Handler("Objects", sParseObjects),
		XML::Handler("width", &mWidth, &minVal, &maxVal),
		XML::Handler("height", &mHeight, &minVal, &maxVal),
		XML::Handler("dpi", &mDPI),
		XML::Handler("layout", ParseLayout, &type),
		XML::Handler::END
	};
		
	elem.Process(handlers, this);
	GetView()->SetLayoutType(type);
} // Read


void
PhotoPrintDoc::sParseObjects(XML::Element &elem, void *userData)
{
	PhotoPrintDoc*	doc = (PhotoPrintDoc*)userData;

	XML::Handler handlers[] = {
		XML::Handler(sParseObject),
		XML::Handler::END
		};
		
	elem.Process(handlers, userData);
	}//end sParseObjects


void
PhotoPrintDoc::sParseObject(XML::Element &elem, void *userData)
{
	PhotoPrintDoc *doc = (PhotoPrintDoc *)userData;
	PhotoItemRef item (nil);
	if (strcmp(elem.GetName(), "photo") == 0) {
		item = new PhotoPrintItem;
		item->Read(elem);
		doc->GetView()->GetModel()->AdoptNewItem(item);	
		}//endif found one
	}//end sParseObject

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

#pragma mark -

/*
HandleAppleEvent {OVERRIDE}
*/
void
PhotoPrintDoc::HandleAppleEvent(
	const AppleEvent	&inAppleEvent,
	AppleEvent			&outAEReply,
	AEDesc				&outResult,
	long				inAENumber)
{
	switch (inAENumber) {
		case ae_Import:
			this->GetView()->ReceiveDragEvent(MAppleEvent(inAppleEvent));
			break;

		default:
			LSingleDoc::HandleAppleEvent(inAppleEvent, outAEReply,
											outResult, inAENumber);
			break;
	}
} // HandleAppleEvent

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
			this->PostAction(new DeleteAction(this, si_DeleteImage));
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

#pragma mark -

//-----------------------------------------------------------------
//GetFileType
//-----------------------------------------------------------------
OSType			
PhotoPrintDoc::GetFileType			(void) const
{
	return mFileType;
}//end GetFileType

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
		GetDescriptor (options.savedFileName);
	
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
	return false;
}//end AskSaveChanges


//-----------------------------------------------------------------
//DoSave
//-----------------------------------------------------------------
void
PhotoPrintDoc::DoOpen(const FSSpec& inSpec) {
	
	mFileSpec = new MFileSpec(inSpec);
	DoRevert();
}//end DoOpen


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
PhotoPrintDoc::DoSaveToSpec	(const FSSpec& inSpec)
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
	this->Write(out);

	// Now that we have a file, update title & icon
	mWindow->SetDescriptor(theSpec.Name());
	if (gWindowProxies) {
		StGrafPortSaver		port;				// Mac OS 8.5 needs this
		::SetWindowProxyFSSpec(mWindow->GetMacWindow(), &inSpec);

		// Be sure the little icon in the window title shows that we're saved
		::SetWindowModified(mWindow->GetMacWindow(), false);
	}

	if (mFileSpec == nil || *mFileSpec != theSpec)
		mFileSpec = new MFileSpec(theSpec);

	mIsSpecified = true;
}//end DoSaveToSpec

/*
sDocHandler
*/
void
PhotoPrintDoc::sDocHandler(XML::Element &elem, void* userData) {
	
	PhotoPrintDoc* pDoc = (PhotoPrintDoc*)userData;
	pDoc->Read(elem);
}//end sDocHandler


//-----------------------------------------------------------------
//DoRevert
//-----------------------------------------------------------------
void			
PhotoPrintDoc::DoRevert			(void)
{
	HORef<char> path (mFileSpec->MakePath());
	XML::FileInputStream file (path);
	XML::Input input(file);

	XML::Handler handlers[] = {
		XML::Handler("Document", sDocHandler),
		XML::Handler::END
		};
		
	try {
		input.Process(handlers, (void*)this);
		}//end try
	catch (const XML::ParseException& e) {
		LStr255 sWhat (e.What());
		LStr255 sLine ("\pline ");
		LStr255 sLineNumber ((short)e.GetLine());
		sLine += sLineNumber;
		LStr255 sColumn ("\pcolumn");
		LStr255 sColumnNumber ((short)e.GetColumn());
		sColumn += sColumnNumber;
		
		::ParamText(sWhat, sLine, sColumn, nil);
		::Alert(alrt_XMLError, nil);
		throw;
		}//catch

}//end DoRevert

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


#pragma mark -

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
			switch (e.GetErrorCode()) {
				case memFullErr:
				case cTempMemErr:
					// do an out-of-memory alert message !!!
					break;
			}//end switch
	}//end catch
	catch (...) {
		//we should never get here, but just in case, silently eat the error
	}//end last chance catch
	placeHolder->RemoveOccupant ();			//	Always RemoveOccupant!
} // DoPrint


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


// ---------------------------------------------------------------------------
// GetDescriptor
// ---------------------------------------------------------------------------
StringPtr		
PhotoPrintDoc::GetDescriptor(Str255		outDescriptor) const
{
	if (IsFileSpecified())
		LString::CopyPStr (mFileSpec->Name(), outDescriptor);
	else {
		if (mScreenView != nil) {
			mScreenView->GetDescriptor(outDescriptor);
			}//endif window has a name
		else {
			outDescriptor[0] = 0;
			}//else unlucky, empty out the name
		}//else no file associated with document
		
	return outDescriptor;
}//end GetDescriptor


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

	// until we switch to Carbon1.1, we may only have a single
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
		}//endif no session open
		
	// we couldn't initialize w/o a session open, deferred until here.
	if (needToInitialize)
		mPrintSpec->SetToSysDefault();
	else
		mPrintSpec->GetPrintSettings();

	return mPrintSpec;
} // GetPrintRec

/*
HandlePrint {OVERRIDE}
	Handle the "print" command
*/
void			
PhotoPrintDoc::HandlePrint(void)
{
	StDesktopDeactivator		deactivator;
	if (PhotoPrintApp::gPalette != nil)
		PhotoPrintApp::gPalette->Hide();	// Deactivating doesn't hide our floater!
	if (PhotoPrintApp::gTools != nil)
		PhotoPrintApp::gTools->Hide();		// Deactivating doesn't hide our floater!

	PhotoPrinter::SetupPrintRecordToMatchProperties(this->GetPrintRec(), &mPrintProperties);
	
	bool						printIt = UPrinting::AskPrintJob(*this->GetPrintRec());

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

// ---------------------------------------------------------------------------
// HandlePageSetup {OVERRIDE}
// ---------------------------------------------------------------------------
void
PhotoPrintDoc::HandlePageSetup()
{
	StDesktopDeactivator	deactivator;
	
	ForceNewPrintSession();

	if (UPrinting::AskPageSetup(*GetPrintRec())) {

		// force a flattenning of the page format so that we can save it
		Handle orphan;
		::PMFlattenPageFormat(GetPrintRec()->GetPageFormat(), &orphan);
		PhotoPrintApp::gFlatPageFormat = new MNewHandle (orphan);

		this->MatchViewToPrintRec();
		this->GetView()->GetLayout()->LayoutImages();
		this->GetWindow()->Refresh();
		}//endif successful setup (assume something changed)
} // HandlePageSetup



void 
PhotoPrintDoc::SetController(OSType newController) {
	GetView()->SetController(newController);
	}//end


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
		mZoomDisplay->Refresh();
	}//endif need to change
}//end SetResolution


/*
UpdatePageNumber
*/
void
PhotoPrintDoc::UpdatePageNumber(const SInt16 inPageCount)
{
	mNumPages = inPageCount;

	// Keep the page count placard up to date
	LPane*		pages = mWindow->FindPaneByID('page');
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
	pages->SetDescriptor(theText);
} // UpdatePageNumber