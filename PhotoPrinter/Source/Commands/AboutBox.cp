/*
	File:		AboutBox.cp

	Contains:	The about box
	
	Written by:	Dav Lion

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		12 jul 2001		dml		add printer creator code
		09 Nov 2000		drd		Dropped gShowing, we can now use EDialog::IsDialogShowing
		08 Nov 2000		drd		Renamed dialog to AboutBox, and added gShowing so we can
								disable the menu item correctly
		18 Sep 2000		rmgw	Add registration.
		21 aug 2000		dml		Created
*/

#include "AboutBox.h"

#include "PhotoPrintApp.h"
#include "Registration.h"

#include "HORef.h"
#include "MPString.h"
#include "PhotoPrinter.h"

const ResIDT	ppob_AboutBox = 1400;
	const PaneIDT	pane_Version 		= 'vers';
	const PaneIDT	pane_Registration	= 'regi';
	const PaneIDT	pane_Printer = 'prnt';
	
const	MessageT	msg_Register	= -1401;

/*
AboutBox
*/
AboutBox::AboutBox(ResIDT			inDialogResID,
					LCommander*		inSuper)
	: EDialog(inDialogResID, inSuper)
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

	mRegister = this->FindPaneByID (pane_Registration);
	if (Registration::IsRegistered ())
		mRegister->Disable ();
	else
		mRegister->Enable ();


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
AboutBox::Run() {
	
	GetDialog()->Show();
	for (;;) {
		MessageT	msg = DoDialog();
		switch (msg) {
			case msg_OK:
				return true;

			case msg_Cancel:
				return false;
				
			case msg_Register:
				if (Registration::RunDialog (this)) mRegister->Disable ();
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
