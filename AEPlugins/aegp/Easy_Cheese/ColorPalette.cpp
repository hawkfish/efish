/*
	File:		ColorPalette.cpp

	Contains:	Implementation of the Color Palette dialog.

	Written by:	Richard Wesley

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights Reserved.

*/

#include "ColorPalette.h"

#include "ChipList.h"
#include "HotChip.h"

#include "adm_suites.h"
#include "adm_tracker.h"

// ---------------------------------------------------------------------------
//		¥ ColorPalette
// ---------------------------------------------------------------------------

ColorPalette::ColorPalette (

	const std::string&	name,
	ASInt32 			id, 
	ASInt32 			options) 
	
	: TabbedPalette (name, id, options)
	
	, mChipWidth (16)
	, mChipHeight (16)
	, mScrollbarWidth (0)
	, mScrollbarHeight (16)
	
	, mMinCols (16)
	, mMaxCols (16)
	, mMinRows (3)
	, mMaxRows (16)
	
	, mMainChips (0)
	
	{ // begin ColorPalette
		
		for (int j = 0; j < kHotChipCount; ++j)
			mHotChips[j] = 0;

		// setup default geometries
		Size ((mMaxCols * mChipWidth) + mScrollbarWidth, (mMaxRows * mChipHeight) + mScrollbarHeight);
		SetMinSize ((mMinCols * mChipWidth) + mScrollbarWidth, (mMinRows * mChipHeight) + mScrollbarHeight);
		SetMaxSize ((mMaxCols * mChipWidth) + mScrollbarWidth, (mMaxRows * mChipHeight) + mScrollbarHeight);

		// make scrolling list
		ASRect	rc;
		rc.top = rc.left = 0;
		rc.right = (short) (mMaxCols * mChipWidth);
		rc.bottom = (short) (mMaxRows * mChipHeight);
		mMainChips = new ChipList (this, 0, rc, mMaxCols, mChipWidth, mChipHeight);
		
		// make hot chips
		rc.top = rc.bottom;
		rc.bottom += mChipHeight;
		
		short hotChipWidth ((rc.right - rc.left - mScrollbarHeight) / kHotChipCount);
		for (int j = 0; j < kHotChipCount; ++j) {
			rc.left = j * hotChipWidth;
			rc.right = rc.left + hotChipWidth;
			mHotChips[j] = new HotChip (this, j, rc);
			} // for
		
		//	select the tab
		ADM::Suites::dialog_group ()->SetTabGroup(*this, sTabGroupName, TRUE);

	} // end ColorPalette
	
// ---------------------------------------------------------------------------
//		¥ ~ColorPalette
// ---------------------------------------------------------------------------

ColorPalette::~ColorPalette (void) 
	
	{ // begin ~ColorPalette
		
		for (int j = kHotChipCount; j-- > 0; )
			delete mHotChips[j];
		
		delete mMainChips;
		
	} // end ~ColorPalette
	
// ---------------------------------------------------------------------------
//		¥ Layout
// ---------------------------------------------------------------------------

void
ColorPalette::Layout(void) 

	{ // begin Layout
		
		if (!mMainChips) return;
		
		ASRect inParentBounds (GetLocalRect ());

		ASRect trimmedR (inParentBounds);
		trimmedR.right -= mScrollbarWidth;
		trimmedR.bottom -= mScrollbarHeight;
		mMainChips->SetBoundsRect(trimmedR);
			
		for (int i = 0; i < kHotChipCount; ++i) {
			ASRect	r (mHotChips[i]->GetBoundsRect ());
			r.top = inParentBounds.bottom - mScrollbarHeight;
			r.bottom = r.top + mScrollbarHeight;
			mHotChips[i]->SetBoundsRect(r);
			} // for	
		
	} // end Layout

// ---------------------------------------------------------------------------
//		¥ Notify
// ---------------------------------------------------------------------------

void
ColorPalette::Notify (

	ADMNotifierRef	notifier)

	{ // begin Notify

		// start off with the default action!!
		TabbedPalette::Notify (notifier);

		do {
			if (!ADM::Suites::notifier ()->IsNotifierType (notifier, kADMBoundsChangedNotifier)) break;
			
			// if there is no item (and thus the dialog itself that's resizing), notify upstream
			ADMItemRef 	theItem (ADM::Suites::notifier ()->GetItem (notifier));
			if (theItem != NULL) break;
			
			Layout ();
		} while (false);

	} // end Notify

// ---------------------------------------------------------------------------
//		¥ Track
// ---------------------------------------------------------------------------

bool
ColorPalette::Track ( 

	ADM::Tracker& tracker)

	{ // begin Track
		
		bool	result = TabbedPalette::Track (tracker);
		
		if (tracker.GetAction () == kADMButtonUpAction) {
			} // if
		
		return result;
		
	} // end Track


