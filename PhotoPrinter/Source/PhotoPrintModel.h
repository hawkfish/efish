// PhotoPrintModel.h
// Copyright © 2000 Electric Fish, Inc.  All Rights Reserved

#pragma once
#include "MRect.h"
#include "PhotoPrintItem.h"
#include <list>

typedef std::list<PhotoItemRef>	PhotoItemList;
typedef	PhotoItemList::iterator	PhotoIterator;

class PhotoPrintDoc;
class PhotoPrintController;
class PhotoPrintView;

class PhotoPrintModel  {
	protected:
		PhotoPrintView*	mPane;
		PhotoPrintDoc*	mDoc;		
		PhotoItemList	mItemList;
		PhotoItemRef	mSelection;
		PhotoDrawingProperties mDrawingProps;
		
	public:
									PhotoPrintModel(PhotoPrintView* inPane);
									PhotoPrintModel(PhotoPrintModel& inOther);
		virtual						~PhotoPrintModel();
		
		virtual void				SetDocument(PhotoPrintDoc* inDoc);
		virtual PhotoPrintDoc*		GetDocument(void) const {return mDoc;};
		virtual void				AdoptNewItem(PhotoItemRef item);
		virtual void				DeleteItem(PhotoItemRef doomed);

				PhotoIterator		begin() 	{return mItemList.begin();};
				PhotoIterator		end()		{return mItemList.end();};

		virtual void 				Select(PhotoItemRef target);
		virtual const PhotoItemRef 	GetSelection(void) const {return mSelection;};
		virtual PhotoPrintView*		GetPane() const {return mPane;};
		virtual PhotoDrawingProperties& GetDrawingProperties(void) {return mDrawingProps;};

		void 						Draw(MatrixRecord* destinationSpace = 0,
										CGrafPtr destPort = 0,
										GDHandle destDevice = 0,
										RgnHandle inClip = nil);

		//utility for remapping all items held in this model 
		virtual void				MapItems(const MRect& sourceRect, const MRect& destRect);
		
		virtual void				SetDirty();			
					
	};//end class PhotoPrintModel