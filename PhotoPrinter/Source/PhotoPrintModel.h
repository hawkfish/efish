/*
	File:		PhotoPrintModel.h

	Contains:	Definition of the main data (model) associated with a document.

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		03 aug 2000		dml		move selection to view
		30 Jun 2000		drd		DeleteAll
		26 Jun 2000		drd		GetCount is UInt32 to avoid conversion
		19 Jun 2000		drd		Added GetCount
		14 Jun 2000		drd		Added IsAnythingSelected
*/

#pragma once
#include "MRect.h"
#include "PhotoPrintItem.h"
#include <list>


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
								PhotoPrintModel(PhotoPrintView* inPane);
								PhotoPrintModel(PhotoPrintModel& inOther);
	virtual						~PhotoPrintModel();
	
	virtual void				SetDocument(PhotoPrintDoc* inDoc);
	virtual PhotoPrintDoc*		GetDocument(void) const {return mDoc;};
	virtual void				AdoptNewItem(PhotoItemRef item);
	virtual	void				DeleteAll()					{ mItemList.clear(); }
	virtual void				DeleteItem(PhotoItemRef doomed);

			PhotoIterator		begin() 					{ return mItemList.begin(); }
			PhotoIterator		end()						{ return mItemList.end(); }
			UInt32				GetCount() const			{ return mItemList.size(); }
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