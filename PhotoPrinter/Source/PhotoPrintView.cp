/*
	File:		PhotoPrintView.cp

	Contains:	MVC view, which displays stuff

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		26 Jul 2001		rmgw	Add AsPascalString(s). Grr.
		26 Jul 2001		rmgw	Factor out XML parsing.  Bug #228.
		26 Jul 2001		drd		235 Initialize mControllerType
		25 Jul 2001		drd		Added asserts before sending messages to mController
		25 Jul 2001		rmgw	Disable desktop in WarnAboutRename.  Bug #230.
		25 Jul 2001		drd		225 Base drag region & translucent image on GetImageMaxBounds
		25 Jul 2001		rmgw	Fix drag region/image offsets.  Bug #221.
		25 Jul 2001		rmgw	Do nothing if changing to the same tool.  Bug #227.
		25 Jul 2001		rmgw	Add OnFilenameChanged.  Bug #219.
		24 Jul 2001		rmgw	Remove unused alert.
		24 Jul 2001		rmgw	Badges need to know about the document. Bug #202.
		24 Jul 2001		rmgw	Respond to OnModelItemsChanged. Bug #220.
		24 jul 2001		dml		217.  AdjustTransforms no longer clamps to window (since fitting inside cell anyway)
		24 Jul 2001		rmgw	Remove bogus OnModelDirtied.
		23 jul 2001		dml		190 SetController cancel rename works
		23 Jul 2001		rmgw	Listen to new model messages.
		23 Jul 2001		drd		199 SwitchLayout sends RemoveEmptyItems
		21 jul 2001		dml		some cosmetic artifacts of badges w/ drag + drop removed
		20 jul 2001		dml		204.  break ListenToMessage into ListenToCommand, ListenToMessage
								fix crash bugs w/ badges.  fix placement problems w/ badges.  cosmetic + efficiency
								issues remain
		20 Jul 2001		rmgw	Fix turned off layout popup.
		20 jul 2001		dml		190.  add warn about Rename tool's power
		20 Jul 2001		rmgw	Undo for layout changes.  Bug #200.
		19 jul 2001		dml		19, 160 SetupDraggedItem doesn't SetDest
		19 Jul 2001		drd		195 SwitchLayout uses GetFirstNonEmptyItem
		18 Jul 2001		rmgw	Fix moves in fixed layouts.  Bug #110.2.
		18 Jul 2001		rmgw	Fix forward dragging in fixed layouts.  Bug #183.
		18 Jul 2001		drd		196 Get rid of DeclareActiveBadge and just do it in CreateBadges
		18 Jul 2001		rmgw	Undo drag support.  Bug #110.
		18 Jul 2001		drd		187 Override AdaptToSuperScroll, and handle page update there instead of DrawSelf
		18 Jul 2001		drd		153 185 186 Added init arg to SetLayoutType
		17 Jul 2001		drd		Turn School-5 into School-10
		17 Jul 2001		rmgw	Add async exception reporting to Draw.
		16 Jul 2001		rmgw	Report errors from drag.  Bug #162.
		16 Jul 2001		rmgw	Handle sorted move.
		16 Jul 2001		drd		172 GetSelectedData('PICT') ignores placeholders
		13 Jul 2001		rmgw	Get event replies; clear selection before drop.
		13 Jul 2001		drd		75 RefreshItem(kImageAndHandles) invalidates bigger rect
		12 Jul 2001		rmgw	Add MakeDragRegion.  Bug #156.
		12 Jul 2001		rmgw	Adjust drop location for forward moves   Bug #155.
		12 Jul 2001		rmgw	Convert drags to 'move <item> to <drop position>'.  Bug #110.
		12 Jul 2001		rmgw	Convert copy drags to 'clone <item> at <drop position>'.
		12 Jul 2001		rmgw	Convert the import event to make new import.
		11 jul 2001		dml		101 add FocusDraw around Refresh, RefreshItem
		11 jul 2001		dml		add defense around AddToSelection (only add iff not there)
		11 Jul 2001		rmgw	Fix ReceiveDragEvent typo.
		11 Jul 2001		drd		143 School Layout disables Landscape popup menu item
		11 Jul 2001		rmgw	Drag and Drop uses only AE now.
		11 Jul 2001		rmgw	Move MakeNewAEXXXItem to PhotoItemModelObject.
		10 Jul 2001		rmgw	Dragged files now just send make new events with no import.
		10 Jul 2001		drd		143 School Layout disables Singles popup menu item
		10 Jul 2001		drd		91 Override ActivateSelf
		10 Jul 2001		drd		Fixed leak in SwitchLayout
		10 jul 2001		dml		add SetPrimarySelection
		09 JUL 2001		dml		135.  Badges must be created w/ bodyToScreen correction
		09 Jul 2001		rmgw	ReceivedDraggedFile now sends AppleEvent.
		06 Jul 2001		drd		124 Moved send of ClearSelection to later in SwitchLayout
		06 Jul 2001		drd		132 ReceiveDraggedFolder checks IsVisible
		06 jul 2001		dml		124	ClearSelection before switching layout type. 
		05 jul 2001		dml		added saner clipping to DrawSelf's item loop (should stop overdrawing scrollbars) ref: 68
		05 jul 2001		dml		25 again.  add optionalOutDestNoCaption parm to AdjustTransforms
		03 jul 2001		dml		104, 25. captions don't rotate with item.  BROKEN (see comment in AdjustTranforms)
		02 Jul 2001		drd		Turned assert in RemoveFromSelection into if
		02 Jul 2001		rmgw	Convert item list to vector representation.
		29 Jun 2001		drd		96 Override InsideDropArea so we can show CopyArrowCursor
		29 Jun 2001		drd		96 ReceiveDragItem checks inCopyData
		29 Jun 2001		rmgw	Set resolution in AddDragFlavors.  Bug #92.
		28 Jun 2001		drd		92 Don’t get drag image from proxy, draw into gOffscreen
		27 Jun 2001		drd		93 ReceiveDraggedFile calls GetErrorAndDescription
		26 Jun 2001		drd		90 DestroyBadges uses clear instead of piecemeal erase;
								90 SetController checks for a change; 93 alert in ReceiveDraggedFile
		26 Jun 2001		drd		Call UCursor::SetArrow() before displaying alert, use StopAlert
		14 Jun 2001		rmgw	CreateBadges now takes commander argument.  Bug #66.
		14 Jun 2001		drd		Removed unused sortedList var from ReceiveDragEvent and ReceiveDraggedFolder
		14 Jun 2001		drd		73 No more gCurTool (so don't need Activate)
		14 Jun 2001		rmgw	Assert on invalid controller; fix memory leak in import.  Bug #71.
		23 May 2001		drd		Use StThemeDrawingState::Normalize only with new PowerPlant
		23 May 2001		drd		Fixed XML parsing so we can handle a list of images;
								IsSelected
		22 May 2001		drd		SwitchLayout hides/shows the duplicated popup
		21 May 2001		drd		Fixed GetSelectedData for multiple PICTs
		16 May 2001		drd		67 Override ApplyForeAndBackColors (fixes drop hilite OS 9)
		27 Apr 2001		drd		67 DoDragReceive now posts an Apple Event
		25 Apr 2001		drd		CreateBadges doesn't make badges for empty items
		25 Apr 2001		drd		Put workaround in DrawSelf to deal with PowerPlant 2.1.1a5
		26 Mar 2001		drd		Hide, Show Min/Max in SwitchLayout
		23 Mar 2001		drd		ListenToMessage does more (using SwitchLayout)
		23 mar 2001		dml		fix GetBodyToScreenMatrix to use header top for offset
		23 mar 2001		dml		add DrawPrintable
		21 Mar 2001		drd		ListenToMessage handles some of the new UI
		14 mar 2001		dml		removed cropping pass through to model from DrawSelf, fix visual glitch when scrolled
		14 Mar 2001		drd		DeclareActiveBadge makes sure we have items
		13 Mar 2001		drd		SetLayoutType checks for existence of placard; DrawSelf forces white
		08 mar 2001		dml		bug 34, bug 58.  use GetBodyToScreenMatrix when drawing body
		15 Feb 2001		rmgw	10 Add RemoveFromSelection that takes iterators
		23 jan 2001		dml		fix evil kDragPromiseFindFile bug w/ enhanced ExtractFSSpec call
		17 jan 2001		dml		DrawSelf sets the ScreenResolution of the DrawingProperties (bug 29)
		5 jan 2001		dml		fix ReceiveDragItem to parse the HandleStream
		4 jan 2001		dml		remove timeLimited drive Annoyingware in SetLayoutType for MWSF
		11 Dec 2000		drd		13 DoDragReceive handles kDragFlavor, started ReceiveDragItem
		7 dec 2000		dml		header + footer on every page!!
		5 dec 2000		dml		draw header and footer, set annoyingware in SetLayoutType
		27 Sep 2000		rmgw	Change ItemIsAcceptable to DragIsAcceptable.
		22 Sep 2000		drd		RefreshItem uses DrawXformedRect instead of RefreshRect
		19 sep 2000		dml		pass click count through to controller
		18 sep 2000		dml		add mCurPage, set in DrawSelf (to handle scrolling)
		07 Sep 2000		drd		Use GetName for name of layout
		07 sep 2000		dml		don't add to selection, have the layout do it since it owns item
		31 aug 2000		dml		add CheckEventQueueForUserCancel (seems inactive)
		30 Aug 2000		drd		GetSelectedData handles kDragFlavor
		30 Aug 2000		drd		Select dragged-in images; new version of AddToSelection
		29 Aug 2000		drd		GetSelectedData (to handle PICT drag)
		29 Aug 2000		drd		AddFlavors, DoDragSendData
		23 Aug 2000		drd		Data of ReceiveDragEvent is in keyAEData
		21 Aug 2000		drd		Added arg to RefreshItem so we can update selection handles
		17 aug 2000		dml		construct with current controller, not necess arrow, set tool on Activate event
		15 aug 2000		dml		use RotateController
		15 Aug 2000		drd		Use CropZoomController
		15 aug 2000		dml		don't call RefreshItem in ReceiveDraggedFile (let layout invoke)
		14 aug 2000		dml		don't sort if layout only has a singel distinct image (e.g MultipleLayouts)
		11 Aug 2000		drd		Use CropController
		11 Aug 2000		drd		Include PhotoPrintCommands.h since that's where tool_ is now
		11 aug 2000		dml		add SetController
		09 aug 2000		dml		make ExtractFSSpecFromDragRef static public
		07 aug 2000		dml		change to use ArrowController
		07 aug 2000		dml		add ClearSelection
		07 Aug 2000		drd		Added a Refresh before LayoutImages to handle changing orientation
		07 aug 2000		dml		change RemoveFromSelection to work backwards, so can delete entire self list
		04 aug 2000		dml		add AddToSelection, RemoveFromSelection
		04 Aug 2000		drd		Renamed GetSelection to Selection (already taken by LPane)
		03 aug 2000		dml		add selection (move from model)
		03 aug 2000		dml		move sorting to model
		02 aug 200		dml		added sort_nothing case to SortFileList
		28 jul 2000		dml		move call to Layout out of ProcessSortedFileList (since called recursively)
		28 Jul 2000		drd		Small optimization/cleanup in DoDragReceive, ProcessSortedFileList
		28 jul 2000		dml		added Assert in SortFileList for bad sorting code
		26 jul 2000		dml		more sorting madness (override DoDragReceive fully)
		26 jul 2000		dml		sort incoming fsspecs
		24 Jul 2000		drd		Use local rect when erasing in DrawSelf; call SetUpdateCommandStatus
								after drag
		20 Jul 2000		drd		AdjustCursorSelf (passes on to controller)
		19 Jul 2000		drd		Removed StCursor from ReceiveDragEvent
		18 jul 2000		dml		add spin cursor to HandleDragEvent
		17 Jul 2000		drd		DrawSelf draws page divider
		13 Jul 2000		drd		Watch cursor in drag-receiving (DoDragReceive, ReceiveDragEvent)
		29 jun 2000		dml		sending Clip again from DrawSelf
		29 Jun 2000		drd		Delete mLayout in destructor
		29 Jun 2000		drd		SetLayoutType sends Initialize
		28 jun 2000		dml		use Layout::LayoutType enums for view creation
		27 jun 2000		dml		add hfsPromise drag receiving
		26 Jun 2000		drd		Override DoDragReceive; minor optimize in ReceiveDragEvent
		26 jun 2000 	dml		fix uninit in ReceiveDragItem, add error-checking
		26 Jun 2000		drd		Set up placard; let layout add dragged items
		23 Jun 2000		drd		ReceiveDragEvent arg is now MAppleEvent; SetLayoutType instead
								of MakeLayout (and handle all layouts)
		21 Jun 2000		drd		Added MakeLayout, ReceiveDragEvent
		21 Jun 2000		drd		ItemIsAcceptable lets the layout do the work
		21 Jun 2000		drd		ReceiveDragItem sends ResolveAlias
		32 jun 2000		dml		don't SetCrop on initial creation (Item now uncropped by default)
		20 jun 2000		dml		using EUtil.  BestFit changes
		20 Jun 2000		drd		Force redraw after drop
		19 Jun 2000		drd		Now have a Layout object
		15 Jun 2000		drd		Erase in Draw
		15 Jun 2000		drd		Call new RefreshItem in ReceiveDraggedFile
		14 Jun 2000		dml		SetupDraggedItem changed to match new ItemProperties
*/

