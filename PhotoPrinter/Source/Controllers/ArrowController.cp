/*
	File:		ArrowController.cp

	Contains:	arrow controller for kilt

	Written by:	dav lion and David Dunham

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		02 Aug 2001		rmgw	Factor in dragging.
		23 Jul 2001		rmgw	Get document from view.
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

#include "PhotoPrintConstants.h"
#include "PhotoPrintDoc.h"
#include "PhotoPrintView.h"
#include "PhotoPrintCommands.h"

//	Toolbox++
#include "MNewHandle.h"
#include "MNewRegion.h"

LGWorld*		
ArrowController::gOffscreen = nil;

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
	if (GetView ()->IsAnythingSelected()) {
		// Promise our flavor
		::AddDragItemFlavor(inDragRef, 1, kDragFlavor, nil, 0L, flavorSenderOnly);
		
		// And promise PICT
		::AddDragItemFlavor(inDragRef, 1, kScrapFlavorTypePicture, nil, 0L, 0);

	}
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
	MNewHandle	theData(GetView ()->GetSelectedData(inFlavor));	// Get rid of handle on exit
	if (theData != nil) {
		StHandleLocker	lock(theData);
		Size	s = theData.GetSize();
		ThrowIfOSErr_(::SetDragItemFlavorData(inDragRef, inItemRef, inFlavor,
			*theData, s, 0L));
	}
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
				PhotoPrintDoc*		doc = mView->GetDocument();
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
	
	//	Create the drag region
	MRegion					theDragRgn (inDragRegion);
	
	HORef<MNewRegion>		imageRgn (new MNewRegion);
	GetView ()->FocusDraw ();
	
	MatrixRecord		mat;
	GetView ()->GetBodyToScreenMatrix(mat);
	
	PhotoItemList		selection (GetView ()->Selection ());
	for (PhotoIterator i (selection.begin ()); i != selection.end (); ++i) {
		MRect	destRect ((*i)->GetImageMaxBounds());	// 225 Be sure it's rotated
		::TransformRect (&mat, &destRect, nil);
		::LocalToGlobal (&destRect.TopLeft ());
		::LocalToGlobal (&destRect.BotRight ());
		
		MNewRegion	outerRgn;				// Make region containing item
		outerRgn = destRect;
		imageRgn->Union (*imageRgn, outerRgn);
		
		MNewRegion	innerRgn;				// Carve out interior of region so
		innerRgn = outerRgn;
		innerRgn.Inset (1, 1);				//   that it's just a one-pixel thick
		outerRgn.Difference (outerRgn, innerRgn);//   outline of the item rectangle
		
		theDragRgn.Union (theDragRgn, outerRgn);
		} // for

	if (selection.size() == 1) {
		// Add translucent drag
		SInt32		response;
		::Gestalt (gestaltDragMgrAttr, &response);
		if (response & (1 << gestaltDragMgrHasImageSupport)) {
			try {
				PhotoItemRef	image(GetView ()->GetPrimarySelection());
				MRect		bounds(image->GetImageMaxBounds());		// 225 Be sure it's rotated
				::TransformRect (&mat, &bounds, nil);

				Point		globalPt, localPt;
				globalPt = localPt = bounds.TopLeft();
				::LocalToGlobal(&globalPt);
				::SubPt(localPt, &globalPt);

				PhotoDrawingProperties	basicProps (false, false, false, GetView ()->GetDocument()->GetResolution());
				delete gOffscreen;					// Kill previous
				gOffscreen = new LGWorld(bounds, 0, useTempMem);
				gOffscreen->BeginDrawing();
				image->Draw(basicProps, &mat, UQDGlobals::GetCurrentPort(), ::GetGDevice());
				gOffscreen->EndDrawing();
				PixMapHandle	imagePixMap = ::GetGWorldPixMap(gOffscreen->GetMacGWorld());

				::SetDragImage(inDragRef, imagePixMap, nil, globalPt, kDragStandardTranslucency);
			} catch (...) {
				// Translucency is not that important, so we ignore exceptions
				// But we do need to make sure we aren't drawing offscreen
				if (gOffscreen) {
					gOffscreen->EndDrawing();
				}
			}
		}
	}
} // end MakeDragRegion

/*
RemoveDragItem {OVERRIDE}
	The drag item originated by inMouseDown was dropped in the trash.
*/
void
ArrowController::RemoveDragItem(const SMouseDownEvent &inMouseDown)
{
#pragma unused(inMouseDown)

	PhotoPrintDoc*		doc = mView->GetDocument();
	doc->ProcessCommand(cmd_Clear, nil);
} // RemoveDragItem
