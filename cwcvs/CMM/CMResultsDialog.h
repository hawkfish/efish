/*
	File:		CMResultsDialog.h

	Contains:	Implementation of the CMM progress dialog.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1997-9 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

         <4>      5/6/99    rmgw    Use Nav services to anchor the save dialog.
         <3>      4/7/99    rmgw    Support CreateDocument.
         <2>     3/30/99    rmgw    Crash recovery merging.
         <1>     3/22/99    rmgw    first checked in.
*/


#pragma once

#include "VCSDialog.h"

#include "VCSDialogScrollbarItem.h"
#include "VCSDialogTextEditItem.h"

class CMResultsDialog : public VCSDialog

	{
	
	protected:
	
		enum {
			kResultsItem = ok + 1,
			kScrollItem,
			kCopyItem,
			kSaveItem,
			
			kResourceID = 2200
			};
			
		const	VCSContext&		mContext;

		VCSDialogControlItem	mOKButton;
		VCSDialogScrollbarItem	mTextScroll;
		VCSDialogTextEditItem	mText;
		VCSDialogControlItem	mCopyButton;
		VCSDialogControlItem	mSaveButton;
		
		//	Events
		virtual	Boolean 		OnFilterEvent		(EventRecord&			theEvent,
													 DialogItemIndex&		itemHit);

		//	Modal dialog handling
		virtual	void			OnSave				(void);
		virtual	Boolean 		OnOK 				(void);
		
		virtual	Boolean 		OnItemHit 			(DialogItemIndex		inItemHit);
		virtual	Boolean			PreModalDialog		(void);

	public:
		
		static	OSErr			DoSaveText			(ConstStringPtr				inDefaultName,
													 Handle						inText,
													 StScrpHandle				inStyle = nil,
													 const	FSSpec*				inLocation = 0);
													 
								CMResultsDialog		(const	VCSContext&			inContext);
		virtual					~CMResultsDialog	(void);
		
		virtual	void			MessageOutput 		(short 						errorlevel,
													 const char *				line1, 
													 const char *				line2, 
													 long 						errorNumber = 0);

		virtual	void			CreateDocument		(const	char*				inName,
													 Handle						inData,
													 Boolean					inDirty = false);
	};
