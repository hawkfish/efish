/*
	File:		PaletteButton.cp

	Contains:	Implementation of a button in the PhotoPrinter floating palette.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		23 jan 2001		dml		fix evil kDragPromiseFindFile bug w/ enhanced ExtractFSSpec call
		27 Sep 2000		rmgw	Change ItemIsAcceptable to DragIsAcceptable.
		20 Sep 2000		drd		Delete our helper layout (for Spotlight cleanliness)
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
#include "SchoolLayout.h"
#include "SingleLayout.h"

#include "MAppleEvent.h"
#include "MDragItemIterator.h"

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
	// Cleanup
	delete mLayout;
	mLayout = nil;
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
DragIsAcceptable {OVERRIDE}
*/
Boolean	
PaletteButton::DragIsAcceptable(DragReference inDragRef)
{
	if (mLayout == nil)
		return false;
	else
		return mLayout->DragIsAcceptable(inDragRef);

} // DragIsAcceptable

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
	// 	Create a "new document" event
	
	//	Queue the event, don't execute it (we are in a drag).
	MAEAddressDesc		realAddress (MFileSpec::sDefaultCreator);	
	MAppleEvent 		aevt(kAECoreSuite, kAECreateElement, realAddress);

		DescType			docType = cDocument;
		aevt.PutParamPtr (typeType, &docType, sizeof (docType), keyAEObjectClass);

		// What kind of template
		docType = this->GetPaneID();
		aevt.PutParamPtr (typeType, &docType, sizeof (docType), keyAERequestedType);

		// Build a list of all files dragged
		MAEList				theList;
		MDragItemIterator	end (inDragRef);
		for (MDragItemIterator item (end); ++item != end; ) {
			FSSpec 	spec;
			Boolean	ioAllowEvilPromise (true);
			PromiseHFSFlavor		promise;
			if (!item.ExtractFSSpec (spec, ioAllowEvilPromise, promise)) continue;
				
			if (ioAllowEvilPromise) continue;	
			theList.PutPtr (typeFSS, &spec, sizeof (spec));
			} // for

		aevt.PutParamDesc (theList, keyAEData);

	// And send it! This will result in a window being opened.
	aevt.Send ();

} // ReceiveDragItem

/*
SuperDeactivate {OVERRIDE}
	We do nothing, because we want to remain active even when our window is deactivated
*/
void
PaletteButton::SuperDeactivate()
{
} // SuperDeactivate
