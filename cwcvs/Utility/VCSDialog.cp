/*
	File:		VCSDialog.cp

	Contains:	General purpose dialog stack object.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1997-2001 Electric Fish Inc.  All Rights Reserved.

	Change History (most recent first):

        <14>     8/24/00    rmgw    Carbonize.
        <13>     3/31/99    rmgw    Add modeless dialog.
        <12>     2/23/99    rmgw    Save and restore A4.
        <11>     11/6/98    rmgw    Foreground default is black.
        <10>     10/8/98    rmgw    Convert to v7 API.
        <8+>     10/6/98    rmgw    Convert to v7 API.
         <8>     9/24/98    rmgw    Add idle handling.
         <7>     2/22/98    rmgw    Fix text backgrounds.
         <6>     2/16/98    rmgw    Fix OnItemClick to test for enabled.
         <5>      2/9/98    rmgw    Fix userItem patching.
         <4>      2/9/98    rmgw    Add dialog items.
         <3>      2/6/98    rmgw    Add item enabling.
         <2>      2/6/98    rmgw    Seperate contruction and setup.
         <1>      2/6/98    rmgw    first checked in.
*/


#include "VCSDialog.h"

#include "VCSDialogItem.h"

#include "StForeColor.h"
#include "StBackColor.h"

#include <SetUpA4.h>

// ---------------------------------------------------------------------------
//		¥ DialogFilterProc
// ---------------------------------------------------------------------------

pascal Boolean 
VCSDialog::DialogFilterProc (

	DialogPtr 			theDialog, 
	EventRecord*		theEvent,
	DialogItemIndex*	itemHit)
	
	{ // begin DialogFilterProc

		EnterCallback();
		
		VCSDialog*	that = (VCSDialog*) ::GetWRefCon (GetDialogWindow (theDialog));
		Boolean		result = that->OnFilterEvent (*theEvent, *itemHit);
	
		ExitCallback();
		
		return result;
		
	} // end DialogFilterProc

// ---------------------------------------------------------------------------
//		¥ UserItemProc
// ---------------------------------------------------------------------------

pascal void 
VCSDialog::UserItemProc (

	DialogPtr 		theDialog, 
	DialogItemIndex	itemHit)
	
	{ // begin UserItemProc
		
		EnterCallback();
		
		VCSDialog*	that = (VCSDialog*) ::GetWRefCon (GetDialogWindow (theDialog));
		that->OnUserItem (itemHit);
		
		ExitCallback();
		
	} // end UserItemProc

// ---------------------------------------------------------------------------
//		¥ VCSDialog
// ---------------------------------------------------------------------------

VCSDialog::VCSDialog (

	const	VCSContext&		inContext,
	short					inDLOGid)
	
	: mPrep (inContext)
	, mDialog (GetNewDialog (inDLOGid, nil, (WindowPtr) -1))
	, mFilterUPP (NewModalFilterUPP (DialogFilterProc))
	, mItems (0)
	, mUserUPP (NewUserItemUPP (UserItemProc))
	
	{ // begin VCSDialog
		
		//	Connect the dialog up
		PrepareCallback();
		SetWRefCon (GetDialogWindow (mDialog), (long) this);
		
		//	Set up the user items
		for (DialogItemIndex i = CountDITL (GetDialogPtr ()); i > 0; --i) {
			short	iType;
			Rect	iRect;
			Handle	iHandle;
			GetDialogItem (GetDialogPtr (), i, &iType, &iHandle, &iRect);
			if (userItem != (iType & ~itemDisable)) continue;

			SetDialogItem (GetDialogPtr (), i, iType, (Handle) mUserUPP, &iRect);
			} // for

	} // end VCSDialog
	
// ---------------------------------------------------------------------------
//		¥ ~VCSDialog
// ---------------------------------------------------------------------------

