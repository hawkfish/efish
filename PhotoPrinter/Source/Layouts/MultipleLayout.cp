/*
	File:		MultipleLayout.cp

	Contains:	Implementation of Layout object, which manages positioning of images.
				MultipleLayout has multiple copies of a single image.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		26 Jun 2000		drd		AddItem, Initialize, LayoutImages
		23 Jun 2000		drd		Use HORef<PhotoPrintModel> in constructor
		19 Jun 2000		drd		Created
*/

#include "MultipleLayout.h"

#include "EUtil.h"

/*
MultipleLayout
*/
MultipleLayout::MultipleLayout(HORef<PhotoPrintModel>& inModel)
	: FixedLayout(inModel)
{
} // MultipleLayout

/*
~MultipleLayout
*/
MultipleLayout::~MultipleLayout()
{
} // ~MultipleLayout

/*
AddItem {OVERRIDE}
	Add an item to the model, handling multiples properly.
	We make each one the same.
*/
void
MultipleLayout::AddItem(PhotoItemRef inItem)
{
	for(PhotoIterator i = mModel->begin(); i != mModel->end(); i++) {
		PhotoItemRef	theItem = *i;
		theItem->SetFile(*inItem);
	}
} // AddItem

/*
CanAddToBackground {OVERRIDE}
*/
bool
MultipleLayout::CanAddToBackground(const UInt16 inCount)
{
	return (inCount == 1);
} // CanAddToBackground

/*
Initialize {OVERRIDE}
*/
void
MultipleLayout::Initialize()
{
	PhotoPrintItem*	theItem = new PhotoPrintItem();
	MRect			bounds1(40, 20, 288, 452);
	theItem->SetDest(bounds1);
	theItem->SetMaxBounds(bounds1);

	mModel->AdoptNewItem(theItem);

	theItem = new PhotoPrintItem();
	MRect			bounds2(350, 20, 598, 452);
	theItem->SetDest(bounds2);
	theItem->SetMaxBounds(bounds2);

	mModel->AdoptNewItem(theItem);

} // Initialize

/*
LayoutImages {OVERRIDE}
	We adjust bounds to maintain aspect ratios
*/
void
MultipleLayout::LayoutImages()
{
	// Place each
	PhotoIterator	iter;
	for (iter = mModel->begin(); iter != mModel->end(); iter++) {
		PhotoItemRef	item = *iter;
		MRect			imageBounds = item->GetNaturalBounds();
		long			propWidth = imageBounds.Width();
		long			propHeight = imageBounds.Height();

		MRect			itemBounds = item->GetMaxBounds();
		long			fitWidth = itemBounds.Width();
		long			fitHeight = itemBounds.Height();
			
		long			outWidth;
		long			outHeight;
		EUtil::BestFit(outWidth, 
							 outHeight,
							 fitWidth,
							 fitHeight,
							 propWidth,
							 propHeight,
							 EUtil::kDontExpand);

		itemBounds.SetWidth(outWidth);
		itemBounds.SetHeight(outHeight);	

		itemBounds.Offset(((fitWidth - itemBounds.Width()) / 2),
							((fitHeight - itemBounds.Height()) / 2));
		item->SetDest(itemBounds);
	}
} // LayoutImages
