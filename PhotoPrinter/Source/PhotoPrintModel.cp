//PhotoPrintModel.cp
// Copyright © 2000 Electric Fish, Inc.  All Rights Reserved

#include "PhotoPrintModel.h"
#include "PhotoPrintView.h"
#include "PhotoPrintController.h"
#include "PhotoPrintDoc.h"

//---------------------------------
// PhotoPrintModel ct
//---------------------------------
PhotoPrintModel::PhotoPrintModel(PhotoPrintView* inView) 
	: mPane (inView)
	, mSelection (0)
	, mDoc (0)
{
	}//end ct


//---------------------------------
// PhotoPrintModel ct
//---------------------------------
PhotoPrintModel::PhotoPrintModel(PhotoPrintModel& other)
	: mPane (other.GetPane())
	, mSelection (0)
	, mDoc (0)
{
		SetDocument(other.GetDocument());
		for (PhotoIterator item (other.begin ()); item != other.end (); ++item) {	// for each item
			PhotoItemRef	copyRef (new PhotoPrintItem (**item));
			AdoptNewItem (copyRef);
          	} // for all items in other

	}//end copy ct




//---------------------------------
// PhotoPrintModel dt
//---------------------------------
PhotoPrintModel::~PhotoPrintModel() {
	
	}//end dt
		


//---------------------------------
// SetDocument
//---------------------------------
void
PhotoPrintModel::SetDocument(PhotoPrintDoc* inDoc) {
	Assert_(mDoc == nil);
	mDoc = inDoc;
	}//end SetDocument


//---------------------------------
// AdoptNewItem
//---------------------------------
void	
PhotoPrintModel::AdoptNewItem(PhotoItemRef item) {
	mItemList.insert(mItemList.end(), item);
	mDoc->GetProperties().SetEmpty(false);
	mDoc->GetProperties().SetDirty(true);
	}//end AdoptNewItem
	

//---------------------------------
// DeleteItem
//---------------------------------
void	
PhotoPrintModel::DeleteItem(PhotoItemRef doomed) {
	mItemList.remove(doomed);
	mDoc->GetProperties().SetDirty(true);
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
						GDHandle destDevice,
						RgnHandle inClip) {
	for(PhotoIterator i = begin(); i != end(); ++i) 
		(*i)->Draw(destinationSpace, destPort, destDevice, inClip);

}//end Draw
	
	
	
//---------------------------------
// MapItems
//---------------------------------
void
PhotoPrintModel::MapItems(const MRect& sourceRect, const MRect& destRect) {
	for (PhotoIterator i = begin(); i != end(); ++i)
		(*i)->MapDestRect(sourceRect, destRect);
	mDoc->GetProperties().SetDirty(true);
}//end MapContents
	
	
	
	
//---------------------------------
// SetDirty
//---------------------------------
void
PhotoPrintModel::SetDirty() {
	mDoc->GetProperties().SetDirty(true);
	GetPane()->Refresh();
	}//end SetDirty
	
	