/*
	File:		PhotoPrintView.cp

	Contains:	MVC view, which displays stuff

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

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
#include "GridLayout.h"
#include "PhotoBadge.h"
#include "PhotoPrintApp.h"
#include "PhotoPrintCommands.h"
#include "PhotoPrintConstants.h"
#include "PhotoPrinter.h"
#include "PhotoPrintEvents.h"
#include "PhotoPrintModel.h"
#include "PhotoPrintPrefs.h"
#include "PhotoUtility.h"
#include "Registration.h"
#include "RotateController.h"
#include "SchoolLayout.h"
#include "SingleLayout.h"
#include "XMLHandleStream.h"
#include "NameController.h" 

#include "xmlinput.h"

#include "HARef.h"
#include "MAEList.h"
#include "MAppleEvent.h"
#include "MDragItemIterator.h"
#include "MFileSpec.h"
#include "MFolderIterator.h"
#include "MNewHandle.h"
#include "MNewRegion.h"
#include "MOpenPicture.h"

#include "ESortedFileList.h"
#include "ESpinCursor.h"
#include "EUtil.h"

#include <UDebugging.h>

const double kRad2Degrees = 57.2958;
const PaneIDT pane_Debug1 = 'dbg1';
const PaneIDT pane_Debug2 = 'dbg2';
const ResIDT	alrt_DragFailure = 132;
const ResIDT	PPob_Badge = 3000;


static OSType	gLayoutInfo[][2] = {
	Layout::kGrid, 0,
	0, 0,
	Layout::kFixed, 1,
	Layout::kFixed, 2,
	Layout::kFixed, 4,
	Layout::kFixed, 6,
	Layout::kFixed, 9,
	Layout::kFixed, 16,
	Layout::kFixed, 25,
	Layout::kFixed, 36,
	0, 0,
	Layout::kSchool, 3,
	Layout::kSchool, 5,
	Layout::kSchool, 13,
	0, 0,
	0, 0
};


//-----------------------------------------------
// PhotoPrintView default constructor
//-----------------------------------------------
PhotoPrintView::PhotoPrintView()
	: LView ()
	, CDragAndDrop ( GetMacWindow(), this)
	, mLayout(nil)
	, mCurPage (1)
{
}

//-----------------------------------------------
// PhotoPrintView copy constructor
//-----------------------------------------------
PhotoPrintView::PhotoPrintView(	const PhotoPrintView		&inOriginal)
	: LView(inOriginal)
	, CDragAndDrop (GetMacWindow(), this)
	, mLayout(nil)
	, mCurPage (1)
{
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
{
}

//-----------------------------------------------
// PhotoPrintView Stream constructor
//-----------------------------------------------
PhotoPrintView::PhotoPrintView(	LStream			*inStream)
	: LView (inStream)
	, CDragAndDrop (GetMacWindow(), this)
	, mLayout(nil)
	, mCurPage (1)
{
	mModel = new PhotoPrintModel(this); 
	SetController(PhotoPrintApp::gCurTool);
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

//--------------------------------------
// Activate
//--------------------------------------
void
PhotoPrintView::Activate() {
	SetController(PhotoPrintApp::gCurTool);
	LView::Activate();
}

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
		::AddDragItemFlavor(inDragRef, 1, 'PICT', nil, 0L, 0);

		if (mSelection.size() == 1) {
			// Add translucent drag
			SInt32		response;
			::Gestalt(gestaltDragMgrAttr, &response);
			if (response & (1 << gestaltDragMgrHasImageSupport)) {
				try {
					PhotoItemRef	image(this->GetPrimarySelection());
					MRect		bounds(image->GetDestRect());

					Point		globalPt, localPt;
					globalPt = localPt = bounds.TopLeft();
					::LocalToGlobal(&globalPt);
					::SubPt(localPt, &globalPt);

					HORef<EGWorld>	proxy = image->GetProxy();
					PixMapHandle	imagePixMap = ::GetGWorldPixMap(proxy->GetMacGWorld());
					::SetDragImage(inDragRef, imagePixMap, nil, globalPt, kDragStandardTranslucency);
				} catch (...) {
					// Translucency is not that important, so we ignore exceptions
				}
			}
		}
	}
} // AddFlavors

//--------------------------------------
// AddToSelection
//	This version takes a list
//--------------------------------------
void
PhotoPrintView::AddToSelection(PhotoItemList& additions)
{
	for (PhotoIterator i = additions.begin(); i != additions.end(); ++i) {
		mSelection.push_back(*i);
		this->RefreshItem(*i, kImageAndHandles);
	}//end for all
}//end AddToSelection

// ---------------------------------------------------------------------------
//	¥ AdjustCursorSelf
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
PhotoPrintView::AdjustTransforms(double& rot, double& /*skew*/, MRect& dest, const PhotoItemRef item)
{
	bool changesMade (false);

	//check to see if there is rotation, if it is different, and if changing rotation is allowed
	if (!PhotoUtility::DoubleEqual(rot, item->GetRotation())) {
		if (!item->GetProperties().GetRotate()) {
			rot = item->GetRotation();
			changesMade = true;
			}//endif item not allowed to rotate (any more)
		}//endif rotation different
					
	// clamp to the window
	MRect copyDest (dest);
	MRect ourBounds;
	CalcLocalFrameRect(ourBounds);
	dest *=	ourBounds;
	if (copyDest != dest)
		changesMade = true;
					

	// see if the item has a max bounds, and if the new set of transforms would extend past 
	if ((!PhotoUtility::DoubleEqual(rot, 0.0)) &&	item->GetMaxBounds()) {
		MatrixRecord m;
		SetIdentityMatrix(&m);
		::RotateMatrix (&m, ::Long2Fix(rot), ::Long2Fix(dest.MidPoint().h), ::Long2Fix(dest.MidPoint().v));
		MRect newDest;
		AlignmentGizmo::FitTransformedRectInside(dest, &m, item->GetMaxBounds(), newDest);
		AlignmentGizmo::MoveMidpointTo(newDest, item->GetMaxBounds(), newDest);
		
		if (newDest != dest) {
			changesMade = true;
			dest = newDest;
			}//endif had to change the rect because of the rotation
		}//endif there is some rotation and constraints
					
	return changesMade;
}//end AdjustTransforms

