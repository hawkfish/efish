/*
	File:		EURLDialogHandler.cp

	Contains:	Implementation of a URL using dialog.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1999 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

         <3>     9/14/00    rmgw    Import to Epp.
         <2>    12/23/99    rmgw    Carbonize.
         <1>     12/9/99    rmgw    first checked in.
*/


#include "EURLDialogHandler.h"

#include "CInternetConfig.h"
#include "CURLPushButton.h"

#include "LStaticText.h"

#include "MFileSpec.h"
#include "MResFile.h"

#include <UEventMgr.h>

// ---------------------------------------------------------------------------
//		¥ EURLDialogHandler
// ---------------------------------------------------------------------------

EURLDialogHandler::EURLDialogHandler (
	
	ResIDT			inResID,
	LCommander*		inSuper,
	short			inEventMask)
	
	: EDialog (inResID, inSuper)
	
	, mIC (0)
	, mEventMask (inEventMask)
	
	{ // begin EURLDialogHandler		

		SetupIC ();

	} // end EURLDialogHandler
	
// ---------------------------------------------------------------------------
//		¥ ~EURLDialogHandler
// ---------------------------------------------------------------------------

EURLDialogHandler::~EURLDialogHandler (void)

	{ // begin ~EURLDialogHandler
		
	} // end ~EURLDialogHandler
	
// ---------------------------------------------------------------------------
//		¥ SetupIC
// ---------------------------------------------------------------------------

void
EURLDialogHandler::SetupIC (void) 

	{ // begin SetupIC

		try {
			MCurResFile	saveFile;
			mIC = new CInternetConfig (MFileSpec::sDefaultFileType); // test application signature

			OSErr err = mIC->Start ();
			ThrowIfOSErr_(err);
			} // try

		catch ( ... ) {
			// .. if we failed no big deal.. we just won't use IC
			delete mIC;
			mIC = 0;
			} // catch

	} // end SetupIC

// ---------------------------------------------------------------------------
//		¥ SetupURL
// ---------------------------------------------------------------------------

void	
EURLDialogHandler::SetupURL (

	PaneIDT		inPaneID)
	
	{ // begin SetupURL
	
		if (mIC) return;
		
		// if internet config not enabled, make the buttons look normal.
		CURLPushButton*	button = dynamic_cast<CURLPushButton*>(GetDialog ()->FindPaneByID (inPaneID));
		if (!button) return;
		
		//	Grab the text traits of the neighboring pane
		ResIDT			txtrID = button->GetUserCon ();
		LPane*			pane = GetDialog ()->FindPaneByID (txtrID);
		LStaticText*	textPane = dynamic_cast<LStaticText*>(pane);
		if (textPane) txtrID = textPane->GetTextTraitsID ();
		
		button->SetTextTraitsID (txtrID);
		button->SetHiliteTextTraitsID (txtrID);

	} // end SetupURL
	
// ---------------------------------------------------------------------------
//		¥ OnURL
// ---------------------------------------------------------------------------

void	
EURLDialogHandler::OnURL (

	PaneIDT		inPaneID)
	
	{ // begin OnURL
	
		if (!mIC) return;
		
		CURLPushButton*	button = dynamic_cast<CURLPushButton*>(GetDialog ()->FindPaneByID (inPaneID));
		if (!button) return;
		
		Str255 theURL;
		button->GetURL (theURL);
		
		MCurResFile	saveFile;
		mIC->DoURL (theURL);

	} // end OnURL
	
// ---------------------------------------------------------------------------
//	¥ DoDialog														  [public]
// ---------------------------------------------------------------------------
//	Handle an Event for a dialog box
//
//	Call this function repeatedly to handle events. If the event triggers
//	a Broadcaster to broadcast a message, the last such message heard by
//	the EURLDialogHandler is returned. Otherwise, this function returns
//	msg_Nothing.

MessageT
EURLDialogHandler::DoDialog (void)

	{ // begin DoDialog
	
		EventRecord macEvent;

		if (IsOnDuty()) {
			UEventMgr::GetMouseAndModifiers(macEvent);
			AdjustCursor(macEvent);
		}

		SetUpdateCommandStatus(false);
		mMessage = msg_Nothing;

		Boolean gotEvent = ::WaitNextEvent(mEventMask, &macEvent,
											mSleepTime, mMouseRgn);
		
			// Let Attachments process the event. Continue with normal
			// event dispatching unless suppressed by an Attachment.
		
		if (LEventDispatcher::ExecuteAttachments(msg_Event, &macEvent)) {
			if (gotEvent) {
				DispatchEvent(macEvent);
			} else {
				UseIdleTime(macEvent);
			}
		}

										// Repeaters get time after every event
		LPeriodical::DevoteTimeToRepeaters(macEvent);
		
										// Update status of menu items
		if (IsOnDuty() && GetUpdateCommandStatus()) {
			UpdateMenus();
		}

		return mMessage;

	} // end DoDialog

