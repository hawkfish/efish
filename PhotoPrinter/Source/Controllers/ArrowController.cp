/*
	File:		ArrowController.cp

	Contains:	arrow controller for kilt

	Written by:	dav lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		24 Aug 2000		drd		Now CDragAndDrop; removed Select
		21 aug 2000		dml		HandleClick sets mScrollPosition
		11 Aug 2000		drd		Simplified Select
		11 aug 2000		dml		whoops.  click-to-select should select as sole primary if already 2ndary
		07 Aug 2000		dml		Created
*/

#include "ArrowController.h"
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
AdjustCursor {OVERRIDE}
*/
void	
ArrowController::AdjustCursorSelf(const Point& /*inViewPt*/)
{
	::InitCursor();
}//end AdjustCursor
	
/*
DoClickBoundingLine
*/
void 
ArrowController::DoClickBoundingLine(ClickEventT& /*inEvent*/)
{
}//end DoClickBoundingLine

/*
DoClickEmpty
*/
void 
ArrowController::DoClickEmpty(ClickEventT& /*inEvent*/) {
	// make nothing selected
	mView->ClearSelection();
}//end DoClickEmpty

/*
DoClickItem
*/
void 
ArrowController::DoClickItem(ClickEventT& inEvent)
{
	// turn the single selection into a list
	PhotoItemList selected;
	selected.push_back(inEvent.target.item);

	// see if shift key is down
	if (inEvent.macEvent.modifiers & kShiftKey) {
		mView->ToggleSelected(selected);
		}//endif shift down so toggle state
	else {	
		// else, replace selection with us
		mView->ClearSelection();
		mView->AddToSelection(selected);
	}//else normal select

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
