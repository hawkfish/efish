/*
	File:		PhotoPrintModel.h

	Contains:	Definition of the main data (model) associated with a document.

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		 2 Jul 2001		rmgw	Add const begin/end.
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

#include <list>
#include "MRect.h"
#include "PhotoPrintItem.h"


class PhotoPrintDoc;
class PhotoPrintController;
class PhotoPrintView;

class PhotoPrintModel  {
protected:
	PhotoPrintView*			mPane;
	PhotoPrintDoc*			mDoc;		
	PhotoItemList			mItemList;
	PhotoDrawingProperties	mDrawingProps;
	
public:
	
	enum {
		kRemove = false,
		kDelete = true
		};
		
								PhotoPrintModel(PhotoPrintView* inPane);
								PhotoPrintModel(PhotoPrintModel& inOther);
	virtual						~PhotoPrintModel();
	
	virtual void				SetDocument(PhotoPrintDoc* inDoc);
	virtual PhotoPrintDoc*		GetDocument(void) const {return mDoc;};
	virtual void				AdoptNewItem(PhotoItemRef item);
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
			bool				IsEmpty() const				{ return mItemList.empty(); }
			void				Sort(void);

	virtual PhotoPrintView*		GetPane() const				{ return mPane; }
	virtual PhotoDrawingProperties& GetDrawingProperties(void)	{ return mDrawingProps; }

	void 						Draw(MatrixRecord* destinationSpace = 0,
									CGrafPtr destPort = 0,
									GDHandle destDevice = 0,
									RgnHandle inClip = nil);

	//utility for remapping all items held in this model 
	virtual void				MapItems(const MRect& sourceRect, const MRect& destRect);
	
	virtual void				SetDirty();			
				
};//end class PhotoPrintModel