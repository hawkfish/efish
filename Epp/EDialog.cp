/*
	File:		EDialog.cp

	Contains:	Implementation of a dialog.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		28 Jun 2000		drd		Created
*/

#include "EDialog.h"

/*
EDialog
*/
EDialog::EDialog(
	ResIDT			inDialogResID,
	LCommander*		inSuper)
	: StDialogHandler(inDialogResID, inSuper)
{
} // EDialog

/*
EDialog
*/
EDialog::EDialog(
		LWindow*		inWindow,
		LCommander*		inSuper)
	: StDialogHandler(inWindow, inSuper)
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
