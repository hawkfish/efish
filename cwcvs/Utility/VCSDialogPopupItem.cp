#include "VCSDialogPopupItem.h"

#include <ControlDefinitions.h>

// ---------------------------------------------------------------------------
//		€ VCSDialogPopupItem
// ---------------------------------------------------------------------------

VCSDialogPopupItem::VCSDialogPopupItem (

	VCSDialog&			inDialog,
	DialogItemIndex		inIndex)
	
	: VCSDialogControlItem (inDialog, inIndex)
	
	{ // begin VCSDialogPopupItem
		
	} // end VCSDialogPopupItem
	
// ---------------------------------------------------------------------------
//		€ ~VCSDialogPopupItem
// ---------------------------------------------------------------------------

VCSDialogPopupItem::~VCSDialogPopupItem (void)

	{ // begin ~VCSDialogPopupItem
		
	} // end ~VCSDialogPopupItem

// ---------------------------------------------------------------------------
//		€ GetMenu
// ---------------------------------------------------------------------------

MenuHandle
VCSDialogPopupItem::GetMenu (void) const
	
	{ // begin GetMenu
		
		return (**((PopupPrivateDataHandle) (**GetControl ()).contrlData)).mHandle;
					
	} // end GetMenu

// ---------------------------------------------------------------------------
//		€ GetDescriptor
// ---------------------------------------------------------------------------

void
VCSDialogPopupItem::GetDescriptor (

	Str255	itemText) const
	
	{ // begin GetDescriptor
		
		VCSDialogItem::GetDescriptor (itemText);
		
		GetMenuItemText (GetMenu (), GetValue (), itemText);
		
	} // end GetDescriptor
	
// ---------------------------------------------------------------------------
//		€ SetDescriptor
// ---------------------------------------------------------------------------

void
VCSDialogPopupItem::SetDescriptor (

	ConstStr255Param	itemText)
	
	{ // begin SetDescriptor
		
		SetMenuItemText (GetMenu (), GetValue (), itemText);
		
	} // end SetDescriptor