#include "PhotoPrintView.h"

#include "AlignmentGizmo.h"
#include "ArrowController.h"
#include "BadgeGroup.h"
#include "CollageLayout.h"
#include "CropZoomController.h"
#include "FileNotifier.h"
#include "GridLayout.h"
#include "ModelAction.h"
#include "PhotoBadge.h"
#include "PhotoExceptionHandler.h"
#include "PhotoItemModelObject.h"
#include "PhotoPrintApp.h"
#include "PhotoPrintCommands.h"
#include "PhotoPrintConstants.h"
#include "PhotoPrintDoc.h"
#include "PhotoPrinter.h"
#include "PhotoPrintEvents.h"
#include "PhotoPrintModel.h"
#include "PhotoPrintPrefs.h"
#include "PhotoPrintResources.h"
#include "PhotoUtility.h"
#include "Registration.h"
#include "RotateController.h"
#include "SchoolLayout.h"
#include "SingleLayout.h"
#include "XMLDocFormatter.h"
#include "XMLHandleStream.h"
#include "NameController.h" 

//	Toolbox++
#include "HARef.h"
#include "MAEDescExtractors.h"
#include "MAEDescIterator.h"
#include "MAEList.h"
#include "MAppleEvent.h"
#include "MDialog.h"
#include "MDialogItem.h"
#include "MDragItemIterator.h"
#include "MFileSpec.h"
#include "MFolderIterator.h"
#include "MKeyMap.h"
#include "MNewDialog.h"
#include "MNewHandle.h"
#include "MNewRegion.h"
#include "MOpenPicture.h"
#include "UState.h"

//	Epp
#include "ESpinCursor.h"
#include "EPostAction.h"
#include "EUserMessage.h"
#include "EUserMessageServer.h"
#include "EUtil.h"
#include "StDisableBroadcaster.h"

#include <UDebugging.h>

const double kRad2Degrees = 57.2958;
const PaneIDT pane_Debug1 = 'dbg1';
const PaneIDT pane_Debug2 = 'dbg2';
const ResIDT	PPob_Badge = 3000;
const ResIDT	dlog_WarnAboutRename = 2020;
const	ResIDT	strn_ViewStrings = 1132;
const PaneIDT	pane_ToolRadioGroup = 'tool';

enum {
	si_DrawOperation = 1,
	si_DragOperation,
	si_SortedMoveMessage,
	si_ImportProblems,
	
	si_Fnord
	};

struct LayoutMenuInfo {
	Layout::LayoutType	type;
	UInt32				count;
	};
	
	
static LayoutMenuInfo	gLayoutInfo[] = {
	{Layout::kGrid, 0},
	{Layout::kFnordLayout, 0},
	{Layout::kFixed, 1},
	{Layout::kFixed, 2},
	{Layout::kFixed, 4},
	{Layout::kFixed, 6},
	{Layout::kFixed, 9},
	{Layout::kFixed, 16},
	{Layout::kFixed, 25},
	{Layout::kFixed, 36},
	{Layout::kFnordLayout, 0},
	{Layout::kSchool, 3},
	{Layout::kSchool, 10},
	{Layout::kSchool, 13},
	{Layout::kFnordLayout, 0},
	{Layout::kFnordLayout, 0}
};

LGWorld*		PhotoPrintView::gOffscreen = nil;

//-----------------------------------------------
// PhotoPrintView default constructor
//-----------------------------------------------
PhotoPrintView::PhotoPrintView()
	: LView ()
	, CDragAndDrop ( GetMacWindow(), this)
	, mLayout(nil)
	, mCurPage (1)
	, mDoc (0)
	, mControllerType(0)
{
	FileNotifier::Listen (this);
}

//-----------------------------------------------
// PhotoPrintView copy constructor
//-----------------------------------------------
PhotoPrintView::PhotoPrintView(	const PhotoPrintView		&inOriginal)
	: LView(inOriginal)
	, CDragAndDrop (GetMacWindow(), this)
	, mLayout(nil)
	, mCurPage (1)
	, mDoc (0)
	, mControllerType(0)
{
	FileNotifier::Listen (this);
}

//-----------------------------------------------
// PhotoPrintView SPaneInfo constructor
//-----------------------------------------------
PhotoPrintView::PhotoPrintView(	const SPaneInfo		&inPaneInfo,
								const SViewInfo		&inViewInfo)
	: LView(inPaneInfo,
			inViewInfo)
	, CDragAndDrop (GetMacWindow(), this) // ?? use UQDGlobals::GetCurrentWindowPort () instead??
	, mCurPage (1)
	, mDoc (0)
	, mControllerType(0)
{
	FileNotifier::Listen (this);
}

//-----------------------------------------------
// PhotoPrintView Stream constructor
//-----------------------------------------------
PhotoPrintView::PhotoPrintView(	LStream			*inStream)
	: LView (inStream)
	, CDragAndDrop (GetMacWindow(), this)
	, mLayout(nil)
	, mModel (new PhotoPrintModel)
	, mCurPage (1)
	, mDoc (PhotoPrintDoc::gCurDocument)
	, mControllerType(0)
{
	// register ourselves as a listener to the model
	Assert_(GetDocument());
	mModel->AddListener (this);
	mModel->AddListener (GetDocument());
	GetDocument()->AddListener(this);

	this->SetController(tool_Arrow, LCommander::GetDefaultCommander ());

	FileNotifier::Listen (this);
}

