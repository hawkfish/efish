/*
	File:		AboutBox.cp

	Contains:	Implementation of the About command handler.
	
	Written by:	Dav Lion

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		27 Sep 2000		rmgw	Add EFish URL.
		12 jul 2001		dml		add printer creator code
		09 Nov 2000		drd		Dropped gShowing, we can now use EDialog::IsDialogShowing
		08 Nov 2000		drd		Renamed dialog to AboutBox, and added gShowing so we can
								disable the menu item correctly
		18 Sep 2000		rmgw	Add registration.
		21 aug 2000		dml		Created
*/

#include "AboutBox.h"

#include "PhotoPrinter.h"
#include "PhotoPrintApp.h"
#include "Registration.h"

#include "HORef.h"
#include "MPString.h"

#pragma mark AboutBox
#include "EURLDialogHandler.h"

class AboutBox : public EURLDialogHandler {
		
	public:
								AboutBox		(ResIDT			inDialogResID,
												 LCommander*	inSuper);
		virtual					~AboutBox		(void);
	
		bool					Run				(void);
};//end class PhotoAboutBox
	
	

const ResIDT	ppob_AboutBox = 1400;
	const PaneIDT	pane_Version 		= 'vers';
	const PaneIDT	pane_Registration	= 'regi';
	const PaneIDT	pane_Printer 		= 'prnt';
	const PaneIDT	pane_EFish 			= 'efsh';
	
const	MessageT	msg_Register	= -1401;
const	MessageT	msg_EFish		= -1402;

/*
AboutBox
*/
AboutBox::AboutBox(ResIDT			inDialogResID,
					LCommander*		inSuper)
	: EURLDialogHandler(inDialogResID, inSuper)
 { // begin AboutBox
 	
 	{
 		// Be sure the resource comes from us
		MCurResFile			fooFile ((short)PhotoPrintApp::gAppResFile);
			
		// application version
		Handle				versionH = ::Get1Resource ('vers', 1);
		if (versionH != nil) {
			VersRecHndl		vers = (VersRecHndl) versionH;
			MPString		version ((**vers).shortVersion);

			// And date
				// !!!

			this->FindPaneByID (pane_Version)->SetDescriptor (version);
		} // if version resource found
	}

	if (Registration::IsRegistered ())
		DisablePaneByID (pane_Registration);
	else
		EnablePaneByID (pane_Registration);


	LPane* printer = FindPaneByID(pane_Printer);
	LStr255 printerCreator (PhotoPrinter::GetCurPrinterCreator());
	printer->SetDescriptor(printerCreator);

}//end ct

/*
~AboutBox
*/
AboutBox::~AboutBox()
{
}//end dt
	
	
bool
AboutBox::Run() 

	{
	
	GetDialog()->Show();
	
	
	for (;;) {
		MessageT	msg = DoDialog();
		switch (msg) {
			case msg_OK:
				return true;

			case msg_Cancel:
				return false;
				
			case msg_Register:
				if (Registration::RunDialog (this)) DisablePaneByID (pane_Registration);
				break;
				
			case msg_EFish:
				OnURL (pane_EFish);
				break;
			}//end switch
		}//end for
	
	}//end	Run

#pragma mark -

AboutCommand::AboutCommand(const CommandT inCommand, 
							PhotoPrintApp* inApp)
:PhotoAppCommandAttachment(inCommand, inApp)
{
}//end ct


AboutCommand::~AboutCommand()
{
}//end dt
	
	
/*
FindCommandStatus {OVERRIDE}
*/
void
AboutCommand::FindCommandStatus(SCommandStatus*	inStatus) {
	*(inStatus->enabled) = !EDialog::IsDialogShowing();
}//end FindCommandStatus

/*
ExecuteCommand {OVERRIDE}
*/
void
AboutCommand::ExecuteCommand(void*				/*inCommandData*/) 
 {
	AboutBox (ppob_AboutBox, mApp).Run();
}//end ExecuteCommand	
