/*
	File:		VCSDialogTextEditItem.h

	Contains:	Interface to a Dialog box edit text item.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1997-9 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

         <4>      4/1/99    rmgw    Add TECalText accessor.
         <3>     3/30/99    rmgw    Crash recovery merging.
         <2>     3/22/99    rmgw    Normalize scroll values.
         <1>     3/22/99    rmgw    first checked in.
*/


#pragma once

#include "VCSDialogItem.h"

#include "VCSDialogScrollbarItem.h"

class VCSDialogTextEditItem : public VCSDialogItem
							, public VCSScrollTarget

	{
		
		TEHandle				mTE;
		VCSDialogScrollbarItem*	mScroll;
		SInt16					mScrollUnits;
		
	public:
	
						VCSDialogTextEditItem	(VCSDialog&				inDialog,
												 DialogItemIndex		inIndex,
												 VCSDialogScrollbarItem*inScroll = 0);
		virtual			~VCSDialogTextEditItem	(void);


		Handle			GetText		 			(void) const;
		
		//	Operations
		virtual	short	GetValue				(void) const;
		virtual	void	SetValue				(DialogItemIndex		itemValue = 0);
		
		virtual	void	GetDescriptor			(Str255					itemText) const;
		virtual	void	SetDescriptor			(ConstStr255Param		itemText);

		virtual	void	OnDraw					(void);
		
		//	Text access
		virtual	Size	GetLength				(void) const;
		 
		virtual	void	GetSelect				(Size&					selStart, 
												 Size&					selEnd) const;
		virtual	void	SetSelect				(Size					selStart = 0, 
												 Size					selEnd = 0x7FFF);
		virtual	StScrpHandle
						GetStyleScrapHandle 	(void) const;
						
		virtual	TEStyleHandle	
						GetStyleHandle			(void) const;
		virtual	void	SetStyleHandle			(TEStyleHandle			inStyle);
						
		virtual	void	SetStyle				(const	TextStyle&		inStyle,
												 short					inMode = doAll,
												 Boolean				inRedraw = true);
		
		virtual	void	Insert					(const	char*			inText,
												 Size					inLength = 0);
		virtual	void	Cut						(void);
		virtual	void	Copy					(void) const;
		virtual	void	Paste					(void);
		virtual	void	Clear					(void);
												 
		//	VCSScrollTarget
		virtual	SInt16	GetScrollMin			(void) const;
		virtual	SInt16	GetScrollMax			(void) const;
		virtual	SInt16	GetScrollValue			(void) const;

		virtual	void	ScrollTo				(short					inLine);
		virtual	void	ScrollBy				(short					inDelta);
		
		virtual	void	CalText					(void);
	};
	
