/*
	File:		SchoolLayout.cp

	Contains:	Implementation of Layout object, which manages positioning of images.
				SchoolLayout has multiple copies of a single image, in varying sizes

	Written by:	David Dunham

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		01 Aug 2001		drd		161 266 Be smarter with mReferenceOrientation
		01 Aug 2001		drd		250 AlignToRightEdge; 161 266 Added arg to GetCellBounds
		25 Jul 2001		drd		15 Use ESpinCursor::SpinWatch instead of UCursor::SetWatch
		23 jul 2001		dml		179 add CalcOrientation
		23 Jul 2001		rmgw	Add doc and type to constructor.
		20 Jul 2001		rmgw	Include PhotoPrintDoc.  Bug #200.
		17 Jul 2001		drd		Turn School-5 into School-10
		10 jul 2001		dml		141.  Deal w/ transformed templates in LayoutImages
		09 Jul 2001		rmgw	AdoptNewItem now returns a PhotoIterator. Bug #142.
		09 Jul 2001		drd		130 Optimize AdjustDocumentOrientation if no need to change
		06 Jul 2001		drd		128 LayoutImages calls SetWatch
		06 jul 2001		dml		Initialize calls AdjustDocumentOrientation
		03 jul 2001		dml		SetDest, SetMaxBounds take PhotoDrawingProperties,
								also, SetMaxBounds occurs BEFORE SetDest
		02 Jul 2001		rmgw	AdoptNewItem now takes a PhotoIterator.
		16 May 2001		drd		38 We can use generic options dialog
		15 Feb 2001		rmgw	10 DeleteAll => RemoveAllItems
		18 Jan 2001		drd		CommitOptionsDialog returns value and has new arg
		07 sep 2000		dml		GetCellBounds calculates with actual resolution!
		29 Aug 2000		drd		Call SetOrientation (for Lexmark printer driver)
		14 aug 2000		dml		pass landscape/portrait to Initialize
		09 Aug 2000		drd		Use FindRadioGroupView
		13 jul 2000		dml		add numPages to AdjustDocumentOrientation
		03 Jul 2000		drd		Changed 13 layout; use FitAndAlignRectInside
		30 Jun 2000		drd		AdjustDocumentOrientation forces portrait; LayoutImages uses new
								GetCellBounds; initialize for various counts; dialog
		29 jun 2000		dml		override AdjustDocumentOrientation to not rotate
		27 jun 2000		dml		use Item::SetScreenDest() to instantiate rotated empty
		27 Jun 2000		drd		Override Initialize
		23 Jun 2000		drd		Use HORef<PhotoPrintModel> in constructor
		19 Jun 2000		drd		Created
*/

#include "SchoolLayout.h"

#include "AlignmentGizmo.h"
#include "EDialog.h"
#include "ESpinCursor.h"
#include "PhotoUtility.h"
#include "PhotoPrintDoc.h"

/*
SchoolLayout
*/
SchoolLayout::SchoolLayout(

	PhotoPrintDoc*				inDoc, 
	HORef<PhotoPrintModel>& 	inModel,
	LayoutType					inType)

	: MultipleLayout(inDoc, inModel, inType)
	, mReferenceOrientation (kLandscape)
{
	mImageCount = 13;
} // SchoolLayout

/*
~SchoolLayout
*/
SchoolLayout::~SchoolLayout()
{
} // ~SchoolLayout

//--------------------------------------------------
//  AddItem (OVERRIDE)
//--------------------------------------------------
PhotoIterator
SchoolLayout::AddItem(

	PhotoItemRef	inItem, 
	PhotoIterator	inBefore)
{
	Assert_(inBefore == mModel->end ());
	// which orientation are we to be?
	OSType newOrientation = inItem->IsPortrait() ? kPortrait : kLandscape;
	if (mReferenceOrientation != newOrientation) {
		mReferenceOrientation = newOrientation;
		mModel->RemoveAllItems();
		this->Initialize();
	}//endif

	return MultipleLayout::AddItem(inItem, mModel->end ());
} // AddItem


