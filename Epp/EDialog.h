/*
	File:		EDialog.h

	Contains:	Definition of a dialog.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		09 Nov 2000		drd		Override UpdateMenus; added IsCommandEnabled, IsDialogShowing
		05 Oct 2000		drd		Took second type out of FindPaneType (for CW Pro 6)
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
	short				mEventMask;
	
public:
						EDialog(
								ResIDT			inDialogResID,
								LCommander*		inSuper,
								short			inEventMask = everyEvent);
						EDialog(
								LWindow*		inWindow,
								LCommander*		inSuper,
								short			inEventMask = everyEvent);
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

	// LEventDispatcher
	virtual	void		UpdateMenus();

	virtual	MessageT	DoDialog 				(void);
	static	bool		IsDialogShowing()	{ return gDialogCount > 0; }

protected:
	virtual	bool		IsCommandEnabled(const CommandT inCommand);

	static	SInt16		gDialogCount;
};

/*
FindPaneByID
*/
inline LPane*
EDialog::FindPaneByID(const PaneIDT	inPaneID) const
{
	return this->GetDialog()->FindPaneByID(inPaneID);
} // FindPaneByID

/*
FindPaneType
*/
template<class T>
inline T*
FindPaneType (const PaneIDT inPaneID, const EDialog* inDialog)
	{return dynamic_cast<T*>(inDialog->FindPaneByID(inPaneID));}
	
/*
FindBevelButton
*/
inline LBevelButton*
EDialog::FindBevelButton(const PaneIDT inPaneID) const
{
	return FindPaneType<LBevelButton> (inPaneID, this);
} // FindBevelButton

/*
FindEditText
*/
inline LEditText*
EDialog::FindEditText(const PaneIDT inPaneID) const
{
	return FindPaneType<LEditText> (inPaneID, this);
} // FindEditText

/*
FindPopupButton
*/
inline LPopupButton*
EDialog::FindPopupButton(const PaneIDT inPaneID) const
{
	return FindPaneType<LPopupButton> (inPaneID, this);
} // FindPopupButton

/*
FindRadioGroupView
*/
inline LRadioGroupView*
EDialog::FindRadioGroupView(const PaneIDT inPaneID) const
{
	return FindPaneType<LRadioGroupView> (inPaneID, this);
} // FindRadioGroupView

