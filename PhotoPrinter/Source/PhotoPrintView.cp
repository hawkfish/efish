/*
	File:		PhotoPrintView.cp

	Contains:	MVC view, which displays stuff

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

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
#include "CollageLayout.h"
#include "ESpinCursor.h"
#include "EUtil.h"
#include "GridLayout.h"
#include "PhotoPrinter.h"
#include "PhotoPrintModel.h"
#include "MAEList.h"
#include "MAppleEvent.h"
#include "MFileSpec.h"
#include "MFolderIterator.h"
#include "MNewRegion.h"
#include "PhotoUtility.h"
#include "PhotoPrintPrefs.h"
#include "ESortedFileList.h"
#include "SchoolLayout.h"
#include "SingleLayout.h"
#include <UDebugging.h>

const double kRad2Degrees = 57.2958;
const PaneIDT pane_Debug1 = 'dbg1';
const PaneIDT pane_Debug2 = 'dbg2';
const ResIDT	alrt_DragFailure = 132;

//-----------------------------------------------
// PhotoPrintView default constructor
//-----------------------------------------------
PhotoPrintView::PhotoPrintView()
	: LView ()
	, CDragAndDrop ( GetMacWindow(), this)
	, mLayout(nil)
{
}

//-----------------------------------------------
// PhotoPrintView copy constructor
//-----------------------------------------------
PhotoPrintView::PhotoPrintView(	const PhotoPrintView		&inOriginal)
	: LView(inOriginal)
	, CDragAndDrop (GetMacWindow(), this)
	, mLayout(nil)
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
{
}

//-----------------------------------------------
// PhotoPrintView Stream constructor
//-----------------------------------------------
PhotoPrintView::PhotoPrintView(	LStream			*inStream)
	: LView (inStream)
	, CDragAndDrop (GetMacWindow(), this)
	, mLayout(nil)
{
	mController = new PhotoPrintController(this);
	mModel = new PhotoPrintModel(this); 
	mController->SetModel(mModel);
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
// AddToSelection
//--------------------------------------
void
PhotoPrintView::AddToSelection(PhotoItemList& additions) {
	for (PhotoIterator i = additions.begin(); i != additions.end(); ++i) {
		mSelection.insert(mSelection.end(), *i);
		}//end for all
	}//end AddToSelection


/*
DoDragReceive {OVERRIDE}
*/
void
PhotoPrintView::DoDragReceive(
	DragReference	inDragRef)
{
	FileRefVector	itemList;
	FullFileList	sortedList;
{
	DragAttributes	dragAttrs;
	::GetDragAttributes(inDragRef, &dragAttrs);

	UInt16	itemCount;				// Number of Items in Drag
	::CountDragItems(inDragRef, &itemCount);

	for (UInt16 item = 1; item <= itemCount; item++) {
		ItemReference	itemRef;
		::GetDragItemReferenceNumber(inDragRef, item, &itemRef);

		FlavorFlags	theFlags;		// We actually only use the flags to see if a flavor exists
		Size		theDataSize;
		if (::GetFlavorFlags(inDragRef, itemRef, mFlavorAccepted, &theFlags) == noErr) {
		  ::GetFlavorDataSize(inDragRef, itemRef, mFlavorAccepted, &theDataSize);
			MFileSpec spec;
			ExtractFSSpecFromDragItem(inDragRef, itemRef, theDataSize, mFlavorAccepted, spec);
			// add spec to list, and to map as key to itemRef
			MFileSpec* newSpec = new MFileSpec(spec);
			itemList.insert(itemList.end(), newSpec);
			}//endif it has the data we need
	}//for all items

	LView::OutOfFocus(nil);
	this->FocusDropArea();

}// end code heavily based on LDragAndDrop's DoDragReceive

	this->ProcessFileList(itemList);

	// Now that we have all the files imported, we can do layout
	mModel->Sort();
	mLayout->LayoutImages();
	this->Refresh();
	LCommander::SetUpdateCommandStatus(true);		// Menu may change due to drag
} // DoDragReceive

