#include "AboutBox.h"
#include "MPString.h"

/*
	File:		AboutBox.cp

	Contains:	header for the about box
	
	Written by:	Dav Lion

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		21 aug 2000		dml		Created
*/



const PaneIDT	pane_Version = 'vers';
const ResIDT	ppob_AboutBox = 128;
#include "PhotoPrintApp.h"
#include "HORef.h"

PhotoAboutBox::PhotoAboutBox(ResIDT			inDialogResID,
							LCommander*		inSuper)

	: StDialogHandler(inDialogResID, inSuper)

 {
	MCurResFile fooFile ((short)PhotoPrintApp::gAppResFile);
		
	// application version
	Handle versionH = ::GetResource ('vers', 1);
	if (versionH != nil) {
		VersRecHndl		vers = (VersRecHndl) versionH;
		MPString		version ((**vers).shortVersion);
		GetDialog()->FindPaneByID (pane_Version)->SetDescriptor (version);
		} // if version resource found

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
	HORef<PhotoAboutBox> pBox = new PhotoAboutBox(ppob_AboutBox, mApp);
	pBox->Run();
	
	}//end ExecuteCommand	
	