//	PhotoPrintDoc.cp
//		Copyright © 2000 Electric Fish, Inc. All rights reserved.


/******** Change Log (most recent first ***************

		1 June 2000		dml			force print record to coincide w/ settings at start of DoPrint

*/


#include "PhotoPrintDoc.h"
#include "PhotoPrinter.h"
#include "PhotoPrintView.h"

const ResIDT PPob_PhotoPrintDocWindow = 1000;
const ResIDT prto_PhotoPrintPrintout = 1002;
const PaneIDT pane_ScreenView = 'scrn';

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


//-----------------------------------------------------------------
//AddEvents
//-----------------------------------------------------------------
void					
PhotoPrintDoc::AddEvents			(void) {
}//end AddEvents

#pragma mark -
#include "PhotoPrintCommand.h"
#include "SaveCommand.h"
//-----------------------------------------------------------------
//AddCommands
//-----------------------------------------------------------------
void					
PhotoPrintDoc::AddCommands			(void)
{
	new PhotoPrintCommand(cmd_Print, this);
	new SaveCommand(cmd_Save, this);
	}//end AddCommands


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
PhotoPrintDoc::AskSaveAs			(FSSpec&			/*outFSSpec*/,
									Boolean			/*inRecordIt*/)
{
	return false;
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
PhotoPrintDoc::DoSave				(void)
{
}//end DoSave



//-----------------------------------------------------------------
//DoRevert
//-----------------------------------------------------------------
void			
PhotoPrintDoc::DoRevert			(void)
{
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



#pragma mark-
void			
PhotoPrintDoc::SpendTime			(const EventRecord&	/*inMacEvent*/) 
{
}//emd SpendTime