VCSDialog::~VCSDialog (void)

	{ // begin ~VCSDialog
	
		if (nil != mDialog) DisposeDialog (mDialog);
		mDialog = nil;

		DisposeUserItemUPP (mUserUPP);
		DisposeModalFilterUPP (mFilterUPP);

	} // end ~VCSDialog

// ---------------------------------------------------------------------------
//		¥ GetItem
// ---------------------------------------------------------------------------

VCSDialogItem*
VCSDialog::GetItem (

	DialogItemIndex	itemNo) const
	
	{ // begin GetItem
		
		for (VCSDialogItem*	i = mItems; i; i = i->GetNext ()) {
			if (i->GetIndex () != itemNo) continue;
			
			return i;
			} // for
			
		return 0;
		
	} // end GetItem
	
// ---------------------------------------------------------------------------
//		¥ AddItem
// ---------------------------------------------------------------------------

void
VCSDialog::AddItem (

	VCSDialogItem*	inItem) 
	
	{ // begin AddItem
		
		inItem->SetNext (mItems);
		mItems = inItem;
		
	} // end AddItem
	
// ---------------------------------------------------------------------------
//		¥ RemoveItem
// ---------------------------------------------------------------------------

void
VCSDialog::RemoveItem (

	VCSDialogItem*	inItem) 
	
	{ // begin RemoveItem
		
		if (mItems == inItem) {
			mItems = inItem->GetNext ();
			return;
			} // if
			
		for (VCSDialogItem*	i = mItems; i; i = i->GetNext ()) {
			if (i->GetNext () != inItem) continue;
			
			i->SetNext (inItem->GetNext ());
			break;
			} // for
		
	} // end RemoveItem
	
// ---------------------------------------------------------------------------
//		¥ GetItemEnable
// ---------------------------------------------------------------------------

Boolean
VCSDialog::GetItemEnable (

	DialogItemIndex	itemNo) const
	
	{ // begin GetItemEnable
		
		VCSDialogItem*	item = GetItem (itemNo);
		if (item) return item->GetEnable ();
		
		short	iType;
		Rect	iRect;
		Handle	iHandle;
		GetDialogItem (GetDialogPtr (), itemNo, &iType, &iHandle, &iRect);
		
		return (iType & itemDisable) ? false : true;
	
	} // end GetItemEnable
	
// ---------------------------------------------------------------------------
//		¥ SetItemEnable
// ---------------------------------------------------------------------------

void
VCSDialog::SetItemEnable (

	DialogItemIndex	itemNo,
	Boolean			itemEnable)
	
	{ // begin SetItemEnable
		
		VCSDialogItem*	item = GetItem (itemNo);
		if (item) {
			item->SetEnable (itemEnable);
			return;
			} // if
			
		short	iType;
		Rect	iRect;
		Handle	iHandle;

		GetDialogItem (GetDialogPtr (), itemNo, &iType, &iHandle, &iRect);
		if (itemEnable)
			iType &= ~itemDisable;
		else iType |= itemDisable;
		SetDialogItem (GetDialogPtr (), itemNo, iType, iHandle, &iRect);
		
	} // end SetItemEnable
	
// ---------------------------------------------------------------------------
//		¥ GetItemValue
// ---------------------------------------------------------------------------

short
VCSDialog::GetItemValue (

	DialogItemIndex	itemNo) const
	
	{ // begin GetItemValue
		
		VCSDialogItem*	item = GetItem (itemNo);
		if (item) return item->GetValue ();
		
		return 0;
	
	} // end GetItemValue
	
// ---------------------------------------------------------------------------
//		¥ SetItemValue
// ---------------------------------------------------------------------------

void
VCSDialog::SetItemValue (

	DialogItemIndex	itemNo,
	short			itemValue)
	
	{ // begin SetItemValue
		
		VCSDialogItem*	item = GetItem (itemNo);
		if (item) item->SetValue (itemValue);
		
	} // end SetItemValue
	
// ---------------------------------------------------------------------------
//		¥ GetItemText
// ---------------------------------------------------------------------------

