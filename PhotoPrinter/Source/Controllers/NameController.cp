/*
	File:		NameController.cp

	Contains:	nametag controller for kilt

	Written by:	dav lion 

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		02 Aug 2001		rmgw	Moved badge creation in.
		22 feb 2001		dml		created
	
	
*/
#include "NameController.h"
#include "PhotoPrintDoc.h"
#include "PhotoBadge.h"
/*
NameController
*/
NameController::NameController(

	PhotoPrintView*	inView, 
	LCommander*		inBadgeCommander)
	
	: PhotoController(inView)
{
		// and make the LTabGroup the target
	LCommander::SwitchTarget (GetView ()->CreateBadges(inBadgeCommander));
}//end ct

/*
~NameController
*/
NameController::~NameController()
{
	GetView ()->DestroyBadges ();
}//end dt

/*
AdjustCursorSelf {OVERRIDE}
	Show the correct cursor -- in our case, always an arrow
*/
void	
NameController::AdjustCursorSelf(const Point& /*inViewPt*/)
{
	::InitCursor();
} // AdjustCursorSelf


/*
DoClickItem
*/
void	
NameController::DoClickItem(ClickEventT& inEvent) {
	// choose the badge which is associated with this item
	PhotoBadge* badge (mView->GetBadgeForItem(inEvent.target.item));
	if (badge)
		LCommander::SwitchTarget(badge->GetNameTag());
}//end DoClickItem



/*
HandleClick {OVERRIDE}
	Main dispatch of clicks
*/
void 
NameController::HandleClick(const SMouseDownEvent &inMouseDown, const MRect& inBounds,
							SInt16 /*inClickCount*/) {
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
		case kClickOnHandle:
		case kClickBoundingLine:
			DoClickItem(clickEvent);
			break;

		default:
			break;
	}//end switch
}//end HandleClick
