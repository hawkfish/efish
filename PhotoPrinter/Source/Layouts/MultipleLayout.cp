/*
	File:		MultipleLayout.cp

	Contains:	Implementation of Layout object, which manages positioning of images.
				MultipleLayout has multiple copies of a single image.

	Written by:	David Dunham and Dav Lion

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		17 Aug 2000		drd		Removed LayoutImages (superclass works just fine);
								GetDistinctImages now inline in header file
		15 Aug 2000		drd		Moved Initialize to superclass
		14 aug 2000		dml		add GetDistinctImages (override)
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

	// typically, layouts take ownership of the incoming. 
	// but we just throw it away, since we use only the replicates
	delete (inItem);
} // AddItem

/*
CanAddToBackground {OVERRIDE}
*/
bool
MultipleLayout::CanAddToBackground(const UInt16 inCount)
{
	return (inCount == 1);
} // CanAddToBackground
