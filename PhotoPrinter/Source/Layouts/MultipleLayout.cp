/*
	File:		MultipleLayout.cp

	Contains:	Implementation of Layout object, which manages positioning of images.
				MultipleLayout has multiple copies of a single image.

	Written by:	David Dunham and Dav Lion

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		23 Jul 2001		rmgw	Add doc and type to constructor.
		20 Jul 2001		rmgw	Include PhotoPrintDoc.  Bug #200.
		09 Jul 2001		rmgw	AdoptNewItem now returns a PhotoIterator. Bug #142.
		06 Jul 2001		rmgw	Use CopyForTemplate.
		03 jul 2001		dml		SetDest, SetMaxBounds take PhotoDrawingProperties
		02 Jul 2001		rmgw	AdoptNewItem now takes a PhotoIterator.
		23 Apr 2001		drd		GetMaximumSize constraint comes from document
		06 feb 2001		dml		replace SetFile call with operator=
		28 feb 2001		dml		optimizations to AddItem 
		07 sep 2000		dml		MakeNewItem sets MaxBounds, now safe to call when nothing yet built
		07 sep 2000		dml		AddItem calls back to doc/view to select
		17 Aug 2000		drd		Override MakeNewImage to make copies
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
#include "PhotoPrintPrefs.h"
#include "PhotoPrintDoc.h"

/*
MultipleLayout
*/
MultipleLayout::MultipleLayout(

	PhotoPrintDoc*				inDoc,
	HORef<PhotoPrintModel>&		inModel,
	LayoutType					inType)

	: FixedLayout(inDoc, inModel, inType)
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
PhotoIterator
MultipleLayout::AddItem(

	PhotoItemRef 	inItem,
	PhotoIterator	inBefore)
{
	Assert_(inBefore == mModel->end());
	
	// as an optimization, we need to make sure the incoming item has a proxy, so that
	// so all the slaves can reference it 
	// otherwise, they will all create their own, which is stupid
	inItem->GetProxy();

	for(PhotoIterator i = mModel->begin(); i != mModel->end(); ++i) {
		PhotoItemRef	theItem = *i;
		//operator= does not change the local position/size, so templates work correctly!
		theItem->CopyForTemplate (*inItem);
		GetView()->AddToSelection(theItem);
	}

	// typically, layouts take ownership of the incoming. 
	// but we just throw it away, since we use only the replicates
	delete (inItem);
	
	return mModel->begin();	//	got a better idea?
	
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
MakeNewImage {OVERRIDE}
	Our new images should be copies
*/
PhotoPrintItem*
MultipleLayout::MakeNewImage()
{
	PhotoPrintItem*	theItem = new PhotoPrintItem();

	// figure out the maximum size
	double		hMax;
	double		vMax;
	PhotoItemProperties::SizeLimitToInches(GetDocument ()->GetMaximumSize(), hMax, vMax);
	// convert inches to screen resolution
	hMax *= GetDocument ()->GetResolution();
	vMax *= GetDocument ()->GetResolution();
	MRect		maximum(0, 0, vMax, hMax);			
	// set it!!
	PhotoDrawingProperties	drawProps (false, false, false, GetDocument()->GetResolution());
	theItem->SetMaxBounds(maximum, drawProps);

	PhotoPrintItem*	firstItem = *mModel->begin();
	if ((firstItem != *mModel->end()) && (!firstItem->IsEmpty()))
		theItem->CopyForTemplate (*firstItem);

	return theItem;
} // MakeNewImage
