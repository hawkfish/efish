/*
	File:		VCSDialogGroupBox.cp

	Contains:	Implementation of a simple group box.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1998 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

         <2>     11/6/98    rmgw    Convert to grayscale.
         <1>     11/4/98    rmgw    Break out group box item.
*/


#include "VCSDialogGroupBox.h"

#include "StForeColor.h"
#include "StBackColor.h"

#include <Palettes.h>

// ---------------------------------------------------------------------------
//		¥ VCSDialogGroupBox
// ---------------------------------------------------------------------------

VCSDialogGroupBox::VCSDialogGroupBox (

	VCSDialog&			inDialog,
	DialogItemIndex		inIndex,
	VCSDialogItem*		inTitle)
	
	: VCSDialogItem (inDialog, inIndex)
	
	, mTitle (inTitle)
	
	{ // begin VCSDialogGroupBox
		
	} // end VCSDialogGroupBox
	
// ---------------------------------------------------------------------------
//		¥ ~VCSDialogGroupBox
// ---------------------------------------------------------------------------

VCSDialogGroupBox::~VCSDialogGroupBox (void)

	{ // begin ~VCSDialogGroupBox
		
	} // end ~VCSDialogGroupBox

// ---------------------------------------------------------------------------
//		¥ OnDraw
// ---------------------------------------------------------------------------

void 
VCSDialogGroupBox::OnDraw (void)
	
	{ // begin OnDraw
		
		Rect	bounds;
		GetBounds (bounds);
		
		Rect	titleBounds;
		if (mTitle) {
			mTitle->GetBounds (titleBounds);
			
			bounds.top = (titleBounds.top + titleBounds.bottom) / 2;
			::InsetRect (&titleBounds, -2, -2);
			} // if
			
		else {
			titleBounds = bounds;
			titleBounds.left = titleBounds.right;
			titleBounds.top = titleBounds.bottom;
			} // else
		
		StBackColor	saveBack;
		RGBColor	backGround;
		::GetBackColor (&backGround);
		
		StForeColor	saveFore;
		RGBColor	foreGround;
		::GetForeColor (&foreGround);
		
		RGBColor	ramp[2] = {StForeColor::sBlackColor, StBackColor::sWhiteColor};
		::GetGray (::GetMainDevice (), &backGround, ramp);
		::GetGray (::GetMainDevice (), &backGround, ramp + 1);
		
		for (short i = 0; i < 2; ++i) {
			::RGBForeColor (ramp + i);
			
			//	Left side
			::MoveTo (bounds.left, bounds.bottom - 1);
			::LineTo (bounds.left, bounds.top);
			
			//	Top side
			::LineTo (titleBounds.left, bounds.top);
			::MoveTo (titleBounds.right, bounds.top);
			::LineTo (bounds.right - 1, bounds.top);
			
			//	Right side
			::RGBForeColor (ramp + (1 - i));
			::LineTo (bounds.right - 1, bounds.bottom - 1);

			//	Right side
			::LineTo (bounds.left, bounds.bottom - 1);
			
			::InsetRect (&bounds, 1, 1);
			} // for
			
	} // end OnDraw
