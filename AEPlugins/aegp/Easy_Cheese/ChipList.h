/*
	File:		ChipList.h

	Contains:	Interface to the Chip List item.

	Written by:	Richard Wesley

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights Reserved.

*/

#pragma once

#include "adm_listview.h"

#include "AEGP_SuiteHandler.h"

class ChipList : public ADM::ListView
{
	
		//	Display Size
	short 			mNumCols;
	short 			mChipWidth;
	short 			mChipHeight;
		
		//	Illegal
					ChipList		(const	ChipList&	other);
	ChipList&		operator=		(const	ChipList&	other);
		
		//	Chip management
	A_Err			AllocateColors	(short				inNum = 256);
	void			FreeColors		(void);
	
protected:

		//	Event Handling
	virtual void 	DrawEntry		(const ADM::Entry*	entry,
									 ADM::Drawer&		drawer);
	virtual void 	NotifyEntry		(ADM::Entry*		entry,
									 ADMNotifierRef 	notifier);

public:
	
		//	Construction/Destruction
					ChipList		(ADM::Dialog*		parent,
									 ASInt32 			itemID,
									 const ASRect&		bounds,
									 short				inNumCols,
									 short				inChipWidth,
									 short				inChipHeight);
	virtual			~ChipList		(void);

		//	Event Tracking
	virtual	bool	Track			(ADM::Tracker&		tracker);
};
