/*
	File:		PhotoPrintDoc.cp

	Contains:	Implementation of the Background Options dialog.

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		14 Jun 2000		drd		ImageOptionsCommand
		14 Jun 2000		drd		BackgroundOptionsCommand
		1 June 2000		dml		force print record to coincide w/ settings at start of DoPrint

*/

#include "PhotoPrintDoc.h"
#include "BackgroundOptions.h"
#include "ImageOptions.h"
#include "PhotoPrintCommands.h"
#include "PrintCommand.h"
#include "PhotoPrinter.h"
#include "PhotoPrintView.h"
#include "SaveCommand.h"

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

const ResIDT PPob_PhotoPrintDocWindow = 1000;
const ResIDT prto_PhotoPrintPrintout = 1002;
const PaneIDT pane_ScreenView = 'scrn';
const ResIDT	alrt_XMLError = 131;

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

	AddCommands();
	AddEvents();
}//end ct

//-----------------------------------------------------------------
//PhotoPrintDoc
//-----------------------------------------------------------------
PhotoPrintDoc::PhotoPrintDoc		(LCommander*		inSuper,
									 const	FSSpec&		inSpec,
									 Boolean inVisible) 
	: LSingleDoc (inSuper)
	, mFileType ('foto')
 {
	CreateWindow(PPob_PhotoPrintDocWindow, inVisible);

	DoOpen(inSpec);

	AddCommands();
	AddEvents();	
 }//end ct

//-----------------------------------------------------------------
//~PhotoPrintDoc
//-----------------------------------------------------------------
PhotoPrintDoc::~PhotoPrintDoc	(void)
{
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
	mWindow = LWindow::CreateWindow(inWindowID, this);
	ThrowIfNil_(mWindow);

	mPhotoPrintView = (PhotoPrintView*) mWindow->FindPaneByID (pane_ScreenView);

	if (inVisible)
		mWindow->Show();

	// base our size on the current page's size
	MRect pageBounds;
	GetPrintRec()->GetPageRect(pageBounds);
	// not get that all important resolution
	SInt16 vRes;
	SInt16 hRes;
	GetPrintRec()->GetResolutions(vRes, hRes);
	// convert those page bounds to a screen resolution rect
	pageBounds.SetHeight(pageBounds.Height() * GetResolution() / vRes);
	pageBounds.SetWidth(pageBounds.Width() * GetResolution() / vRes);
	
	mHeight = pageBounds.Height() / GetResolution();
	mWidth = pageBounds.Width() / GetResolution();

	mScreenView = dynamic_cast<PhotoPrintView*>(mWindow->FindPaneByID(pane_ScreenView));	
	ThrowIfNil_(mScreenView);
	
	MRect screenViewFrame;
	mScreenView->CalcPortFrameRect(screenViewFrame);
	mScreenView->ResizeImageTo(pageBounds.Width() , pageBounds.Height(), Refresh_Yes);

	// link ourselves to the view
	mScreenView->GetModel()->SetDocument(this);
}// end CreateWindow								 

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

	XML::Handler handlers[] = {
		XML::Handler("Print_Properties", PrintProperties::sParseProperties, (void*)&mPrintProperties),
		XML::Handler("Objects", sParseObjects),
		XML::Handler("width", &mWidth, &minVal, &maxVal),
		XML::Handler("height", &mHeight, &minVal, &maxVal),
		XML::Handler("dpi", &mDPI),
		XML::Handler::END
		};
		
	elem.Process(handlers, this);
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
		}//endif need to change
}//end SetResolution

#pragma mark -

//-----------------------------------------------------------------
//DoPrint
//-----------------------------------------------------------------
void			
PhotoPrintDoc::DoPrint				(void)
{
	
	StPrintSession openDriver (*GetPrintRec());

	PhotoPrinter::SetupPrintRecordToMatchProperties(GetPrintRec(), &mPrintProperties);
	
	UDesktop::Deactivate();
	bool	printIt = UPrinting::AskPrintJob(*GetPrintRec());
	UDesktop::Activate();

	if (printIt) {

		HORef<LPrintout>		thePrintout (LPrintout::CreatePrintout (prto_PhotoPrintPrintout));
		thePrintout->SetPrintSpec(*GetPrintRec());
		LPlaceHolder			*placeHolder = (LPlaceHolder*) thePrintout->FindPaneByID ('TBox');
		HORef<PhotoPrinter>		pPrinter = new PhotoPrinter(this, mPhotoPrintView, GetPrintRec(), 
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
						// do an out-of-memory alert message
						break;
				}//end switch
			}//end catch
		catch (...) {
			//we should never get here, but just in case, silently eat the error
			}//end last chance catch
		placeHolder->RemoveOccupant ();		//	Always RemoveOccupant!

	}//endif user really wants to print

	
}//end DoPrint

//-----------------------------------------------------------------
//DoPrintPreview
//-----------------------------------------------------------------
void			
PhotoPrintDoc::DoPrintPreview		(void)
{
}//end DoPrintPreview




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
//		€ GetPrintRec
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
 