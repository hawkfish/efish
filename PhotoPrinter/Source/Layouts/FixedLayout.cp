/*
	File:		FixedLayout.cp

	Contains:	Implementation of Layout object, which manages positioning of images.
				FixedLayout has more than one image, of fixed number and size.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		15 Aug 2000		drd		Moved Initialize here (from MultipleLayout)
		09 Aug 2000		drd		Added dialog handling (and moved mImageCount to this class)
		23 Jun 2000		drd		Use HORef<PhotoPrintModel> in constructor
		19 Jun 2000		drd		Created
*/

#include "FixedLayout.h"
#include "EDialog.h"

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
	// See if there's anything we can take over
	PhotoIterator	i;
	for (i = mModel->begin(); i != mModel->end(); i++) {
		if ((*i)->IsEmpty()) {
			(*i)->SetFile(*inItem);
			mDocument->GetView()->RefreshItem(*i);
			delete inItem;							// Since it's not in the model
			return;
		}
	}

	mModel->AdoptNewItem(inItem);
	mDocument->GetView()->RefreshItem(inItem);
} // AddItem

/*
CanAddToBackground {OVERRIDE}
*/
bool
FixedLayout::CanAddToBackground(const UInt16 inCount)
{
	return (inCount == 1);
} // CanAddToBackground

/*
CommitOptionsDialog {OVERRIDE}
*/
void
FixedLayout::CommitOptionsDialog(EDialog& inDialog)
{
	Layout::CommitOptionsDialog(inDialog);

	LRadioGroupView*	layoutRadioGroup = inDialog.FindRadioGroupView('numb');
	if (layoutRadioGroup != nil) {
		PaneIDT		cur = layoutRadioGroup->GetCurrentRadioID();
		this->SetImageCount(cur);
	}
} // CommitOptionsDialog

/*
Initialize {OVERRIDE}
*/
void
FixedLayout::Initialize()
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
SetImageCount
*/
void
FixedLayout::SetImageCount(const UInt32 inCount)
{
	mImageCount = inCount;

	// !!!
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