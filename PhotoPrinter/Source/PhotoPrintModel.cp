/*
	File:		PhotoPrintModel.cp

	Contains:	Implementation of the model, which has all the data (in the MVC pattern).

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		16 mar 2001		dml		do Draw RectInRgn test in destination coords!
		15 Feb 2001		rmgw	10 Bottleneck ALL item deletion in iterator routine
		4  jan 2000		dml		make sure that DeleteLastItem and DeleteItems remove from pane's selection also
		18 Sep 2000		drd		Draw passes spin cursor down
		31 aug 2000		dml		added 	CheckEventQueueForUserCancel() to Draw
		16 Aug 2000		drd		Added DeleteLastItem
		15 aug 2000		dml		stop --howMany in Sort (even though not used)
		11 Aug 2000		drd		DocumentProperties no longer has Empty
		04 Aug 2000		drd		Added disposal arg to DeleteItems
		04 aug 2000		dml		change DeleteItem to DeleteItems
		03 Aug 2000		drd		Tweaked sort constant names
		03 aug 2000		dml		move selection to view
		02 aug 2000		dml		add Sort
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
#include "ESortedFileList.h"
#include "PhotoPrintPrefs.h"

//---------------------------------
// PhotoPrintModel ct
//---------------------------------
PhotoPrintModel::PhotoPrintModel(PhotoPrintView* inView) 
	: mPane (inView)
	, mDoc (PhotoPrintDoc::gCurDocument)
{
}//end ct

//---------------------------------
// PhotoPrintModel ct
//---------------------------------
PhotoPrintModel::PhotoPrintModel(PhotoPrintModel& other)
	: mPane (other.GetPane())
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
	mDoc->GetProperties().SetDirty(true);
}//end AdoptNewItem
	
//---------------------------------
// 	RemoveItems
//---------------------------------
//	Basic item removal method.
void	
PhotoPrintModel::RemoveItems (

	PhotoIterator 	inBegin,
	PhotoIterator 	inEnd,
	const bool 		inDelete)

{ // begin RemoveItems

	//	Clear the selection
	GetPane()->RemoveFromSelection (inBegin, inEnd);
	
	//	Remove the items
	for (PhotoIterator i = inBegin; i != inEnd;) {
		//	Increment the iterator to make sure it is valid after the remove
		PhotoItemRef	item = *i++;
		
		//	Do this here because the iterators may be to mItemListÉ
		if (inDelete) delete (item);		
		
		mItemList.remove (item);
		} // if
		
	//	Flag the document as dirty
	mDoc->GetProperties().SetDirty (true);

}//end RemoveItems

//---------------------------------
// RemoveItems
//---------------------------------
void	
PhotoPrintModel::RemoveItems(

	PhotoItemList& 	doomed, 
	const bool 		inDelete)
{
	RemoveItems (doomed.begin(), doomed.end(), inDelete);

}//end DeleteItem

//---------------------------------
// RemoveLastItem
//	Removes the last item in our list, optionally deleting the object
//---------------------------------
void
PhotoPrintModel::RemoveLastItem(const bool inDelete)
{
	if (mItemList.rbegin () == mItemList.rend ()) return;
	
	PhotoItemList listCopy (mItemList.rbegin (), ++mItemList.rbegin ());
	RemoveItems (listCopy, inDelete);
}

//---------------------------------
// RemoveAllItems
//	Removes the last item in our list, optionally deleting the object
//---------------------------------
void
PhotoPrintModel::RemoveAllItems(const bool inDelete)
{
	RemoveItems (mItemList, inDelete);
}

//---------------------------------
// Draw
//
// inClip is in destination space coordinates.
//---------------------------------
void	
PhotoPrintModel::Draw(MatrixRecord* destinationSpace,
						CGrafPtr destPort,
						GDHandle destDevice,
						RgnHandle inClip)
{	
	HORef<ESpinCursor>	spinCursor = new ESpinCursor(kFirstSpinCursor, kNumCursors,
										ESpinCursor::kDontShow);
	
	for(PhotoIterator i = begin(); i != end(); ++i) {
		if (inClip) {
			MRect dest ((*i)->GetDestRect());
			if (destinationSpace)
				::TransformRect(destinationSpace, &dest, NULL);
			if (::RectInRgn(&dest, inClip) == false)
				continue;
			}//endif we have a clip region
		(*i)->Draw(GetDrawingProperties(), destinationSpace, destPort, destDevice, inClip,
			spinCursor);		
		if (::CheckEventQueueForUserCancel())
			break;
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

//---------------------------------
// Sort
//---------------------------------
void
PhotoPrintModel::Sort()
{
	// make the basic predicate (type of sort, not direction)
	HORef<SortedFilePredicate::Predicate> comp;
	switch (PhotoPrintPrefs::Singleton()->GetSorting()) {
		case sort_Name: 
			comp = new SortedFilePredicate::NameComparator;
			break;
		case sort_Creation:
			comp = new SortedFilePredicate::CreatedComparator;
			break;
		case sort_Modification:
			comp = new SortedFilePredicate::ModifiedComparator;
			break;
		case sort_None: 
			return;
		default:
			SignalStringLiteral_("Illegal Sorting code ");
			break;
		}//switch

	UInt32 howMany = GetCount();
	
	// now, use the comparator (and interpret ascending/descending via a NOT comparator)
	FullFileList			sortedList;
	if (PhotoPrintPrefs::Singleton()->GetSortAscending()) {
		MakeSortedFileList (sortedList, begin(), end(), *comp);
		}//endif sort upward
	else {
		SortedFilePredicate::Not notComp = (comp);
		MakeSortedFileList (sortedList, begin(), end(), notComp);
		}//else	

	// empty out our list, and fill it with the sorted list
	mItemList.clear();
	for (FullFileList::iterator i (sortedList.begin()); i != sortedList.end(); ++i) {
		FileProvider		provider ((*i)->first);
		PhotoPrintItem*		item = dynamic_cast<PhotoPrintItem*>((FileSpecProvider*)provider);
		mItemList.insert(mItemList.end(), item);
	}//for all items in list
}//end Sort
