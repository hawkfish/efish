/*
	File:		ArrowController.cp

	Contains:	arrow controller for kilt

	Written by:	dav lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		30 Aug 2000		drd		Moved DoClickEmpty (and most of DoClickItem) to PhotoController
		29 Aug 2000		drd		Override AddFlavors, DoDragSendData, RemoveDragItem
		24 Aug 2000		drd		Now CDragAndDrop; removed Select
		21 aug 2000		dml		HandleClick sets mScrollPosition
		11 Aug 2000		drd		Simplified Select
		11 aug 2000		dml		whoops.  click-to-select should select as sole primary if already 2ndary
		07 Aug 2000		dml		Created
*/

#include "ArrowController.h"
#include "PhotoPrintDoc.h"
#include "PhotoPrintView.h"

/*
ArrowController
*/
ArrowController::ArrowController(PhotoPrintView* inView)
	: PhotoController(inView), CDragAndDrop(inView->GetMacWindow(), inView)
{
}//end ct

/*
~ArrowController
*/
ArrowController::~ArrowController()
{
}//end dt

/*
AddFlavors {OVERRIDE}
	Add flavored items to the DragTask.
*/
void
ArrowController::AddFlavors(DragReference inDragRef)
{
	// Let the Drag Manager know that we'll be supporting promises
	this->InstallDragSendData(inDragRef);

	// We don't have any idea of the selection, so pass on the message to someone who does
	mView->AddFlavors(inDragRef);
} // AddFlavors

/*
AdjustCursorSelf {OVERRIDE}
	Show the correct cursor -- in our case, always an arrow
*/
void	
ArrowController::AdjustCursorSelf(const Point& /*inViewPt*/)
{
	::InitCursor();
} // AdjustCursorSelf
	
/*
DoClickBoundingLine
*/
void 
ArrowController::DoClickBoundingLine(ClickEventT& /*inEvent*/)
{
}//end DoClickBoundingLine

/*
DoClickItem {OVERRIDE}
*/
void 
ArrowController::DoClickItem(ClickEventT& inEvent)
{
	PhotoController::DoClickItem(inEvent);		// Call inherited

	// Handle drag & drop (if any)
	this->ClickIsDragEvent(inEvent, nil);
}//end DoClickItem

/*
DoClickHandle
*/
void 
ArrowController::DoClickHandle(ClickEventT& /*inEvent*/) {
	// !!! todo: resize
}//end DoClickHandle

/*
DoDragSendData {OVERRIDE}
	Send the data associated with a particular drag item

	This methods gets called if you installed the optional DragSendDataProc
	for this DragItem.
*/
void
ArrowController::DoDragSendData(
	FlavorType		inFlavor,
	ItemReference	inItemRef,
	DragReference	inDragRef)
{
	// We don't have any idea of the selection, so pass on the message to someone who does
	mView->DoDragSendData(inFlavor, inItemRef, inDragRef);
} // DoDragSendData

/*
HandleClick {OVERRIDE}
	Main dispatch of clicks
*/
void 
ArrowController::HandleClick(const SMouseDownEvent &inMouseDown, const MRect& inBounds) {
	mBounds = inBounds;

	// Build our parameter block -- the first part is just the SMouseDownEvent
	ClickEventT		clickEvent;
	::BlockMoveData(&inMouseDown, &clickEvent, sizeof(SMouseDownEvent));
	// And fill in the rest (analyze what the click represents)
	this->InterpretClick(clickEvent);

	switch (clickEvent.type) {
		case kClickEmpty:
			DoClickEmpty(clickEvent);
			break;

		case kClickInsideItem:
			DoClickItem(clickEvent);
			break;

		case kClickOnHandle:
			DoClickHandle(clickEvent);
			break;

		case kClickBoundingLine:
			DoClickBoundingLine(clickEvent);
			break;

		default:
			break;
	}//end switch
}//end HandleClick

/*
RemoveDragItem {OVERRIDE}
	The drag item originated by inMouseDown was dropped in the trash.
*/
void
ArrowController::RemoveDragItem(const SMouseDownEvent &inMouseDown)
{
#pragma unused(inMouseDown)

	PhotoPrintDoc*		doc = mView->GetModel()->GetDocument();
	doc->ProcessCommand(cmd_Clear, nil);
} // RemoveDragItem
