

/*
	File:		EDialog.cp

	Contains:	Implementation of a dialog.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		09 Nov 2000		drd		Override UpdateMenus; added IsCommandEnabled, gDialogCount
		14 Sep 2000		rmgw	DoDialog, mEventMask
		28 Jun 2000		drd		HidePaneByID, ShowPaneByID
		28 Jun 2000		drd		Created
*/

#include "EDialog.h"

#include <UEventMgr.h>

// Globals
SInt16		EDialog::gDialogCount = 0;

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
	gDialogCount++;
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
	gDialogCount++;
} // EDialog

EDialog::~EDialog()
{
	gDialogCount--;
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

// ---------------------------------------------------------------------------
//	¥ DoDialog														  [public]
// ---------------------------------------------------------------------------
//	Handle an Event for a dialog box
//
//	Call this function repeatedly to handle events. If the event triggers
//	a Broadcaster to broadcast a message, the last such message heard by
//	the EDialog is returned. Otherwise, this function returns msg_Nothing.
//
//	This is the same as the one in StDialogHandler, but it allows event
//	masking (e.g. disallowing apple events).

MessageT
EDialog::DoDialog (void)
{ // begin DoDialog

	EventRecord macEvent;

	if (this->IsOnDuty()) {
		UEventMgr::GetMouseAndModifiers(macEvent);
		this->AdjustCursor(macEvent);
	}

	SetUpdateCommandStatus(false);
	mMessage = msg_Nothing;

	Boolean gotEvent = ::WaitNextEvent(mEventMask, &macEvent,
										mSleepTime, mMouseRgn);
	
		// Let Attachments process the event. Continue with normal
		// event dispatching unless suppressed by an Attachment.
	
	if (LEventDispatcher::ExecuteAttachments(msg_Event, &macEvent)) {
		if (gotEvent) {
			this->DispatchEvent(macEvent);
		} else {
			this->UseIdleTime(macEvent);
		}
	}

									// Repeaters get time after every event
	LPeriodical::DevoteTimeToRepeaters(macEvent);
	
									// Update status of menu items
	if (this->IsOnDuty() && GetUpdateCommandStatus()) {
		this->UpdateMenus();
	}

	return mMessage;
} // end DoDialog

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
IsCommandEnabled
*/
bool
EDialog::IsCommandEnabled(const CommandT inCommand)
{
	Boolean		isEnabled = false;
	Boolean		usesMark = false;
	UInt16		mark;
	Str255		itemName;

	LCommander*	theTarget = LCommander::GetTarget();

	itemName[0] = 0;

	if (theTarget != nil) {
		theTarget->ProcessCommandStatus(inCommand, isEnabled,
								usesMark, mark, itemName);
	}

	return isEnabled;
} // IsCommandEnabled

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

/*
UpdateMenus {OVERRIDE}
	Override because the Aqua menu items need some special handling
*/
void
EDialog::UpdateMenus()
{
	StDialogHandler::UpdateMenus();

	long	response;
	OSErr	err = ::Gestalt(gestaltMenuMgrAttr, &response);
	if ((err == noErr) && (response & gestaltMenuMgrAquaLayoutMask)) {
		if (this->IsCommandEnabled(cmd_Preferences)) {
			::EnableMenuCommand(0, 'pref');
		} else {
			::DisableMenuCommand(0, 'pref');
		}
		if (this->IsCommandEnabled(cmd_Quit)) {
			::EnableMenuCommand(0, 'quit');
		} else {
			::DisableMenuCommand(0, 'quit');
		}
	}
} // UpdateMenus
