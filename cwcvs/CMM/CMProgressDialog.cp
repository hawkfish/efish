/*
	File:		CMProgressDialog.cp

	Contains:	Implementation of the CMM Progress dialog.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1997-9 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

         <3>     3/31/99    rmgw    Add progress bar and stop button.
         <2>     3/22/99    rmgw    Grayscale dialogs.
         <1>     3/22/99    rmgw    first checked in.
*/


#include "CMProgressDialog.h"

#include <ControlDefinitions.h>
#include <Fonts.h>

#include <string.h>

// ---------------------------------------------------------------------------
//		€ CMProgressDialog
// ---------------------------------------------------------------------------

CMProgressDialog::CMProgressDialog (

	const VCSContext&	inContext)
	
	: VCSDialog (inContext, kResourceID)
	
	, mStopButton (*this, kProgressStopItem)
	, mLine1 (*this, kProgressLine1Item)
	, mLine2 (*this, kProgressLine2Item)
	, mProgressBar (*this, kProgressBarItem)
	
	{ // begin CMProgressDialog
	
		::SetPort (GetDialogPtr ());
		::TextFont (kPlatformDefaultGuiFontID);
		::TextFace (bold);
		::TextSize (10);
		
		Boolean	indet = true;
		::SetControlData (mProgressBar.GetControl (), kControlNoPart, kControlProgressBarIndeterminateTag, sizeof (indet), (Ptr) &indet);
		
		::SetDialogCancelItem (GetDialogPtr (), kProgressStopItem);

	} // end CMProgressDialog
	
// ---------------------------------------------------------------------------
//		€ ~CMProgressDialog
// ---------------------------------------------------------------------------

CMProgressDialog::~CMProgressDialog (void)

	{ // begin ~CMProgressDialog
	
	} // end ~CMProgressDialog

// ---------------------------------------------------------------------------
//		€ OnIdle
// ---------------------------------------------------------------------------

Boolean 
CMProgressDialog::OnIdle (

	EventRecord&		theEvent,
	DialogItemIndex&	itemHit)
	
	{ // begin OnIdle
		
		::SendControlMessage (mProgressBar.GetControl (), kControlMsgIdle, 0);

		return VCSDialog::OnIdle (theEvent, itemHit);
	
	} // end OnIdle

// ---------------------------------------------------------------------------
//		€ OnStop
// ---------------------------------------------------------------------------

Boolean 
CMProgressDialog::OnStop (void)
	
	{ // begin OnStop
	
		return false;
				
	} // end OnStop
	
// ---------------------------------------------------------------------------
//		€ OnItemHit
// ---------------------------------------------------------------------------

Boolean 
CMProgressDialog::OnItemHit (

	DialogItemIndex	inItemHit)
	
	{ // begin OnItemHit
		
		switch (inItemHit) {
			case ok:
				return OnStop ();
			} // switch
			
		return VCSDialog::OnItemHit (inItemHit);
		
	} // end OnItemHit
	
// ---------------------------------------------------------------------------
//		€ ReportProgress
// ---------------------------------------------------------------------------

void
CMProgressDialog::ReportProgress (

	const char *line1, 
	const char *line2)
	
	{ // begin ReportProgress
		
		Str255	text;
		
		if (line1)
			::BlockMoveData (line1, text + 1, text[0] = ::strlen (line1));
		else text[0] = 0;
		SetItemText (kProgressLine1Item, text);

		if (line2)
			::BlockMoveData (line2, text + 1, text[0] = ::strlen (line2));
		else text[0] = 0;
		SetItemText (kProgressLine2Item, text);
		
		::SendControlMessage (mProgressBar.GetControl (), kControlMsgIdle, 0);
		
	} // end ReportProgress

// ---------------------------------------------------------------------------
//		€ CompletionRatio
// ---------------------------------------------------------------------------

void
CMProgressDialog::CompletionRatio (

	SInt16 	total, 
	SInt16 	completed)
	
	{ // begin CompletionRatio

		Boolean	indet = (total < 2);
		::SetControlData (mProgressBar.GetControl (), kControlNoPart, kControlProgressBarIndeterminateTag, sizeof (indet), (Ptr) &indet);
		mProgressBar.SetMinValue (0);
		mProgressBar.SetMaxValue (total);
		mProgressBar.SetMaxValue (completed);
		
	} // end CompletionRatio

