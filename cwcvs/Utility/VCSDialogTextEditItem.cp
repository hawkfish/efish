/*
	File:		VCSDialogTextEditItem.cp

	Contains:	Implementation of a Dialog box scrolling text field.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1997-9 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

         <5>      4/1/99    rmgw    Use line height in max  calculation.
         <4>     3/30/99    rmgw    Crash recovery merging.
         <3>     3/22/99    rmgw    Grayscale dialogs.
         <2>     3/22/99    rmgw    Normalize scroll values.
         <1>     3/22/99    rmgw    first checked in.
*/


#include "VCSDialogTextEditItem.h"

#include "StForeColor.h"
#include "StBackColor.h"

#include <Fonts.h>
#include <Scrap.h>
#include <TextUtils.h>

const	short	kMargin = 4;

// ---------------------------------------------------------------------------
//		¥ VCSDialogTextEditItem
// ---------------------------------------------------------------------------

VCSDialogTextEditItem::VCSDialogTextEditItem (

	VCSDialog&				inDialog,
	DialogItemIndex			inIndex,
	VCSDialogScrollbarItem*	inScroll)
	
	: VCSDialogItem (inDialog, inIndex)
	
	, mTE (nil)
	, mScroll (inScroll)
	, mScrollUnits (12)
	
	{ // begin VCSDialogTextEditItem
		
		//	Set up the port
		SavePort	savePort;
		SetPortWindowPort (GetDialogWindow (GetDialogPtr ()));

		Rect	iRect;
		GetBounds (iRect);

		::InsetRect (&iRect, kMargin, kMargin);
	
		::TextFont (kPlatformDefaultGuiFontID);
		::TextSize (9);
		mTE = ::TEStyleNew (&iRect,&iRect);
		
		if (mScroll) mScroll->SetScrollTarget (this);
		
	} // end VCSDialogTextEditItem
	
// ---------------------------------------------------------------------------
//		¥ ~VCSDialogTextEditItem
// ---------------------------------------------------------------------------

VCSDialogTextEditItem::~VCSDialogTextEditItem (void)

	{ // begin ~VCSDialogTextEditItem
		
		if (mTE) ::TEDispose (mTE);
		mTE = nil;
		
	} // end ~VCSDialogTextEditItem

// ---------------------------------------------------------------------------
//		¥ GetText
// ---------------------------------------------------------------------------

Handle
VCSDialogTextEditItem::GetText (void) const
	
	{ // begin GetText
		
		return ::TEGetText (mTE);
					
	} // end GetText
	
// ---------------------------------------------------------------------------
//		¥ GetValue
// ---------------------------------------------------------------------------

short
VCSDialogTextEditItem::GetValue (void) const
	
	{ // begin GetValue
		
		Str255	text;
		GetDescriptor (text);

		long	value;
		::StringToNum (text, &value);
		
		return value;
	
	} // end GetValue
	
// ---------------------------------------------------------------------------
//		¥ SetValue
// ---------------------------------------------------------------------------

void
VCSDialogTextEditItem::SetValue (

	short	inValue)
	
	{ // begin SetValue
		
		Str255	text;
		::NumToString (inValue, text);
		SetDescriptor (text);
	
	} // end SetValue

// ---------------------------------------------------------------------------
//		¥ GetDescriptor
// ---------------------------------------------------------------------------

void
VCSDialogTextEditItem::GetDescriptor (

	Str255	itemText) const
	
	{ // begin GetDescriptor
		
		GetDialogItemText (GetText (), itemText);
		
	} // end GetDescriptor
	
// ---------------------------------------------------------------------------
//		¥ SetDescriptor
// ---------------------------------------------------------------------------

void
VCSDialogTextEditItem::SetDescriptor (

	ConstStr255Param	itemText)
	
	{ // begin SetDescriptor
		
		SetDialogItemText (GetText (), itemText);
		
	} // end SetDescriptor
	
// ---------------------------------------------------------------------------
//		¥ OnDraw
// ---------------------------------------------------------------------------

void
VCSDialogTextEditItem::OnDraw (void)

	{ // begin OnDraw
		
		StForeColor	saveFore;
		StBackColor	saveBack;

		Rect	iRect;
		GetBounds (iRect);
		::FrameRect(&iRect);
		
		::InsetRect (&iRect, 1, 1);
		::EraseRect (&iRect);
		
		::TEUpdate (&iRect, mTE);
	
	} // end OnDraw

#pragma mark -

// ---------------------------------------------------------------------------
//		¥ GetLength
// ---------------------------------------------------------------------------

Size
VCSDialogTextEditItem::GetLength (void) const

	{ // begin GetLength
		
		return (**mTE).teLength;
		
	} // end GetLength

// ---------------------------------------------------------------------------
//		¥ GetSelect
// ---------------------------------------------------------------------------

void
VCSDialogTextEditItem::GetSelect (

	Size&	selStart, 
	Size&	selEnd) const

	{ // begin GetSelect
		
		selStart = (**mTE).selStart;
		selEnd = (**mTE).selEnd;
		
	} // end GetSelect

// ---------------------------------------------------------------------------
//		¥ SetSelect
// ---------------------------------------------------------------------------

void
VCSDialogTextEditItem::SetSelect (

	Size	selStart, 
	Size	selEnd)

	{ // begin SetSelect
		
		::TESetSelect (selStart, selEnd, mTE);
		
	} // end SetSelect


// ---------------------------------------------------------------------------
//		¥ GetStyleScrapHandle
// ---------------------------------------------------------------------------

