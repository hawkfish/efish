/*
	File:		SchoolLayout.cp

	Contains:	Implementation of Layout object, which manages positioning of images.
				SchoolLayout has multiple copies of a single image, in varying sizes

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		27 Jun 2000		drd		Override Initialize
		23 Jun 2000		drd		Use HORef<PhotoPrintModel> in constructor
		19 Jun 2000		drd		Created
*/

#include "SchoolLayout.h"

/*
SchoolLayout
*/
SchoolLayout::SchoolLayout(HORef<PhotoPrintModel>& inModel)
	: MultipleLayout(inModel)
{
} // SchoolLayout

/*
~SchoolLayout
*/
SchoolLayout::~SchoolLayout()
{
} // ~SchoolLayout

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
	bounds.SetWidth(docW);
	bounds.SetHeight(docH / 2);
	bounds.Inset(this->GetGutter(), this->GetGutter());
	theItem->SetDest(bounds);
	theItem->SetMaxBounds(bounds);

	mModel->AdoptNewItem(theItem);

	// second one
	theItem = new PhotoPrintItem();
	bounds.Inset(-this->GetGutter(), -this->GetGutter());
	bounds.SetWidth(docW / 2);
	bounds.Offset(0, docH / 2);
	bounds.Inset(this->GetGutter(), this->GetGutter());
	theItem->SetDest(bounds);
	theItem->SetMaxBounds(bounds);
	theItem->SetRotation(90.0);

	mModel->AdoptNewItem(theItem);

	// Third one
	theItem = new PhotoPrintItem();
	bounds.Offset(docW / 2, 0);
	theItem->SetDest(bounds);
	theItem->SetMaxBounds(bounds);
	theItem->SetRotation(90.0);

	mModel->AdoptNewItem(theItem);
} // Initialize
