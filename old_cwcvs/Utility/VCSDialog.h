/*
	File:		VCSDialog.h

	Contains:	General purpose stack dialog handler interface.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1997-8 Electric Fish Inc.  All Rights Reserved.

	Change History (most recent first):

         <8>     10/8/98    rmgw    Convert to v7 API.
         <7>     9/24/98    rmgw    Add idle handling.
         <6>     9/22/98    rmgw    Convert to CWP3.1.
         <5>     2/22/98    rmgw    Fix text backgrounds.
         <4>      2/9/98    rmgw    Add dialog items.
         <3>      2/6/98    rmgw    Add item enabling.
         <2>      2/6/98    rmgw    Seperate contruction and setup.
         <2>      2/6/98    rmgw    Seperate contruction and setup.
         <1>      2/6/98    rmgw    first checked in.
*/


#pragma once

#include <QuickDraw.h>

class SavePort {

		GrafPtr			mPort;
	
	public:
	
						SavePort	(void)	{GetPort (&mPort);};
						~SavePort	(void)	{SetPort (mPort);};
	};

#include "VCSDialogPrep.h"

#include <Dialogs.h>

class VCSDialogItem;

class VCSDialog {
		
		//	Dialog
		VCSDialogPrep	mPrep;
		SavePort		mSavePort;
		DialogPtr		mDialog;
		ModalFilterUPP	mFilterUPP;
		
		//	Items
		VCSDialogItem*	mItems;
		UserItemUPP		mUserUPP;
		
		static	pascal	Boolean 
						DialogFilterProc(DialogPtr 				theDialog, 
										 EventRecord*			theEvent,
										 DialogItemIndex*		itemHit);
	
		static	pascal	void 
						UserItemProc	(DialogPtr 				theDialog, 
										 DialogItemIndex		itemNo);
	
	protected:
		
		//	Mouse handling
		virtual	Boolean OnItemClick		(Point					localWhere,
										 EventRecord&			theEvent,
										 DialogItemIndex&		itemHit);
		virtual	Boolean OnDialogClick	(Point					localWhere,
										 EventRecord&			theEvent,
										 DialogItemIndex&		itemHit);
		virtual	Boolean OnMouseDown		(EventRecord&			theEvent,
										 DialogItemIndex&		itemHit);
		
		//	Events
		virtual	Boolean OnIdle			(EventRecord&			theEvent,
										 DialogItemIndex&		itemHit);
		virtual	Boolean OnUpdate		(EventRecord&			theEvent,
										 DialogItemIndex&		itemHit);
		virtual	Boolean OnFilterEvent	(EventRecord&			theEvent,
										 DialogItemIndex&		itemHit);
		virtual	void	OnUserItem		(DialogItemIndex		itemHit);
		
		//	Modal dialog handling
		virtual	Boolean OnItemHit 		(DialogItemIndex		inItemHit);
		virtual	Boolean	PreModalDialog	(void);
		
	public:
		
		static	void	SetParamText	(ConstStr255Param		param1 = nil,
										 ConstStr255Param		param2 = nil,
										 ConstStr255Param		param3 = nil,
										 ConstStr255Param		param4 = nil)
										{::ParamText (param1, param2, param3, param4);};
										 
						VCSDialog		(const	VCSContext&		inContext,
										 short					inDLOGid);
		virtual			~VCSDialog		(void);
		
		//	Item management
		VCSDialogItem*	GetItem 		(DialogItemIndex		itemNo) const;
		void			AddItem			(VCSDialogItem*			inItem);
		void			RemoveItem		(VCSDialogItem*			inItem);
		
		//	Access
		DialogPtr		GetDialogPtr	(void) const;
		
		Boolean			GetItemEnable	(DialogItemIndex		itemNo) const;
		void			SetItemEnable	(DialogItemIndex		itemNo,
										 Boolean				itemEnable);
										 
		short			GetItemValue	(DialogItemIndex		itemNo) const;
		void			SetItemValue	(DialogItemIndex		itemNo,
										 short					itemValue);
										 
		void			GetItemText		(DialogItemIndex		itemNo,
										 Str255					itemText) const;
		void			SetItemText		(DialogItemIndex		itemNo,
										 ConstStr255Param		itemText);

		//	Actions
		virtual	DialogItemIndex	
						DoModalDialog 	(void);
	};


//	===	Inlines	===

inline DialogPtr 
VCSDialog::GetDialogPtr (void) const
	{return mDialog;};
	
