/*
	File:		HotChip.cp

	Contains:	Implementation of the Hot Chip item.

	Written by:	Richard Wesley

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights Reserved.

*/

#include "HotChip.h"

#include "adm_drawer.h"
#include "adm_suites.h"
#include "adm_tracker.h"

// ---------------------------------------------------------------------------
//		¥ HotChip
// ---------------------------------------------------------------------------

HotChip::HotChip (

	ADM::Dialog*		parent,
	ASInt32 			itemID,
	const ASRect&		bounds,
	const ASRGBColor*	inColor) 
	
	{ // begin HotChip
	
		ADMItemRef item = Create (parent, itemID, kADMFrameType, bounds);
		
		if (inColor)
			mColor = *inColor;
		else mColor.red = mColor.green = mColor.blue = 0xFFFF;

	} // end HotChip
	
// ---------------------------------------------------------------------------
//		¥ ~HotChip
// ---------------------------------------------------------------------------

HotChip::~HotChip (void) 
	
	{ // begin ~HotChip
	
	} // end ~HotChip
	
// ---------------------------------------------------------------------------
//		¥ Draw
// ---------------------------------------------------------------------------

void  
HotChip::Draw (

	ADM::Drawer&	inDrawer) 
	
	{ // begin Draw
	
		ASRect r (GetBoundsRect ());
		Item::Draw (inDrawer);
		
		short top (r.top);
		short left (r.left);
		r.left -= left;
		r.right -= left;
		
		inDrawer.SetColor (mColor);
		inDrawer.FillRect (r);
	
	} // end Draw
	
// ---------------------------------------------------------------------------
//		¥ Notify
// ---------------------------------------------------------------------------

void 
HotChip::Notify (

	ADMNotifierRef inNotifier) 
	
	{ // begin Notify
	
		ASRect r (GetBoundsRect ());
		ASPoint p;
		p.v = r.top; p.h = r.left;
		
		ASRGBColor 		newColor;				
		if (ADM::Suites::basic ()->ChooseColor (p, &mColor, &newColor))	
			SetColor (newColor);
	
	} // end Notify

// ---------------------------------------------------------------------------
//		¥ SetColor
// ---------------------------------------------------------------------------

void  
HotChip::SetColor (

	const ASRGBColor &inColor) 

	{ // begin SetColor
	
		mColor = inColor;
		
		Invalidate();

	} // end SetColor

// ---------------------------------------------------------------------------
//		¥ Track
// ---------------------------------------------------------------------------

bool  
HotChip::Track (

	ADM::Tracker&	tracker) 

	{ // begin Track

		tracker.Abort();

		return true;

	} // end Track

