/*
	File:		PaletteButton.cp

	Contains:	Implementation of a button in the PhotoPrinter floating palette.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		23 Aug 2000		drd		ReceiveDragItem passes data as keyAEData
		27 jun 2000		dml		send deferred aevt in ReceiveDragItem,
								  add support for hfsPromise in DoDragReceive
		26 Jun 2000		drd		Be sure to initialize dataSize before calling GetFlavorData
		23 Jun 2000		drd		Layout needs HORef<PhotoPrintModel>
		21 Jun 2000		drd		Override EnterDropArea, LeaveDropArea, SuperDeactivate; use
								DoDragReceive instead of ReceiveDragItem
		21 Jun 2000		drd		Created
*/

#include "PaletteButton.h"

#include "CollageLayout.h"
#include "GridLayout.h"
#include "MAppleEvent.h"
#include "SchoolLayout.h"
#include "SingleLayout.h"

/*
PaletteButton
*/
PaletteButton::PaletteButton(LStream*	inStream)
	: LCmdBevelButton(inStream)
	, CDragAndDrop(this->GetMacWindow(), this)
{
	HORef<PhotoPrintModel> 	noModel;

	switch (this->GetPaneID()) {
		case 'grid':
			mLayout = new GridLayout(noModel);
			break;

		case 'sing':
			mLayout = new SingleLayout(noModel);
			break;

		case '2fix':
			mLayout = new FixedLayout(noModel);
			break;

		case '2dup':
			mLayout = new MultipleLayout(noModel);
			break;

		case 'mult':
			mLayout = new SchoolLayout(noModel);
			break;

		case 'coll':
			mLayout = new CollageLayout(noModel);
			break;

		case 'prin':
			mLayout = new GridLayout(noModel);
			break;

		default:
			mLayout = nil;
			break;
	}
} // PaletteButton

/*
~PaletteButton
*/
PaletteButton::~PaletteButton()
{
} // ~PaletteButton

/*
EnterDropArea {OVERRIDE}
*/
void
PaletteButton::EnterDropArea(
	DragReference		inDragRef,
	Boolean				inDragHasLeftSender)
{
#pragma unused(inDragRef, inDragHasLeftSender)

	this->FocusDropArea();
	this->HotSpotAction(kControlButtonPart, true, false);	// Do action for click inside
} // EnterDropArea

/*
ItemIsAcceptable {OVERRIDE}
*/
Boolean	
PaletteButton::ItemIsAcceptable(DragReference inDragRef, ItemReference inItemRef)
{
	if (mLayout == nil)
		return false;
	else
		return mLayout->ItemIsAcceptable(inDragRef, inItemRef, mFlavorAccepted);
} // ItemIsAcceptable

/*
LeaveDropArea {OVERRIDE}
*/
void
PaletteButton::LeaveDropArea(DragReference inDragRef)
{
#pragma unused(inDragRef)

	this->FocusDropArea();
	this->HotSpotAction(kControlButtonPart, false, true);	// Undo visual effect
} // LeaveDropArea

/*
DoDragReceive {OVERRIDE}
*/
void
PaletteButton::DoDragReceive(
	DragReference	inDragRef)
{
	// Create a "new document" event
	MAEAddressDesc		realAddress (MFileSpec::sDefaultCreator);
	MAppleEvent 		aevt(kAECoreSuite, kAECreateElement, realAddress);
	DescType			docType = cDocument;
	aevt.PutParamPtr(typeType, &docType, sizeof(DescType), keyAEObjectClass);

	// What kind of template
	docType = this->GetPaneID();
	aevt.PutParamPtr(typeType, &docType, sizeof(DescType), keyAERequestedType);

	// Build a list of all files dragged
	UInt16	itemCount;				// Number of Items in Drag
	::CountDragItems(inDragRef, &itemCount);

	MAEList				theList;
	for (UInt16 item = 1; item <= itemCount; item++) {
		ItemReference	itemRef;
		::GetDragItemReferenceNumber(inDragRef, item, &itemRef);

		Size			dataSize;
		
		HFSFlavor		data;
		dataSize = sizeof(data);
		if (::GetFlavorData(inDragRef, itemRef, kDragFlavorTypeHFS, &data, &dataSize, 0) == noErr) {
			theList.PutPtr(typeFSS, &data.fileSpec, sizeof(data.fileSpec));
			continue;
		}//endif found an hfs flavor
	
		PromiseHFSFlavor	promise;
		dataSize = sizeof(promise);
		if (::GetFlavorData (inDragRef, itemRef, flavorTypePromiseHFS, &promise, &dataSize, 0) == noErr) {
			
			FSSpec spec;
			dataSize = sizeof (spec);
			if ((::GetFlavorData (inDragRef, itemRef, promise.promisedFlavor, &spec, &dataSize, 0) == noErr) &&
				 (dataSize > 0)) {
				theList.PutPtr(typeFSS, &spec, sizeof(spec));
				continue;
				}//endif hfs was found inside promise
			}//endif promise was found

		}//for

	aevt.PutParamDesc(theList, keyAEData);

	// And send it! This will result in a window being opened.
	aevt.Send();
} // ReceiveDragItem

/*
SuperDeactivate {OVERRIDE}
	We do nothing, because we want to remain active even when our window is deactivated
*/
void
PaletteButton::SuperDeactivate()
{
} // SuperDeactivate
