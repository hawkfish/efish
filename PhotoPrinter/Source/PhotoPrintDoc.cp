//	PhotoPrintDoc.cp
//		Copyright © 2000 Electric Fish, Inc. All rights reserved.

#include "PhotoPrintDoc.h"
#include "PhotoPrinter.h"

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

}// end CreateWindow								 


//-----------------------------------------------------------------
//AddEvents
//-----------------------------------------------------------------
void					
PhotoPrintDoc::AddEvents			(void) {
}//end AddEvents

//-----------------------------------------------------------------
//AddCommands
//-----------------------------------------------------------------
void					
PhotoPrintDoc::AddCommands			(void){
	}//end AddCommands

//-----------------------------------------------------------------
//PhotoPrintDoc
//-----------------------------------------------------------------
PhotoPrintDoc::PhotoPrintDoc		(LCommander*		inSuper, 
									Boolean inVisible)
	:LSingleDoc (inSuper)
	, mFileType ('foto')
{
	CreateWindow(PPob_PhotoPrintDocWindow, inVisible);
	mPrintSpec.SetToSysDefault();
}//end ct

//-----------------------------------------------------------------
//PhotoPrintDoc
//-----------------------------------------------------------------
PhotoPrintDoc::PhotoPrintDoc		(LCommander*		inSuper,
									 const	FSSpec&		/*inSpec*/,
									 Boolean inVisible) 
	: LSingleDoc (inSuper)
	, mFileType ('foto')
 {
	CreateWindow(PPob_PhotoPrintDocWindow, inVisible);
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
		case cmd_Print:
			DoPrint();
			break;
	
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

#pragma mark
//-----------------------------------------------------------------
//DoPrint
//-----------------------------------------------------------------
void			
PhotoPrintDoc::DoPrint				(void)
{
	
	StPrintSession openDriver (mPrintSpec);

	UDesktop::Deactivate();
	bool	printIt = UPrinting::AskPrintJob(mPrintSpec);
	UDesktop::Activate();

	if (printIt) {
		HORef<LPrintout>		thePrintout (LPrintout::CreatePrintout (prto_PhotoPrintPrintout));
		thePrintout->SetPrintSpec(mPrintSpec);
		LPlaceHolder			*placeHolder = (LPlaceHolder*) thePrintout->FindPaneByID ('TBox');
		HORef<PhotoPrinter>		pPrinter = new PhotoPrinter(this, mPhotoPrintView, &mPrintSpec, thePrintout->GetMacPort());
		
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
