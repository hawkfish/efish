/*
	File:		ArrowController.cp

	Contains:	arrow controller for kilt

	Written by:	dav lion

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		11 aug 2000		dml		whoops.  click-to-select should select as sole primary if already 2ndary
		07 Aug 2000		dml		Created
*/

#include "ArrowController.h"
#include "PhotoPrintView.h"

ArrowController::ArrowController(PhotoPrintView* inView)
	: PhotoController(inView)
{
}//end ct


ArrowController::~ArrowController() {
	}//end dt


void	
ArrowController::AdjustCursor(const Point& /*inPortPt*/) {
	::InitCursor();
	}//end AdjustCursor
	
	
void 
ArrowController::DoClickBoundingLine(ClickEventT& /*inEvent*/)  {
	}//end DoClickBoundingLine


void 
ArrowController::DoClickEmpty(ClickEventT& /*inEvent*/) {
	// make nothing selected
	mView->ClearSelection();
	}//end DoClickEmpty

void 
ArrowController::DoClickItem(ClickEventT& inEvent) {

	// turn the single selection into a list
	PhotoItemList selected;
	selected.insert(selected.end(), inEvent.target.item);
	// see if shift key is down
	if (inEvent.modifierKeys & kShiftKey) {
		mView->ToggleSelected(selected);
		}//endif shift down so toggle state
	else {	
		// else, replace selection with us
		mView->ClearSelection();
		mView->AddToSelection(selected);
		}//else normal select
}//end DoClickItem


void 
ArrowController::DoClickHandle(ClickEventT& /*inEvent*/) {
	}//end DoClickHandle



void 
ArrowController::HandleClick(const SMouseDownEvent &inMouseDown, const MRect& inBounds) {
	mBounds = inBounds;
	ClickEventT clickEvent;
	clickEvent.where = inMouseDown.whereLocal;
	InterpretClick(clickEvent);
	
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




void	
ArrowController::Select(PhotoItemList newSelection, bool inRefresh) {
	if (inRefresh)
		HighlightSelection(newSelection);
}//end Select





