/*
	File:		ChipList.cp

	Contains:	Implementation of the Chip List item.

	Written by:	Richard Wesley

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights Reserved.

*/

#include "ChipList.h"

#include "adm_drawer.h"
#include "adm_suites.h"
#include "adm_tracker.h"

// ---------------------------------------------------------------------------
//		¥ ChipList
// ---------------------------------------------------------------------------

ChipList::ChipList (

	ADM::Dialog*		parent,
	ASInt32 			itemID,
	const ASRect&		bounds,
	short				inNumCols,
	short				inChipWidth,
	short				inChipHeight) 
	
	: ADM::ListView (parent, itemID, bounds)
	
	, mNumCols (inNumCols)
	, mChipWidth (inChipWidth)
	, mChipHeight (inChipHeight)
	
	{ // begin ChipList
		
		//	Set list constraints
		SetEntrySize (mChipWidth * mNumCols, mChipHeight);
		
		//	Test data - remove!
		for (int i = 0; i < 6; ++i)
			InsertEntry (i, i);

	} // end ChipList
	
// ---------------------------------------------------------------------------
//		¥ ~ChipList
// ---------------------------------------------------------------------------

ChipList::~ChipList (void) 
	
	{ // begin ~ChipList
	
	} // end ~ChipList
	
// ---------------------------------------------------------------------------
//		¥ DrawEntry
// ---------------------------------------------------------------------------

void
ChipList::DrawEntry (

	const ADM::Entry*	entry, 
	ADM::Drawer&		drawer)  
	
	{ // begin DrawEntry
	
		ASRGBColor evenColor;
		ASRGBColor oddColor;
		evenColor.red = evenColor.green = evenColor.blue = 16384;
		oddColor.red = 12345; oddColor.green = 23456; oddColor.blue = 01234;
		if (entry->GetIndex () % 2 == 0)
			drawer.SetColor (evenColor);
		else drawer.SetColor (oddColor);
		
		ASRect r (entry->GetLocalRect());
		drawer.FillRect (r);			
			
	} // end DrawEntry

// ---------------------------------------------------------------------------
//		¥ NotifyEntry
// ---------------------------------------------------------------------------

void 
ChipList::NotifyEntry (

	ADM::Entry*		entry, 
	ADMNotifierRef	notifier) 
	
	{ // begin NotifyEntry
		
		entry->Invalidate ();

		//figure out which chip
		// bring up the color choose
		// install new color for that chip
		// invalidate that chip (row)

	} // end NotifyEntry

// ---------------------------------------------------------------------------
//		¥ Track
// ---------------------------------------------------------------------------

bool 
ChipList::Track (

	ADM::Tracker &tracker) 
	
	{ // begin Track
		
		tracker.Abort ();
		
		return true;

	} // end Track