//-----------------------------------------------
// ~PhotoPrintView 
//-----------------------------------------------
PhotoPrintView::~PhotoPrintView()
{
	delete mLayout;
}// ~PhotoPrintView

//-----------------------------------------------
// FinishCreateSelf.  Find all those handy panes which ought to be setup by now
//-----------------------------------------------
void
PhotoPrintView::FinishCreateSelf()
{
} // FinishCreateSelf

#pragma mark -

/*
ActivateSelf {OVERRIDE}
*/
void
PhotoPrintView::ActivateSelf()
{
	// 91 Keep track of the current document
	PhotoPrintApp::gSingleton->SetDefaultSubModel(this->GetDocument());
}

/*
AdaptToSuperScroll {OVERRIDE}
	Adjust state of Pane when its SuperView scrolls by the specified amounts.
	We override in order to keep the page number current.
*/
void
PhotoPrintView::AdaptToSuperScroll(
	SInt32	inHorizScroll,
	SInt32	inVertScroll)
{
	LView::AdaptToSuperScroll(inHorizScroll, inVertScroll);

	// If we've only scrolled horizontally, page number won't change
	if (inVertScroll != 0) {
		// you'd think this should be scrollPos, but it's actually imagePos.  and it's negative. 
		SPoint32		imagePos;
		this->GetImageLocation(imagePos);

		// use imagePos.h to determine mCurPage
		mCurPage = -imagePos.v / GetDocument()->GetPageHeight();
		++mCurPage;			//(pages start at 1, not 0)

		GetDocument()->UpdatePageNumber(GetDocument()->GetPageCount());
	}
} // 

/*
AddFlavors {OVERRIDE}
	Add flavored items to the DragTask.
	It's a little strange to have this here (not the model), but the view is the only one which knows
	which images are selected.
*/
void
PhotoPrintView::AddFlavors(DragReference inDragRef)
{
	if (this->IsAnythingSelected()) {
		// Promise our flavor
		::AddDragItemFlavor(inDragRef, 1, kDragFlavor, nil, 0L, flavorSenderOnly);
		
		// And promise PICT
		::AddDragItemFlavor(inDragRef, 1, kScrapFlavorTypePicture, nil, 0L, 0);

	}
} // AddFlavors

//--------------------------------------
// AddToSelection
//	This version takes one item
//--------------------------------------
void
PhotoPrintView::AddToSelection(PhotoItemRef inAddition)
{
	if (std::find(mSelection.begin(), mSelection.end(), inAddition) == mSelection.end()) {
		mSelection.push_back(inAddition);

		if (mSelection.size() == 1)
			SetPrimarySelection(inAddition);

		this->RefreshItem(inAddition, kImageAndHandles);
		}//endif not already there
}//end AddToSelection


//--------------------------------------
// AddToSelection
//	This version takes a list
//--------------------------------------
void
PhotoPrintView::AddToSelection(PhotoItemList& additions)
{
	for (PhotoIterator i = additions.begin(); i != additions.end(); ++i) {
		if (std::find(mSelection.begin(), mSelection.end(), *i) == mSelection.end()) {
			mSelection.push_back(*i);

			if (mSelection.size() == 1)
				SetPrimarySelection(*i);

			this->RefreshItem(*i, kImageAndHandles);
			}//endif it's not already there
	}//end for all
}//end AddToSelection

// ---------------------------------------------------------------------------
//	• AdjustCursorSelf
// ---------------------------------------------------------------------------
//	Set the cursor shape when the cursor is inside a Pane
//
//	The input point is in Port coordinates. Use the inMacEvent->modifiers
//	if the cursor shape depends on whether modifier keys (such as Option)
//	are down.

void
PhotoPrintView::AdjustCursorSelf(
	Point				inPortPt,
	const EventRecord&	/* inMacEvent */)
{
	Assert_(mController != nil);
	mController->AdjustCursor(inPortPt);
} // AdjustCursorSelf

//-----------------------------------------------
// AdjustTransforms.  
// called by controller before installing changes in item
// to ensure that any constraints on transforms (snap-to-grid, rotational increment)
// are enforced.  May change incoming values as needed.
// returns if any values were changed.
//-----------------------------------------------

bool		
PhotoPrintView::AdjustTransforms(double& rot, double& /*skew*/, MRect& dest, const PhotoItemRef item,
									MRect* optionalOutDestNoCaption)
{
	bool changesMade (false);

	//check to see if there is rotation, if it is different, and if changing rotation is allowed
	if (!PhotoUtility::DoubleEqual(rot, item->GetRotation())) {
		if (!item->GetProperties().GetRotate()) {
			rot = item->GetRotation();
			changesMade = true;
			}//endif item not allowed to rotate (any more)
		}//endif rotation different
					
// clamp to the window ...
// taken out 25 jul 2001 slithy
// since RotateController is currently only client of this function, it doesn't need to clamp
// because lower block does a FitTransformedRectInside (cell) 
					

	// see if the item has a max bounds, and if the new set of transforms would extend past 
	if ((!PhotoUtility::DoubleEqual(rot, 0.0)) &&	item->GetImageMaxBounds()) {
		MatrixRecord m;
		SetIdentityMatrix(&m);
		::RotateMatrix (&m, ::Long2Fix(rot), ::Long2Fix(dest.MidPoint().h), ::Long2Fix(dest.MidPoint().v));
		MRect newDest;
		AlignmentGizmo::FitTransformedRectInside(dest, &m, item->GetImageMaxBounds(), newDest);
		AlignmentGizmo::MoveMidpointTo(newDest, item->GetMaxBounds(), newDest);
		

		// the rotate controller calls this function, and then takes the resulting dest rect and creates a RotateAction
		// However, along the path of performing the RotateAction, the dest is installed and then the caption is applied
		// to it.  If the caption has already been applied (in effect, by using ImageMaxBounds instead of MaxBounds), 
		// then the result is the caption area is removed twice, and we get a shrunken image
		// this cheezy solution (2 days before alpha) is to provide an optional rect calculated against
		// the max bounds (including caption) for use in preparation of the RotateAction
		if (optionalOutDestNoCaption) {
			AlignmentGizmo::FitTransformedRectInside(dest, &m, item->GetMaxBounds(), *optionalOutDestNoCaption);
			AlignmentGizmo::MoveMidpointTo(*optionalOutDestNoCaption, item->GetMaxBounds(), *optionalOutDestNoCaption);
			}//endif

		if (newDest != dest) {
			changesMade = true;
			dest = newDest;
			}//endif had to change the rect because of the rotation
		}//endif there is some rotation and constraints
					
	return changesMade;
}//end AdjustTransforms


void
PhotoPrintView::ApplyForeAndBackColors() const
{
	StColorState::Normalize();	// Counteract any theme stuff
#if __PowerPlant__ >= 0x02114004
	StThemeDrawingState::Normalize();
#else
	if (UEnvironment::HasFeature(env_HasAppearance11)) {
		::NormalizeThemeDrawingState();
	}
#endif
} // ApplyForeAndBackColors

//--------------------------------------
// ClearSelection
//	Deselects everything
//--------------------------------------
void
PhotoPrintView::ClearSelection()
{
	for (PhotoIterator i = mSelection.begin(); i != mSelection.end(); ++i) 
		this->RefreshItem(*i, kImageAndHandles);
	mSelection.clear();
}//end ClearSelection


//-----------------------------------------------
// ClickSelf.  Handle 'dat event
//-----------------------------------------------
void		
PhotoPrintView::ClickSelf(const SMouseDownEvent &inMouseDown) {
	this->FocusDraw();

	MRect			rFrame;
	CalcPortFrameRect(rFrame);
	SDimension32	imageDimensions;
	GetImageSize(imageDimensions);
	rFrame.SetWidth(imageDimensions.width);
	rFrame.SetHeight(imageDimensions.height);

	Assert_(mController != nil);
	mController->HandleClick(inMouseDown, rFrame, GetClickCount());
}//end ClickSelf

//--------------------------------------------
//	CreateBadges
//--------------------------------------------
void
PhotoPrintView::CreateBadges(LCommander* inBadgeCommander) {
	mBadgeGroup = new BadgeGroup(inBadgeCommander);
	PhotoIterator	i (GetModel()->begin());
	MatrixRecord	bodyToScreen;
	this->GetBodyToScreenMatrix(bodyToScreen);

	while (i != GetModel()->end()) {
		if (!(*i)->IsEmpty()) {
			PhotoBadge* newBadge (dynamic_cast<PhotoBadge*>(UReanimator::CreateView(PPob_Badge, this, mBadgeGroup)));
			newBadge->SetItem(GetDocument (), *i);

			MRect imageLoc ((*i)->GetImageRect());
			::TransformRect(&bodyToScreen, &imageLoc, nil);
			newBadge->PlaceInSuperFrameAt(imageLoc.left, imageLoc.top, Refresh_Yes);

			mBadgeMap[*i] = newBadge;
			
			// arbitrarily, make the first badge the latent sub (concept is that you'll start renaming first item)
			if (i == GetModel()->begin()) {
				mBadgeGroup->SetLatentSub(newBadge->GetNameTag());
				}//endif badges active
		}//endif not empty item (placeholder)
		++i;
	}//end while still items to make

}//end CreateBadges

