/*
	File:		VCSStdDialog.cp

	Contains:	Base dialog with standard buttons.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1997-8 Electric Fish Inc.  All Rights Reserved.

	Change History (most recent first):

         <3>    10/23/98    rmgw    Add default font.
         <2>     10/8/98    rmgw    Convert to v7 API.
         <1>      2/9/98    rmgw    first checked in.
*/


#include "VCSStdDialog.h"

#include <Fonts.h>

// ---------------------------------------------------------------------------
//		¥ VCSStdDialog
// ---------------------------------------------------------------------------

VCSStdDialog::VCSStdDialog (

	const	VCSContext&		inPB,
	short					inDLOGid)
	
	: VCSDialog (inPB, inDLOGid)
	
	, mOKButton (*this, ok)
	, mCancelButton (*this, cancel)
	
	{ // begin VCSStdDialog
		
		::SetPortWindowPort (GetDialogWindow (GetDialogPtr ()));
		::TextFont (kPlatformDefaultGuiFontID);
		::TextFace (bold);
		::TextSize (10);
		
	} // end VCSStdDialog
	
// ---------------------------------------------------------------------------
//		¥ ~VCSStdDialog
// ---------------------------------------------------------------------------

VCSStdDialog::~VCSStdDialog (void)

	{ // begin ~VCSStdDialog
	
	} // end ~VCSStdDialog

// ---------------------------------------------------------------------------
//		¥ OnOK
// ---------------------------------------------------------------------------

Boolean 
VCSStdDialog::OnOK (void)
	
	{ // begin OnOK
	
		return false;
				
	} // end OnOK
	
// ---------------------------------------------------------------------------
//		¥ OnCancel
// ---------------------------------------------------------------------------

Boolean 
VCSStdDialog::OnCancel (void)
	
	{ // begin OnCancel
	
		return false;
				
	} // end OnCancel
	
// ---------------------------------------------------------------------------
//		¥ OnItemHit
// ---------------------------------------------------------------------------

Boolean 
VCSStdDialog::OnItemHit (

	DialogItemIndex	inItemHit)
	
	{ // begin OnItemHit
		
		switch (inItemHit) {
			case ok:
				return OnOK ();
			
			case cancel:
				return OnCancel ();
			} // switch
		
		return VCSDialog::OnItemHit (inItemHit);
		
	} // end OnItemHit
	
// ---------------------------------------------------------------------------
//		¥ PreModalDialog
// ---------------------------------------------------------------------------

Boolean
VCSStdDialog::PreModalDialog (void)
	
	{ // begin PreModalDialog
		
		//	Initialize the default buttons
		SetDialogDefaultItem (GetDialogPtr (), ok);
		SetDialogCancelItem (GetDialogPtr (), cancel);
		
		return VCSDialog::PreModalDialog ();
		
	} // end true
	
