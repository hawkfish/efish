/*
	File:		VCSDialogTextItem.cp

	Contains:	Dialog item for text.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1997-8 Electric Fish Inc.  All Rights Reserved.

	Change History (most recent first):

         <3>     12/4/98    rmgw    Check text being set to avoid flicker.
         <2>     9/22/98    rmgw    Convert to CWP3.1.
         <1>      2/9/98    rmgw    first checked in.
*/


#include "VCSDialogTextItem.h"

#include <TextUtils.h>

// ---------------------------------------------------------------------------
//		€ VCSDialogTextItem
// ---------------------------------------------------------------------------

VCSDialogTextItem::VCSDialogTextItem (

	VCSDialog&			inDialog,
	DialogItemIndex		inIndex)
	
	: VCSDialogItem (inDialog, inIndex)
	
	{ // begin VCSDialogTextItem
		
	} // end VCSDialogTextItem
	
// ---------------------------------------------------------------------------
//		€ ~VCSDialogTextItem
// ---------------------------------------------------------------------------

VCSDialogTextItem::~VCSDialogTextItem (void)

	{ // begin ~VCSDialogTextItem
		
	} // end ~VCSDialogTextItem

// ---------------------------------------------------------------------------
//		€ GetText
// ---------------------------------------------------------------------------

Handle
VCSDialogTextItem::GetText (void) const
	
	{ // begin GetText
		
		short	iType;
		Rect	iRect;
		Handle	iHandle;

		GetDialogItem (GetDialogPtr (), GetIndex (), &iType, &iHandle, &iRect);
		
		return (iType & (statText | editText)) 
					? iHandle
					: nil;
					
	} // end GetText
	
// ---------------------------------------------------------------------------
//		€ GetValue
// ---------------------------------------------------------------------------

short
VCSDialogTextItem::GetValue (void) const
	
	{ // begin GetValue
		
		long	value;
		Str255	text;
		GetDescriptor (text);
		StringToNum (text, &value);
		
		return value;
	
	} // end GetValue
	
// ---------------------------------------------------------------------------
//		€ SetValue
// ---------------------------------------------------------------------------

void
VCSDialogTextItem::SetValue (

	short	inValue)
	
	{ // begin SetValue
		
		Str255	text;
		::NumToString (inValue, text);
		SetDescriptor (text);
	
	} // end SetValue

// ---------------------------------------------------------------------------
//		€ GetDescriptor
// ---------------------------------------------------------------------------

void
VCSDialogTextItem::GetDescriptor (

	Str255	itemText) const
	
	{ // begin GetDescriptor
		
		GetDialogItemText (GetText (), itemText);
		
	} // end GetDescriptor
	
// ---------------------------------------------------------------------------
//		€ SetDescriptor
// ---------------------------------------------------------------------------

void
VCSDialogTextItem::SetDescriptor (

	ConstStr255Param	itemText)
	
	{ // begin SetDescriptor
		
		//	Avoid flicker
		Str255	oldText;
		GetDescriptor (oldText);
		if (::EqualString (oldText, itemText, true, true)) return;
		
		SetDialogItemText (GetText (), itemText);
		
	} // end SetDescriptor
	
