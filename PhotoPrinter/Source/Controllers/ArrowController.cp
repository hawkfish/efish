/*
	File:		ArrowController.cp

	Contains:	arrow controller for kilt

	Written by:	dav lion and David Dunham

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		12 Jul 2001		rmgw	Add MakeDragRegion.  Bug #156.
		10 jul 2001		dml		fix DoClickItem again (shift-key)
		10 jul 2001		dml		revert DoClickItem logic, missing semi, call SetPrimarySelection instead
		09 jul 2001		dml		fix DoClickItem logic
		26 Jun 2001		drd		86 2click doesn't show dialog for placeholder
		23 May 2001		drd		DoClickItem makes sure not to deselect, so we can drag a group
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
#include "PhotoPrintCommands.h"

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
	
	// Clicking on an already-selected image can start a drag, so we don't want to call
	// inherited (which clears the selection). Unless the shift key is down, since that
	// will deselect it.
	PhotoItemRef	theImage = inEvent.target.item;
	if ((mView->IsSelected(theImage)) && (!(inEvent.macEvent.modifiers & kShiftKey))) {
		mView->SetPrimarySelection(theImage);
		}
	else {
		PhotoController::DoClickItem(inEvent);		// Call inherited to handle potential toggle
	}//else something new clicked

	// Handle drag & drop (if any)
	if (mView->IsAnythingSelected())
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
ArrowController::HandleClick(const SMouseDownEvent &inMouseDown, const MRect& inBounds,
							SInt16 inClickCount) {
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
			if (inClickCount == 1)
				DoClickItem(clickEvent);
			else {
				PhotoPrintDoc*		doc = mView->GetModel()->GetDocument();
				PhotoItemRef		theImage = clickEvent.target.item;
				if (!theImage->IsEmpty()) {
					doc->ProcessCommand(cmd_ImageOptions, nil);
				}
			}//else it's a multi-click, bring up the image options dialog
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

// ---------------------------------------------------------------------------
//	¥ MakeDragRegion											  [public]
// ---------------------------------------------------------------------------

void
ArrowController::MakeDragRegion (
	
	DragReference	inDragRef, 
	RgnHandle 		inDragRegion) 

{ // begin MakeDragRegion

	// We don't have any idea of the selection, so pass on the message to someone who does
	mView->MakeDragRegion(inDragRef, inDragRegion);
} // MakeDragRegion

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
