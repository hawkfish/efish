//PhotoPrintModel.cp
// Copyright © 2000 Electric Fish, Inc.  All Rights Reserved

#include "PhotoPrintModel.h"
#include "PhotoPrintView.h"
#include "PhotoPrintController.h"

//---------------------------------
// PhotoPrintModel ct
//---------------------------------
PhotoPrintModel::PhotoPrintModel(PhotoPrintView* inView,
								PhotoPrintController* inController) 
	: mPane (inView)
	, mController (inController)
	, mSelection (0)
{
	}//end ct


//---------------------------------
// PhotoPrintModel dt
//---------------------------------
PhotoPrintModel::~PhotoPrintModel() {
	}//end dt
		

//---------------------------------
// AdoptNewItem
//---------------------------------
void	
PhotoPrintModel::AdoptNewItem(PhotoItemRef item) {
	mItemList.insert(mItemList.end(), item);
	}//end AdoptNewItem
	

//---------------------------------
// DeleteItem
//---------------------------------
void	
PhotoPrintModel::DeleteItem(PhotoItemRef doomed) {
	mItemList.remove(doomed);
	}//end DeleteItem


//---------------------------------
// Select
//---------------------------------
void	
PhotoPrintModel::Select(PhotoItemRef target) {
	if (mSelection != target) {
		mSelection = target;
		mPane->Refresh();
		}//end
	}//end Select
	
	
	
//---------------------------------
// Draw
//---------------------------------
void	
PhotoPrintModel::Draw(MatrixRecord* destinationSpace,
						CGrafPtr destPort,
						GDHandle destDevice) {
	for(PhotoIterator i = begin(); i != end(); ++i) 
		(*i)->Draw(destinationSpace, destPort, destDevice);

}//end Draw
	