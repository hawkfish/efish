/*
	File:		MultipleLayout.cp

	Contains:	Implementation of Layout object, which manages positioning of images.
				MultipleLayout has multiple copies of a single image.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		29 jun 2000		dml		use FitAndAlignRectInside instead of BestFit + manual alignment
		27 Jun 2000		drd		LayoutImages sends AdjustDocumentOrientation
		26 Jun 2000		drd		AddItem, Initialize, LayoutImages
		23 Jun 2000		drd		Use HORef<PhotoPrintModel> in constructor
		19 Jun 2000		drd		Created
*/

#include "MultipleLayout.h"
#include "AlignmentGizmo.h"
#include "EUtil.h"

/*
MultipleLayout
*/
MultipleLayout::MultipleLayout(HORef<PhotoPrintModel>& inModel)
	: FixedLayout(inModel)
{
	mType = kMultiple;
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
	// First be sure the paper is switched optimally
	this->AdjustDocumentOrientation();

	// Place each
	PhotoIterator	iter;
	for (iter = mModel->begin(); iter != mModel->end(); iter++) {
		PhotoItemRef	item = *iter;
		MRect			imageBounds = item->GetNaturalBounds();

		MRect itemBounds;
		AlignmentGizmo::FitAndAlignRectInside(imageBounds, item->GetMaxBounds(), kAlignAbsoluteCenter,
												itemBounds, EUtil::kDontExpand);						
		item->SetDest(itemBounds);
	}
} // LayoutImages
