/*
	File:		PhotoPrintView.cp

	Contains:	MVC view, which displays stuff

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

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

#include "ArrowController.h"
#include "CollageLayout.h"
#include "CropZoomController.h"
#include "GridLayout.h"
#include "PhotoPrintApp.h"
#include "PhotoPrintCommands.h"
#include "PhotoPrintConstants.h"
#include "PhotoPrinter.h"
#include "PhotoPrintModel.h"
#include "PhotoPrintPrefs.h"
#include "PhotoUtility.h"
#include "Registration.h"
#include "RotateController.h"
#include "SchoolLayout.h"
#include "SingleLayout.h"
#include "XMLHandleStream.h"

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


static void	sItemHandler(XML::Element &elem, void* userData);
static void sParseItem(XML::Element &elem, void* userData);


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
	SetController(PhotoPrintApp::gCurTool);
	mModel = new PhotoPrintModel(this); 
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


/*
DoDragReceive {OVERRIDE}
*/
void
PhotoPrintView::DoDragReceive(
	DragReference	inDragRef)
{
	FileRefVector	itemList;

	MDragItemIterator	end (inDragRef);
	for (MDragItemIterator i = end; ++i != end; ) {
		if (i.HasFlavor(kDragFlavor)) {
			mFlavorAccepted = kDragFlavor;
		} else {
			FSSpec 		spec;
			if (!i.ExtractFSSpec (spec)) continue;
			
			// add spec to list, and to map as key to itemRef
			itemList.insert (itemList.end(), new MFileSpec(spec));
		}
	} // for

	LView::OutOfFocus(nil);
	this->FocusDropArea();

	if (itemList.empty()) {
		LDropArea::DoDragReceive(inDragRef);		// Call inherited to process each item
	} else {
		this->ClearSelection();						// Deselect, so we can select new ones
		this->ProcessFileList(itemList);
	}

	// Now that we have all the files imported, we can do layout
	if (mLayout->GetDistinctImages() > 1)
		mModel->Sort();
	this->Refresh();								// Doc orientation may change, so refresh before AND after
	mLayout->LayoutImages();
	this->Refresh();
	LCommander::SetUpdateCommandStatus(true);		// Menu may change due to drag
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
				proxy->CopyImage(UQDGlobals::GetCurrentPort(), combinedBounds, srcCopy, nil);
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
ProcessFileList
*/
void
PhotoPrintView::ProcessFileList(FileRefVector& files)
{
	ESpinCursor	spinCursor(kFirstSpinCursor, kNumCursors);
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

	MAEList		docList;
	inAppleEvent.GetParamDesc(docList, typeAEList, keyAEData);
	SInt32		numDocs = docList.GetCount();

	// Loop through all items in the list
		// Extract descriptor for the document
		// Coerce descriptor data into a FSSpec
		// put the FSSpec into a vector (for later sorting)
	std::vector<FileRef>	items;
	FullFileList	sortedList;
	for (SInt32 i = 1; i <= numDocs; i++) {
		AEKeyword	theKey;
		FSSpec		theFileSpec;
		docList.GetNthPtr(theSize, theKey, theType, i, typeFSS, (Ptr)&theFileSpec, sizeof(FSSpec));

		MFileSpec*		theSpec = new MFileSpec(theFileSpec);
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
//		¥ ReceiveDragItem
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

	Handle			h = ::NewHandle(inDataSize);
	ThrowIfNil_(h);
	::HLock(h);
	ThrowIfOSErr_(::GetFlavorData(inDragRef, inItemRef, kDragFlavor, *h, &inDataSize, 0));

	XMLHandleStream		stream(h);	
	XML::Input			in(stream);

	try { //PhotoItem will throw if it can't find a QTImporter
		StDisableDebugThrow_();
		StDisableDebugSignal_();
		PhotoItemRef newItem = new PhotoPrintItem();

		XML::Handler handlers[] = {
			XML::Handler("Objects", sItemHandler),
			XML::Handler::END
		};

		in.Process(handlers, (void*)newItem);
		this->SetupDraggedItem(newItem);
		mLayout->AddItem(newItem);
		// Hey!  newItem is likely destroyed by now, so don't do anything with it
		// all ownership is given over to the layout at this point.
		newItem = nil;
	}//end try
	catch (...) {
		//silently fail. !!! should put up an alert or log
	}//catch
}



/*
sItemHandler
*/
void
sItemHandler(XML::Element &elem, void* userData) {
	
	XML::Handler handlers[] = {
		XML::Handler("photo", sParseItem),
		XML::Handler::END
		};
		
	elem.Process(handlers, userData);
}//end sDocHandler


/*
sParseItem
*/
void
sParseItem(XML::Element &elem, void* userData) {
	PhotoPrintItem*	pItem = (PhotoPrintItem*)userData;
	pItem->Read(elem);
}// sParseItem



//--------------------------------------
// RemoveFromSelection
//--------------------------------------
void
PhotoPrintView::RemoveFromSelection(PhotoItemList& deletions) {
	PhotoItemRef	oldPrimary (this->GetPrimarySelection());
	for (ReversePhotoIterator i = deletions.rbegin(); i != deletions.rend(); ++i) {
		mSelection.remove(*i);
		this->RefreshItem(*i, kImageAndHandles);
		}//end for all
	
	if (this->GetPrimarySelection() && (oldPrimary != this->GetPrimarySelection()))
		this->RefreshItem(this->GetPrimarySelection(), kImageAndHandles);
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
	}//end switch
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


// ---------------------------------------------------------------------------
// ToggleSelected
// ---------------------------------------------------------------------------
void					
PhotoPrintView::ToggleSelected(PhotoItemList& togglees) {
	PhotoItemRef	oldPrimary(this->GetPrimarySelection());
	for (PhotoIterator i = togglees.begin(); i != togglees.end(); ++i) {
		PhotoIterator where = find(mSelection.begin(), mSelection.end(), *i);
		if (where != mSelection.end())
			mSelection.remove(*i);
		else
			mSelection.insert(mSelection.end(), *i);
		this->RefreshItem(*i, kImageAndHandles);
	}//end for

	if (this->GetPrimarySelection() && (oldPrimary != this->GetPrimarySelection()))
		this->RefreshItem(this->GetPrimarySelection(), kImageAndHandles);
}//end ToggleSelected

#pragma mark -

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
					
	return changesMade;
}//end AdjustTransforms

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
	::EraseRect(&rFrame);

	MRect			visible;
	this->CalcRevealedRect();
	this->GetRevealedRect(visible);
	
	// you'd think this should be scrollPos, but it's actually imagePos.  and it's negative. 
	SPoint32		imagePos;
	this->GetImageLocation(imagePos);
	visible.Offset(visible.left - imagePos.h, visible.top - imagePos.v);
	

	// use imagePos.h to determine mCurPage
	mCurPage = -imagePos.v / mModel->GetDocument()->GetPageHeight();
	++mCurPage;			//(pages start at 1, not 0)
	mModel->GetDocument()->UpdatePageNumber(mModel->GetDocument()->GetPageCount());

	MNewRegion		clip;
	clip = visible;

	DrawHeader();
	DrawFooter();

	// Draw page dividing lines if necessary
	if (mModel->GetDocument()->GetPageCount() > 1 && !mModel->GetDrawingProperties().GetPrinting()) {
		StColorPenState		savePen;
		Pattern				grey;
		UQDGlobals::GetGrayPat(&grey);
		::PenPat(&grey);
		SInt16				p = mModel->GetDocument()->GetPageCount();
		SInt16				pageHeight = imageDimensions.height / p;
		DrawHeader();
		DrawFooter();
		for (; p > 1; p--) {
			SInt16			y = pageHeight * (p - 1);
			::MoveTo(0, y);
			::LineTo(rFrame.right, y);
			DrawHeader(y);
			DrawFooter(y);
		}
	}



	if (mModel) {
		StPortOriginState	saveState (curPort);
		mModel->Draw(0,
					(CGrafPtr)curPort,
					curDevice,
					clip);
		}//endif something to draw


	if (mController && mModel)
		mController->Select(this->Selection());
} // DrawSelf

