// PhotoPrintModel.h
// Copyright © 2000 Electric Fish, Inc.  All Rights Reserved

#pragma once
#include "MRect.h"
#include "PhotoPrintItem.h"
#include <list>

typedef std::list<PhotoItemRef>	PhotoItemList;
typedef	PhotoItemList::iterator	PhotoIterator;

class PhotoPrintController;
class PhotoPrintView;

class PhotoPrintModel  {
	protected:
		PhotoPrintView*	mPane;
		PhotoPrintController*	mController;
		
		PhotoItemList	mItemList;
		PhotoItemRef	mSelection;
		
	public:
									PhotoPrintModel(PhotoPrintView* inPane,
													PhotoPrintController* inController);
		virtual						~PhotoPrintModel();
		
		virtual void				AdoptNewItem(PhotoItemRef item);
		virtual void				DeleteItem(PhotoItemRef doomed);

				PhotoIterator		begin() 	{return mItemList.begin();};
				PhotoIterator		end()		{return mItemList.end();};

		virtual void 				Select(PhotoItemRef target);
		virtual const PhotoItemRef 	GetSelection(void) const {return mSelection;};
		virtual PhotoPrintView*	GetPane() {return mPane;};

		void Draw(MatrixRecord* destinationSpace = 0,
					CGrafPtr destPort = 0,
					GDHandle destDevice = 0);
	};//end class PhotoPrintModel