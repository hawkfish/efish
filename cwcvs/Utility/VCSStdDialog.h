/*
	File:		VCSStdDialog.h

	Contains:	Base dialog with standard buttons.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1997-2001 Electric Fish Inc.  All Rights Reserved.

	Change History (most recent first):

         <2>     10/8/98    rmgw    Convert to v7 API.
         <1>      2/9/98    rmgw    first checked in.
*/


#pragma once

#include "VCSDialog.h"

#include "VCSDialogControlItem.h"

class VCSStdDialog : public VCSDialog {
		
		VCSDialogControlItem	mOKButton;
		VCSDialogControlItem	mCancelButton;
		
	protected:
		
		//	Modal dialog handling
		virtual	Boolean OnOK 			(void);
		virtual	Boolean OnCancel 		(void);
		
		virtual	Boolean OnItemHit 		(DialogItemIndex		inItemHit);
		virtual	Boolean	PreModalDialog	(void);
		
	public:
		
		enum {
			kVCSStdOKItem = ok,
			kVCSStdCancelItem = cancel,
			kVCSStdFirstFreeItem
			};
			
						VCSStdDialog	(const VCSContext&		inPB,
										 short					inDLOGid);
		virtual			~VCSStdDialog	(void);
		
	};

