/*
	File:		PhotoPrintModel.h

	Contains:	Definition of the main data (model) associated with a document.

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		24 Jul 2001		rmgw	Remove SetDirty (ick!)
		23 Jul 2001		rmgw	Broadcast change messages.
		23 Jul 2001		drd		205 GetNonEmptyCount
		20 Jul 2001		drd		199 RemoveEmptyItems
		19 Jul 2001		drd		195 GetFirstNonEmptyItem
		16 Jul 2001		rmgw	Listen for low memory.  Bug #163.
		09 Jul 2001		rmgw	AdoptNewItem now returns a PhotoIterator. Bug #142.
		06 jul 2001		dml		include ESortedFileList.h
		02 Jul 2001		rmgw	AdoptNewItem now takes a PhotoIterator.
		02 Jul 2001		rmgw	Add const begin/end.
		21 Mar 2001		drd		Added IsEmpty accessor
		15 Feb 2001		rmgw	10 Bottleneck ALL item deletion in iterator routine
		16 Aug 2000		drd		Added DeleteLastItem
		04 Aug 2000		drd		Added disposal arg to DeleteItems
		04 aug 2000		dml		change DeleteItem to DeleteItems (takes list)
		03 aug 2000		dml		move selection to view
		30 Jun 2000		drd		DeleteAll
		26 Jun 2000		drd		GetCount is UInt32 to avoid conversion
		19 Jun 2000		drd		Added GetCount
		14 Jun 2000		drd		Added IsAnythingSelected
*/

#pragma once

#include <LBroadcaster.h>
#include <LListener.h>

#include <list>
#include "MRect.h"
#include "PhotoPrintItem.h"

class PhotoPrintModel 	: public LBroadcaster
						, public LListener

{
protected:
	PhotoItemList			mItemList;
	PhotoDrawingProperties	mDrawingProps;
	
public:
	
	enum {
		kRemove = false,
		kDelete = true
		};
		
	enum {
		msg_ModelItemsAdded 	= 5001,		//	MessageRange*
		msg_ModelItemsChanged,				//	MessageRange*
		msg_ModelItemsRemoved,				//	MessageRange*
		msg_ModelPropsChanged,				//	PhotoPrintModel*
		msg_ModelSorted						//	PhotoPrintModel*
		};
		
	struct MessageRange {
		ConstPhotoIterator	mBegin;
		ConstPhotoIterator	mEnd;
		};
		
								PhotoPrintModel	(void);
								PhotoPrintModel	(const PhotoPrintModel& inOther);
	virtual						~PhotoPrintModel();
	
	virtual PhotoIterator		AdoptNewItem	(PhotoItemRef 	item,
												 PhotoIterator	inBefore);
	virtual void				RemoveEmptyItems(const bool 	inDelete = kRemove);
	virtual void				RemoveItems		(PhotoIterator 	inBegin,
												 PhotoIterator 	inEnd,
												 const bool 	inDelete = kRemove);
	virtual void				RemoveItems		(PhotoItemList& doomed, 
												 const bool 	inDelete = kRemove);
	virtual	void				RemoveLastItem	(const bool 	inDelete = kRemove);
	virtual	void				RemoveAllItems	(const bool 	inDelete = kRemove);

			PhotoIterator		begin() 					{ return mItemList.begin(); }
			PhotoIterator		end()						{ return mItemList.end(); }
			
			ConstPhotoIterator	begin() const				{ return mItemList.begin(); }
			ConstPhotoIterator	end() const					{ return mItemList.end(); }
			
			UInt32				GetCount() const			{ return mItemList.size(); }
			PhotoItemRef		GetFirstNonEmptyItem() const;
			UInt32				GetNonEmptyCount() const;
			bool				IsEmpty() const				{ return mItemList.empty(); }
			void				Sort(void);

	virtual PhotoDrawingProperties& GetDrawingProperties(void)	{ return mDrawingProps; }

	void 						Draw(MatrixRecord* destinationSpace = 0,
									CGrafPtr destPort = 0,
									GDHandle destDevice = 0,
									RgnHandle inClip = nil);

	//utility for remapping all items held in this model 
	virtual void				MapItems(const MRect& sourceRect, const MRect& destRect);
	
	virtual	void				ListenToMessage 	(MessageT	inMessage, 
													void *		ioParam);
};//end class PhotoPrintModel