/*
	File:		VCSDialogCheckboxItem.cp

	Contains:	Dialog item for checkboxes.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1997-8 Electric Fish Inc.  All Rights Reserved.

	Change History (most recent first):

         <1>      2/9/98    rmgw    first checked in.
*/


#include "VCSDialogCheckboxItem.h"

// ---------------------------------------------------------------------------
//		€ VCSDialogCheckboxItem
// ---------------------------------------------------------------------------

VCSDialogCheckboxItem::VCSDialogCheckboxItem (

	VCSDialog&			inDialog,
	DialogItemIndex		inIndex)
	
	: VCSDialogControlItem (inDialog, inIndex)
	
	{ // begin VCSDialogCheckboxItem
		
	} // end VCSDialogCheckboxItem
	
// ---------------------------------------------------------------------------
//		€ ~VCSDialogCheckboxItem
// ---------------------------------------------------------------------------

VCSDialogCheckboxItem::~VCSDialogCheckboxItem (void)

	{ // begin ~VCSDialogCheckboxItem
		
	} // end ~VCSDialogCheckboxItem

// ---------------------------------------------------------------------------
//		€ OnClick
// ---------------------------------------------------------------------------

Boolean 
VCSDialogCheckboxItem::OnClick (
	
	Point			localWhere,
	EventRecord&	theEvent,
	short&			itemHit)
	
	{ // begin OnClick
		
		ControlHandle ch = GetControl ();
		if (0 == TrackControl (ch, localWhere, nil) != 0) 
			return VCSDialogControlItem::OnClick (localWhere, theEvent, itemHit);
			
		SetControlValue (ch, GetControlValue(ch) ? 0 : 1);
		theEvent.what = nullEvent; /* handled it */
		return true;
			
	} // end OnClick
