/*
	File:		SSTaskDialog.h

	Contains:	Interface to the Comment/Task dialog.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1997-8 Electric Fish Inc. All Rights Reserved.

	Change History (most recent first):

         <1>     3/30/99    rmgw    first checked in.
         <1>     3/23/99    rmgw    first checked in.
*/


#pragma once

#include "VCSStdDialog.h"

#include "VCSDialogCheckboxItem.h"
#include "VCSDialogTextItem.h"

#include <Resources.h>

class SSTaskDialog : public VCSStdDialog

	{
		
		VCSDialogTextItem		mStringText;
		VCSDialogCheckboxItem	mRepeat;
		
		const	Size			mSize;
		
	protected:
		
		//	Modal dialog handling
		virtual	Boolean OnItemHit 				(DialogItemIndex		inItemHit);

	public:
	
		enum TaskDialogItems {
			kTaskDialogCancelItem = cancel,
			kTaskDialogPromptItem,
			kTaskDialogTextItem,
			kTaskDialogUseForAllItem
			};
	
						SSTaskDialog			(const VCSContext&		inPB,
												 ResID					inDLOGid,
												 Size					inSize);

		static	Boolean DoDialog 				(const 	VCSContext&		inPB,
												 ResID					inDLOGid,
												 ConstStr255Param		inPrompt,
												 ConstStr255Param		inFileTask,
												 const	long			inTaskSize,
												 Boolean&				ioUseForAll,
												 StringPtr				ioPreviousTask);
	};
	
	
