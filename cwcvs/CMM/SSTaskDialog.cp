/*
	File:		SSTaskDialog.cp

	Contains:	Implementation of the Comment/Task dialog.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1997-8 Electric Fish Inc. All Rights Reserved.

	Change History (most recent first):

         <1>     3/30/99    rmgw    first checked in.
         <1>     3/23/99    rmgw    first checked in.
*/


#include "SSTaskDialog.h"

// ---------------------------------------------------------------------------
//		€ SSTaskDialog
// ---------------------------------------------------------------------------

SSTaskDialog::SSTaskDialog (

	
	const VCSContext&	inPB,
	short				inDLOGid,
	Size				inSize)
	
	: VCSStdDialog (inPB, inDLOGid)
	
	, mSize (inSize)
	, mStringText (*this, kTaskDialogTextItem)
	, mRepeat (*this, kTaskDialogUseForAllItem)
	
	{ // begin SSTaskDialog
	
	} // end SSTaskDialog
	
// ---------------------------------------------------------------------------
//		€ OnItemHit
// ---------------------------------------------------------------------------

Boolean 
SSTaskDialog::OnItemHit (

	short	inItemHit)
	
	{ // begin OnItemHit
		
		Str255		text;

		switch (inItemHit) {
			case kTaskDialogTextItem:
				GetItemText (kTaskDialogTextItem, text);
				SetItemEnable (ok, text[0] < mSize);
				break;
			} // switch
		
		return VCSStdDialog::OnItemHit (inItemHit);
		
	} // end OnItemHit

// ---------------------------------------------------------------------------
//		€ DoDialog
// ---------------------------------------------------------------------------

Boolean 
SSTaskDialog::DoDialog (
	
	const 	VCSContext&		inPB,
	ResID					inDLOGid,
	ConstStr255Param		inPrompt,
	ConstStr255Param		inFileTask,
	const	long			inTaskSize,
	Boolean&				ioUseForAll,
	StringPtr				ioPreviousTask)
	
	{ // begin DoDialog
		
		if (ioUseForAll) return true;

		VCSDialog::SetParamText (inPrompt);
		SSTaskDialog	d (inPB, inDLOGid, inTaskSize);
		d.SetItemText (kTaskDialogTextItem, inFileTask[0] ? inFileTask : ioPreviousTask);
		SelectDialogItemText (d.GetDialogPtr (), kTaskDialogTextItem, 0, 0x7FFF);
		if (ok != d.DoModalDialog ()) return false;
		
		ioUseForAll = d.GetItemValue (kTaskDialogUseForAllItem) ? true : false;
		d.GetItemText (kTaskDialogTextItem, ioPreviousTask);
		
	CleanUp:
	
		return true;
	
	} // end DoDialog
