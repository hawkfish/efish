/*
	File:		SchoolLayout.cp

	Contains:	Implementation of Layout object, which manages positioning of images.
				SchoolLayout has multiple copies of a single image, in varying sizes

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

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
#include "PhotoUtility.h"

/*
SchoolLayout
*/
SchoolLayout::SchoolLayout(HORef<PhotoPrintModel>& inModel)
	: MultipleLayout(inModel)
	, mReferenceOrientation (kLandscape)
{
	mType = kSchool;
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
void
SchoolLayout::AddItem(PhotoItemRef inItem)
{
	// which orientation are we to be?
	OSType newOrientation = inItem->IsPortrait() ? kPortrait : kLandscape;
	if (mReferenceOrientation != newOrientation) {
		mReferenceOrientation = newOrientation;
		Initialize();
		}//endif
		
	MultipleLayout::AddItem(inItem);
} // AddItem



/*
AdjustDocumentOrientation (OVERRIDE)
*/
void		
SchoolLayout::AdjustDocumentOrientation(SInt16 numPages) {
	EPrintSpec*		spec = (EPrintSpec*)mDocument->GetPrintRec();
	spec->SetOrientation(kPortrait);

	mDocument->MatchViewToPrintRec(numPages);
} // AdjustDocumentOrientation

/*
CommitOptionsDialog {OVERRIDE}
*/
void
SchoolLayout::CommitOptionsDialog(EDialog& inDialog)
{
	MultipleLayout::CommitOptionsDialog(inDialog);

	LRadioGroupView*	layoutRadioGroup = inDialog.FindRadioGroupView('layo');
	if (layoutRadioGroup != nil) {
		PaneIDT		cur = layoutRadioGroup->GetCurrentRadioID();
		this->SetImageCount(cur);
	}
} // CommitOptionsDialog

/*
Initialize {OVERRIDE}
*/
void
SchoolLayout::Initialize()
{
	SInt16		docW = (SInt16)(mDocument->GetWidth() * mDocument->GetResolution() + 0.5);
	SInt16		docH = (SInt16)(mDocument->GetHeight() * mDocument->GetResolution() + 0.5);

	PhotoPrintItem*	theItem = new PhotoPrintItem();
	MRect			bounds;
	this->GetCellBounds(1, bounds);
	theItem->SetDest(bounds);
	theItem->SetMaxBounds(bounds);

	mModel->AdoptNewItem(theItem);

	UInt32		i;

	if (mImageCount == 3) {
		// The smallest ones
		for (i = 2; i <= 3; i++) {
			theItem = new PhotoPrintItem();
			this->GetCellBounds(i, bounds);
			theItem->SetDest(bounds);
			theItem->SetMaxBounds(bounds);

			mModel->AdoptNewItem(theItem);
		}
	} else if (mImageCount == 5) {
		// The smallest ones
		for (i = 2; i <= 5; i++) {
			theItem = new PhotoPrintItem();
			this->GetCellBounds(i, bounds);
			theItem->SetMaxBounds(bounds);
			theItem->SetRotation(90.0); // set Rotation FIRST!!
			theItem->SetScreenDest(bounds);

			mModel->AdoptNewItem(theItem);
		}
	} else {
		// second one
		theItem = new PhotoPrintItem();
		this->GetCellBounds(2, bounds);
		theItem->SetDest(bounds);
		theItem->SetMaxBounds(bounds);

		mModel->AdoptNewItem(theItem);

		// Third one
		theItem = new PhotoPrintItem();
		this->GetCellBounds(3, bounds);
		theItem->SetMaxBounds(bounds);
		theItem->SetDest(bounds);

		mModel->AdoptNewItem(theItem);

		// The smallest ones
		for (i = 4; i <= 13; i++) {
			theItem = new PhotoPrintItem();
			this->GetCellBounds(i, bounds);
			theItem->SetMaxBounds(bounds);
			theItem->SetDest(bounds);

			mModel->AdoptNewItem(theItem);
		}
	}
} // Initialize

/*
GetCellBounds {OVERRIDE}
*/
void
SchoolLayout::GetCellBounds(
	const UInt32	inIndex,
	MRect&			outBounds)
{
	SInt16		docW = (SInt16)(mDocument->GetWidth() * mDocument->GetResolution() + 0.5);
	SInt16		docH = (SInt16)(mDocument->GetHeight() * mDocument->GetResolution() + 0.5);

	SInt16		w;
	MRect		cellBounds;

	if (mImageCount == 13) {
		if (inIndex == 1) {
			::SetRect(&outBounds, 0, 0, 6 * kDPI, 4 * kDPI);
			if (mReferenceOrientation == kPortrait) {
				SInt32	temp (outBounds.Height());
				outBounds.SetHeight(outBounds.Width());
				outBounds.SetWidth(temp);
				}				
			// Center horizontally
			outBounds.Offset((docW - outBounds.Width()) / 2, 0);
		} else if (inIndex <= 3) {
			w = (docW - this->GetGutter()) / 2;
			this->GetCellBounds(1, cellBounds);
			outBounds.top = cellBounds.bottom + this->GetGutter();
			outBounds.left = (w + this->GetGutter()) * (inIndex - 2);
			outBounds.SetWidth(w);
			outBounds.SetHeight(w * 3 / 4);
		} else if (inIndex <= 8) {
			w = (docW - this->GetGutter() * 4) / 5;
			this->GetCellBounds(2, cellBounds);
			outBounds.top = cellBounds.bottom + this->GetGutter();
			outBounds.left = (w + this->GetGutter()) * (inIndex - 4);
			outBounds.SetWidth(w);
			outBounds.SetHeight(w * 3 / 4);
		} else {
			w = (docW - this->GetGutter() * 4) / 5;
			this->GetCellBounds(4, cellBounds);
			outBounds.top = cellBounds.bottom + this->GetGutter();
			outBounds.left = (w + this->GetGutter()) * (inIndex - 9);
			outBounds.SetWidth(w);
			outBounds.SetHeight(w * 3 / 4);
		}
	} else if (mImageCount == 3) {
		if (inIndex == 1) {
			::SetRect(&outBounds, 0, 0, docW, docW * 3 / 4);
		} else {
			w = (docW - this->GetGutter()) / 2;
			this->GetCellBounds(1, cellBounds);
			outBounds.top = cellBounds.bottom + this->GetGutter();
			outBounds.left = (w + this->GetGutter()) * (inIndex - 2);
			outBounds.SetWidth(w);
			outBounds.SetHeight(w * 3 / 4);
		}
	} else {
		if (inIndex == 1) {
			::SetRect(&outBounds, 0, 0, docW, docW * 3 / 4);
		} else {
			w = (docW - this->GetGutter() * 3) / 4;
			this->GetCellBounds(1, cellBounds);
			outBounds.top = cellBounds.bottom + this->GetGutter();
			outBounds.left = (w + this->GetGutter()) * (inIndex - 2);
			outBounds.SetWidth(w);
			outBounds.SetHeight(w * 4 / 3);
		}
	}
} // GetCellBounds

/*
LayoutImages {OVERRIDE}
*/
void
SchoolLayout::LayoutImages()
{
	this->AdjustDocumentOrientation();

	SInt16		docW = (SInt16)(mDocument->GetWidth() * mDocument->GetResolution() + 0.5);
	SInt16		docH = (SInt16)(mDocument->GetHeight() * mDocument->GetResolution() + 0.5);

	// Place each
	PhotoIterator	iter;
	SInt16			i = 0;
	for (iter = mModel->begin(); iter != mModel->end(); iter++) {
		i++;									// Keep track of how many

		PhotoItemRef	item = *iter;
		MRect			itemBounds = item->GetNaturalBounds();

		MRect			cellBounds;
		this->GetCellBounds(i, cellBounds);
		AlignmentGizmo::FitAndAlignRectInside(itemBounds, cellBounds, kAlignAbsoluteCenter, itemBounds);

		item->SetDest(itemBounds);
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
	mModel->DeleteAll();

	// Make new ones (pass in which orientation to use)
	this->Initialize();

	// Populate them
	this->AddItem(theItem);

	mDocument->GetView()->Refresh();
} // SetImageCount

/*
SetupOptionsDialog {OVERRIDE}
*/
void
SchoolLayout::SetupOptionsDialog(EDialog& inDialog)
{
	MultipleLayout::SetupOptionsDialog(inDialog);

	LRadioGroupView*	layoutRadioGroup = inDialog.FindRadioGroupView('layo');
	if (layoutRadioGroup != nil) {
		layoutRadioGroup->SetCurrentRadioID(mImageCount);
	}
} // SetupOptionsDialog