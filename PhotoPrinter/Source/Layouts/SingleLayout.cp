/*
	File:		SingleLayout.cp

	Contains:	Implementation of Layout object, which manages positioning of images.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):
		
		20 Jul 2001		rmgw	Include PhotoPrintDoc.  Bug #200.
		03 jul 2001		dml		SetDest, SetMaxBounds take PhotoDrawingProperties
		02 Jul 2001		rmgw	AdoptNewItem now takes a PhotoIterator.
		07 mar 2001		dml		mImageCount = 1!
		27 Jun 2000		drd		Override Initialize (to make a single item)
		26 Jun 2000		drd		Now a subclass of MultipleLayout
		23 Jun 2000		drd		Use HORef<PhotoPrintModel> in constructor
		19 Jun 2000		drd		Created
*/

#include "SingleLayout.h"

#include "PhotoPrintDoc.h"

/*
SingleLayout
*/
SingleLayout::SingleLayout(HORef<PhotoPrintModel>& inModel)
	: MultipleLayout(inModel)
{
	mType = kSingle;
	mImageCount = 1;
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

	PhotoDrawingProperties	drawProps (false, false, false, mModel->GetDocument()->GetResolution());
	theItem->SetMaxBounds(bounds, drawProps);
	theItem->SetDest(bounds, drawProps);

	mModel->AdoptNewItem(theItem, mModel->end ());
} // Initialize