//--------------------------------------------
//	DestroyBadges
//--------------------------------------------
void
PhotoPrintView::DestroyBadges() {
	if (mBadgeGroup == nil)
		return;

	// nothing in the badge group can be target since we're about to start killing the badges
	// so force a switch up the chain from the badgeGroup
	LCommander::ForceTargetSwitch(mBadgeGroup->GetSuperCommander());
	
	for (BadgeMap::iterator i = mBadgeMap.begin(); i != mBadgeMap.end(); ++i) {
		PhotoBadge*		pDoomed = i->second;
		MRect			doomedBounds;
		pDoomed->CalcPortFrameRect(doomedBounds);
		this->InvalPortRect(&doomedBounds);
		delete pDoomed;
	}//for
	
	mBadgeMap.clear();		// Empty out the whole thing
	mBadgeGroup = 0;
}//end DestroyBadges


/*
DoDragReceive {OVERRIDE}
*/
void
PhotoPrintView::DoDragReceive(
	DragReference	inDragRef)
{
	StDisableDebugThrow_();
	StDisableDebugSignal_();
	
	DefaultExceptionHandler		dragHandler (MPString (strn_ViewStrings, si_DragOperation).AsPascalString ());
	
	//	Make the action
	EPostAction					dragAction (GetDocument());
	
	try {
		dragAction = new ModelAction (GetDocument(), si_DropImage);
		} // try
		
	catch (LException& e) {
		//	Can't undo...keep going
		if (!ExceptionHandler::HandleKnownExceptions (e, true))
			throw;
	} // catch
	
	//	Handle the drag
	try {
		MAEList				fileList;

		MDragItemIterator	end (inDragRef);
		for (MDragItemIterator i = end; ++i != end; ) {
			if (i.HasFlavor (kDragFlavor)) {
				mFlavorAccepted = kDragFlavor;
			} // if
			
			else {
				FSSpec 				spec;
				Boolean				ioAllowEvilPromise (false);
				PromiseHFSFlavor	promise;
				if (!i.ExtractFSSpec (spec, ioAllowEvilPromise, promise)) continue;
				
				fileList.PutPtr (typeFSS, &spec, sizeof (spec));
			} // else
		} // for

		LView::OutOfFocus(nil);
		this->FocusDropArea ();

		if (0 == fileList.GetCount ()) {
			LDropArea::DoDragReceive(inDragRef);		// Call inherited to process each item
			// ReceiveDragItem will be called for each kDragFlavor item.
			} // if
			
		else {
			//	make new import at <drop position> with data {files}
			MAppleEvent				createEvent (kAECoreSuite, kAECreateElement);
				//	keyAEObjectClass
				DescType				classKey = PhotoPrintDoc::cImportClass;
				createEvent.PutParamPtr (typeType, &classKey, sizeof (classKey), keyAEObjectClass);
				
				//	keyAEInsertHere
				StAEDescriptor	locationDesc;
				MakeDropAELocation (locationDesc, inDragRef);
				createEvent.PutParamDesc (locationDesc, keyAEInsertHere);
				
				//	keyAEPropData
				createEvent.PutParamDesc (fileList, keyAEData);
			
			MAppleEvent				createResult (createEvent, kAEWaitReply | kAENeverInteract);
			// Will be handled by PhotoPrintDoc::HandleCreateImportEvent
			
			//	Remove result message and queue it up
			if (createResult.HasParam (keyAEResultInfo)) {
				//	Creaet the message
				EUserMessage			msg (MPString (strn_ViewStrings, si_ImportProblems).AsPascalString (), kCautionIcon);
				
				//	Add the details
				DescType				actualType;
				Size					actualSize;
				createResult.GetParamSize (actualType, actualSize, keyAEResultInfo);
				
				EUserMessage::TextRef	details (new MNewHandle (actualSize));
				details->Lock ();
				createResult.GetParamPtr (actualType, actualSize, **details, details->GetSize (), typeText, keyAEResultInfo);
				details->Unlock ();
				msg.SetDetails (details);
				
				//	Tell the user
				EUserMessageServer::GetSingleton ()->QueueUserMessage (msg);
			} // if
		} // else
			
	} // try
	
	catch (LException& e) {
		if (!ExceptionHandler::HandleKnownExceptions(e, true))
			throw;
	} // catch
	
} // DoDragReceive

/*
DoDragSendData {OVERRIDE}
	Send the data associated with a particular drag item

	This methods gets called if you installed the optional DragSendDataProc
	for this DragItem.

	It's a little strange to have this here (not the model), but the view is the only one which knows
	which images are selected.
*/
void
PhotoPrintView::DoDragSendData(
	FlavorType		inFlavor,
	ItemReference	inItemRef,
	DragReference	inDragRef)
{
	MNewHandle	theData(this->GetSelectedData(inFlavor));	// Get rid of handle on exit
	if (theData != nil) {
		StHandleLocker	lock(theData);
		Size	s = theData.GetSize();
		ThrowIfOSErr_(::SetDragItemFlavorData(inDragRef, inItemRef, inFlavor,
			*theData, s, 0L));
	}
} // DoDragSendData

// ---------------------------------------------------------------------------
//	• DragIsAcceptable												  [public]
// ---------------------------------------------------------------------------
//	Return whether a DropArea can accept the specified Drag
//
//	A Drag is acceptable if all items in the Drag are acceptable

Boolean
PhotoPrintView::DragIsAcceptable (
	
	DragReference	inDragRef)

{
	return mLayout->DragIsAcceptable (inDragRef);
	
} // DragIsAcceptable



void
PhotoPrintView::DrawHeader(SInt32 yOffset)
{
	PrintProperties& props (GetDocument()->GetPrintProperties());

	MRect bounds;	
	PhotoPrinter::CalculateHeaderRect(GetDocument()->GetPrintRec(), 
										&props, bounds, GetDocument()->GetResolution());
	bounds.Offset(0, yOffset);

	::TextFont(GetDocument()->GetProperties().GetFontNumber());
	SInt16 unscaledFontSize (GetDocument()->GetProperties().GetFontSize());
	::TextSize(unscaledFontSize * ((double)GetDocument()->GetResolution() / 72.0));

	MPString header (GetDocument()->GetProperties().GetHeader());	
	UTextDrawing::DrawWithJustification(header.Chars(), ::StrLength(header), bounds, teJustCenter, true);	
}//end DrawHeader


void
PhotoPrintView::DrawFooter(SInt32 yOffset)
{
	PrintProperties& props (GetDocument()->GetPrintProperties());

	MRect bounds;	
	PhotoPrinter::CalculateFooterRect(GetDocument()->GetPrintRec(), 
										&props, bounds, GetDocument()->GetResolution());
	bounds.Offset(0, yOffset);

	::TextFont(GetDocument()->GetProperties().GetFontNumber());
	SInt16 unscaledFontSize (GetDocument()->GetProperties().GetFontSize());
	::TextSize(unscaledFontSize * ((double)GetDocument()->GetResolution() / 72.0));

	MPString footer (GetDocument()->GetProperties().GetFooter());	
	UTextDrawing::DrawWithJustification(footer.Chars(), ::StrLength(footer), bounds, teJustCenter, true);	
}//end DrawFooter



static	RGBColor	gFiftyPercentGray  = {32767, 32767, 32767};


void
PhotoPrintView::DrawPrintable(SInt32 yOffset) {
	PrintProperties&	props (GetDocument()->GetPrintProperties());
	MRect				printable;	
	PhotoPrinter::CalculatePrintableRect(GetDocument()->GetPrintRec(), 
										&props, printable, GetDocument()->GetResolution());
	printable.Offset(0, yOffset);
	printable.SetWidth(printable.Width() - 1);
	printable.SetHeight(printable.Height()  - 1);
	StColorPenState		saveState;
	Pattern				grayPat;
	UQDGlobals::GetGrayPat(&grayPat);
	::PenPat(&grayPat);
	::RGBForeColor(&gFiftyPercentGray);
	::FrameRect(&printable);
}//end DrawPrintable

