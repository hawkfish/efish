/*
	File:		FixedLayout.cp

	Contains:	Implementation of Layout object, which manages positioning of images.
				FixedLayout has more than one image, of fixed number and size.

	Written by:	David Dunham and Dav Lion

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		18 Jan 2001		drd		CommitOptionsDialog returns value and has new arg
		16 jan 2001		dml		add gNeedDoubleOrientationSetting for lexmark debugging
		18 Sep 2000		drd		Allow dropping multiple items
		14 Sep 2000		drd		Added arg to GetDimensions
		07 sep 2000		dml		Initialize should call MakeNewImage, which should set max bounds!
		07 Sep 2000		drd		Override GetName to return size
		07 sep 2000		dml		cleanup AddItem. add TryToFillFirstEmpty, override AdjustDocumentOrientation 
		07 sep 2000		dml		AddItem selects via doc/view
		18 Aug 2000		drd		Use MakeNewImage in CommitOptionsDialog (so subclasses can override)
		16 Aug 2000		drd		CommitOptionsDialog changes number of items; Intialize
								sends LayoutImages so we respect preferences
		15 Aug 2000		drd		Moved Initialize here (from MultipleLayout)
		09 Aug 2000		drd		Added dialog handling (and moved mImageCount to this class)
		23 Jun 2000		drd		Use HORef<PhotoPrintModel> in constructor
		19 Jun 2000		drd		Created
*/

#include "FixedLayout.h"
#include "EDialog.h"
#include "PhotoPrinter.h"
#include "PhotoPrintPrefs.h"


static bool gNeedDoubleOrientationSetting = false;


/*
FixedLayout
*/
FixedLayout::FixedLayout(HORef<PhotoPrintModel>& inModel)
	: GridLayout(inModel)
{
	mImageCount = 2;
	mType = kFixed;
} // FixedLayout

/*
~FixedLayout
*/
FixedLayout::~FixedLayout()
{
} // ~FixedLayout

/*
AddItem {OVERRIDE}
	Add an item to the model, handling multiples properly. We replace placeholders.
	We are responsible for ownership of the item, either passing it on to the model or freeing it.
*/
void
FixedLayout::AddItem(PhotoItemRef inItem)
{
	if (TryToFillFirstEmpty(inItem))
		return;

// if we are here, that means there was no empty slot.  soo, 
// time to add an extra page!!
	mNumPages++;
	this->Initialize();
	Assert_(TryToFillFirstEmpty(inItem));

} // AddItem

/*
AdjustDocumentOrientation {OVERRIDE}
*/
void
FixedLayout::AdjustDocumentOrientation(SInt16 /*numPages*/)
{
	// printable area (taking into account margins, etc)
	MRect		printableArea;
	EPrintSpec* spec = mDocument->GetPrintRec();
	PhotoPrinter::CalculateBodyRect(spec, &(mDocument->GetPrintProperties()), printableArea);

	// Figure
	OSType		orientation;
	this->CalculateGrid(printableArea, mImageCount, mRows, mColumns, orientation);

	mItemsPerPage = this->GetRows() * this->GetColumns();
	mNumPages = mModel->GetCount() / mItemsPerPage;
	if (mNumPages * mItemsPerPage < mModel->GetCount()) 
		mNumPages++; 

	spec->SetOrientation(orientation);
	if (gNeedDoubleOrientationSetting)
		spec->SetOrientation(orientation);			// ??? Lexmark seems to need this
	mDocument->MatchViewToPrintRec(mNumPages); // do this anyway, since changes according to #pages
} // AdjustDocumentOrientation


/*
CanAddToBackground {OVERRIDE}
*/
bool
FixedLayout::CanAddToBackground(const UInt16 inCount)
{
#pragma unused(inCount)

	return true;								// Since we make more pages, it's OK to accept any number
} // CanAddToBackground

