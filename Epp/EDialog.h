/*
	File:		EDialog.h

	Contains:	Definition of a dialog.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		28 Jun 2000		drd		Created
*/

#pragma once

#include <UModalDialogs.h>

class EDialog : public StDialogHandler
{
public:
						EDialog(
								ResIDT			inDialogResID,
								LCommander*		inSuper);
						EDialog(
								LWindow*		inWindow,
								LCommander*		inSuper);
	virtual				~EDialog();

	virtual	void		DisablePaneByID(const PaneIDT	inPane);
	virtual	void		EnablePaneByID(const PaneIDT	inPane);
	virtual	LPane*		FindPaneByID(const PaneIDT	inPaneID) const;
	virtual	void		HidePaneByID(const PaneIDT	inPane);
	virtual	void		ShowPaneByID(const PaneIDT	inPane);
};

/*
FindPaneByID
*/
inline LPane*
EDialog::FindPaneByID(const PaneIDT	inPaneID) const
{
	return this->GetDialog()->FindPaneByID(inPaneID);
} // FindPaneByID