//-----------------------------------------------
// DrawSelf  if there is a selection, then select it
//-----------------------------------------------
void
PhotoPrintView::DrawSelf() {

	StDisableDebugThrow_();
	StDisableDebugSignal_();
	
	DefaultExceptionHandler		dragHandler (MPString (strn_ViewStrings, si_DrawOperation).AsPascalString ());

	GrafPtr			curPort;
	::GetPort(&curPort);
	GDHandle		curDevice = ::GetGDevice();

	MRect			rFrame;
	SDimension32	imageDimensions;
	this->GetImageSize(imageDimensions);
	rFrame.SetWidth(imageDimensions.width);
	rFrame.SetHeight(imageDimensions.height);
	{
		StColorState	saveColors;
		StColorState::Normalize();	// Counteract any theme stuff
		Pattern			whiteBackground;
		::BackPat(UQDGlobals::GetWhitePat(&whiteBackground));
		::EraseRect(&rFrame);
	}

	this->DrawPrintable();							// Draw rectangle around printable area
	this->DrawHeader();
	this->DrawFooter();

	// Draw page dividing lines if necessary
	if (GetDocument()->GetPageCount() > 1 && !mModel->GetDrawingProperties().GetPrinting()) {
		StColorPenState		savePen;
		StColorPenState::SetGrayPattern();
		SInt16				p = GetDocument()->GetPageCount();
		SInt16				pageHeight = imageDimensions.height / p;
		this->DrawHeader();
		this->DrawFooter();
		for (; p > 1; p--) {
			SInt16			y = pageHeight * (p - 1);
			::MoveTo(0, y);
			::LineTo(rFrame.right, y);
			this->DrawPrintable(y);
			this->DrawHeader(y);
			this->DrawFooter(y);
		}
	}


	MRect viewRevealed;
	CalcRevealedRect();
	GetRevealedRect(viewRevealed);
	Point viewOrigin;
	GetPortOrigin(viewOrigin);
	viewRevealed.Offset(viewOrigin.h, viewOrigin.v);
	MNewRegion			clip;
	clip = viewRevealed;

	if (mModel) {
		StPortOriginState	saveState (curPort);
		MRestoreValue<PhotoDrawingProperties> saveProps (GetModel()->GetDrawingProperties());
		GetModel()->GetDrawingProperties().SetScreenRes(GetDocument()->GetResolution());
		
		// create the xlation matrix to move from paper's origin (image basis) to screen origin (0,0)
		MatrixRecord		paperToScreen;
		this->GetBodyToScreenMatrix(paperToScreen);

		mModel->Draw(&paperToScreen,
					(CGrafPtr)curPort,
					curDevice,
					clip);
	}//endif something to draw

	// Draw the selection gadgets (if we have a selection)
	if (mController && mModel && !this->Selection().empty())
		mController->Select(this->Selection());
} // DrawSelf


// ---------------------------------------------------------------------------
//	• FindDropItem											  [public]
// ---------------------------------------------------------------------------
//	Creates a location specifier for the dropped items.  The two possibilities
//	are before the item under the cursor or at the end (if nothing was under).
//	We use Controller::InterpretClick to decide where things go and use the
//	ClickEventT.target.item field.

PhotoItemRef
PhotoPrintView::FindDropItem (
	
	DragReference	inDragRef) const

{ // begin FindDropItem

	//	Find the hit item
	PhotoController::ClickEventT	clickEvent;
	clickEvent.macEvent.what = mouseUp;
	clickEvent.macEvent.message = 0;
	clickEvent.macEvent.when = ::TickCount ();
	clickEvent.macEvent.modifiers = ::GetCurrentKeyModifiers ();
	::GetDragMouse (inDragRef, &clickEvent.macEvent.where, &clickEvent.wherePort);

	clickEvent.wherePort = clickEvent.macEvent.where;
	this->GlobalToPortPoint (clickEvent.wherePort);
	clickEvent.whereLocal = clickEvent.wherePort;
	this->PortToLocalPoint (clickEvent.whereLocal);

	Assert_(mController != nil);
	mController->InterpretClick(clickEvent);
	
	return clickEvent.target.item;
} // end FindDropItem

/*
GetBadgeForItem
*/
PhotoBadge*
PhotoPrintView::GetBadgeForItem(PhotoItemRef inItem) {
	BadgeMap::iterator i (mBadgeMap.find(inItem));
	if (i != mBadgeMap.end())
		return (*i).second;
	else
		return nil;
} //end GetBadgeForItem


/*
* paper often starts in negative space.
* however, PP windows are always positive (or 0,0).
* here is an xlation from paper space (in which images are based) to screen space
* taking into account margins + header
*/
void
PhotoPrintView::GetBodyToScreenMatrix(MatrixRecord& outMatrix) {
	::SetIdentityMatrix(&outMatrix);
	MRect headerRect;
	PhotoPrinter::CalculateHeaderRect(GetDocument()->GetPrintRec(), 
										&(GetDocument()->GetPrintProperties()),
										headerRect, GetDocument()->GetResolution());
	::TranslateMatrix(&outMatrix, ::FixRatio(headerRect.left, 1), ::FixRatio(headerRect.top, 1));
}//end GetBodyToScreenMatrix


/*
GetSelectedData
	Returns a handle consisting of data of the specified type.
	Caller owns the handle.
*/
Handle
PhotoPrintView::GetSelectedData(const OSType inType) const
{
	ConstPhotoIterator	i;
	
	switch (inType) {
		case kScrapFlavorTypePicture:
			{
			MRect		combinedBounds;
			for (i = mSelection.begin(); i != mSelection.end(); ++i) {
				if ((*i)->IsEmpty())
					continue;					// Nothing to do for a placeholder
				MRect		bounds((*i)->GetDestRect());
				// Accumulate the rectangles; note that the first one has to be a special case
				if (combinedBounds.IsEmpty())
					combinedBounds = bounds;
				else
					combinedBounds += bounds;
			}

			MNewPicture		pict;
			{
				MOpenPicture	draw(pict, combinedBounds);
				::ClipRect(&combinedBounds);
				for (i = mSelection.begin(); i != mSelection.end(); ++i) {
					if ((*i)->IsEmpty())
						continue;				// Nothing to do for a placeholder
					HORef<EGWorld>	proxy = (*i)->GetProxy();
					MRect		bounds((*i)->GetDestRect());
					proxy->CopyImage(UQDGlobals::GetCurrentPort(), bounds, srcCopy, nil);
				}
			}
			return reinterpret_cast<Handle>(pict.Detach());
			} // case
		
		case kXMLFlavor:
			{
			XMLHandleStream		stream;	
			XML::Output			out(stream);
			
			XMLDocFormatter		formatter (out);
			formatter.FormatItems (mSelection.begin(), mSelection.end());

			return stream.DetachDataHandle();
			} // case
			
		case kObjectRefFlavor:
			{
			StAEDescriptor	superSpec;
			GetDocument()->MakeSpecifier (superSpec);
			
			//	Build the specifier list
			MAEList			specifierList;
			for (i = mSelection.begin (); i != mSelection.end (); ++i) {
				SInt32			modelIndex = 1 + (std::find (mModel->begin(), mModel->end (), *i) - mModel->begin());
				
				StAEDescriptor	absPosKeyData;
				OSErr			err = ::CreateOffsetDescriptor (modelIndex, absPosKeyData);
				ThrowIfOSErr_(err);

				StAEDescriptor	outSelfSpecifier;
				err = ::CreateObjSpecifier (PhotoItemModelObject::cClass, superSpec,
						formAbsolutePosition, absPosKeyData, false,
						outSelfSpecifier);
				ThrowIfOSErr_(err);
				
				specifierList.PutDesc (outSelfSpecifier);
			} // for
			
			//	Flatten it
			StHandleBlock		specData (specifierList.GetDataSize ());
			StHandleLocker		specLock (specData);
			specifierList.CopyData (*specData, ::GetHandleSize (specData));

			return specData.Release ();
			} // case
		} // switch

	return nil;

} // GetSelectedData

//-----------------------------------------------
// GetSelection
//-----------------------------------------------
PhotoItemRef
PhotoPrintView::GetPrimarySelection() const
{
	if (mSelection.begin() == mSelection.end())
		return (PhotoItemRef)nil;
	else
		return (*(mSelection.begin()));
}//end GetSelection 

/*
InsideDropArea {OVERRIDE}
	The mouse location is where the mouse actually is on the screen. The
	alternative is the pinned location, which is _usually_ the same location,
	but can be different if the cursor is being constrained by a tracking handler.
	This is useful when you want an area within a view to be 'off-limits' to
	the ongoing drag.

	If we did want to do something based on where the cursor currently is in
	our area (such as indicating an insertion point or something), it would
	usually be best to use the pinned location for that work.

	Both mouse locations are in local coordinates
*/
void	PhotoPrintView::InsideDropArea(
	DragReference	inDragRef,
	Point&			inMouseLocation,
	Point&			inPinnedLocation)
{
#pragma unused(inMouseLocation, inPinnedLocation)

	MKeyMap			keymap;
	Boolean			dragIsFromThisView = this->CheckIfViewIsAlsoSender(inDragRef);
	Boolean			copyData = !(dragIsFromThisView && !keymap.ScanPressed(MKeyMap::kOptionScan));
	if (copyData) {
		::SetThemeCursor(kThemeCopyArrowCursor);
	} else {
		UCursor::SetArrow();
	}
} // InsideDropArea

/*
IsAnythingSelected
*/
bool					
PhotoPrintView::IsAnythingSelected() const
{
	return mSelection.size() > 0;
}//end IsAnythingSelected

/*
IsSelected
	Determine if a specific item is selected
*/
bool
PhotoPrintView::IsSelected(PhotoItemRef inItem) {
	return std::find(mSelection.begin(), mSelection.end(), inItem) != mSelection.end();
} // IsSelected