//--------------------------------------
// AddToSelection
//	This version takes one item
//--------------------------------------
void
PhotoPrintView::AddToSelection(PhotoItemRef inAddition)
{
	mSelection.push_back(inAddition);
	this->RefreshItem(inAddition, kImageAndHandles);
}//end AddToSelection

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

	mController->HandleClick(inMouseDown, rFrame, GetClickCount());
}//end ClickSelf

//--------------------------------------------
//	CreateBadges
//--------------------------------------------
void
PhotoPrintView::CreateBadges() {
	mBadgeGroup = new BadgeGroup(mModel->GetDocument());
	PhotoIterator i (mModel->begin());
	while (i != mModel->end()) {
		if (!(*i)->IsEmpty()) {
			PhotoBadge* newBadge (dynamic_cast<PhotoBadge*>(UReanimator::CreateView(PPob_Badge, this, mBadgeGroup)));
			newBadge->SetItem(*i);

			MRect imageLoc ((*i)->GetImageRect());
			newBadge->PlaceInSuperFrameAt(imageLoc.left, imageLoc.top, Refresh_Yes);

			mBadgeMap[*i] = newBadge;
		}
		++i;
		}//end while still items to make

	this->DeclareActiveBadge();
	LCommander::SwitchTarget(mBadgeGroup);
}//end CreateBadges


//--------------------------------------------
//	DeclareActiveBadge
//--------------------------------------------
void
PhotoPrintView::DeclareActiveBadge(void) {
	if (mModel->GetCount() > 0 && mBadgeMap.size() > 0) {
		mBadgeGroup->SetLatentSub(mBadgeMap[*(mModel->begin())]->GetNameTag());
	}
}//end DeclareActiveBadge


//--------------------------------------------
//	DestroyBadges
//--------------------------------------------
void
PhotoPrintView::DestroyBadges() {
	for (BadgeMap::iterator i = mBadgeMap.begin(); i != mBadgeMap.end(); ) {
		PhotoItemRef key = (*i).first;
		PhotoBadge* pDoomed = (*i++).second;
		MRect doomedBounds;
		pDoomed->CalcPortFrameRect(doomedBounds);
		this->InvalPortRect(&doomedBounds);
		mBadgeMap.erase(key);
		delete (pDoomed);
		}//for
	mBadgeGroup = nil;
}//end DestroyBadges


