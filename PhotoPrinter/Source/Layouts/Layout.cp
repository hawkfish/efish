/*
	File:		Layout.cp

	Contains:	Implementation of Layout object, which manages positioning of images.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		30 Jun 2000		drd		SetupOptionsDialog
		28 jun 2000		dml		fix AdjustDocumentOrientation (EPrintSPec*!!)
		27 Jun 2000		drd		AdjustDocumentOrientation, CountOrientation
		27 jun 2000		dml		add HFSPromise Drag Receiving
		26 Jun 2000		drd		AddItem
		23 Jun 2000		drd		Use HORef<PhotoPrintModel> in constructor
		21 Jun 2000		drd		ItemIsAcceptable; allow nil model
		19 Jun 2000		drd		Added mRows, mColumns, mGutter
		19 Jun 2000		drd		Created
*/

#include "Layout.h"

#include "EDialog.h"
#include <LGAColorSwatchControl.h>
#include "PhotoUtility.h"

/*
Layout
*/
Layout::Layout(HORef<PhotoPrintModel>& inModel)
	: mType (kUnspecified)
	, mModel(inModel)
	, mRows(1)
	, mColumns(1)
	, mGutter(kDefaultGutter)
{
	if (mModel == nil)
		mDocument = nil;
	else
		mDocument = mModel->GetDocument();
} // Layout

/*
~Layout
*/
Layout::~Layout()
{
} // ~Layout

/*
AddItem
	Add an item to the model, handling multiples properly. Default is to just add
	to the model.
*/
void
Layout::AddItem(PhotoItemRef inItem)
{
	mModel->AdoptNewItem(inItem);
} // AddItem

/*
AdjustDocumentOrientation
	Set the paper to landscape or portrait orientation to fit the most items
*/
void
Layout::AdjustDocumentOrientation()
{
	UInt32		l = this->CountOrientation(kLandscape);
	UInt32		p = this->CountOrientation(kPortrait);

	EPrintSpec* spec = (EPrintSpec*)mDocument->GetPrintRec();
	// Note that we have a slight bias for landscape (since most pictures are done that way)
	if (p > l) {
		spec->SetOrientation(kPortrait);
	} else {
		spec->SetOrientation(kLandscape);
	}

	mDocument->MatchViewToPrintRec();
} // AdjustDocumentOrientation

/*
CountOrientation
*/
UInt32
Layout::CountOrientation(const OSType inType) const
{
	UInt32		c = 0;

	PhotoIterator	i;
	for (i = mModel->begin(); i != mModel->end(); i++) {
		PhotoItemRef	item = *i;
		if (inType == kLandscape && item->IsLandscape() || inType == kPortrait &&
			item->IsPortrait())
			c++;
	}

	return c;
} // CountOrientation

/*
ItemIsAcceptable
*/
bool
Layout::ItemIsAcceptable(
	DragReference	inDragRef,
	ItemReference	inItemRef,
	FlavorType&		outFlavor)
{
	// Get actual count
	UInt16		count;
	::CountDragItems(inDragRef, &count);
	// We may not want multiple items
	if (!this->CanAddToBackground(count))
		return false;

	FlavorFlags	theFlags;

	Boolean		happy = true;
	do {
		if (::GetFlavorFlags(inDragRef, inItemRef, kDragFlavorTypeHFS, &theFlags) == noErr) {
			outFlavor = kDragFlavorTypeHFS;

			// ??? we really should look at the file type here (i.e. let QuickTime determine if it
			// can be imported), so we can give a proper drag hilite instead of failing later

			// Our layout may not want multiple items -- we consider a folder to be multiple items
			break;
			}//endif hfs flavor
		if (::GetFlavorFlags(inDragRef, inItemRef, kDragFlavorTypePromiseHFS, &theFlags) == noErr) {
			outFlavor = kDragFlavorTypePromiseHFS;
			break;
			}//endif hfs promise
		happy = false;
	} while (false);
			
	return happy;
} // ItemIsAcceptable

/*
SetupOptionsDialog
	Subclasses should override (and call inherited)
*/
void
Layout::SetupOptionsDialog(EDialog& inDialog)
{
	LGAColorSwatchControl*	color = dynamic_cast<LGAColorSwatchControl*>(inDialog.FindPaneByID('bCol'));
	if (color != nil) {
		color->SetSwatchColor(Color_White);
	}
} // SetupOptionsDialog
