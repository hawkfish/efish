/*
	File:		PhotoPrintDoc.cp

	Contains:	Implementation of the document (which manages a window and file).

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

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
#include "ImageOptions.h"
#include "PhotoPrintCommands.h"
#include "PrintCommand.h"
#include "PhotoPrintApp.h"
#include "PhotoPrinter.h"
#include "PhotoPrintView.h"
#include "SaveCommand.h"
#include "PageSetupCommand.h"
#include "Layout.h"

// Toolbox++
#include "MNavDialogOptions.h"
#include "MAEList.h"
#include "MAEDesc.h"
#include "MNavPutFile.h"
#include "MNavReplyRecord.h"
#include "MAppleEvent.h"
#include "MPString.h"
#include "MAlert.h"

#include "xmloutput.h"
#include "xmlinput.h"
#include "xmlfile.h"

#include <iostream>
#include <map.h>

// Globals
PhotoPrintDoc*	PhotoPrintDoc::gCurDocument = nil;

const ResIDT PPob_PhotoPrintDocWindow = 1000;
const ResIDT prto_PhotoPrintPrintout = 1002;
const PaneIDT pane_ScreenView = 'scrn';
const ResIDT	alrt_XMLError = 131;

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
	CreateWindow(PPob_PhotoPrintDocWindow, inVisible);
	Initialize();
	
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
	CreateWindow(PPob_PhotoPrintDocWindow, inVisible);

	DoOpen(inSpec);
	Initialize();
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
PhotoPrintDoc::Initialize() {

	AddCommands();
	AddEvents();	
	mPrintProperties.SetAlternate(true);
}//end Initialize



//-----------------------------------------------------------------
//AddCommands
//-----------------------------------------------------------------
void					
PhotoPrintDoc::AddCommands			(void)
{
	// File menu
	new PrintCommand(cmd_Print, this);
	new PageSetupCommand(cmd_PageSetup, this);
	new SaveCommand(cmd_Save, this);
	new SaveCommand(cmd_SaveAs, this);

	// Options menu
	new BackgroundOptionsCommand(cmd_BackgroundOptions, this);
	new ImageOptionsCommand(cmd_ImageOptions, this);
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

	mPhotoPrintView = (PhotoPrintView*) mWindow->FindPaneByID (pane_ScreenView);

	if (inVisible)
		mWindow->Show();

	MatchViewToPrintRec();
}// end CreateWindow								 


void
PhotoPrintDoc::MatchViewToPrintRec(SInt16 inPageCount) {
	mNumPages = inPageCount;

	// base our size on the current page's size
	MRect pageBounds;
	PhotoPrinter::CalculatePrintableRect(GetPrintRec(), &GetPrintProperties(), 
										 pageBounds, GetResolution());
		
	// multiply by number of pages required (most cases == 1)
	// additional pages always go down!
	pageBounds.SetHeight(pageBounds.Height() * GetPageCount());
	
	mHeight = (double)pageBounds.Height() / this->GetResolution();
	mWidth = (double)pageBounds.Width() / this->GetResolution();

	mScreenView = dynamic_cast<PhotoPrintView*>(mWindow->FindPaneByID(pane_ScreenView));	
	ThrowIfNil_(mScreenView);
	
	MRect		screenViewFrame;
	mScreenView->CalcPortFrameRect(screenViewFrame);
	mScreenView->ResizeFrameTo(pageBounds.Width(), pageBounds.Height(), Refresh_No);
	mScreenView->ResizeImageTo(pageBounds.Width(), pageBounds.Height(), Refresh_No);

	// Since the background is what sits inside the LScrollerView, we need to change
	// is size as well
	LView*	background = dynamic_cast<LView*>(mWindow->FindPaneByID('back'));
	background->ResizeImageTo(pageBounds.Width(), pageBounds.Height(), Refresh_Yes);

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

	out.WriteElement("Layout", 	LayoutMapper::Find(GetView()->GetLayout()->GetType()));

	out.WriteElement("width", mWidth);
	out.WriteElement("height", mHeight);
	out.WriteElement("dpi", mDPI);

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

void PhotoPrintDoc::Read(XML::Element &elem)
{
	double	minVal (0.0);
	double	maxVal (200000.0);
	OSType	type;

	XML::Handler handlers[] = {
		XML::Handler("Print_Properties", PrintProperties::sParseProperties, (void*)&mPrintProperties),
		XML::Handler("Objects", sParseObjects),
		XML::Handler("width", &mWidth, &minVal, &maxVal),
		XML::Handler("height", &mHeight, &minVal, &maxVal),
		XML::Handler("dpi", &mDPI),
		XML::Handler("Layout", sParseLayout, &type),
		XML::Handler::END
		};
		
	elem.Process(handlers, this);
	GetView()->SetLayoutType(type);
}


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


void
PhotoPrintDoc::ParseLayout(XML::Element &elem, void *userData) {
	OSType* pLayout ((OSType*)userData);

	XML::Char tmp[64];
	size_t len = elem.ReadData(tmp, sizeof(tmp));
	tmp[len] = 0;
	
	*pLayout = LayoutMapper::Lookup(tmp);	
	}//end ParseAlignment


void
PhotoPrintDoc::sParseLayout(XML::Element &elem, void *userData) {
	((PhotoPrintDoc *)userData)->ParseLayout(elem, userData);
	}// StaticParseBound

#pragma mark -
//-----------------------------------------------------------------
//ObeyCommand
//-----------------------------------------------------------------
 Boolean			
 PhotoPrintDoc::ObeyCommand			(CommandT			inCommand,
									 void*				ioParam)
 {
	Boolean		cmdHandled = true;

	switch (inCommand) {
		default:
			cmdHandled = LSingleDoc::ObeyCommand(inCommand, ioParam);
		}//end switch

	return cmdHandled;
 }//end ObeyCommand

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
PhotoPrintDoc::DoSave				()
{
	DoSaveToSpec(*mFileSpec);
}//end DoSave


//-----------------------------------------------------------------
//DoSaveToSpec
//-----------------------------------------------------------------
void			
PhotoPrintDoc::DoSaveToSpec	(const FSSpec& inSpec)
{
	MFileSpec	fSpec	(inSpec, false);
	HORef<char> path (fSpec.MakePath());
	
	XML::FileOutputStream file(path);
	XML::Output out(file);

	// and write the data
	Write(out);
	
	if (*mFileSpec != fSpec)
		mFileSpec = new MFileSpec (fSpec);
}//end DoSaveToSpec



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


//-----------------------------------------------------------------
//SetResolution
//-----------------------------------------------------------------
void
PhotoPrintDoc::SetResolution(SInt16 inRes)
{
	if (inRes != mDPI) {
		MRect screenViewFrame;
		mScreenView->CalcPortFrameRect(screenViewFrame);
		screenViewFrame.SetWidth(screenViewFrame.Width() * inRes / mDPI);
		screenViewFrame.SetHeight(screenViewFrame.Height() * inRes / mDPI);		
		mDPI = inRes;
		mScreenView->ResizeImageTo(screenViewFrame.Width() , screenViewFrame.Height(), Refresh_Yes);
		// ??? set background view instead?
	}//endif need to change
}//end SetResolution

#pragma mark -

//-----------------------------------------------------------------
//DoPrint
//-----------------------------------------------------------------
void			
PhotoPrintDoc::DoPrint				(void)
{
	StDesktopDeactivator		deactivator;
	PhotoPrintApp::gPalette->Hide();	// Deactivating doesn't hide out floater!

	StPrintSession				openDriver(*this->GetPrintRec());

	PhotoPrinter::SetupPrintRecordToMatchProperties(this->GetPrintRec(), &mPrintProperties);
	
	bool						printIt = UPrinting::AskPrintJob(*this->GetPrintRec());

	if (printIt) {
		HORef<LPrintout>		thePrintout (LPrintout::CreatePrintout (prto_PhotoPrintPrintout));
		thePrintout->SetPrintSpec(*this->GetPrintRec());
		LPlaceHolder			*placeHolder = (LPlaceHolder*) thePrintout->FindPaneByID ('TBox');
		HORef<PhotoPrinter>		pPrinter = new PhotoPrinter(this, mPhotoPrintView, this->GetPrintRec(), 
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
		placeHolder->RemoveOccupant ();		//	Always RemoveOccupant!

	}//endif user really wants to print

	// ??? Is there a window hiding class?
	PhotoPrintApp::gPalette->Show();
}//end DoPrint

//-----------------------------------------------------------------
//DoPrintPreview
//-----------------------------------------------------------------
void			
PhotoPrintDoc::DoPrintPreview		(void)
{
}//end DoPrintPreview


// ---------------------------------------------------------------------------
// DoPageSetup
// ---------------------------------------------------------------------------
void
PhotoPrintDoc::DoPageSetup()
{
	StPrintSession			session (*GetPrintRec());
	StDesktopDeactivator	deactivator;

	EPrintSpec	vanilla (*GetPrintRec());
	UPrinting::AskPageSetup(*GetPrintRec());
	if (vanilla != *GetPrintRec()) {
		this->MatchViewToPrintRec();
		this->GetView()->GetLayout()->LayoutImages();
		this->GetWindow()->Refresh();
	}//endif something changed
} // DoPageSetup

// ---------------------------------------------------------------------------
// GetDescriptor
// ---------------------------------------------------------------------------
StringPtr		
PhotoPrintDoc::GetDescriptor(Str255		outDescriptor) const
{
	if (IsFileSpecified())
		LString::CopyPStr (mFileSpec->Name(), outDescriptor);
	else {
		if (mPhotoPrintView != nil) {
			mPhotoPrintView->GetDescriptor(outDescriptor);
			}//endif window has a name
		else {
			outDescriptor[0] = 0;
			}//else unlucky, empty out the name
		}//else no file associated with document
		
	return outDescriptor;
}//end GetDescriptor

// ---------------------------------------------------------------------------
//		´ GetPrintRec
// Will construct if necessary.  Attentive to existing session
// ---------------------------------------------------------------------------

HORef<EPrintSpec>
PhotoPrintDoc::GetPrintRec (void)

	{ // begin PrintRec
	
	if (mEPrintSpec == nil) {
		HORef<StPrintSession> possibleScope = 0;
	
		{
			mEPrintSpec = new EPrintSpec();

			if (!UPrinting::SessionIsOpen())
				possibleScope = new StPrintSession (*mEPrintSpec);
			
			mEPrintSpec->SetToSysDefault();
			}//endif opened


		}//endif need to create the record 
		return mEPrintSpec;
		
	} // end PrintRec


#pragma mark-
void			
PhotoPrintDoc::SpendTime			(const EventRecord&	/*inMacEvent*/) 
{
}//emd SpendTime