/*
DoDragReceive {OVERRIDE}
*/
void
PhotoPrintView::DoDragReceive(
	DragReference	inDragRef)
{
	FileRefVector	itemList;
	MAEList				theList;

	MDragItemIterator	end (inDragRef);
	for (MDragItemIterator i = end; ++i != end; ) {
		if (i.HasFlavor(kDragFlavor)) {
			mFlavorAccepted = kDragFlavor;
		} else {
			FSSpec 		spec;
			Boolean		ioAllowEvilPromise (false);
			PromiseHFSFlavor	promise;
			if (!i.ExtractFSSpec (spec, ioAllowEvilPromise, promise)) continue;
			
			// add spec to list, and to map as key to itemRef
			itemList.insert (itemList.end(), new MFileSpec(spec));
			theList.PutPtr (typeFSS, &spec, sizeof (spec));
		}
	} // for

	LView::OutOfFocus(nil);
	this->FocusDropArea();

	if (itemList.empty()) {
		LDropArea::DoDragReceive(inDragRef);		// Call inherited to process each item
		// ReceiveDragItem will be called for each item.
		// We then fall through and post an Apple Event, which will 
	}

	// Turn this into an Apple Event so it happens after the drag, not during.
	// In order to get it to post, rather than send, the event, we make an address
	// based on our signature, NOT the default address which is based on kCurrentProcess.
	MAEAddressDesc	realAddress (MFileSpec::sDefaultCreator);
	MAppleEvent		openEvent(kAEPhotoPrintSuite, kAEImport, realAddress);
	StAEDescriptor	modelSpec;
	mModel->GetDocument()->MakeSpecifier(modelSpec);
	openEvent.PutParamDesc(modelSpec, keyDirectObject);
	openEvent.PutParamDesc(theList, keyAEData);

	openEvent.Send();
	// Will be handled by ReceiveDragEvent
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
//	¥ DragIsAcceptable												  [public]
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
	PrintProperties& props (GetModel()->GetDocument()->GetPrintProperties());

	MRect bounds;	
	PhotoPrinter::CalculateHeaderRect(GetModel()->GetDocument()->GetPrintRec(), 
										&props, bounds, GetModel()->GetDocument()->GetResolution());
	bounds.Offset(0, yOffset);

	::TextFont(GetModel()->GetDocument()->GetProperties().GetFontNumber());
	SInt16 unscaledFontSize (GetModel()->GetDocument()->GetProperties().GetFontSize());
	::TextSize(unscaledFontSize * ((double)GetModel()->GetDocument()->GetResolution() / 72.0));

	MPString header (GetModel()->GetDocument()->GetProperties().GetHeader());	
	UTextDrawing::DrawWithJustification(header.Chars(), ::StrLength(header), bounds, teJustCenter, true);	
}//end DrawHeader


void
PhotoPrintView::DrawFooter(SInt32 yOffset)
{
	PrintProperties& props (GetModel()->GetDocument()->GetPrintProperties());

	MRect bounds;	
	PhotoPrinter::CalculateFooterRect(GetModel()->GetDocument()->GetPrintRec(), 
										&props, bounds, GetModel()->GetDocument()->GetResolution());
	bounds.Offset(0, yOffset);

	::TextFont(GetModel()->GetDocument()->GetProperties().GetFontNumber());
	SInt16 unscaledFontSize (GetModel()->GetDocument()->GetProperties().GetFontSize());
	::TextSize(unscaledFontSize * ((double)GetModel()->GetDocument()->GetResolution() / 72.0));

	MPString footer (GetModel()->GetDocument()->GetProperties().GetFooter());	
	UTextDrawing::DrawWithJustification(footer.Chars(), ::StrLength(footer), bounds, teJustCenter, true);	
}//end DrawFooter



static	RGBColor	gFiftyPercentGray  = {32767, 32767, 32767};