/*
AdjustDocumentOrientation (OVERRIDE)
*/
void		
SchoolLayout::AdjustDocumentOrientation(SInt16 numPages) {
	EPrintSpec*		spec = (EPrintSpec*)GetDocument()->GetPrintRec();
	if (spec->GetOrientation() != kPortrait) {
		spec->SetOrientation(kPortrait, PhotoUtility::gNeedDoubleOrientationSetting);
	}

	GetDocument()->MatchViewToPrintRec(numPages);
} // AdjustDocumentOrientation

/*
AlignToRightEdge
*/
void
SchoolLayout::AlignToRightEdge(MRect& ioBounds, const SInt16 inWidth)
{
	if (ioBounds.right != inWidth)
		ioBounds.Offset(inWidth - ioBounds.right, 0);
} // AlignToRightEdge

OSType		
SchoolLayout::CalcOrientation() const
{
	return kPortrait;
}//end


/*
Initialize {OVERRIDE}
*/
void
SchoolLayout::Initialize()
{
	this->AdjustDocumentOrientation();

	PhotoPrintItem*	theItem = new PhotoPrintItem();
	MRect			bounds;
	this->GetCellBounds(1, bounds, kRecalcIfNeeded);
	PhotoDrawingProperties	drawProps (false, false, false, GetDocument()->GetResolution());
	theItem->SetMaxBounds(bounds, drawProps);
	if (mReferenceOrientation == kPortrait) {
		theItem->SetRotation(90.0); // set Rotation FIRST!!
	}

	theItem->SetDest(bounds, drawProps);

	mModel->AdoptNewItem(theItem, mModel->end ());

	UInt32		i;

	if (mImageCount == 3) {
		// The smallest ones
		for (i = 2; i <= 3; i++) {
			theItem = new PhotoPrintItem();
			this->GetCellBounds(i, bounds, kRecalcIfNeeded);
			theItem->SetMaxBounds(bounds, drawProps);
			if (mReferenceOrientation == kPortrait) {
				theItem->SetRotation(90.0); // set Rotation FIRST!!
			}
			theItem->SetDest(bounds, drawProps);

			mModel->AdoptNewItem(theItem, mModel->end ());
		}
	} else if (mImageCount == 10) {
		// The small ones
		for (i = 2; i <= 5; i++) {
			theItem = new PhotoPrintItem();
			this->GetCellBounds(i, bounds, kRecalcIfNeeded);
			theItem->SetMaxBounds(bounds, drawProps);
			if (mReferenceOrientation == kLandscape) {
				theItem->SetRotation(90.0); // set Rotation FIRST!!
			}
			theItem->SetDest(bounds, drawProps);// needed to setup  for SetScreenDest call since empty
			theItem->SetScreenDest(bounds, drawProps);

			mModel->AdoptNewItem(theItem, mModel->end ());
		}
		// And a row of even smaller ones
		for (i = 6; i <= 10; i++) {
			theItem = new PhotoPrintItem();
			this->GetCellBounds(i, bounds, kRecalcIfNeeded);
			theItem->SetMaxBounds(bounds, drawProps);
			if (mReferenceOrientation == kPortrait) {
				theItem->SetRotation(90.0); // set Rotation FIRST!!
			}
			theItem->SetDest(bounds, drawProps);

			mModel->AdoptNewItem(theItem, mModel->end ());
		}
	} else {
		// second one
		theItem = new PhotoPrintItem();
		this->GetCellBounds(2, bounds, kRecalcIfNeeded);
		theItem->SetMaxBounds(bounds, drawProps);
		if (mReferenceOrientation == kPortrait) {
			theItem->SetRotation(90.0); // set Rotation FIRST!!
		}
		theItem->SetDest(bounds, drawProps);

		mModel->AdoptNewItem(theItem, mModel->end ());

		// Third one
		theItem = new PhotoPrintItem();
		this->GetCellBounds(3, bounds, kRecalcIfNeeded);
		theItem->SetMaxBounds(bounds, drawProps);
		if (mReferenceOrientation == kPortrait) {
			theItem->SetRotation(90.0); // set Rotation FIRST!!
		}
		theItem->SetDest(bounds, drawProps);

		mModel->AdoptNewItem(theItem, mModel->end ());

		// The smallest ones
		for (i = 4; i <= 13; i++) {
			theItem = new PhotoPrintItem();
			this->GetCellBounds(i, bounds, kRecalcIfNeeded);
			theItem->SetMaxBounds(bounds, drawProps);
			if (mReferenceOrientation == kPortrait) {
				theItem->SetRotation(90.0); // set Rotation FIRST!!
			}
			theItem->SetDest(bounds, drawProps);

			mModel->AdoptNewItem(theItem, mModel->end ());
		}
	}
} // Initialize

