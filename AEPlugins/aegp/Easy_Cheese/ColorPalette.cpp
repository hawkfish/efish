/*
	File:		ColorPalette.cpp

	Contains:	Implementation of the Color Palette dialog.

	Written by:	Richard Wesley

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights Reserved.

	26 Oct 2001		dml		min rows = 0, call SetTitle in ct

*/

#include "ColorPalette.h"

#include "ChipList.h"
#include "HotChip.h"

#include "adm_persist.h"
#include "adm_suites.h"
#include "adm_tracker.h"

//	Windoze lameness
#ifndef _MIN
#define _MIN min
#endif

static ASRGBColor
sDefaultHot [] = {
	{0xFFFF, 0xFFFF, 0xFFFF},	//	White
	{0, 0, 0},					//	Black

	{0xFFFF, 0, 0},				//	Red
	{0, 0xFFFF, 0},				//	Green
	{0, 0, 0xFFFF},				//	Blue
	
	{0, 0xFFFF, 0xFFFF},		//	Cyan
	{0xFFFF, 0, 0xFFFF},		//	Magenta
	{0xFFFF, 0xFFFF, 0}			//	Yellow
	};

static const char
sSection [] = "Color Pal";

static const char
sHotChipCountKey [] = "Hot Chip Count";

static const char
sHotChipKey [] = "Hot Chips";

static const char
sChipListCountKey [] = "Chip List Count";

static const char
sChipListKey [] = "Chip List";

typedef std::vector<ASRGBColor>	ColorVector;

		//	Component counts
static	const	short 	kMinCols = 16;
static	const	short 	kMaxCols = 16;

static	const	short 	kMinRows = 0;
static	const	short 	kMaxRows = 16;

		//	Component sizes
static	const	short 	kChipWidth = 16;
static	const	short 	kChipHeight = 16;

static	const	short 	kScrollbarWidth = 16;
static	const	short 	kScrollbarHeight = 16;

static	const	short 	kListMarginWidth = 3;
static	const	short 	kListMarginHeight = 3;

static	const	short 	kMinListWidth = kMinCols * kChipWidth + kScrollbarWidth + kListMarginWidth;
static	const	short 	kMaxListWidth = kMaxCols * kChipWidth + kScrollbarWidth + kListMarginWidth;

static	const	short 	kMinListHeight = kMinRows * kChipHeight + kListMarginHeight;
static	const	short 	kMaxListHeight = kMaxRows * kChipHeight + kListMarginHeight;

static	const	short 	kHotChipPanelHeight = kScrollbarHeight;
static	const	short 	kHotChipPanelWidth = kMaxListWidth - kScrollbarWidth;

static	const	short 	kDialogMinWidth = kMinListWidth;
static	const	short 	kDialogMaxWidth = kDialogMinWidth;
static	const	short 	kDialogWidth = kDialogMaxWidth;

static	const	short 	kDialogMinHeight = kMinListHeight + kHotChipPanelHeight;
static	const	short 	kDialogMaxHeight = kMaxListHeight + kHotChipPanelHeight;
static	const	short 	kDialogHeight = kDialogMaxHeight;

// ---------------------------------------------------------------------------
//		¥ ColorPalette
// ---------------------------------------------------------------------------

ColorPalette::ColorPalette (

	const std::string&	name,
	ASInt32 			id, 
	ASInt32 			options) 
	
	: TabbedPalette (name, id, options)
	
	, mMainChips (0)
	, mHotChips (kHotChipCount, 0)
	
	{ // begin ColorPalette
		
		// setup default geometries
		Size (kDialogWidth, kDialogHeight);
		SetMinSize (kDialogMinWidth, kDialogMinHeight);
		SetMaxSize (kDialogMaxWidth, kDialogMaxHeight);

		// make scrolling list
		ASRect	rc;
		rc.top = rc.left = 0;
		rc.right = kMaxListWidth;
		rc.bottom = kMaxListHeight;
		mMainChips = new ChipList (this, 0, rc, kMaxCols, kChipWidth, kChipHeight);
		
		// make hot chips
		rc.top = rc.bottom;
		rc.bottom += kHotChipPanelHeight;
		
		short hotChipWidth (kHotChipPanelWidth / mHotChips.size ());
		for (int j = 0; j < mHotChips.size (); ++j) {
			rc.left = j * hotChipWidth;
			rc.right = rc.left + hotChipWidth;
			mHotChips[j] = new HotChip (this, j, rc, sDefaultHot + j);
			} // for
		
		//	select the tab
		ADM::Suites::dialog_group ()->SetTabGroup(*this, sTabGroupName, TRUE);
		
		RestoreColors ();
		
		SetTitle(name);
		
	} // end ColorPalette
	
