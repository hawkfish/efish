/*
	File:		VCSDialogItem.h

	Contains:	Base dialog item representation.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1997-8 Electric Fish Inc.  All Rights Reserved.

	Change History (most recent first):

         <3>     12/4/98    rmgw    Add Show/Hide operations.
         <2>    10/22/98    rmgw    Add Get/SetBounds.
         <1>      2/9/98    rmgw    first checked in.
*/


#pragma once

#include "VCSDialog.h"

class VCSDialogItem

	{
		
		VCSDialog&				mDialog;
		const	DialogItemIndex	mIndex;
		VCSDialogItem*			mNext;
	
	public:
	
								VCSDialogItem	(VCSDialog&				inDialog,
												 DialogItemIndex		inIndex);
		virtual					~VCSDialogItem	(void);
		
		//	List management
		VCSDialog&				GetDialog		(void) const;
		DialogPtr				GetDialogPtr	(void) const;
		DialogItemIndex			GetIndex		(void) const;

		VCSDialogItem*			GetNext			(void) const;
		void					SetNext			(VCSDialogItem*			inNext = 0);
		
		//	Operations
		virtual	void			Show			(void);
		virtual	void			Hide			(void);

		virtual	Boolean			GetEnable		(void) const;
		virtual	void			SetEnable		(Boolean				itemEnable = true);
										 
		virtual	void			GetBounds		(Rect&					outBounds) const;
		virtual	void			SetBounds		(const	Rect&			inBounds);

		virtual	short			GetValue		(void) const;
		virtual	void			SetValue		(short					itemValue = 0);
										 
		virtual	void			GetDescriptor	(Str255					itemText) const;
		virtual	void			SetDescriptor	(ConstStr255Param		itemText);
		
		//	Events
		virtual	Boolean 		OnClick			(Point					localWhere,
												 EventRecord&			theEvent,
												 DialogItemIndex&		itemHit);
		virtual	void	 		OnDraw			(void);
	};
	
//	Inline functions
inline VCSDialog&
VCSDialogItem::GetDialog (void) const
	{return mDialog;}

inline DialogPtr
VCSDialogItem::GetDialogPtr (void) const
	{return GetDialog ().GetDialogPtr ();}

inline DialogItemIndex
VCSDialogItem::GetIndex (void) const
	{return mIndex;}

inline VCSDialogItem* 
VCSDialogItem::GetNext (void) const
	{return mNext;}
	
inline void
VCSDialogItem::SetNext (VCSDialogItem* inNext)
	{mNext = inNext;}
