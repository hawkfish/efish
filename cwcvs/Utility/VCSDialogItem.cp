#include "VCSDialogItem.h"

#include "VCSDialog.h"

// ---------------------------------------------------------------------------
//		¥ VCSDialogItem
// ---------------------------------------------------------------------------

VCSDialogItem::VCSDialogItem (

	VCSDialog&			inDialog,
	DialogItemIndex		inIndex)
	
	: mDialog (inDialog)
	, mIndex (inIndex)
	
	{ // begin VCSDialogItem
		
		GetDialog ().AddItem (this);
		
	} // end VCSDialogItem
	
// ---------------------------------------------------------------------------
//		¥ ~VCSDialogItem
// ---------------------------------------------------------------------------

VCSDialogItem::~VCSDialogItem (void)

	{ // begin ~VCSDialogItem
		
		GetDialog ().RemoveItem (this);

	} // end ~VCSDialogItem

// ---------------------------------------------------------------------------
//		¥ GetEnable
// ---------------------------------------------------------------------------

Boolean
VCSDialogItem::GetEnable (void) const
	
	{ // begin GetEnable
		
		short	iType;
		Rect	iRect;
		Handle	iHandle;

		GetDialogItem (GetDialogPtr (), GetIndex (), &iType, &iHandle, &iRect);
		
		return (iType & itemDisable) ? false : true;
	
	} // end GetEnable
	
// ---------------------------------------------------------------------------
//		¥ SetEnable
// ---------------------------------------------------------------------------

void
VCSDialogItem::SetEnable (

	Boolean	itemEnable)
	
	{ // begin SetEnable
		
		short	iType;
		Rect	iRect;
		Handle	iHandle;

		GetDialogItem (GetDialogPtr (), GetIndex (), &iType, &iHandle, &iRect);
		if (itemEnable)
			iType &= ~itemDisable;
		else iType |= itemDisable;
		SetDialogItem (GetDialogPtr (), GetIndex (), iType, iHandle, &iRect);

	} // end SetEnable
	
// ---------------------------------------------------------------------------
//		¥ Show
// ---------------------------------------------------------------------------

void
VCSDialogItem::Show (void)
	
	{ // begin Show
		
		::ShowDialogItem (GetDialogPtr (), GetIndex ());
	
	} // end Show
	
// ---------------------------------------------------------------------------
//		¥ Hide
// ---------------------------------------------------------------------------

void
VCSDialogItem::Hide (void)
	
	{ // begin Hide
		
		::HideDialogItem (GetDialogPtr (), GetIndex ());

	} // end Hide
	
// ---------------------------------------------------------------------------
//		¥ SetShow
// ---------------------------------------------------------------------------

void
VCSDialogItem::SetShow (

	Boolean	inShow)
	
	{ // begin SetShow
		
		if (inShow)
			Show ();
		else Hide ();

	} // end SetShow
	
// ---------------------------------------------------------------------------
//		¥ GetBounds
// ---------------------------------------------------------------------------

void
VCSDialogItem::GetBounds (

	Rect&	outBounds) const
	
	{ // begin GetBounds
		
		short	iType;
		Handle	iHandle;

		::GetDialogItem (GetDialogPtr (), GetIndex (), &iType, &iHandle, &outBounds);
		
	} // end GetBounds
	
// ---------------------------------------------------------------------------
//		¥ SetBounds
// ---------------------------------------------------------------------------

void
VCSDialogItem::SetBounds (

	const	Rect&	inBounds)
	
	{ // begin SetBounds
		
		short	iType;
		Rect	iRect;
		Handle	iHandle;

		::GetDialogItem (GetDialogPtr (), GetIndex (), &iType, &iHandle, &iRect);
		::SetDialogItem (GetDialogPtr (), GetIndex (), iType, iHandle, &inBounds);

	} // end SetBounds
	
// ---------------------------------------------------------------------------
//		¥ GetValue
// ---------------------------------------------------------------------------

short
VCSDialogItem::GetValue (void) const
	
	{ // begin GetValue
		
		return 0;
	
	} // end GetValue
	
// ---------------------------------------------------------------------------
//		¥ SetValue
// ---------------------------------------------------------------------------

void
VCSDialogItem::SetValue (

	short	)
	
	{ // begin SetValue
		
	} // end SetValue
	
// ---------------------------------------------------------------------------
//		¥ GetDescriptor
// ---------------------------------------------------------------------------

void
VCSDialogItem::GetDescriptor (

	Str255	itemText) const
	
	{ // begin GetDescriptor
		
		itemText[0] = 0;
		
	} // end GetDescriptor
	
// ---------------------------------------------------------------------------
//		¥ SetDescriptor
// ---------------------------------------------------------------------------

void
VCSDialogItem::SetDescriptor (

	ConstStr255Param	)
	
	{ // begin SetDescriptor
		
	} // end SetDescriptor
	
// ---------------------------------------------------------------------------
//		¥ OnClick
// ---------------------------------------------------------------------------

Boolean 
VCSDialogItem::OnClick (
	
	Point			,
	EventRecord&	theEvent,
	short&			itemHit)
	
	{ // begin OnClick
		
		return StdFilterProc (GetDialogPtr (), &theEvent, &itemHit);
			
	} // end OnClick

// ---------------------------------------------------------------------------
//		¥ OnDraw
// ---------------------------------------------------------------------------

void 
VCSDialogItem::OnDraw (void)
	
	{ // begin OnDraw
		
	} // end OnDraw

