/*
	File:		AboutBox.cp

	Contains:	Implementation of the About command handler.
	
	Written by:	Dav Lion

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

	 	15 Nov 2001		rmgw	Add serial number display.
	 	14 Nov 2001		rmgw	Registration changes.
		 1 Nov 2001		rmgw	eSellerate changes.
		30 Oct 2001		rmgw	Disable registration using IsRegistered.
		29 Oct 2001		rmgw	Set global registration.
		17 Aug 2001		drd		Show QuickTime version (and switch CarbonLib to EUtil::FormatGestaltVersion);
								adjust URL for Platinum
		30 Jul 2001		drd		Show CarbonLib version
		27 Jul 2001		rmgw	Add EFish URL.
		12 jul 2001		dml		add printer creator code
		09 Nov 2000		drd		Dropped gShowing, we can now use EDialog::IsDialogShowing
		08 Nov 2000		drd		Renamed dialog to AboutBox, and added gShowing so we can
								disable the menu item correctly
		18 Sep 2000		rmgw	Add registration.
		21 aug 2000		dml		Created
*/

#include "AboutBox.h"
#include "EUtil.h"
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
	const PaneIDT	pane_CarbonLabel	= 'carL';
	const PaneIDT	pane_CarbonVersion	= 'carV';
	const PaneIDT	pane_QuickTimeVersion	= 'qtiV';
	const PaneIDT	pane_SerialLabel	= 'serL';
	const PaneIDT	pane_SerialNumber	= 'serN';

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

	// The URL button is placed OK for Aqua, so adjust if we're not in Aqua
	LPane*	urlButton = this->FindPaneByID(pane_EFish);
	if (!UEnvironment::HasFeature(env_HasAquaTheme)) {
		urlButton->MoveBy(-12, -1, Refresh_No);		// These magic numbers eyeballed by drd
	}

	LPane* printer = this->FindPaneByID(pane_Printer);
	LStr255			printerCreator(PhotoPrinter::GetCurPrinterCreator());
	printer->SetDescriptor(printerCreator);

	LPane*			carbonVersion = this->FindPaneByID(pane_CarbonVersion);
	if (PhotoPrintApp::gOSX) {
		LPane*	carbonLabel = this->FindPaneByID(pane_CarbonLabel);
		carbonLabel->Hide();
		carbonVersion->Hide();
	} else {
		LStr255		carbVers;
		EUtil::FormatGestaltVersion(gestaltCarbonVersion, carbVers);
		carbonVersion->SetDescriptor(carbVers);
	}
	LPane*			qtVersion = this->FindPaneByID(pane_QuickTimeVersion);
	if (qtVersion != nil) {
		LStr255		qtVers;
		EUtil::FormatGestaltVersion(gestaltQuickTimeVersion, qtVers);
		qtVersion->SetDescriptor(qtVers);
	}
	
	LStr255			serial;
	if (Registration::GetSerialNumber (serial)) {
		ShowPaneByID (pane_SerialLabel);
		ShowPaneByID (pane_SerialNumber);
		GetDialog ()->SetDescriptorForPaneID (pane_SerialNumber, serial);
		} // if
		
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
				GetDialog()->Hide();
				PhotoPrintApp::gIsRegistered = Registration::DoPurchaseDialog (this);
				if (Registration::IsRegistered ()) 
					DisablePaneByID (pane_Registration);
				GetDialog()->Show();
				GetDialog()->Select();
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
