/*
	File:		FixedLayout.cp

	Contains:	Implementation of Layout object, which manages positioning of images.
				FixedLayout has more than one image, of fixed number and size.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

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
	: Layout(inModel)
{
	mImageCount = 1;
	mType = kFixed;
} // FixedLayout

/*
~FixedLayout
*/
FixedLayout::~FixedLayout()
{
} // ~FixedLayout

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