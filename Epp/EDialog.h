/*
	File:		EDialog.h

	Contains:	Definition of a dialog.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		12 Jul 2000		drd		Added FindBevelButton, FindEditText, FindPopupButton, FindRadioGroupView
								to avoid ugly casts
		28 Jun 2000		drd		Created
*/

#pragma once

#include <UModalDialogs.h>

#include <LBevelButton.h>
#include <LEditText.h>
#include <LPopupButton.h>
#include <LRadioGroupView.h>

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
			LBevelButton*	FindBevelButton(const PaneIDT inPaneID) const;
			LEditText*	FindEditText(const PaneIDT inPaneID) const;
	virtual	LPane*		FindPaneByID(const PaneIDT	inPaneID) const;
			LPopupButton*	FindPopupButton(const PaneIDT inPaneID) const;
			LRadioGroupView*	FindRadioGroupView(const PaneIDT inPaneID) const;
	virtual	void		HidePaneByID(const PaneIDT	inPane);
	virtual	void		ShowPaneByID(const PaneIDT	inPane);
};

/*
FindBevelButton
*/
inline LBevelButton*
EDialog::FindBevelButton(const PaneIDT inPaneID) const
{
	return dynamic_cast<LBevelButton*>(this->FindPaneByID(inPaneID));
} // FindBevelButton

/*
FindEditText
*/
inline LEditText*
EDialog::FindEditText(const PaneIDT inPaneID) const
{
	return dynamic_cast<LEditText*>(this->FindPaneByID(inPaneID));
} // FindEditText

/*
FindPaneByID
*/
inline LPane*
EDialog::FindPaneByID(const PaneIDT	inPaneID) const
{
	return this->GetDialog()->FindPaneByID(inPaneID);
} // FindPaneByID

/*
FindPopupButton
*/
inline LPopupButton*
EDialog::FindPopupButton(const PaneIDT inPaneID) const
{
	return dynamic_cast<LPopupButton*>(this->FindPaneByID(inPaneID));
} // FindPopupButton

/*
FindRadioGroupView
*/
inline LRadioGroupView*
EDialog::FindRadioGroupView(const PaneIDT inPaneID) const
{
	return dynamic_cast<LRadioGroupView*>(this->FindPaneByID(inPaneID));
} // FindRadioGroupView