void
PhotoPrintView::DrawPrintable(SInt32 yOffset) {
	PrintProperties&	props (GetModel()->GetDocument()->GetPrintProperties());
	MRect				printable;	
	PhotoPrinter::CalculatePrintableRect(GetModel()->GetDocument()->GetPrintRec(), 
										&props, printable, GetModel()->GetDocument()->GetResolution());
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

#pragma mark -

//-----------------------------------------------
// DrawSelf  if there is a selection, then select it
//-----------------------------------------------
void
PhotoPrintView::DrawSelf() {
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

	// you'd think this should be scrollPos, but it's actually imagePos.  and it's negative. 
	SPoint32		imagePos;
	this->GetImageLocation(imagePos);

	// use imagePos.h to determine mCurPage
	mCurPage = -imagePos.v / mModel->GetDocument()->GetPageHeight();
	++mCurPage;			//(pages start at 1, not 0)
	{
		StClipRgnState		workAroundPowerPlant211a5Bug;
		StPortOriginState	needThisToo(curPort);
		// On the other hand, this calls SetDescriptor, which it may not have toÉ
		mModel->GetDocument()->UpdatePageNumber(mModel->GetDocument()->GetPageCount());
	}

//	MNewRegion		clip (GetLocalUpdateRgn()); 

	this->DrawPrintable();							// Draw rectangle around printable area
	this->DrawHeader();
	this->DrawFooter();

	// Draw page dividing lines if necessary
	if (mModel->GetDocument()->GetPageCount() > 1 && !mModel->GetDrawingProperties().GetPrinting()) {
		StColorPenState		savePen;
		StColorPenState::SetGrayPattern();
		SInt16				p = mModel->GetDocument()->GetPageCount();
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

	if (mModel) {
		StPortOriginState	saveState (curPort);
		MRestoreValue<PhotoDrawingProperties> saveProps (GetModel()->GetDrawingProperties());
		GetModel()->GetDrawingProperties().SetScreenRes(GetModel()->GetDocument()->GetResolution());
		
		// create the xlation matrix to move from paper's origin (image basis) to screen origin (0,0)
		MatrixRecord		paperToScreen;
		this->GetBodyToScreenMatrix(paperToScreen);

		mModel->Draw(&paperToScreen,
					(CGrafPtr)curPort,
					curDevice,
					NULL /*clip*/);
	}//endif something to draw

	// Draw the selection gadgets (if we have a selection)
	if (mController && mModel && !this->Selection().empty())
		mController->Select(this->Selection());
} // DrawSelf


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
	}//end GetBadgeForItem


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
	PhotoPrinter::CalculateHeaderRect(GetModel()->GetDocument()->GetPrintRec(), 
										&(GetModel()->GetDocument()->GetPrintProperties()),
										headerRect, GetModel()->GetDocument()->GetResolution());
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

	if (inType == 'PICT') {
		MRect		combinedBounds;
		for (i = mSelection.begin(); i != mSelection.end(); ++i) {
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
				HORef<EGWorld>	proxy = (*i)->GetProxy();
				MRect		bounds((*i)->GetDestRect());
				proxy->CopyImage(UQDGlobals::GetCurrentPort(), bounds, srcCopy, nil);
			}
			return reinterpret_cast<Handle>(pict.Detach());
		}
	} else if (inType == kDragFlavor) {
		XMLHandleStream		stream;	
		XML::Output			out(stream);
		// write objects
		out.BeginElement("Objects");
		for (i = mSelection.begin(); i != mSelection.end(); ++i) {
			out.BeginElement("photo");
			(*i)->Write(out);
			out.EndElement();
		}
		out.EndElement();	// Objects

		return stream.DetachDataHandle();
	}

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
	return find(mSelection.begin(), mSelection.end(), inItem) != mSelection.end();
} // IsSelected

/*
ListenToMessage {OVERRIDE}
	We'll be getting messages from the bevel buttons
*/
void
PhotoPrintView::ListenToMessage(
	MessageT	inMessage,
	void		*ioParam)
{
	SInt32			theValue;
	PhotoPrintDoc*	theDoc = mModel->GetDocument();

	switch (inMessage) {
		case 'dupl':
			theValue = *(SInt32*)ioParam;
			this->SwitchLayout(theDoc->GetLayout(), theValue);
			break;

		case 'layo':
			theValue = *(SInt32*)ioParam;
			this->SwitchLayout(theValue, theDoc->GetDuplicated());
			break;
	} // end switch
} // ListenToMessage