// ---------------------------------------------------------------------------
//		¥ ~ColorPalette
// ---------------------------------------------------------------------------

ColorPalette::~ColorPalette (void) 
	
	{ // begin ~ColorPalette
		
		//	Save persistent data
		SaveColors ();

		//	Delete the hot chips
		for (HotChipTable::reverse_iterator i = mHotChips.rbegin (); i != mHotChips.rend (); ++i)
			delete *i;
		
		//	Delete the chip list
		delete mMainChips;
		
	} // end ~ColorPalette
	
// ---------------------------------------------------------------------------
//		¥ SaveColors
// ---------------------------------------------------------------------------

void
ColorPalette::SaveColors (void) const
	
	{ // begin SaveColors
		
		//	Persistent data temps
		ADM::Persist		persist (sSection);
		ColorVector			colors;
		A_long				colorCount;
		
		//	Save main palette
		colors.resize (mMainChips->GetColorCount (), sDefaultHot[0]);
		for (ColorVector::iterator i = colors.begin (); i != colors.end (); ++i)
			*i = mMainChips->GetColor (i - colors.begin ());
			
		colorCount = colors.size ();
		persist.SetLong (sChipListCountKey, colorCount);
		persist.SetData (sChipListKey, colors.begin (), colors.size () * sizeof (ASRGBColor));
		
		//	Save the hot chips
		colors.resize (mHotChips.size (), sDefaultHot[0]);
		for (ColorVector::iterator j = colors.begin (); j != colors.end (); ++j)
			*j = mHotChips[j - colors.begin ()]->GetColor ();
		
		colorCount = colors.size ();
		persist.SetLong (sHotChipCountKey, colorCount);
		persist.SetData (sHotChipKey, colors.begin (), colors.size () * sizeof (ASRGBColor));

	} // end SaveColors
	
// ---------------------------------------------------------------------------
//		¥ RestoreColors
// ---------------------------------------------------------------------------

void
ColorPalette::RestoreColors (void) 
	
	{ // begin RestoreColors
		
		//	Load persistent data
		ADM::Persist		persist (sSection);
		ColorVector			colors;
		A_long				colorCount;
		
		//	Load chip list data (if any)
		if (persist.DoesKeyExist (sChipListCountKey) && persist.DoesKeyExist (sChipListKey)) {
			//	Read the data
			persist.GetLong (sChipListCountKey, colorCount);
			colors.resize (colorCount, sDefaultHot[0]);
			persist.GetData (sChipListKey, colors.begin (), colors.size () * sizeof (ASRGBColor));
			
			//	Stuff it in the list
			mMainChips->SetColorCount (colorCount);
			for (ChipList::ColorIndex i = 0; i < colorCount; ++i)
				mMainChips->SetColor (i, colors[i], false);
			mMainChips->Invalidate ();
			} // if
		
		//	Load hot chip data (if any)
		if (persist.DoesKeyExist (sHotChipCountKey) && persist.DoesKeyExist (sHotChipKey)) {
			//	Read the data
			persist.GetLong (sHotChipCountKey, colorCount);
			colors.resize (colorCount, sDefaultHot[0]);
			persist.GetData (sHotChipKey, colors.begin (), colors.size () * sizeof (ASRGBColor));
			
			colorCount = std::_MIN ((A_long) kHotChipCount, colorCount);
			for (int j = 0; j < colorCount; ++j) 
				mHotChips[j]->SetColor (colors[j]);
			} // for
			
	} // end RestoreColors
	
// ---------------------------------------------------------------------------
//		¥ Layout
// ---------------------------------------------------------------------------

void
ColorPalette::Layout(void) 

	{ // begin Layout
		
		if (!mMainChips) return;
		
		ASRect inParentBounds (GetLocalRect ());

		ASRect trimmedR (inParentBounds);
		//trimmedR.right -= kScrollbarWidth;
		trimmedR.bottom -= kHotChipPanelHeight;
		mMainChips->SetBoundsRect(trimmedR);
			
		for (int i = 0; i < mHotChips.size (); ++i) {
			ASRect	r (mHotChips[i]->GetBoundsRect ());
			r.top = inParentBounds.bottom - kHotChipPanelHeight;
			r.bottom = r.top + kHotChipPanelHeight;
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


