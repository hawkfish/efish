/*
	File:		PhotoPrintView.cp

	Contains:	MVC view, which displays stuff

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

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
#include "EUtil.h"
#include "GridLayout.h"
#include "PhotoPrinter.h"
#include "PhotoPrintModel.h"
#include "MAEList.h"
#include "MAppleEvent.h"
#include "MFileSpec.h"
#include "MFolderIterator.h"
#include "MNewRegion.h"
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
{
}

//-----------------------------------------------
// PhotoPrintView copy constructor
//-----------------------------------------------
PhotoPrintView::PhotoPrintView(	const PhotoPrintView		&inOriginal)
	: LView(inOriginal)
	, CDragAndDrop (GetMacWindow(), this)
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
{}//end dt

//-----------------------------------------------
// FinishCreateSelf.  Find all those handy panes which ought to be setup by now
//-----------------------------------------------
void
PhotoPrintView::FinishCreateSelf()
{
} // FinishCreateSelf

#pragma mark -

/*
DoDragReceive {OVERRIDE}
*/
void
PhotoPrintView::DoDragReceive(
	DragReference	inDragRef)
{
	CDragAndDrop::DoDragReceive(inDragRef);			// Call inherited

	mLayout->LayoutImages();
	this->Refresh();								// ??? Redraw everything (should depend on layout)
} // ReceiveDragItem

//-----------------------------------------------
// ItemIsAcceptable
//-----------------------------------------------
Boolean	
PhotoPrintView::ItemIsAcceptable( DragReference inDragRef, ItemReference inItemRef)
{
	return mLayout->ItemIsAcceptable(inDragRef, inItemRef, mFlavorAccepted);
} // ItemIsAcceptable

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
		// Import it
	for (SInt32 i = 1; i <= numDocs; i++) {
		AEKeyword	theKey;
		FSSpec		theFileSpec;
		docList.GetNthPtr(theSize, theKey, theType, i, typeFSS, (Ptr)&theFileSpec, sizeof(FSSpec));

		MFileSpec 		theSpec(theFileSpec);
		Boolean			targetIsFolder, wasAliased;
		
		try {
			StDisableDebugThrow_();
			theSpec.ResolveAlias(targetIsFolder, wasAliased);
			}//end try
		catch(...) {
			StDesktopDeactivator deactivator;
			::ParamText(theSpec.Name(), nil, nil, nil);
			::Alert(alrt_DragFailure, nil);			
			}//catch

		if (targetIsFolder)
			this->ReceiveDraggedFolder(theSpec);
		else
			this->ReceiveDraggedFile(theSpec);
	}
	mLayout->LayoutImages();
	this->Refresh();
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
		//silently fail.  should put up an alert or log
	}//catch
}//end ReceiveDraggedFile

//-----------------------------------------------
// ReceiveDraggedFolder
//-----------------------------------------------
void
PhotoPrintView::ReceiveDraggedFolder(const MFileSpec& inFolder)
{
	MFolderIterator end (inFolder.Volume(), inFolder.GetDirID());
	for (MFolderIterator fi (end); ++fi != end;) {
		MFileSpec fileOrFolder (fi.Name(), fi.Directory(), fi.Volume());
		if (fi.IsFolder ()) { // we could ask the MFileSpec, but the iterator already has the info constructed
			ReceiveDraggedFolder(fileOrFolder);
			}//endif we found a folder
		else
			ReceiveDraggedFile(fileOrFolder);			
		}//for

}//end ReceiveDraggedFolder

//-----------------------------------------------
// ReceiveDragItem
//-----------------------------------------------
void
PhotoPrintView::ReceiveDragItem( DragReference inDragRef, 
									ItemReference inItemRef,
								  	Size inDataSize, 
								  	Boolean /*inCopyData*/, 
								  	Boolean /*inFromFinder*/, 
								  	Rect& /*inItemBounds*/)
{
	do {
		//	Validate data
		Size			dataSize (inDataSize);
		MFileSpec		theSpec;
		
		switch (mFlavorAccepted) {
			case kDragFlavorTypePromiseHFS: {
				PromiseHFSFlavor	promise;
				ThrowIfOSErr_(::GetFlavorData (inDragRef, inItemRef, flavorTypePromiseHFS, &promise, &dataSize, 0));
				if (dataSize <= 0) break;	// sanity!
				
				dataSize = sizeof (FSSpec);
				ThrowIfOSErr_(::GetFlavorData (inDragRef, inItemRef, promise.promisedFlavor, &theSpec, &dataSize, 0));
				if (dataSize <= 0) break;	// sanity!
				}//case
				break;
			case kDragFlavorTypeHFS: {
				HFSFlavor		data;
				ThrowIfOSErr_(::GetFlavorData (inDragRef, inItemRef, mFlavorAccepted, &data, &dataSize, 0));
				if (dataSize <= 0) break;	// sanity!
				theSpec = data.fileSpec;
				}//case
				break;
			}//end switch
			
		
		Boolean			targetIsFolder, wasAliased;

		try {
			StDisableDebugThrow_();
			theSpec.ResolveAlias(targetIsFolder, wasAliased);
			}//end try
		catch (...) {
			::SysBeep(60);
			break;
			}//end

		if (targetIsFolder)
			this->ReceiveDraggedFolder(theSpec);
		else
			this->ReceiveDraggedFile(theSpec);
	} while (false);
}//end ReceiveDragItem								  

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

#pragma mark -

#include "PhotoUtility.h"
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

	//check to see if there is rotation, and if
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
	FocusDraw ();

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
	GrafPtr		curPort;
	GDHandle	curDevice;
	::GetPort(&curPort);
	curDevice = ::GetGDevice();

	{
		MRect			rFrame;
		this->CalcPortFrameRect(rFrame);
		SDimension32	imageDimensions;
		this->GetImageSize(imageDimensions);
		rFrame.SetWidth(imageDimensions.width);
		rFrame.SetHeight(imageDimensions.height);
		::EraseRect(&rFrame);
	}

	MRect		visible;
	CalcRevealedRect();
	GetRevealedRect(visible);
	MNewRegion	clip;
	clip = visible;

	if (mModel) {
		StPortOriginState saveState (curPort);
		SPoint32 superLocation;
		GetSuperView()->GetImageLocation(superLocation);
		mModel->Draw(0,
					(CGrafPtr)curPort,
					curDevice// ,
					/*clip*/);
		}//endif something to draw
		
	if (mController && mModel)
		mController->Select(mModel->GetSelection());
}//end DrawSelf

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