void
VCSDialog::GetItemText (

	DialogItemIndex	itemNo,
	Str255			itemText) const
	
	{ // begin GetItemText
		
		itemText[0] = 0;
		
		VCSDialogItem*	item = GetItem (itemNo);
		if (item) item->GetDescriptor (itemText);
		
	} // end GetItemText
	
// ---------------------------------------------------------------------------
//		¥ SetItemText
// ---------------------------------------------------------------------------

void
VCSDialog::SetItemText (

	DialogItemIndex		itemNo,
	ConstStr255Param	itemText)
	
	{ // begin SetItemText
		
		VCSDialogItem*	item = GetItem (itemNo);
		if (item) item->SetDescriptor (itemText);
			
	} // end SetItemText
	
// ---------------------------------------------------------------------------
//		¥ OnItemClick
// ---------------------------------------------------------------------------

Boolean 
VCSDialog::OnItemClick (
	
	Point				localWhere,
	EventRecord&		theEvent,
	DialogItemIndex&	itemHit)
	
	{ // begin OnItemClick
		
		VCSDialogItem*	item = GetItem (itemHit);
		if (item && item->GetEnable ()) return item->OnClick (localWhere, theEvent, itemHit);

		return StdFilterProc (GetDialogPtr (), &theEvent, &itemHit);
			
	} // end OnItemClick

// ---------------------------------------------------------------------------
//		¥ OnDialogClick
// ---------------------------------------------------------------------------

Boolean 
VCSDialog::OnDialogClick (

	Point				,
	EventRecord&		theEvent,
	DialogItemIndex&	itemHit)
	
	{ // begin OnDialogClick
		
		return StdFilterProc (GetDialogPtr (), &theEvent, &itemHit);
	
	} // end OnDialogClick

// ---------------------------------------------------------------------------
//		¥ OnIdle
// ---------------------------------------------------------------------------

Boolean 
VCSDialog::OnIdle (

	EventRecord&		theEvent,
	DialogItemIndex&	itemHit)
	
	{ // begin OnIdle
		
		return StdFilterProc (GetDialogPtr (), &theEvent, &itemHit);
	
	} // end OnIdle

// ---------------------------------------------------------------------------
//		¥ OnMouseDown
// ---------------------------------------------------------------------------

Boolean 
VCSDialog::OnMouseDown (

	EventRecord&		theEvent,
	DialogItemIndex&	itemHit)
	
	{ // begin OnMouseDown
		
		Point		localWhere = theEvent.where;
		GlobalToLocal (&localWhere);
		
		itemHit = 1 + FindDialogItem (GetDialogPtr (), localWhere);
		if (itemHit > 0) return OnItemClick (localWhere, theEvent, itemHit);
	
		return OnDialogClick (localWhere, theEvent, itemHit);
	
	} // end OnMouseDown

// ---------------------------------------------------------------------------
//		¥ OnUpdate
// ---------------------------------------------------------------------------

Boolean 
VCSDialog::OnUpdate (

	EventRecord&		theEvent,
	DialogItemIndex&	itemHit)
	
	{ // begin OnUpdate
		
		Boolean	result = StdFilterProc (GetDialogPtr (), &theEvent, &itemHit);
		
		StForeColor	saveFore (&StBackColor::sWhiteColor);
		StBackColor	saveBack;
		
		DialogPtr	d (GetDialogPtr ());
		
		//	Set up the user items
		for (DialogItemIndex i = CountDITL (d); i > 0; --i) {
			short	iType;
			Rect	iRect;
			Handle	iHandle;
			GetDialogItem (d, i, &iType, &iHandle, &iRect);
			if (editText != (iType & ~itemDisable)) continue;
			
			for (int i = 0; i < 2; ++i) {
				InsetRect (&iRect, -1, -1);
				FrameRect (&iRect);
				} // for
			} // for
		
		return result;
		
	} // end OnUpdate

