/*
	File:		VCSDialogControlItem.cp

	Contains:	Dialog item for controls.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1997-8 Electric Fish Inc.  All Rights Reserved.

	Change History (most recent first):

         <2>     3/22/99    rmgw    Add Min/Max access.
         <1>      2/9/98    rmgw    first checked in.
*/


#include "VCSDialogControlItem.h"

// ---------------------------------------------------------------------------
//		€ VCSDialogControlItem
// ---------------------------------------------------------------------------

VCSDialogControlItem::VCSDialogControlItem (

	VCSDialog&			inDialog,
	DialogItemIndex		inIndex)
	
	: VCSDialogItem (inDialog, inIndex)
	
	{ // begin VCSDialogControlItem
		
	} // end VCSDialogControlItem
	
// ---------------------------------------------------------------------------
//		€ ~VCSDialogControlItem
// ---------------------------------------------------------------------------

VCSDialogControlItem::~VCSDialogControlItem (void)

	{ // begin ~VCSDialogControlItem
		
	} // end ~VCSDialogControlItem

// ---------------------------------------------------------------------------
//		€ GetControl
// ---------------------------------------------------------------------------

ControlHandle
VCSDialogControlItem::GetControl (void) const
	
	{ // begin GetControl
		
		short	iType;
		Rect	iRect;
		Handle	iHandle;

		GetDialogItem (GetDialogPtr (), GetIndex (), &iType, &iHandle, &iRect);
		
		return (iType & ctrlItem) 
					? (ControlHandle) iHandle
					: nil;
					
	} // end GetControl
	
// ---------------------------------------------------------------------------
//		€ SetEnable
// ---------------------------------------------------------------------------

void
VCSDialogControlItem::SetEnable (

	Boolean	itemEnable)
	
	{ // begin SetEnable
		
		VCSDialogItem::SetEnable (itemEnable);
		HiliteControl (GetControl (), itemEnable ? kControlNoPart : kControlInactivePart);

	} // end SetEnable
	
// ---------------------------------------------------------------------------
//		€ GetMinValue
// ---------------------------------------------------------------------------

short
VCSDialogControlItem::GetMinValue (void) const
	
	{ // begin GetMinValue
		
		return GetControlMinimum (GetControl ());
	
	} // end GetMinValue
	
// ---------------------------------------------------------------------------
//		€ SetMinValue
// ---------------------------------------------------------------------------

void
VCSDialogControlItem::SetMinValue (

	short	inMin)
	
	{ // begin SetMinValue
		
		SetControlMinimum (GetControl (), inMin);
	
	} // end SetMinValue

// ---------------------------------------------------------------------------
//		€ GetMaxValue
// ---------------------------------------------------------------------------

short
VCSDialogControlItem::GetMaxValue (void) const
	
	{ // begin GetMaxValue
		
		return GetControlMaximum (GetControl ());
	
	} // end GetMaxValue
	
// ---------------------------------------------------------------------------
//		€ SetMaxValue
// ---------------------------------------------------------------------------

void
VCSDialogControlItem::SetMaxValue (

	short	inMax)
	
	{ // begin SetMaxValue
		
		SetControlMaximum (GetControl (), inMax);
	
	} // end SetMaxValue

// ---------------------------------------------------------------------------
//		€ GetValue
// ---------------------------------------------------------------------------

short
VCSDialogControlItem::GetValue (void) const
	
	{ // begin GetValue
		
		return GetControlValue (GetControl ());
	
	} // end GetValue
	
// ---------------------------------------------------------------------------
//		€ SetValue
// ---------------------------------------------------------------------------

void
VCSDialogControlItem::SetValue (

	short	inValue)
	
	{ // begin SetValue
		
		SetControlValue (GetControl (), inValue);
	
	} // end SetValue

// ---------------------------------------------------------------------------
//		€ GetDescriptor
// ---------------------------------------------------------------------------

void
VCSDialogControlItem::GetDescriptor (

	Str255	itemText) const
	
	{ // begin GetDescriptor
		
		GetControlTitle (GetControl (), itemText);
		
	} // end GetDescriptor
	
// ---------------------------------------------------------------------------
//		€ SetDescriptor
// ---------------------------------------------------------------------------

void
VCSDialogControlItem::SetDescriptor (

	ConstStr255Param	itemText)
	
	{ // begin SetDescriptor
		
		SetControlTitle (GetControl (), itemText);
		
	} // end SetDescriptor
	