/*
ObjectsHandler
	This function handles the "Objects" tag in our XML file, which represents a collection
	of images
*/
void
PhotoPrintView::ObjectsHandler(XML::Element &elem, void* userData) {
	
	XML::Handler handlers[] = {
		XML::Handler("photo", PhotoPrintView::PhotoHandler),
		XML::Handler::END
		};
		
	elem.Process(handlers, userData);
} // ObjectsHandler

/*
PhotoHandler
	This function handles the "photo" tag in our XML file, which represents a single image
*/
void
PhotoPrintView::PhotoHandler(XML::Element &elem, void* userData) {
	PhotoPrintView*		view = static_cast<PhotoPrintView*>(userData);

	PhotoPrintItem*		item = new PhotoPrintItem();
	item->Read(elem);
	view->SetupDraggedItem(item);
	view->GetLayout()->AddItem(item);		// It will be adopted
} // PhotoHandler

/*
ProcessFileList
*/
void
PhotoPrintView::ProcessFileList(FileRefVector& files)
{
	ESpinCursor		spinCursor(kFirstSpinCursor, kNumCursors);
	for (FileRefVector::iterator i (files.begin()); i != files.end(); ++i) {
		if ((*i)->IsFolder ()) { // we could ask the MFileSpec, but the iterator already has the info constructed
			ReceiveDraggedFolder(*(*i));
		}//endif we found a folder
		else
			ReceiveDraggedFile(*(*i));			
		spinCursor.Spin();
		if (::CheckEventQueueForUserCancel())
			break;
	}//for
}//end ProcessFileList


//-----------------------------------------------
// ReceiveDragEvent
//	Passed on by a PaletteButton
//-----------------------------------------------
void
PhotoPrintView::ReceiveDragEvent(const MAppleEvent&	inAppleEvent)
{
	DescType	theType;
	Size		theSize;

//	OSType		tmplType;
//	inAppleEvent.GetParamPtr(theType, theSize, &tmplType, sizeof(tmplType), typeType, keyAERequestedType);

	MAEList		docList;							// List of dragged filesystem items
	inAppleEvent.GetParamDesc(docList, typeAEList, keyAEData);
	SInt32		numDocs = docList.GetCount();

	// Loop through all items in the list
		// Extract descriptor for the document
		// Coerce descriptor data into a FSSpec
		// put the FSSpec into a vector (for later sorting)
	std::vector<FileRef>	items;
	FullFileList	sortedList;		// ??? unused
	for (SInt32 i = 1; i <= numDocs; i++) {
		AEKeyword	theKey;
		FSSpec		theFileSpec;
		docList.GetNthPtr(theSize, theKey, theType, i, typeFSS, (Ptr)&theFileSpec, sizeof(FSSpec));

		FileRef			theSpec (new MFileSpec(theFileSpec));
		Boolean			targetIsFolder, wasAliased;
		
		try {
			StDisableDebugThrow_();
			theSpec->ResolveAlias(targetIsFolder, wasAliased);
			items.insert(items.end(), theSpec);
		}//end try
		catch(...) {
			StDesktopDeactivator	deactivator;
			::ParamText(theSpec->Name(), nil, nil, nil);
			::Alert(alrt_DragFailure, nil);			
		}//catch
	}//end for

	this->ClearSelection();							// Deselect, so we can select new ones
	if (numDocs > 0)
		this->ProcessFileList(items);
	
	// Now that we have all the files imported, we can do layout
	if (mLayout->GetDistinctImages() > 1)
		mModel->Sort();
	this->Refresh();								// Doc orientation may change, so refresh before AND after
	mLayout->LayoutImages();
	this->Refresh();
	LCommander::SetUpdateCommandStatus(true);		// Menu may change due to drag
} // ReceiveDragEvent

//-----------------------------------------------
// ReceiveDraggedFile
//-----------------------------------------------
void
PhotoPrintView::ReceiveDraggedFile(const MFileSpec& inFile) 
{
	try { //PhotoItem will throw if it can't find a QTImporter
		StDisableDebugThrow_();
		StDisableDebugSignal_();
		PhotoItemRef newItem = new PhotoPrintItem(inFile);
		this->SetupDraggedItem(newItem);
		mLayout->AddItem(newItem);
		// Hey!  newItem is likely destroyed by now, so don't do anything with it
		// all ownership is given over to the layout at this point.
		newItem = nil;
	}//end try
	catch (...) {
		//silently fail. !!! should put up an alert or log
	}//catch
}//end ReceiveDraggedFile

