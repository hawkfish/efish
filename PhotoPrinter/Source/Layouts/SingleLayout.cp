/*
	File:		SingleLayout.cp

	Contains:	Implementation of Layout object, which manages positioning of images.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		27 Jun 2000		drd		Override Initialize (to make a single item)
		26 Jun 2000		drd		Now a subclass of MultipleLayout
		23 Jun 2000		drd		Use HORef<PhotoPrintModel> in constructor
		19 Jun 2000		drd		Created
*/

#include "SingleLayout.h"

/*
SingleLayout
*/
SingleLayout::SingleLayout(HORef<PhotoPrintModel>& inModel)
	: MultipleLayout(inModel)
{
	mType = kSingle;
} // SingleLayout

/*
~SingleLayout
*/
SingleLayout::~SingleLayout()
{
} // ~SingleLayout

/*
CanAddToBackground {OVERRIDE}
*/
bool
SingleLayout::CanAddToBackground(const UInt16 inCount)
{
	return (inCount == 1);
} // CanAddToBackground

/*
Initialize {OVERRIDE}
*/
void
SingleLayout::Initialize()
{
	SInt16		docW = (SInt16)(mDocument->GetWidth() * mDocument->GetResolution() + 0.5);
	SInt16		docH = (SInt16)(mDocument->GetHeight() * mDocument->GetResolution() + 0.5);

	PhotoPrintItem*	theItem = new PhotoPrintItem();
	MRect			bounds;
	bounds.SetWidth(docW);
	bounds.SetHeight(docH);
	bounds.Inset(this->GetGutter(), this->GetGutter());
	theItem->SetDest(bounds);
	theItem->SetMaxBounds(bounds);

	mModel->AdoptNewItem(theItem);
} // Initialize
