/*
	File:		PhotoPrintModel.cp

	Contains:	Implementation of the model, which has all the data (in the MVC pattern).

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		19 jul 2000		dml		Draw culls based on clipRgn
		18 jul 2000		dml		added spinning cursor during Draw.  cursors defined in PhotoUtility.h
		29 Jun 2000		drd		Destructor gets rid of items
		20 Jun 2000		drd		Use PhotoPrintDoc::gCurDocument, so we're complete at constructor time
*/

#include "PhotoPrintModel.h"
#include "PhotoPrintView.h"
#include "PhotoPrintController.h"
#include "PhotoPrintDoc.h"
#include "ESpinCursor.h"
#include "PhotoUtility.h"

//---------------------------------
// PhotoPrintModel ct
//---------------------------------
PhotoPrintModel::PhotoPrintModel(PhotoPrintView* inView) 
	: mPane (inView)
	, mSelection (0)
	, mDoc (PhotoPrintDoc::gCurDocument)
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
	for (PhotoIterator i = begin(); i != end(); ++i)
		delete (*i);
} // ~PhotoPrintModel

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
// Draw
//---------------------------------
void	
PhotoPrintModel::Draw(MatrixRecord* destinationSpace,
						CGrafPtr destPort,
						GDHandle destDevice,
						RgnHandle inClip) {
	
	HORef<ESpinCursor>	spinCursor = new ESpinCursor(kFirstSpinCursor, kNumCursors);
	
	for(PhotoIterator i = begin(); i != end(); ++i) {
		if (inClip) {
			const MRect& dest = (*i)->GetDestRect();
			if (::RectInRgn(&dest, inClip) == false)
				continue;
			}//endif we have a clip region
		(*i)->Draw(GetDrawingProperties(), destinationSpace, destPort, destDevice, inClip);
		spinCursor->Spin();
		}//for
}//end Draw
	
//---------------------------------
// MapItems
//---------------------------------
void
PhotoPrintModel::MapItems(const MRect& sourceRect, const MRect& destRect) {
	for (PhotoIterator i = begin(); i != end(); ++i)
		(*i)->MapDestRect(sourceRect, destRect);
	mDoc->GetProperties().SetDirty(true);
} // MapItems
	
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
// SetDirty
//---------------------------------
void
PhotoPrintModel::SetDirty() {
	mDoc->GetProperties().SetDirty(true);
	GetPane()->Refresh();
}//end SetDirty
	
//---------------------------------
// SetDocument
//---------------------------------
void
PhotoPrintModel::SetDocument(PhotoPrintDoc* inDoc) {
	Assert_(mDoc == nil);
	mDoc = inDoc;
}//end SetDocument