StScrpHandle
VCSDialogTextEditItem::GetStyleScrapHandle (void) const

	{ // begin GetStyleScrapHandle
		
		return ::TEGetStyleScrapHandle (mTE);
		
	} // end GetStyleScrapHandle

// ---------------------------------------------------------------------------
//		¥ GetStyleHandle
// ---------------------------------------------------------------------------

TEStyleHandle
VCSDialogTextEditItem::GetStyleHandle (void) const

	{ // begin GetStyleHandle
		
		return ::TEGetStyleHandle (mTE);
		
	} // end GetStyleHandle

// ---------------------------------------------------------------------------
//		¥ SetStyleHandle
// ---------------------------------------------------------------------------

void
VCSDialogTextEditItem::SetStyleHandle (

	TEStyleHandle	inStyle)

	{ // begin SetStyleHandle
		
		::TESetStyleHandle (inStyle, mTE);
		
		if (mScroll) mScroll->AlignToTarget ();

	} // end SetStyleHandle

// ---------------------------------------------------------------------------
//		¥ SetStyle
// ---------------------------------------------------------------------------

void
VCSDialogTextEditItem::SetStyle (
	
	const	TextStyle&	newStyle,
	short				mode,
	Boolean				redraw)

	{ // begin SetStyle
		
		::TESetStyle (mode, &newStyle, redraw, mTE);

		if (mScroll) mScroll->AlignToTarget ();

	} // end SetStyle

// ---------------------------------------------------------------------------
//		¥ Insert
// ---------------------------------------------------------------------------

void
VCSDialogTextEditItem::Insert (

	const	char*		text, 
	Size				length)

	{ // begin Insert
		
		::TEInsert (text, length, mTE);
		
		if (mScroll) mScroll->AlignToTarget ();

	} // end Insert

// ---------------------------------------------------------------------------
//		¥ Cut
// ---------------------------------------------------------------------------

void
VCSDialogTextEditItem::Cut (void)

	{ // begin Cut
		
		::ClearCurrentScrap ();
		::TECut (mTE);

		if (mScroll) mScroll->AlignToTarget ();

	} // end Cut

// ---------------------------------------------------------------------------
//		¥ Copy
// ---------------------------------------------------------------------------

void
VCSDialogTextEditItem::Copy (void) const

	{ // begin Copy
		
		::ClearCurrentScrap ();
		::TECopy (mTE);

	} // end Copy

// ---------------------------------------------------------------------------
//		¥ Paste
// ---------------------------------------------------------------------------

void
VCSDialogTextEditItem::Paste (void)

	{ // begin Paste
		
		::TEPaste (mTE);

		if (mScroll) mScroll->AlignToTarget ();

	} // end Paste

// ---------------------------------------------------------------------------
//		¥ Clear
// ---------------------------------------------------------------------------

void
VCSDialogTextEditItem::Clear (void)

	{ // begin Clear
		
		::TEDelete (mTE);

		if (mScroll) mScroll->AlignToTarget ();

	} // end Clear

#pragma mark -

// ---------------------------------------------------------------------------
//		¥ GetScrollMin
// ---------------------------------------------------------------------------

SInt16
VCSDialogTextEditItem::GetScrollMin (void) const

	{ // begin GetScrollMin
		
		return 0;
		
	} // end GetScrollMin

// ---------------------------------------------------------------------------
//		¥ GetScrollMax
// ---------------------------------------------------------------------------

SInt16
VCSDialogTextEditItem::GetScrollMax (void) const

	{ // begin GetScrollMax
		
		short	displayHeight = (**mTE).destRect.bottom - (**mTE).destRect.top;
		short	textHeight = ::TEGetHeight ((**mTE).nLines, 0, mTE);
		
		if (displayHeight >= textHeight) return 0;
		 
		return (textHeight - displayHeight) / mScrollUnits;
		
	} // end GetScrollMax

// ---------------------------------------------------------------------------
//		¥ GetScrollValue
// ---------------------------------------------------------------------------

SInt16
VCSDialogTextEditItem::GetScrollValue (void) const

	{ // begin GetScrollValue
		
		return ((**mTE).viewRect.top - (**mTE).destRect.top) / mScrollUnits;
	
	} // end GetScrollValue

// ---------------------------------------------------------------------------
//		¥ ScrollBy
// ---------------------------------------------------------------------------

void
VCSDialogTextEditItem::ScrollBy (

	short	inDelta)

	{ // begin ScrollBy

		StForeColor	saveFore;
		StBackColor	saveBack;

		short	atLine = GetScrollValue ();
		short	minLine = GetScrollMin ();
		short	maxLine = GetScrollMax ();

		if ((atLine + inDelta) < minLine)
			inDelta = minLine - atLine;
		if ((atLine + inDelta) > maxLine)
			inDelta = maxLine - atLine;
				
		short	scrollPix = inDelta * mScrollUnits;
		::TEScroll (0, -scrollPix, mTE);
	
	} // end ScrollBy

// ---------------------------------------------------------------------------
//		¥ ScrollTo
// ---------------------------------------------------------------------------

void
VCSDialogTextEditItem::ScrollTo (

	short	inLine)

	{ // begin ScrollTo
		
		ScrollBy (inLine - GetScrollValue ());
	
	} // end ScrollTo

// ---------------------------------------------------------------------------
//		¥ CalText
// ---------------------------------------------------------------------------

void
VCSDialogTextEditItem::CalText (void)

	{ // begin CalText
		
		::TECalText (mTE);
	
		if (mScroll) mScroll->AlignToTarget ();

	} // end CalText
