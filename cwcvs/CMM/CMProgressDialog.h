/*
	File:		CMProgressDialog.h

	Contains:	Interface to the CMM Progress dialog.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1997-9 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

         <2>     3/31/99    rmgw    Add progress bar and stop button.
         <1>     3/22/99    rmgw    first checked in.
*/


#include "VCSDialog.h"

#include "VCSDialogTextItem.h"
#include "VCSDialogControlItem.h"

class CMProgressDialog : public VCSDialog

	{
	
	protected:
	
		enum {
			kProgressStopItem = ok,
			kProgressLine1Item,
			kProgressLine2Item,
			kProgressBarItem,
			
			kResourceID = 2100
			};
		
		VCSDialogControlItem	mStopButton;
		VCSDialogTextItem		mLine1;
		VCSDialogTextItem		mLine2;
		VCSDialogControlItem	mProgressBar;
		
		//	Events
		virtual	Boolean 		OnIdle				(EventRecord&				theEvent,
													 DialogItemIndex&			itemHit);

		//	Modal dialog handling
		virtual	Boolean 		OnStop 				(void);
		
		virtual	Boolean 		OnItemHit 			(DialogItemIndex			inItemHit);
		
	public:
	
								CMProgressDialog	(const	VCSContext&			inContext);
		virtual					~CMProgressDialog	(void);
		
		virtual	void			ReportProgress		(const char *				line1,
													 const char *				line2 = 0);

		virtual	void			CompletionRatio 	(SInt16 					total, 
													 SInt16 					completed);

	};
	