// ---------------------------------------------------------------------------
//		¥ OnActivate
// ---------------------------------------------------------------------------

Boolean 
VCSDialog::OnActivate (

	EventRecord&		theEvent,
	DialogItemIndex&	itemHit)
	
	{ // begin OnActivate
		
		Boolean	result (::StdFilterProc (GetDialogPtr (), &theEvent, &itemHit));
		
		OnItemHit (kFirstTimeItem);
		
		return result;
		
	} // end OnActivate

// ---------------------------------------------------------------------------
//		¥ OnFilterEvent
// ---------------------------------------------------------------------------

Boolean 
VCSDialog::OnFilterEvent (

	EventRecord&		theEvent,
	DialogItemIndex&	itemHit)
	
	{ // begin OnFilterEvent
		
		//	Set up the port
		SavePort	savePort;
		SetPortWindowPort (GetDialogWindow (GetDialogPtr ()));
			
		// is it a mouse click in the persistent answer checkbox?
		switch (theEvent.what) {
			case nullEvent:
				return OnIdle (theEvent, itemHit);
				
			case mouseDown:
				return OnMouseDown (theEvent, itemHit);
				
			case updateEvt:
				return OnUpdate (theEvent, itemHit);
				
			case activateEvt:
				return OnActivate (theEvent, itemHit);
			} // switch
	
		return StdFilterProc (GetDialogPtr (), &theEvent, &itemHit);
	
	} // end OnFilterEvent

// ---------------------------------------------------------------------------
//		¥ OnUserItem
// ---------------------------------------------------------------------------

void 
VCSDialog::OnUserItem (

	DialogItemIndex	itemHit)
	
	{ // begin OnUserItem
		
		//	Set up the port
		SavePort	savePort;
		SetPortWindowPort (GetDialogWindow (GetDialogPtr ()));
			
		VCSDialogItem*	item = GetItem (itemHit);
		if (item) item->OnDraw ();
	
	} // end OnUserItem

// ---------------------------------------------------------------------------
//		¥ OnItemHit
// ---------------------------------------------------------------------------

Boolean 
VCSDialog::OnItemHit (

	DialogItemIndex	)
	
	{ // begin OnItemHit
		
		return true;
		
	} // end OnItemHit
	
// ---------------------------------------------------------------------------
//		¥ PreModalDialog
// ---------------------------------------------------------------------------

Boolean
VCSDialog::PreModalDialog (void)
	
	{ // begin PreModalDialog
		
		return true;
		
	} // end PreModalDialog
	
// ---------------------------------------------------------------------------
//		¥ DoModalDialog
// ---------------------------------------------------------------------------

DialogItemIndex
VCSDialog::DoModalDialog (void)

	{ // begin DoModalDialog
		
		short	itemHit = cancel;
		
		if (!PreModalDialog ()) goto CleanUp;
		
		::ShowWindow (GetDialogWindow (GetDialogPtr ()));
		
		do {
			::ModalDialog (mFilterUPP, &itemHit);
			} while (OnItemHit (itemHit));
	
	CleanUp:
	
		return itemHit;
		
	} // end DoModalDialog

// ---------------------------------------------------------------------------
//		¥ DoModelessDialog
// ---------------------------------------------------------------------------

DialogItemIndex 
VCSDialog::DoModelessDialog (

	EventRecord&		theEvent)
	
	{ // begin DoModelessDialog
		
		DialogItemIndex	result = 0;

		do {
			if (!::IsDialogEvent (&theEvent)) break;
			
			DialogItemIndex	itemHit = 0;
			if (OnFilterEvent (theEvent, itemHit)) {
				result = itemHit;
				break;
				} // if
				
			DialogPtr	d;
			if (!::DialogSelect (&theEvent, &d, &itemHit)) break;
			if (d != GetDialogPtr ()) break;
			
			result = OnItemHit (itemHit) ? 0 : itemHit;
			} while (false);
		
		return result;

	} // end DoModelessDialog

