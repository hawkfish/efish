/*
	File:		ChipList.h

	Contains:	Interface to the Chip List item.

	Written by:	Richard Wesley

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights Reserved.

*/

#pragma once

#include "adm_listview.h"

#include "AEGP_SuiteHandler.h"

#include <vector>

class ChipList : public ADM::ListView
{
	
		//	Table types
	typedef	short	CellIndex;
		
		//	Display Size
	CellIndex 		mNumCols;
	short 			mChipWidth;
	short 			mChipHeight;
		
		//	Colors
	typedef	ASRGBColor				ColorType;
	typedef	std::vector<ColorType>	ColorTable;
	typedef	ColorTable::size_type	ColorIndex;
	ColorTable		mColors;
		
		//	Tracking helpers
	ASPoint			mTrackPoint;
	
		//	Illegal
					ChipList		(const	ChipList&	other);
	ChipList&		operator=		(const	ChipList&	other);
	
	
protected:
		
		//	Table primitives
	CellIndex		GetEntryColumns	(const ADM::Entry*	entry) const;
	
	ColorIndex		ColorIndexFromCell
									(const ADM::Entry*	entry,
									 CellIndex			col) const;

	ADM::Entry*		EntryFromColorIndex (ColorIndex		inIndex) const;
	CellIndex		ColumnFromColorIndex (ColorIndex	inIndex) const;

	ASRect			GetCellRect 	(const ADM::Entry*	entry,
									 CellIndex			col) const;

	void			InvalidateColor	(ColorIndex			inIndex) const;

		//	Event Handling
	virtual void 	DrawEntry		(const ADM::Entry*	entry,
									 ADM::Drawer&		drawer);
	virtual void 	NotifyEntry		(ADM::Entry*		entry,
									 ADMNotifierRef 	notifier);
	virtual	bool	TrackEntry		(ADM::Entry*		entry,
									 ADM::Tracker&		tracker);

public:
	
		//	Construction/Destruction
					ChipList		(ADM::Dialog*		parent,
									 ASInt32 			itemID,
									 const ASRect&		bounds,
									 CellIndex			inNumCols,
									 short				inChipWidth,
									 short				inChipHeight);
	virtual			~ChipList		(void);
		
		//	Table access
	CellIndex		GetColumns		(void) const {return mNumCols;}
							 
		//	Color access
	ColorIndex		GetColorCount	(void) const {return mColors.size ();}
	void			SetColorCount	(ColorIndex			inCount);
	
	const	ColorType&		
					GetColor		(ColorIndex			inIndex) const {return mColors[inIndex];};
	void			SetColor		(ColorIndex			inIndex,
									 const ColorType&	inColor,
									 bool				inRefresh = true);
	
	void			SetDefaultColors (void);
};
