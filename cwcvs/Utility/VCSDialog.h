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
		
		enum {kFirstTimeItem = -1};
		
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
		virtual	Boolean OnActivate		(EventRecord&			theEvent,
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
	