/*
ExtractFSSpecFromDragItem
*/
void
PhotoPrintView::ExtractFSSpecFromDragItem(DragReference inDragRef, 
										ItemReference inItemRef,
									  	Size inDataSize,
							  			const FlavorType expectedType,
							  			MFileSpec& outFileSpec)
{
	Size			dataSize (inDataSize);

	switch (expectedType) {
		case kDragFlavorTypePromiseHFS: {
			PromiseHFSFlavor	promise;
			ThrowIfOSErr_(::GetFlavorData (inDragRef, inItemRef, flavorTypePromiseHFS, &promise, &dataSize, 0));
			if (dataSize <= 0) break;	// sanity!
			
			dataSize = sizeof (FSSpec);
			ThrowIfOSErr_(::GetFlavorData (inDragRef, inItemRef, promise.promisedFlavor, &outFileSpec, &dataSize, 0));
			if (dataSize <= 0) break;	// sanity!
			}//case
			break;
		case kDragFlavorTypeHFS: {
			HFSFlavor		data;
			ThrowIfOSErr_(::GetFlavorData (inDragRef, inItemRef, mFlavorAccepted, &data, &dataSize, 0));
			if (dataSize <= 0) break;	// sanity!
			outFileSpec = data.fileSpec;
			}//case
			break;
	}//end switch
}//end ExtractFSSpecFromDragItem



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


bool					
PhotoPrintView::IsAnythingSelected() const
{
	return mSelection.size() > 0;
}//end IsAnythingSelected



//-----------------------------------------------
// ItemIsAcceptable
//-----------------------------------------------
Boolean	
PhotoPrintView::ItemIsAcceptable( DragReference inDragRef, ItemReference inItemRef)
{
	return mLayout->ItemIsAcceptable(inDragRef, inItemRef, mFlavorAccepted);
} // ItemIsAcceptable

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

	OSType		tmplType;
	inAppleEvent.GetParamPtr(theType, theSize, &tmplType, sizeof(tmplType), typeType, keyAERequestedType);

	MAEList		docList;
	inAppleEvent.GetParamDesc(docList, typeAEList, keyDirectObject);
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

	this->ProcessFileList(items);
	
	// Now that we have all the files imported, we can do layout
	mModel->Sort();
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
		this->RefreshItem(newItem);
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
		}//end all items in that folder

	this->ProcessFileList(itemsInFolder);
}//end ReceiveDraggedFolder					  



//--------------------------------------
// RemoveFromSelection
//--------------------------------------
void
PhotoPrintView::RemoveFromSelection(PhotoItemList& deletions) {
	for (ReversePhotoIterator i = deletions.rbegin(); i != deletions.rend(); ++i) {
		mSelection.remove(*i);
		}//end for all
	}//end RemoveFromSelection



//---------------------------------
// Select
//---------------------------------
void	
PhotoPrintView::Select(const PhotoItemList& targets) {
	mSelection.clear();
	mSelection = targets;
	Refresh();
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
	for (PhotoIterator i = togglees.begin(); i != togglees.end(); ++i) {
		PhotoIterator where = find(mSelection.begin(), mSelection.end(), *i);
		if (where != mSelection.end())
			mSelection.remove(*i);
		else
			mSelection.insert(mSelection.end(), *i);
		}//end for
	}//end
	


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

	mController->HandleClick(inMouseDown, rFrame);
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
	
	MNewRegion		clip;
	clip = visible;

	// Draw page dividing lines if necessary
	if (mModel->GetDocument()->GetPageCount() > 1 && !mModel->GetDrawingProperties().GetPrinting()) {
		StColorPenState		savePen;
		Pattern				grey;
		UQDGlobals::GetGrayPat(&grey);
		::PenPat(&grey);
		SInt16				p = mModel->GetDocument()->GetPageCount();
		SInt16				pageHeight = imageDimensions.height / p;
		for (; p > 1; p--) {
			SInt16			y = pageHeight * (p - 1);
			::MoveTo(0, y);
			::LineTo(rFrame.right, y);
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
PhotoPrintView::RefreshItem(PhotoItemRef inItem)
{
	MRect		bounds(inItem->GetDestRect());
	this->RefreshRect(bounds);
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
	placard->SetDescriptor(LStr255(Layout::str_LayoutNames, mLayout->GetNameIndex()));
} // SetLayoutType