/*
GetCellBounds {OVERRIDE}
*/
void
SchoolLayout::GetCellBounds(
	const UInt32	inIndex,
	MRect&			outBounds,
	const bool		inRaw)
{
	// Don't round -- if we are a wee bit small it's better than going outside available area
	SInt16		docW = (SInt16)(GetDocument()->GetWidth() * GetDocument()->GetResolution());
	SInt16		docH = (SInt16)(GetDocument()->GetHeight() * GetDocument()->GetResolution());

	SInt16		w;
	MRect		cellBounds;

	// Figure out the total height. If we don't fit, then we'll have to adjust width (since
	// that's what we calculate off of).
	if (inRaw == kRecalcIfNeeded) {
		this->GetCellBounds(mImageCount, cellBounds, kRawBounds);
		if (cellBounds.bottom > docH) {
			SInt16		availableHeight = docH;
			if (mImageCount == 13) {
				// We're always making the first one 4 * 6, so we can only scale the rest
				availableHeight -= 4 * this->GetDocument()->GetResolution();
				cellBounds.bottom -= 4 * this->GetDocument()->GetResolution();
			}
			docW *= ((double) availableHeight / (double) cellBounds.bottom);
		}
	}

	if (mImageCount == 13) {
		if (inIndex == 1) {
			::SetRect(&outBounds, 0, 0, 6 * GetDocument()->GetResolution(), 4 * GetDocument()->GetResolution());
			// Center horizontally
			outBounds.Offset((docW - outBounds.Width()) / 2, 0);
		} else if (inIndex <= 3) {
			w = (docW - this->GetGutter()) / 2;
			this->GetCellBounds(1, cellBounds, inRaw);
			outBounds.top = cellBounds.bottom + this->GetGutter();
			outBounds.left = (w + this->GetGutter()) * (inIndex - 2);
			outBounds.SetWidth(w);
			outBounds.SetHeight(w * 3 / 4);
			if (inIndex == 3)
				this->AlignToRightEdge(outBounds, docW);
		} else if (inIndex <= 8) {
			w = (docW - this->GetGutter() * 4) / 5;
			this->GetCellBounds(2, cellBounds, inRaw);
			outBounds.top = cellBounds.bottom + this->GetGutter();
			outBounds.left = (w + this->GetGutter()) * (inIndex - 4);
			outBounds.SetWidth(w);
			outBounds.SetHeight(w * 3 / 4);
			if (inIndex == 8)
				this->AlignToRightEdge(outBounds, docW);
		} else {
			w = (docW - this->GetGutter() * 4) / 5;
			this->GetCellBounds(4, cellBounds, inRaw);
			outBounds.top = cellBounds.bottom + this->GetGutter();
			outBounds.left = (w + this->GetGutter()) * (inIndex - 9);
			outBounds.SetWidth(w);
			outBounds.SetHeight(w * 3 / 4);
			if (inIndex == 13)
				this->AlignToRightEdge(outBounds, docW);
		}
	} else if (mImageCount == 3) {
		if (inIndex == 1) {
			::SetRect(&outBounds, 0, 0, docW, docW * 3 / 4);
		} else {
			w = (docW - this->GetGutter()) / 2;
			this->GetCellBounds(1, cellBounds, inRaw);
			outBounds.top = cellBounds.bottom + this->GetGutter();
			outBounds.left = (w + this->GetGutter()) * (inIndex - 2);
			outBounds.SetWidth(w);
			outBounds.SetHeight(w * 3 / 4);
			if (inIndex == 3)
				this->AlignToRightEdge(outBounds, docW);
		}
	} else {	// 10
		if (inIndex == 1) {
			::SetRect(&outBounds, 0, 0, docW, docW * 3 / 4);
		} else if (inIndex <= 5) {
			w = (docW - this->GetGutter() * 3) / 4;
			this->GetCellBounds(1, cellBounds, inRaw);
			outBounds.top = cellBounds.bottom + this->GetGutter();
			outBounds.left = (w + this->GetGutter()) * (inIndex - 2);
			outBounds.SetWidth(w);
			outBounds.SetHeight(w * 4 / 3);
			if (inIndex == 5)
				this->AlignToRightEdge(outBounds, docW);
		} else {
			w = (docW - this->GetGutter() * 4) / 5;
			this->GetCellBounds(4, cellBounds, inRaw);
			outBounds.top = cellBounds.bottom + this->GetGutter();
			outBounds.left = (w + this->GetGutter()) * (inIndex - 6);
			outBounds.SetWidth(w);
			outBounds.SetHeight(w * 3 / 4);
			if (inIndex == 10)
				this->AlignToRightEdge(outBounds, docW);
		}
	}
} // GetCellBounds

