/*
	File:		AboutBox.cp

	Contains:	header for the about box
	
	Written by:	Dav Lion

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		18 Sep 2000		rmgw	Add registration.
		21 aug 2000		dml		Created
*/


#include "AboutBox.h"

#include "PhotoPrintApp.h"
#include "Registration.h"

#include "HORef.h"
#include "MPString.h"

const ResIDT	ppob_AboutBox = 1400;
	const PaneIDT	pane_Version 		= 'vers';
	const PaneIDT	pane_Registration	= 'regi';
	
const	MessageT	msg_Register	= -1401;

PhotoAboutBox::PhotoAboutBox(ResIDT			inDialogResID,
							LCommander*		inSuper)

	: StDialogHandler(inDialogResID, inSuper)

 
 { // begin PhotoAboutBox
 	
 	{
		MCurResFile fooFile ((short)PhotoPrintApp::gAppResFile);
			
		// application version
		Handle versionH = ::GetResource ('vers', 1);
		if (versionH != nil) {
			VersRecHndl		vers = (VersRecHndl) versionH;
			MPString		version ((**vers).shortVersion);
			GetDialog()->FindPaneByID (pane_Version)->SetDescriptor (version);
			} // if version resource found
		}
	
	mRegister = GetDialog()->FindPaneByID (pane_Registration);
	if (Registration::IsRegistered ())
		mRegister->Disable ();
	else mRegister->Enable ();
	
	}//end ct


PhotoAboutBox::~PhotoAboutBox() {
	}//end dt
	
	
bool
PhotoAboutBox::Run() {
	
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



AboutCommand::AboutCommand(const CommandT inCommand, 
							PhotoPrintApp* inApp)
:PhotoAppCommandAttachment(inCommand, inApp)
{

	}//end ct
	
	

AboutCommand::~AboutCommand() {
	}//end dt
	
	

void
AboutCommand::FindCommandStatus(SCommandStatus*	inStatus) {
	*(inStatus->enabled) = true;
	}//end FindCommandStatus
	


void
AboutCommand::ExecuteCommandNumber(CommandT			/*inCommand*/,
							 void*				/*inCommandData*/) 
 {
	
	PhotoAboutBox (ppob_AboutBox, mApp).Run();
	
	}//end ExecuteCommand	
	