//-----------------------------------------------
// ReceiveDraggedFolder
//-----------------------------------------------
void
PhotoPrintView::ReceiveDraggedFolder(const MFileSpec& inFolder)
{
	FullFileList	sortedList;
	MFolderIterator end (inFolder.Volume(), inFolder.GetDirID());
	FileRefVector	itemsInFolder;
	
	//iterate through the folder, adding each item to a vector
	for (MFolderIterator fi (end); ++fi != end;) {
		MFileSpec* fileOrFolder = new MFileSpec (fi.Name(), fi.Directory(), fi.Volume());
		itemsInFolder.insert(itemsInFolder.end(), fileOrFolder);
		if (::CheckEventQueueForUserCancel())
			break;
		}//end all items in that folder

	this->ProcessFileList(itemsInFolder);
}//end ReceiveDraggedFolder					  

// ============================================================================
//		¥ ReceiveDragItem {OVERRIDE}
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
#pragma unused(inCopyData, inFromFinder, inItemBounds)

	Handle				h = ::NewHandle(inDataSize);
	ThrowIfNil_(h);
	::HLock(h);
	ThrowIfOSErr_(::GetFlavorData(inDragRef, inItemRef, kDragFlavor, *h, &inDataSize, 0));

	XMLHandleStream		stream(h);		// LHandleStream assumes ownership of the handle
	XML::Input			in(stream);

	try { //PhotoItem will throw if it can't find a QTImporter
		StDisableDebugThrow_();
		StDisableDebugSignal_();

		XML::Handler handlers[] = {
			XML::Handler("Objects", ObjectsHandler),
			XML::Handler::END
		};

		in.Process(handlers, static_cast<void*>(this));
	}//end try
	catch (...) {
		//silently fail. !!! should put up an alert or log
	}//catch
} // ReceiveDragItem


