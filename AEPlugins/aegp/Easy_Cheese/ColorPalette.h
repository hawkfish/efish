/*
	File:		ColorPalette.h

	Contains:	Interface to the Color Palette dialog.

	Written by:	Richard Wesley

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights Reserved.

*/

#pragma once

#include "TabbedPalette.h"

class ChipList;
class HotChip;

class ColorPalette : public TabbedPalette

{
		//	Constants
	enum {kHotChipCount = 8};
		
		//	Types
	typedef	std::vector<HotChip*>	HotChipTable;
	
		//	Component sizes
	short 			mChipWidth;
	short 			mChipHeight;
	short 			mScrollbarWidth;
	short 			mScrollbarHeight;
		
		//	Component counts
	short 			mMinCols;
	short 			mMaxCols;
	short 			mMinRows;
	short 			mMaxRows;
	
		//	Items
	ChipList*		mMainChips;
	HotChipTable	mHotChips;
	
		//	Illegal
					ColorPalette	(const	ColorPalette&	other);
	ColorPalette&	operator=		(const	ColorPalette&	other);
		
protected:

		//	Event Handling
	void 			Layout			(void);
	virtual void 	Notify			(ADMNotifierRef 		notifier);

public:
	
		//	Construction/Destruction
					ColorPalette	(const std::string&		name,
									 ASInt32 				id = 0, 
									 ASInt32 				options = 0);
	virtual			~ColorPalette	(void);
		
		//	Serialization
	void			SaveColors		(void) const;
	void			RestoreColors	(void);
	
		//	Event Tracking
	virtual	bool	Track			(ADM::Tracker&			tracker);
};
