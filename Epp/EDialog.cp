

/*
	File:		EDialog.cp

	Contains:	Implementation of a dialog.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		14 Sep 2000		rmgw	DoDialog, mEventMask
		28 Jun 2000		drd		HidePaneByID, ShowPaneByID
		28 Jun 2000		drd		Created
*/

#include "EDialog.h"

#include <UEventMgr.h>

/*
EDialog
*/
EDialog::EDialog(
	ResIDT			inDialogResID,
	LCommander*		inSuper,
	short			inEventMask)
	: StDialogHandler(inDialogResID, inSuper)
	, mEventMask (inEventMask)
{
} // EDialog

/*
EDialog
*/
EDialog::EDialog(
	LWindow*		inWindow,
	LCommander*		inSuper,
	short			inEventMask)
	: StDialogHandler(inWindow, inSuper)
	, mEventMask (inEventMask)
{
} // EDialog

EDialog::~EDialog()
{
} // ~EDialog

/*
DisablePaneByID
*/
void
EDialog::DisablePaneByID(const PaneIDT	inPane)
{		
	LPane*	pane = this->FindPaneByID(inPane);
	if (pane != nil)
		pane->Disable();
} // DisablePaneByID

/*
EnablePaneByID
*/
void
EDialog::EnablePaneByID(const PaneIDT	inPane)
{		
	LPane*	pane = this->FindPaneByID(inPane);
	if (pane != nil)
		pane->Enable();
} // EnablePaneByID

/*
HidePaneByID
*/
void
EDialog::HidePaneByID(const PaneIDT	inPane)
{		
	LPane*	pane = this->FindPaneByID(inPane);
	if (pane != nil)
		pane->Hide();
} // HidePaneByID

/*
ShowPaneByID
*/
void
EDialog::ShowPaneByID(const PaneIDT	inPane)
{		
	LPane*	pane = this->FindPaneByID(inPane);
	if (pane != nil)
		pane->Show();
} // ShowPaneByID

// ---------------------------------------------------------------------------
//	¥ DoDialog														  [public]
// ---------------------------------------------------------------------------
//	Handle an Event for a dialog box
//
//	Call this function repeatedly to handle events. If the event triggers
//	a Broadcaster to broadcast a message, the last such message heard by
//	the URLDialogHandler is returned. Otherwise, this function returns
//	msg_Nothing.
//
//	This is the same as the one in StDialogHandler, but it allows event
//	masking (e.g. disallowing apple events).

MessageT
EDialog::DoDialog (void)

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