/*
ListenToCommand
	We'll be getting messages from the bevel buttons.  They should go up the
	command hierarchy, but what the hell
*/
void
PhotoPrintView::ListenToCommand(
	MessageT	inMessage,
	void		*ioParam)
{

	PhotoPrintDoc*		theDoc = GetDocument();
	SInt32				theValue;
	
	Layout::LayoutType	theType = GetLayout ()->GetType ();
	UInt32				theCount = GetModel ()->GetCount ();
	SInt32				theDuplicated = theDoc->GetDuplicated();
	
	EPostAction			theAction (theDoc);
	
	try {theAction = new ModelAction (theDoc, si_ChangeLayoutType);} catch (...) {};
	
	switch (inMessage) {
		case 'dupl':
			theDuplicated = *(SInt32*)ioParam;
			break;

		case cmd_ReLayout:
			theValue = *(SInt32*)ioParam;
			theType = gLayoutInfo[theValue - 1].type;
			theCount = gLayoutInfo[theValue - 1].count;
			break;
	} // end switch

	if (theType == Layout::kFixed && theDuplicated == 2)
		theType = Layout::kMultiple;
			
	else if (theType == Layout::kMultiple && theDuplicated == 1)
		theType = Layout::kFixed;
			
	this->SwitchLayout (theType, theCount);

} // ListenToCommand

/*
ListenToMessage {OVERRIDE}
	We'll be getting messages from the bevel buttons
*/
void
PhotoPrintView::ListenToMessage(
	MessageT	inMessage,
	void		*ioParam)
{
	switch (inMessage) {
		case 'dupl':
		case cmd_ReLayout:
			ListenToCommand (inMessage, ioParam);
			break;

		case msg_ModelChanged: 
			OnModelChanged ((PhotoPrintDoc*) ioParam);
			break;	

		case PhotoPrintModel::msg_ModelItemsAdded: 
			OnModelItemsAdded ((PhotoPrintModel::MessageRange*) ioParam);
			break;	
		
		case PhotoPrintModel::msg_ModelItemsChanged: 
			OnModelItemsChanged ((PhotoPrintModel::MessageRange*) ioParam);
			break;	
		
		case PhotoPrintModel::msg_ModelItemsRemoved: 
			OnModelItemsRemoved ((PhotoPrintModel::MessageRange*) ioParam);
			break;
			
		case FileNotifier::msg_FilenameChanged:
			OnFilenameChanged ((const FSSpec*) ioParam);
			break;
	} // switch

} // ListenToMessage

// ---------------------------------------------------------------------------
//	• MakeDragRegion											  [public]
// ---------------------------------------------------------------------------

void
PhotoPrintView::MakeDragRegion (
	
	DragReference	inDragRef, 
	RgnHandle 		inDragRegion) 

