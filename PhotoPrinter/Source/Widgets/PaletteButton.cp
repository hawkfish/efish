/*
	File:		PaletteButton.cp

	Contains:	Implementation of a button in the PhotoPrinter floating palette.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

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
	switch (this->GetPaneID()) {
		case 'grid':
			mLayout = new GridLayout(nil);
			break;

		case 'sing':
			mLayout = new SingleLayout(nil);
			break;

		case '2fix':
			mLayout = new FixedLayout(nil);
			break;

		case '2dup':
			mLayout = new MultipleLayout(nil);
			break;

		case 'mult':
			mLayout = new SchoolLayout(nil);
			break;

		case 'coll':
			mLayout = new CollageLayout(nil);
			break;

		case 'prin':
			mLayout = new GridLayout(nil);
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
ReceiveDragItem {OVERRIDE}
*/
void
PaletteButton::ReceiveDragItem(
	DragReference	inDragRef, 
	ItemReference	inItemRef,
	Size			/*inDataSize*/, 
	Boolean			/*inCopyData*/, 
	Boolean			/*inFromFinder*/, 
	Rect&			/*inItemBounds*/)
{
	// !!! Probably the real thing to do is send an Apple Event with all the files in itŠ

	MAppleEvent 		aevt(kAECoreSuite, kAECreateElement);
	DescType			docType = cDocument;
	aevt.PutParamPtr(typeType, &docType, sizeof(DescType), keyAEObjectClass);
	docType = this->GetPaneID();
	aevt.PutParamPtr(typeType, &docType, sizeof(DescType), 'type');

	// And send it! This will result in a window being opened.
	UAppleEventsMgr::SendAppleEvent(aevt);
} // ReceiveDragItem