/*
Refresh
*/
void
PhotoPrintView::Refresh() {
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
	MRect		bounds(inItem->GetDestRect());

	// ??? really cheesy way to do this
	if (inHandles == kImageAndHandles)
		bounds.Inset(-PhotoController::kHandleSize, -PhotoController::kHandleSize);

	MatrixRecord		mat;
	::SetIdentityMatrix(&mat);
	::RotateMatrix(&mat, ::Long2Fix(inItem->GetRotation()), ::Long2Fix(bounds.MidPoint().h),
		::Long2Fix(bounds.MidPoint().v));

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
	PhotoIterator 	inBegin,
	PhotoIterator 	inEnd) 
	
	{
	
	PhotoItemRef	oldPrimary (this->GetPrimarySelection());
	for (PhotoIterator i = inBegin; i != inEnd;) {
		//	Increment the iterator to make sure it is valid after the remove
		PhotoItemRef	item = *i++;
		
		mSelection.remove (item);
		this->RefreshItem (item, kImageAndHandles);
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
PhotoPrintView::SetController(OSType newController) {
	switch (newController) {
		default:
			SignalString_("Invalid controller");
			//	Fall through to prevent crashÉ
			
		case tool_Arrow:
			mController = new ArrowController(this);
			break;

		case tool_Crop:
			mController = new CropController(this);
			break;

		case tool_Rotate:
			mController = new RotateController(this);
			break;

		case tool_Zoom:
			mController = new CropZoomController(this);
			break;
	
		case tool_Name:
			mController = new NameController(this);
			this->CreateBadges();
			break;
	}//end switch

	if (newController != tool_Name)
		this->DestroyBadges();
}//end SetController


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

	MRect itemBounds = item->GetNaturalBounds();
	SInt32	objectWidth (itemBounds.Width());
	SInt32	objectHeight (itemBounds.Height());
	
	SDimension32	imageSize;
	GetImageSize(imageSize);
	SPoint32		imageLocation;
	GetImageLocation (imageLocation);
	SInt32	boundingWidth (imageSize.width);
	SInt32	boundingHeight (imageSize.height);
		
	SInt32	outWidth;
	SInt32	outHeight;
	EUtil::BestFit(outWidth, 
					 outHeight,
					 boundingWidth,
					 boundingHeight,
					 objectWidth,
					 objectHeight,
					 false);

	itemBounds.SetWidth(outWidth);
	itemBounds.SetHeight(outHeight);	

	itemBounds.Offset((imageLocation.h + ((imageSize.width - itemBounds.Width()) / 2) -
						itemBounds.left),
						(imageLocation.v + ((imageSize.height - itemBounds.Height()) / 2) -
						itemBounds.top));
	item->SetDest(itemBounds);
}//end SetupDraggedItem


/*
SetLayoutType
*/
void
PhotoPrintView::SetLayoutType(const OSType inType)
{
	Layout*		oldLayout = mLayout;
	switch (inType) {
		case Layout::kGrid:
			mLayout = new GridLayout(mModel);
			break;

		case Layout::kSingle:
			mLayout = new SingleLayout(mModel);
			break;

		case Layout::kFixed:
			mLayout = new FixedLayout(mModel);
			break;

		case Layout::kMultiple:
			mLayout = new MultipleLayout(mModel);
			break;

		case Layout::kSchool:
			mLayout = new SchoolLayout(mModel);
			break;

		case Layout::kCollage:
			mLayout = new CollageLayout(mModel);
			break;
		
		default:
			ThrowOSErr_(paramErr);
			break;
	}

	// Now that we've safely replaced it, get rid of the old one
	delete oldLayout;

	// Make placeholders as needed
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

/*
SwitchLayout
*/
void
PhotoPrintView::SwitchLayout(const SInt32 inType, const SInt32 inDuplicated)
{
	this->Refresh();					// Doc orientation may change, so refresh before AND after

	// Get a copy of the first item in case we need it for populating
	PhotoItemRef	theItem = nil;
	if (!mModel->IsEmpty())
		theItem = new PhotoPrintItem(**mModel->begin());

	// Figure out what to switch to
	OSType			theType = gLayoutInfo[inType - 1][0];
	OSType			theCount = gLayoutInfo[inType - 1][1];
	if (theType == Layout::kFixed && inDuplicated == 2)
		theType = Layout::kMultiple;

	this->SetLayoutType(theType);

	// Update the UI if needed
	PhotoPrintDoc*	theDoc = mModel->GetDocument();
	switch (theType) {
		case Layout::kGrid:
			theDoc->JamDuplicated(1);	// Avoid getting a message for this!
			break;
		case Layout::kSchool:
			theDoc->JamDuplicated(2);	// Avoid getting a message for this!
			break;
	}
	if (theType == Layout::kGrid) {
		theDoc->GetMinMaxGroup()->Show();
		theDoc->GetDuplicatedPopup()->Hide();
	} else {
		theDoc->GetMinMaxGroup()->Hide();
		theDoc->GetDuplicatedPopup()->Show();
	}

	// Repopulate the new layout
	if (theItem != nil && (theType == Layout::kMultiple || theType == Layout::kSchool))
		mLayout->AddItem(theItem);

	mLayout->SetImageCount(theCount);
	mLayout->LayoutImages();			// Be sure any new images show up in the right place

	this->Refresh();
} // SwitchLayout

// ---------------------------------------------------------------------------
// ToggleSelected
// ---------------------------------------------------------------------------
void					
PhotoPrintView::ToggleSelected(PhotoItemList& togglees) {
	PhotoItemRef	oldPrimary(this->GetPrimarySelection());
	for (PhotoIterator i = togglees.begin(); i != togglees.end(); ++i) {
		if (this->IsSelected(*i))
			mSelection.remove(*i);
		else
			mSelection.insert(mSelection.end(), *i);
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
	for (BadgeMap::iterator i = mBadgeMap.begin(); i != mBadgeMap.end(); ++i) {
		PhotoItemRef item = (*i).first;
		PhotoBadge* badge = (*i).second;
		MRect imageLoc (item->GetImageRect());
		badge->PlaceInSuperFrameAt(imageLoc.left, imageLoc.top, Refresh_Yes);
	}//for
}//end UpdateBadges