{ // begin MakeDragRegion
	
	//	Create the drag region
	MRegion					theDragRgn (inDragRegion);
	
	HORef<MNewRegion>		imageRgn (new MNewRegion);
	this->FocusDraw ();
	
	MatrixRecord		mat;
	this->GetBodyToScreenMatrix(mat);

	for (PhotoIterator i (mSelection.begin ()); i != mSelection.end (); ++i) {
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

	if (mSelection.size() == 1) {
		// Add translucent drag
		SInt32		response;
		::Gestalt (gestaltDragMgrAttr, &response);
		if (response & (1 << gestaltDragMgrHasImageSupport)) {
			try {
				PhotoItemRef	image(this->GetPrimarySelection());
				MRect		bounds(image->GetImageMaxBounds());		// 225 Be sure it's rotated
				::TransformRect (&mat, &bounds, nil);

				Point		globalPt, localPt;
				globalPt = localPt = bounds.TopLeft();
				::LocalToGlobal(&globalPt);
				::SubPt(localPt, &globalPt);

				PhotoDrawingProperties	basicProps (false, false, false, GetDocument()->GetResolution());
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

// ---------------------------------------------------------------------------
//	• MakeDropAELocation											  [public]
// ---------------------------------------------------------------------------
//	Creates a location specifier for the dropped items.  The two possibilities
//	are before the item under the cursor or at the end (if nothing was under).
//	We use Controller::InterpretClick to decide where things go and use the
//	ClickEventT.target.item field.

void
PhotoPrintView::MakeDropAELocation (
	
	AEDesc&			outLoc,
	DragReference	inDragRef) const

{ // begin MakeDropAELocation
	
	MakeItemAELocation (outLoc, FindDropItem (inDragRef));

} // end MakeDropAELocation

// ---------------------------------------------------------------------------
//	• MakeItemAELocation											  [public]
// ---------------------------------------------------------------------------
//	Creates a location specifier for the dropped items.  The two possibilities
//	are before the item under the cursor or at the end (if nothing was under).
//	We use Controller::InterpretClick to decide where things go and use the
//	ClickEventT.target.item field.

void
PhotoPrintView::MakeItemAELocation (
	
	AEDesc&			outLoc,
	PhotoItemRef	inItem) const

{ // begin MakeItemAELocation

	//	Insertion point
	StAEDescriptor	docSpec;
	GetDocument()->MakeSpecifier (docSpec);
	
	if (inItem) {
		SInt32			modelIndex = 1 + (std::find (mModel->begin(), mModel->end (), inItem) - mModel->begin());
			
		StAEDescriptor	absPosKeyData;
		ThrowIfOSErr_(::CreateOffsetDescriptor (modelIndex, absPosKeyData));

		StAEDescriptor	itemSpec;
		ThrowIfOSErr_(::CreateObjSpecifier (PhotoItemModelObject::cClass, docSpec,
											formAbsolutePosition, absPosKeyData, false,
											itemSpec));
			
		UAEDesc::MakeInsertionLoc (itemSpec, kAEBefore, &outLoc);
	} // if
	
	else UAEDesc::MakeInsertionLoc (docSpec, kAEEnd, &outLoc);

} // end MakeItemAELocation

/*
OnFilenameChanged
*/
void
PhotoPrintView::OnFilenameChanged(
	const	FSSpec*		inNewSpec)
{
	for (PhotoIterator i = mModel->begin (); i != mModel->end (); ++i) {
			// Be sure we get a new spec
		(*i)->ClearFileSpec();					
		HORef<MFileSpec>	iSpec = (*i)->GetFileSpec ();
		if (*iSpec != *inNewSpec) continue;
		
			//	Refresh the item
		RefreshItem (*i);
			
			//	And its badge
		if (mBadgeGroup) {
			LPane*	theBadge (mBadgeMap[*i]);
			Assert_(theBadge);
			
			theBadge->SetDescriptor (inNewSpec->name);
			theBadge->Refresh ();
		} // if
	} // for

} // OnFilenameChanged

/*
OnModelChanged
*/
void
PhotoPrintView::OnModelChanged(
	PhotoPrintDoc*		/*inDirtyDoc*/)
{
	if (mBadgeGroup) {
		LCommander* oldBadgeSuper (mBadgeGroup->GetSuperCommander());
		DestroyBadges();
		CreateBadges(oldBadgeSuper);
		}//if there are some badges

} // OnModelChanged

/*
OnModelItemsAdded
*/
void
PhotoPrintView::OnModelItemsAdded(
	PhotoPrintModel::MessageRange*	/*inRange*/)
{
} // OnModelItemsAdded

/*
OnModelItemsChanged
*/
void
PhotoPrintView::OnModelItemsChanged(
	PhotoPrintModel::MessageRange*	inRange)
{
	for (ConstPhotoIterator i = inRange->mBegin; i != inRange->mEnd; ++i)
		RefreshItem (*i);

} // OnModelItemsChanged

/*
OnModelItemsRemoved
*/
void
PhotoPrintView::OnModelItemsRemoved(
	PhotoPrintModel::MessageRange*	inRange)
{
	RemoveFromSelection (inRange->mBegin, inRange->mEnd);
} // OnModelItemsRemoved

// ============================================================================
//		• ReceiveDragItem {OVERRIDE}
// ============================================================================
void
PhotoPrintView::ReceiveDragItem(
	DragReference	inDragRef,
	ItemReference	inItemRef,
	Size			inDataSize,		// How much data there is
	Boolean			inCopyData,  	// Should we copy the data?
	Boolean			inFromFinder,	// Data came from the Finder
	Rect&			inItemBounds)	// In Local coordinates
{
#pragma unused(inFromFinder, inItemBounds)
	
		//	Read the object reference list
		MNewHandle		inData (inDataSize);
		inData.Lock ();
		ThrowIfOSErr_(::GetFlavorData(inDragRef, inItemRef, kObjectRefFlavor, *inData, &inDataSize, 0));
		
		//	Gross kludge to build a desc under Carbon
		StAEDescriptor	inDescList ('BLOB', *inData, inData.GetSize ());
		inDescList.mDesc.descriptorType = typeAEList;

		//	Useful data
		MAEAddressDesc	realAddress (MFileSpec::sDefaultCreator);
		
		//	Where things were dropped
		PhotoItemRef	dropItem = FindDropItem (inDragRef);
		
		//	Get the actual objects (lazy instantiation)
		typedef	std::pair<PhotoItemRef, PhotoPrintDoc*>	DragPair;
		typedef	std::vector<DragPair>					DragPairList;
		DragPairList	dragPairs;

		MAEDescIterator	end (inDescList);
		for (MAEDescIterator i (end); ++i != end; ) {
			StAEDescriptor			token;
			ThrowIfOSErr_(LModelDirector::Resolve (*i, token));
			
			LModelObject*			tokenItem (LModelObject::GetModelFromToken (token));
			if (tokenItem->GetModelKind () != PhotoItemModelObject::cClass) continue;
			
			PhotoItemModelObject*	pimo = dynamic_cast<PhotoItemModelObject*> (tokenItem);
			if (!pimo) continue;
			
			dragPairs.push_back (DragPair (pimo->GetPhotoItem (), pimo->GetDoc ()));
		} // for

		if (inCopyData) {
			// Deselect, so we can select new ones
			this->ClearSelection();							
			
			//	Drag in the new ones
			for (DragPairList::iterator i = dragPairs.begin (); i != dragPairs.end (); ++i)
			{
				StAEDescriptor			token;
				i->second->GetPhotoItemModel (i->first, token);
				
				LModelObject*			tokenItem (LModelObject::GetModelFromToken (token));
				MAppleEvent				cloneEvent (kAECoreSuite, kAEClone);
					//	keyDirectObject
					StAEDescriptor			itemDesc;
					tokenItem->MakeSpecifier (itemDesc);
					cloneEvent.PutParamDesc (itemDesc, keyDirectObject);
					
					//	keyAEInsertHere
					StAEDescriptor	locationDesc;
					MakeItemAELocation (locationDesc, dropItem);
					cloneEvent.PutParamDesc (locationDesc, keyAEInsertHere);
				
				//	Will be handled by PhotoItemModelObject::HandleClone
				MAppleEvent				cloneResult (cloneEvent, kAEWaitReply | kAENeverInteract);
				//	Remove error message and queue it up
			} // for
		} // if
		
		else {
			if (PhotoPrintPrefs::Singleton()->GetSorting() != sort_None) {
				EUserMessageServer::GetSingleton ()->QueueUserMessage (EUserMessage (MPString (strn_ViewStrings, si_SortedMoveMessage).AsPascalString (), kNoteIcon));
				PhotoPrintPrefs::Singleton()->SetSorting(sort_None);
				} // if
				
			//	Get the position of the drop item
			PhotoIterator	dropIterator = std::find (mModel->begin(), mModel->end (), dropItem);
			
			//	Placeholder restrictions
			if (GetLayout ()->HasPlaceholders ()) {
				if (dragPairs.size () > 1) return;	//	Disallow multiple moves
				if (!dropItem) return;				//	Disallow moves to the end
				if (!dropItem->IsEmpty ()) return;	//	Disallow moves to occupied cells
				//	So all that is left is single moves to empty cells.
				} // if
				
			//	Check for nop drags - drags to any of the items in the list
			//	Probably not right, but good enough for a first cut.
			//	Also adjust the drop location for any items being moved forward
			//	Unless the layout has fixed slots!
			for (DragPairList::iterator i = dragPairs.begin (); i != dragPairs.end (); ++i) {
				if (i->first == dropItem) return;
				
				//	If the moved item is before the drop location
				if (GetLayout ()->HasPlaceholders ()) continue;
				
				if (dropIterator == mModel->end ()) continue;
				if (dropIterator == std::find (mModel->begin(), dropIterator, i->first)) continue;
				
				++dropIterator;
				} // if
			
			//	Change to the adjusted drop item
			dropItem = (dropIterator == mModel->end ()) ? 0 : *dropIterator;
			
			//	OK, just move these suckers
			for (DragPairList::iterator i = dragPairs.begin (); i != dragPairs.end (); ++i)
			{
				StAEDescriptor			token;
				i->second->GetPhotoItemModel (i->first, token);
				
				LModelObject*			tokenItem (LModelObject::GetModelFromToken (token));
				MAppleEvent				moveEvent (kAECoreSuite, kAEMove);
					//	keyDirectObject
					StAEDescriptor			itemDesc;
					tokenItem->MakeSpecifier (itemDesc);
					moveEvent.PutParamDesc (itemDesc, keyDirectObject);
					
					//	keyAEInsertHere
					StAEDescriptor	locationDesc;
					MakeItemAELocation (locationDesc, dropItem);
					moveEvent.PutParamDesc (locationDesc, keyAEInsertHere);
				
				//	Will be handled by PhotoItemModelObject::HandleMove
				MAppleEvent				moveResult (moveEvent, kAEWaitReply | kAENeverInteract);
				//	Remove error messages and queue them up
			} // for
		} // else
	
} // ReceiveDragItem


/*
Refresh
*/
void
PhotoPrintView::Refresh() {
	FocusDraw();
	this->UpdateBadges(true);
	LView::Refresh();
}//end


/*
RefreshItem
	Force redraw of one item
*/
void
PhotoPrintView::RefreshItem(PhotoItemRef inItem, const bool inHandles) {
	Assert_(inItem != nil);
	FocusDraw();
	MRect		bounds(inItem->GetMaxBounds());

	// ??? really cheesy way to do this
	// Note that we do one extra pixel here because when the handles are rotated, we would
	// leave stray pixels around. If we built the actual region, we wouldn’t need to do this.
	if (inHandles == kImageAndHandles)
		bounds.Inset(-PhotoController::kHandleSize - 1, -PhotoController::kHandleSize - 1);

	MatrixRecord		mat;
	::SetIdentityMatrix(&mat);
//	::RotateMatrix(&mat, ::Long2Fix(inItem->GetRotation()), ::Long2Fix(bounds.MidPoint().h),
//		::Long2Fix(bounds.MidPoint().v));

	MatrixRecord paperToScreen;
	GetBodyToScreenMatrix(paperToScreen);
	::ConcatMatrix(&paperToScreen, &mat);

	PhotoUtility::DrawXformedRect(bounds, &mat, kInvalidate);
} // RefreshItem

//--------------------------------------
// RemoveFromSelection
//--------------------------------------

void
PhotoPrintView::RemoveFromSelection(
	ConstPhotoIterator 	inBegin,
	ConstPhotoIterator 	inEnd) 
{
	PhotoItemList	deadItems (inBegin, inEnd);
	
	PhotoItemRef	oldPrimary (this->GetPrimarySelection());
	for (PhotoIterator i = deadItems.begin (); i != deadItems.end (); ++i) {
		PhotoIterator	dead = std::find (mSelection.begin (), mSelection.end (), *i);
		if (dead != mSelection.end()) {
			mSelection.erase (dead);
			this->RefreshItem (*i, kImageAndHandles);
		}
	}//end for all
	
	if (this->GetPrimarySelection() && (oldPrimary != this->GetPrimarySelection()))
		this->RefreshItem(this->GetPrimarySelection(), kImageAndHandles);
}//end RemoveFromSelection


//--------------------------------------
// RemoveFromSelection
//--------------------------------------
void
PhotoPrintView::RemoveFromSelection(PhotoItemList& deletions) {
	RemoveFromSelection (deletions.begin(), deletions.end());
}//end RemoveFromSelection

//---------------------------------
// Select
//---------------------------------
void	
PhotoPrintView::Select(const PhotoItemList& targets)
{
	mSelection.clear();
	mSelection = targets;
	this->Refresh();		// ??? overkill, ought to refresh each image
}//end Select	


//-----------------------------------------------
// Selection
//-----------------------------------------------
const PhotoItemList&
PhotoPrintView::Selection() const
{
	return mSelection;
}//end Selection 


//-----------------------------------------------
// SetController
//-----------------------------------------------
void
PhotoPrintView::SetController(

	OSType 		newController, 
	LCommander*	inBadgeCommander) 
	
{
	if (mControllerType == newController) return;
	
	switch (newController) {
		default:
			SignalString_("Invalid controller");
			//	Fall through to prevent crash…
			
		case tool_Arrow:
			mController = new ArrowController(this);
			mControllerType = newController;
			break;

		case tool_Crop:
			mController = new CropController(this);
			mControllerType = newController;
			break;

		case tool_Rotate:
			mController = new RotateController(this);
			mControllerType = newController;
			break;

		case tool_Zoom:
			mController = new CropZoomController(this);
			mControllerType = newController;
			break;
	
		case tool_Name:
		{
			//see if we need to warn, and if we do, whether people want this tool
			if (WarnAboutRename()) {
		
				NameController*		curController = dynamic_cast<NameController*>((PhotoController*)mController);
				// 90 Be sure we don't already have a NameController
				if (curController == nil) {
					mController = new NameController(this);
					mControllerType = newController;
					this->CreateBadges(inBadgeCommander);
					
					// and make the LTabGroup the target
					LCommander::SwitchTarget(mBadgeGroup);
					}//endif need to create the controller
				}//endif ok to perform this switch
			else {
				LWindow*	theWindow = LWindow::FetchWindowObject(this->GetMacWindow());
				LPane* oldToolButton (theWindow->FindPaneByID(mControllerType));
				if (oldToolButton != nil)
					oldToolButton->SetValue(Button_On);
				}//else uh-oh, pretend we're an a
		
			break;
		}
	}//end switch

	if (newController != tool_Name)
		this->DestroyBadges();
}//end SetController


//-----------------------------------------------
// SetDocument
//-----------------------------------------------
void
PhotoPrintView::SetDocument(

	PhotoPrintDoc*	inNewDoc) 

{
	if (mDoc == inNewDoc) return;
	
	if (mDoc) mModel->RemoveListener (mDoc);
	mDoc = inNewDoc;
	if (mDoc) mModel->AddListener (mDoc);
	
}//end SetDocument


//-----------------------------------------------
// SetupDraggedItem
//  		some of this could be done on construction of the item
// 	but factoring it out here allows cleaner subclassing
//-----------------------------------------------
void
PhotoPrintView::SetupDraggedItem(PhotoItemRef item) 
{
	item->GetProperties().SetAlignment(kAlignAbsoluteCenter);
	item->GetProperties().SetMaximize(true);
	item->GetProperties().SetAspect(true);

	//removed large chunk concerning rectangles which was outdated and causing efficiency problems slithy 19 jul 2001

}//end SetupDraggedItem


/*
SetLayoutType
*/
void
PhotoPrintView::SetLayoutType(const OSType inType, const bool inInit)
{
	Layout*		oldLayout = mLayout;
	switch (inType) {
		case Layout::kGrid:
			mLayout = new GridLayout(GetDocument (), mModel);
			break;

		case Layout::kSingle:
			mLayout = new SingleLayout(GetDocument (), mModel);
			break;

		case Layout::kFixed:
			mLayout = new FixedLayout(GetDocument (), mModel);
			break;

		case Layout::kMultiple:
			mLayout = new MultipleLayout(GetDocument (), mModel);
			break;

		case Layout::kSchool:
			mLayout = new SchoolLayout(GetDocument (), mModel);
			break;

		case Layout::kCollage:
			mLayout = new CollageLayout(GetDocument (), mModel);
			break;
		
		default:
			ThrowOSErr_(paramErr);
			break;
	}

	// Now that we've safely replaced it, get rid of the old one
	delete oldLayout;

	// Make placeholders as needed (185 note that if we read in the file, we don't need to)
	if (inInit == kInitialize)
		mLayout->Initialize();

	LWindow*	theWindow = LWindow::FetchWindowObject(this->GetMacWindow());
	LPane*		placard = theWindow->FindPaneByID('ptxt');
	if (placard != nil) {
		LStr255		theName(mLayout->GetName());
		placard->SetDescriptor(mLayout->GetName());
	}
	
	if (!PhotoPrintApp::gIsRegistered) {	
		mLayout->SetAnnoyingwareNotice(true, annoy_header);
	}//endif need to slam in the annoyingware notice
	
//if (Registration::IsTimeLimited) {
//		mLayout->SetAnnoyingwareNotice(true, annoy_header);
//	}//endif

} // SetLayoutType


// find the newPrimary in selection list, move it to front
void
PhotoPrintView::SetPrimarySelection(PhotoItemRef newPrimary) {
	PhotoItemRef oldPrimary (GetPrimarySelection());
	if (newPrimary != oldPrimary) {
		PhotoIterator	oldPosition = std::find (mSelection.begin (), mSelection.end (), newPrimary);
		if (oldPosition != mSelection.end()) {
			mSelection.erase(oldPosition);
			mSelection.insert(mSelection.begin(), newPrimary);

			this->RefreshItem(oldPrimary, kImageAndHandles);
			this->RefreshItem(newPrimary, kImageAndHandles);

			}//endif found it in selection
		}//endif

	// this would be a perfect place to set the active name badge, but i can't get it to work perfectly  . . .

	}//end SetPrimarySelection


/*
SwitchLayout
*/
void
PhotoPrintView::SwitchLayout(
	Layout::LayoutType	theType, 
	UInt32				theCount)
{
	this->Refresh();					// Doc orientation may change, so refresh before AND after

	// Get a copy of the first item in case we need it for populating
	PhotoItemRef	theItem = nil;
	if (!mModel->IsEmpty() && (theType == Layout::kMultiple || theType == Layout::kSchool)) {
		PhotoItemRef	firstImage = mModel->GetFirstNonEmptyItem();
		if (firstImage != nil)
			theItem = new PhotoPrintItem(*firstImage);

		// if we are switching to a multiple (or school) then empty out the model before setting the type
		this->GetModel()->RemoveAllItems();
	}
	this->SetLayoutType(theType, kInitialize);

	// Update the UI if needed
	PhotoPrintDoc*	theDoc = GetDocument();
	switch (theType) {
		case Layout::kGrid:
		case Layout::kFixed:
			theDoc->JamDuplicated(1);	// Avoid getting a message for this!
			break;
		case Layout::kSchool:
		case Layout::kMultiple:
			theDoc->JamDuplicated(2);	// Avoid getting a message for this!
			break;
	}
	LBevelButton*	duplicated = theDoc->GetDuplicatedPopup();
	if (theType == Layout::kGrid) {
		theDoc->GetMinMaxGroup()->Show();
		duplicated->Hide();
	} else {
		theDoc->GetMinMaxGroup()->Hide();
		duplicated->Show();
	}
	MenuRef			duplicatedMenu = duplicated->GetMacMenuH();
	LBevelButton*	orientation = theDoc->GetOrientationPopup();
	MenuRef			orientationMenu = orientation->GetMacMenuH();
	if (theType == Layout::kSchool) {
		::DisableMenuItem(duplicatedMenu, 1);
		::DisableMenuItem(orientationMenu, 1);
	} else {
		::EnableMenuItem(duplicatedMenu, 1);
		::EnableMenuItem(orientationMenu, 1);
	}
	
	// Repopulate the new layout (the main “if” is above, when we made theItem)
	if (theItem != nil) {
		mLayout->AddItem(theItem, GetModel ()->end ());
	}//endif

	this->ClearSelection();				// There’s no guarantee that the old items are still around

	// 199 If we're switching back to a Grid, we don't want any placeholders
	if (!mLayout->HasPlaceholders())
		this->GetModel()->RemoveEmptyItems(PhotoPrintModel::kDelete);
	
	mLayout->SetImageCount(theCount);
	mLayout->LayoutImages();			// Be sure any new images show up in the right place

	this->Refresh();

	//	Update the menu values  Should be somewhere called by UpdateMenus…
	{
		LBevelButton*	layoutPopup = theDoc->GetLayoutPopup ();
		StDisableBroadcaster	disable (layoutPopup);
		
		StValueChanger<Layout::LayoutType>	saveType (theType, (theType == Layout::kMultiple) ? Layout::kFixed : theType);

		LayoutMenuInfo*	begin (gLayoutInfo);
		LayoutMenuInfo*	end (begin + sizeof (gLayoutInfo) / sizeof (gLayoutInfo[0]));
		for (LayoutMenuInfo* i = gLayoutInfo; i != end; ++i) {
			if (i->type != theType) continue;
			if (i->count && (i->count != theCount)) continue;
			
			layoutPopup->SetCurrentMenuItem ((i - gLayoutInfo) + 1);
			break;
		} // for
	}
} // SwitchLayout

// ---------------------------------------------------------------------------
// ToggleSelected
// ---------------------------------------------------------------------------
void					
PhotoPrintView::ToggleSelected(PhotoItemList& togglees) {
	PhotoItemRef	oldPrimary(this->GetPrimarySelection());
	for (PhotoIterator i = togglees.begin(); i != togglees.end(); ++i) {
		PhotoIterator	si = std::find (mSelection.begin (), mSelection.end (), *i);
		if (si != mSelection.end ())
			mSelection.erase(si);
		else
			mSelection.insert(si, *i);
		this->RefreshItem(*i, kImageAndHandles);
	}//end for

	if (this->GetPrimarySelection() && (oldPrimary != this->GetPrimarySelection()))
		this->RefreshItem(this->GetPrimarySelection(), kImageAndHandles);
}//end ToggleSelected

/*
UpdateBadges
*/
void
PhotoPrintView::UpdateBadges(bool /*inState*/) {
	if (mBadgeGroup == nil)
		return;

	MatrixRecord	bodyToScreen;
	this->GetBodyToScreenMatrix(bodyToScreen);
	for (BadgeMap::iterator i = mBadgeMap.begin(); i != mBadgeMap.end(); ++i) {
		PhotoItemRef item = (*i).first;
		PhotoBadge* badge = (*i).second;
		MRect imageLoc (item->GetImageRect());
		::TransformRect(&bodyToScreen, &imageLoc, nil);
		badge->PlaceInSuperFrameAt(imageLoc.left, imageLoc.top, Refresh_Yes);
	}//for

}//end UpdateBadges


// For use in WarnAboutAlternate
enum {
	kWarnOK = 1,
	kWarnCancel,
	kWarnBlurb,
	kWarnDontShowAgain,
	kWarnFnord
};


bool
PhotoPrintView::WarnAboutRename	(void) {
	bool bHappy (true);

	do {
		PhotoPrintPrefs*	prefs = PhotoPrintPrefs::Singleton();
		
		// if they don't want to be warned, we done
		if (!prefs->GetWarnRename())
			continue;
		
		
		StDesktopDeactivator	deactivate;
		
		MNewDialog dlog (dlog_WarnAboutRename);
		WindowRef dlogWindow (dlog.GetWindowRef());
		::ShowWindow(dlogWindow);
		::BringToFront(dlogWindow);
		
		dlog.SetDefaultItem();
		dlog.SetCancelItem();
		bool done (false);
		short item;
		short state;
		do {
			dlog.DoModal(item);
			switch (item) {
				case kWarnOK:
					done = true;
					break;
				case kWarnCancel:
					bHappy = false;
					done = true;
					break;
				case kWarnDontShowAgain:
					MDialogItem(dlog, kWarnDontShowAgain) >> state;
					MDialogItem(dlog, kWarnDontShowAgain) << !state;
					prefs->SetWarnRename(state);
					break;
					
				}//end switch
			} while (!done);
				
			
		::HideWindow(dlogWindow);
		::SendBehind(dlogWindow, (WindowRef)0);

		} while (false);
		
	return bHappy;
	}//end WarnAboutRename
	