/*
RefreshItem
	Force redraw of one item
*/
void
PhotoPrintView::RefreshItem(PhotoItemRef inItem, const bool inHandles)
{
	Assert_(inItem != nil);
	MRect		bounds(inItem->GetDestRect());

	// ??? really cheesy way to do this
	if (inHandles == kImageAndHandles)
		bounds.Inset(-PhotoController::kHandleSize, -PhotoController::kHandleSize);

	MatrixRecord		mat;
	::SetIdentityMatrix(&mat);
	::RotateMatrix(&mat, ::Long2Fix(inItem->GetRotation()), ::Long2Fix(bounds.MidPoint().h),
		::Long2Fix(bounds.MidPoint().v));

	PhotoUtility::DrawXformedRect(bounds, &mat, kInvalidate);
} // RefreshItem

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
	LStr255		theName(mLayout->GetName());
	placard->SetDescriptor(mLayout->GetName());
	
	if (!PhotoPrintApp::gIsRegistered) {	
#warning CHANGE ME TO annoy_diagonal 
		mLayout->SetAnnoyingwareNotice(true, annoy_header);
	}//endif need to slam in the annoyingware notice
	
//if (Registration::IsTimeLimited) {
//		mLayout->SetAnnoyingwareNotice(true, annoy_header);
//	}//endif


} // SetLayoutType