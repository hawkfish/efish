/*
	File:		Layout.cp

	Contains:	Implementation of Layout object, which manages positioning of images.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		21 Jun 2000		drd		ItemIsAcceptable; allow nil model
		19 Jun 2000		drd		Added mRows, mColumns, mGutter
		19 Jun 2000		drd		Created
*/

#include "Layout.h"

/*
Layout
*/
Layout::Layout(PhotoPrintModel* inModel)
	: mModel(inModel)
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

	Boolean		happy = false;
	if (::GetFlavorFlags(inDragRef, inItemRef, kDragFlavorTypeHFS, &theFlags) == noErr) {
		outFlavor = kDragFlavorTypeHFS;

		// ??? we really should look at the file type here (i.e. let QuickTime determine if it
		// can be imported), so we can give a proper drag hilite instead of failing later

		// Our layout may not want multiple items -- we consider a folder to be multiple items
		// !!!

		happy = true;			
	}//endif

	return happy;
} // ItemIsAcceptable