/*
CommitOptionsDialog {OVERRIDE}
*/
bool
FixedLayout::CommitOptionsDialog(EDialog& inDialog, const bool inDoLayout)
{
#pragma unused(inDoLayout)

	bool			needsLayout = Layout::CommitOptionsDialog(inDialog, kDontLayout);

	LRadioGroupView*	layoutRadioGroup = inDialog.FindRadioGroupView('numb');
	if (layoutRadioGroup != nil) {
		PaneIDT		cur = layoutRadioGroup->GetCurrentRadioID();
		this->SetImageCount(cur);
	}

	// Get rid of extra images. !!! not undoable
	while (mModel->GetCount() > mImageCount) {
		mModel->DeleteLastItem(PhotoPrintModel::kDelete);
		mDocument->GetProperties().SetDirty(true);
		needsLayout = true;
	}
	// Make new items if necessary
	while (mModel->GetCount() < mImageCount) {
		mModel->AdoptNewItem(this->MakeNewImage());
		needsLayout = true;
	}

	if (needsLayout) {
		mDocument->GetView()->Refresh();		// In case orientation changes
		this->LayoutImages();
		mDocument->GetView()->Refresh();
	}

	return needsLayout;
} // CommitOptionsDialog

/*
GetName {OVERRIDE}
	Return the name of this layout. We override to include the size of the images
	(since they're all the same, and don't change).
*/
LStr255
FixedLayout::GetName() const
{
	LStr255			theName(Layout::GetName());		// Get the basic name (i.e. "Fixed")
	PhotoItemRef	firstImage = *mModel->begin();
	LStr255			theSize;
	firstImage->GetDimensions(theSize, mDocument->GetResolution(), PhotoPrintItem::si_DimensionsInParens);
	theName += theSize;

	return theName;
} // GetName

/*
Initialize {OVERRIDE}
*/
void
FixedLayout::Initialize()
{
	// Just make two items, their size doesn't matter
	PhotoPrintItem*	theItem = MakeNewImage();
	mModel->AdoptNewItem(theItem);

	theItem = MakeNewImage();
	mModel->AdoptNewItem(theItem);

	// Create them according to the grid
	this->LayoutImages();
} // Initialize

/*
MakeNewImage
*/
PhotoPrintItem*
FixedLayout::MakeNewImage()
{
	PhotoPrintItem* newItem = new PhotoPrintItem();

	// figure out the maximum sizes
	double		hMax;
	double		vMax;
	PhotoItemProperties::SizeLimitToInches(PhotoPrintPrefs::Singleton()->GetMaximumSize(), hMax, vMax);
	// convert inches to screen resolution
	hMax *= mDocument->GetResolution();
	vMax *= mDocument->GetResolution();
	MRect		maximum(0, 0, vMax, hMax);			

	newItem->SetMaxBounds(maximum);
	return newItem;
} // MakeNewImage

/*
SetImageCount
	Caller should make sure to manage the items ???
*/
void
FixedLayout::SetImageCount(const UInt32 inCount)
{
	mImageCount = inCount;
} // SetImageCount

/*
SetupOptionsDialog {OVERRIDE}
*/
void
FixedLayout::SetupOptionsDialog(EDialog& inDialog)
{
	Layout::SetupOptionsDialog(inDialog);

	LRadioGroupView*	layoutRadioGroup = inDialog.FindRadioGroupView('numb');
	if (layoutRadioGroup != nil) {
		layoutRadioGroup->SetCurrentRadioID(mImageCount);
	}
} // SetupOptionsDialog




bool
FixedLayout::TryToFillFirstEmpty(PhotoItemRef inItem) {

	// See if there's anything we can take over
	PhotoIterator	i;
	for (i = mModel->begin(); i != mModel->end(); i++) {
		if ((*i)->IsEmpty()) {
			(*i)->SetFile(*inItem);
			mDocument->GetView()->AddToSelection(*i);
			delete inItem;							// Since it's not in the model
			return true;
		}//endif empty
	}//for all items in model

return false;
}//end TryToFillFirstEmpty