/*
LayoutImages {OVERRIDE}
*/
void
SchoolLayout::LayoutImages()
{
	ESpinCursor::SpinWatch();

	this->AdjustDocumentOrientation();

	// Place each
	PhotoIterator	iter;
	SInt16			i = 0;
	for (iter = mModel->begin(); iter != mModel->end(); iter++) {
		++i;									// Keep track of how many

		PhotoItemRef	item = *iter;
		MRect			itemBounds = item->GetNaturalBounds();
		if (!itemBounds)
			itemBounds = item->GetMaxBounds(); // empty items have only max, natural is unset

		MRect			cellBounds;
		this->GetCellBounds(i, cellBounds, kRecalcIfNeeded);

		PhotoDrawingProperties	drawProps (false, false, false, GetDocument()->GetResolution());
		item->SetMaxBounds(cellBounds, drawProps);
		
		if (!PhotoUtility::DoubleEqual(0.0, item->GetRotation())) {
			MatrixRecord m;
						
			SetIdentityMatrix(&m);
			::RotateMatrix (&m, ::Long2Fix(item->GetRotation()), ::Long2Fix(itemBounds.MidPoint().h), ::Long2Fix(itemBounds.MidPoint().v));

			AlignmentGizmo::FitTransformedRectInside(itemBounds, &m, cellBounds, itemBounds);
			AlignmentGizmo::MoveMidpointTo(itemBounds, cellBounds, itemBounds);
			}//endif rotated
		else 
			AlignmentGizmo::FitAndAlignRectInside(itemBounds, cellBounds, kAlignAbsoluteCenter, itemBounds);

		item->SetDest(itemBounds, drawProps);
	}
} // LayoutImages

/*
SetImageCount {OVERRIDE}
*/
void
SchoolLayout::SetImageCount(const UInt32 inCount)
{
	mImageCount = inCount;

	// Get a copy of the first item
	PhotoItemRef	theItem = new PhotoPrintItem(**mModel->begin());

	// Get rid of any items that were previously there
	mModel->RemoveAllItems (PhotoPrintModel::kDelete);

	// Make new ones 
	this->Initialize();

	// Populate them
	this->AddItem(theItem, mModel->end ());

	// and figure out where they go
	this->LayoutImages();

	GetView ()->Refresh();
} // SetImageCount
