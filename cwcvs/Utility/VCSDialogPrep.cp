/*
	File:		VCSDialogPrep.cp

	Contains:	Stack object for IDE dialog activation.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1997-8 Electric Fish Inc.  All Rights Reserved.

	Change History (most recent first):

         <4>     12/4/98    rmgw    Check request before calling.
         <3>     10/8/98    rmgw    Convert to v7 API.
         <2>     2/26/98    rmgw    Check for nil PB.
         <1>      2/6/98    rmgw    first checked in.
*/


#include "VCSDialogPrep.h"

// ---------------------------------------------------------------------------
//		¥ VCSDialogPrep
// ---------------------------------------------------------------------------

VCSDialogPrep::VCSDialogPrep (

	const	VCSContext&	inContext)
	
	: mContext (inContext)
	
	{ // begin VCSDialogPrep
		
		switch (mContext.GetRequest ()) {
			case reqInitialize:
			case reqTerminate:
				break;
				
			default:
				mContext.PreDialog ();
				break;
			} // switch
			
	} // end VCSDialogPrep
	
// ---------------------------------------------------------------------------
//		¥ ~VCSDialogPrep
// ---------------------------------------------------------------------------

VCSDialogPrep::~VCSDialogPrep (void)
	
	{ // begin ~VCSDialogPrep
	
		switch (mContext.GetRequest ()) {
			case reqInitialize:
			case reqTerminate:
				break;
				
			default:
				mContext.PostDialog ();
				break;
			} // switch
			
	} // end ~VCSDialogPrep