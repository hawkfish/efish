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
	CellIndex			inNumCols,
	short				inChipWidth,
	short				inChipHeight) 
	
	: ADM::ListView (parent, itemID, bounds)
	
	, mNumCols (inNumCols)
	, mChipWidth (inChipWidth)
	, mChipHeight (inChipHeight)
	
	{ // begin ChipList
		
		//	Set list constraints
		SetEntrySize (mChipWidth * mNumCols, mChipHeight);
		
		//	Add starter data
		SetDefaultColors ();

	} // end ChipList
	
// ---------------------------------------------------------------------------
//		¥ ~ChipList
// ---------------------------------------------------------------------------

ChipList::~ChipList (void) 
	
	{ // begin ~ChipList
	
	} // end ~ChipList
	
#pragma mark -

// ---------------------------------------------------------------------------
//		¥ ColorIndexFromCell
// ---------------------------------------------------------------------------

ChipList::ColorIndex
ChipList::ColorIndexFromCell (

	const ADM::Entry*	entry,
	CellIndex			col) const
	
	{ // begin ColorIndexFromCell
		
		return entry->GetIndex () * GetColumns () + col;
		
	} // end ColorIndexFromCell

// ---------------------------------------------------------------------------
//		¥ EntryFromColorIndex
// ---------------------------------------------------------------------------

ADM::Entry*
ChipList::EntryFromColorIndex (

	ColorIndex			inIndex) const
	
	{ // begin EntryFromColorIndex
		
		return GetEntry (inIndex / GetColumns ());
		
	} // end EntryFromColorIndex

// ---------------------------------------------------------------------------
//		¥ ColumnFromColorIndex
// ---------------------------------------------------------------------------

ChipList::CellIndex
ChipList::ColumnFromColorIndex (

	ColorIndex			inIndex) const
	
	{ // begin ColumnFromColorIndex
		
		return inIndex % GetColumns ();
		
	} // end ColumnFromColorIndex

// ---------------------------------------------------------------------------
//		¥ GetEntryColumns
// ---------------------------------------------------------------------------

ChipList::CellIndex
ChipList::GetEntryColumns (

	const ADM::Entry*	entry) const
	
	{ // begin GetEntryColumns
		
		CellIndex	col = GetColumns ();
		ColorIndex	i = ColorIndexFromCell (entry, col);
		
		return (i > GetColorCount ()) ? (GetColorCount () % col) : col;
		
	} // end GetEntryColumns

#pragma mark -

// ---------------------------------------------------------------------------
//		¥ GetCellRect
// ---------------------------------------------------------------------------

ASRect
ChipList::GetCellRect (

	const ADM::Entry*	entry,
	CellIndex			col) const
	
	{ // begin GetCellRect
		
		ASRect r (entry->GetLocalRect());
		r.left += mChipWidth * col;
		r.right = r.left + mChipWidth;
		
		return r;
		
	} // end GetCellRect

// ---------------------------------------------------------------------------
//		¥ InvalidateColor
// ---------------------------------------------------------------------------

void
ChipList::InvalidateColor (

	ColorIndex			inIndex) const
	
	{ // begin InvalidateColorIndex
		
		const	ADM::Entry*	entry = EntryFromColorIndex (inIndex);
		CellIndex			col = ColumnFromColorIndex (inIndex);
		ASRect				r (GetCellRect (entry, col));
		entry->Invalidate (r);
		
	} // end InvalidateColor

#pragma mark -

// ---------------------------------------------------------------------------
//		¥ SetColor
// ---------------------------------------------------------------------------

void 
ChipList::SetColor (

	ColorIndex			inIndex,
	const	ColorType&	inColor,
	bool				inRefresh) 
	
	{ // begin SetColor
		
		mColors[inIndex] = inColor;
		
		if (inRefresh) InvalidateColor (inIndex);

	} // end SetColor

// ---------------------------------------------------------------------------
//		¥ SetColorCount
// ---------------------------------------------------------------------------

void 
ChipList::SetColorCount (

	ColorIndex			inCount) 
	
	{ // begin SetColorCount
		
		//	Resize table, filling with black
		ColorType			rgb;
		rgb.red = rgb.green = rgb.blue = 0;
		mColors.resize (inCount, rgb);
		
		//	Resize entries
		DeleteAll ();
		
		size_t	entryCount = (inCount + GetColumns () - 1) / GetColumns ();
		for (size_t i = 0; i < entryCount; ++i)
			InsertEntry (i, i, false);
			
	} // end SetColorCount

// ---------------------------------------------------------------------------
//		¥ SetDefaultColors
// ---------------------------------------------------------------------------

void 
ChipList::SetDefaultColors (void) 
	
	{ // begin SetDefaultColors
		
		ColorIndex			index = 0;
		ColorType			rgb;
		const	long		fullValue = 0xFFFF;
		
		//	16x16 grid
		SetColorCount (256);
		
		//	White
		rgb.red = rgb.green = rgb.blue = fullValue;
		SetColor (index++, rgb, false);
		
		//	80 = 4x4x5 reds
		for (short i = 0; i < 4; ++i) {
			rgb.green = (i * fullValue) / 4;
			
			for (short j = 0; j < 4; ++j) {
				rgb.blue = (j * fullValue) / 4;
				
				for (short k = 1; k < 6; ++k) {
					rgb.red = (k * fullValue) / 5;
					
					SetColor (index++, rgb, false);
					} // for
				} // for
			} // for
			
		//	80 = 4x4x5 greens
		for (short i = 0; i < 4; ++i) {
			rgb.blue = (i * fullValue) / 4;
			
			for (short j = 0; j < 4; ++j) {
				rgb.red = (j * fullValue) / 4;
				
				for (short k = 1; k < 6; ++k) {
					rgb.green = (k * fullValue) / 5;
					
					SetColor (index++, rgb, false);
					} // for
				} // for
			} // for
			
		//	80 = 4x4x5 greens
		for (short i = 0; i < 4; ++i) {
			rgb.red = (i * fullValue) / 4;
			
			for (short j = 0; j < 4; ++j) {
				rgb.green = (j * fullValue) / 4;
				
				for (short k = 1; k < 6; ++k) {
					rgb.blue = (k * fullValue) / 5;
					
					SetColor (index++, rgb, false);
					} // for
				} // for
			} // for
			
		//	14 grays
		for (short i = 1; i < 15; ++i) {
			rgb.red = rgb.green = rgb.blue = fullValue - (i * fullValue) / 15;
			
			SetColor (index++, rgb, false);
			} // for
				
		//	Black
		rgb.red = rgb.green = rgb.blue = 0;
		SetColor (index++, rgb, false);
		
		//	Redraw
		Invalidate ();
		
	} // end SetDefaultColors

// ---------------------------------------------------------------------------
//		¥ DrawEntry
// ---------------------------------------------------------------------------

void
ChipList::DrawEntry (

	const ADM::Entry*	entry, 
	ADM::Drawer&		drawer)  
	
	{ // begin DrawEntry
	
		CellIndex	count = GetEntryColumns (entry);
		
		for (CellIndex col = 0; col < count; ++col) {
			ASRect r (GetCellRect (entry, col));
			drawer.SetColor (GetColor (ColorIndexFromCell (entry, col)));
			drawer.FillRect (r);			
			} // for
			
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

