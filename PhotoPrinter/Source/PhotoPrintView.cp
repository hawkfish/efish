//PhotoPrintView.cp
// Copyright © 2000 Electric Fish, Inc.  All Rights Reserved

#include "PhotoPrintView.h"
#include "PhotoPrintModel.h"
#include "MFileSpec.h"
#include "MFolderIterator.h"

const double kRad2Degrees = 57.2958;
const PaneIDT pane_Debug1 = 'dbg1';
const PaneIDT pane_Debug2 = 'dbg2';


//-----------------------------------------------
// PhotoPrintView empty constructor
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
PhotoPrintView::FinishCreateSelf() {
	}//FinishCreateSelf





#pragma mark -
//-----------------------------------------------
// ItemIsAcceptable
//-----------------------------------------------
Boolean	
PhotoPrintView::ItemIsAcceptable( DragReference inDragRef, ItemReference inItemRef)
{
	FlavorFlags	theFlags;

	FlavorType outType;
	::GetFlavorType(inDragRef, inItemRef, 1, &outType);

	Boolean bHappy (false);
	if (::GetFlavorFlags(inDragRef, inItemRef, kDragFlavorTypeHFS, &theFlags) == noErr) {
		mFlavorAccepted = kDragFlavorTypeHFS;
		bHappy = true;			
		}//endif

	return (bHappy);	

}//end ItemIsAcceptable




//-----------------------------------------------
// ReceiveDraggedFolder
//-----------------------------------------------
void
PhotoPrintView::ReceiveDraggedFolder(const MFileSpec& inFolder){
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


#include <UDebugging.h>
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
		SetupDraggedItem(newItem);
		mModel->AdoptNewItem(newItem);
		}//end try
	catch (...) {
		//silently fail.  should put up an alert or log
		}//catch
}//end ReceiveDraggedFile





//-----------------------------------------------
// SetupDraggedItem
//  		some of this could be done on construction of the item
// 	but factoring it out here allows cleaner subclassing
//-----------------------------------------------
#include "PhotoPrinter.h"
void
PhotoPrintView::SetupDraggedItem(PhotoItemRef item) 
{
	item->GetProperties().SetCenter(true);
	item->GetProperties().SetMaximize(true);
	item->GetProperties().SetAspect(true);

	MRect itemBounds = item->GetNaturalBounds();
	long	propWidth (itemBounds.Width());
	long	propHeight (itemBounds.Height());
	
	SDimension32	imageSize;
	GetImageSize(imageSize);
	SPoint32		imageLocation;
	GetImageLocation (imageLocation);
	long	fitWidth (imageSize.width);
	long	fitHeight (imageSize.height);
		
	long	outWidth;
	long	outHeight;
	PhotoPrinter::BestFit(outWidth, 
						 outHeight,
						 fitWidth,
						 fitHeight,
						 propWidth,
						 propHeight);

	itemBounds.SetWidth(outWidth);
	itemBounds.SetHeight(outHeight);	

	itemBounds.Offset((imageLocation.h + ((imageSize.width - itemBounds.Width()) / 2) -
						itemBounds.left),
						(imageLocation.v + ((imageSize.height - itemBounds.Height()) / 2) -
						itemBounds.top));
	item->SetDest(itemBounds);

}//end SetupDraggedItem


//-----------------------------------------------
// ReceiveDragItem
//-----------------------------------------------
void
PhotoPrintView::ReceiveDragItem( DragReference inDragRef, 
									ItemReference inItemRef,
								  	Size /*inDataSize*/, 
								  	Boolean /*inCopyData*/, 
								  	Boolean /*inFromFinder*/, 
								  	Rect& /*inItemBounds*/)
{
	do {

		//	Validate data
		Size			dataSize;

		HFSFlavor		data;
		::GetFlavorData (inDragRef, inItemRef, mFlavorAccepted, &data, &dataSize, 0);
		
		MFileSpec 	fSpec (data.fileSpec);
		if (fSpec.IsFolder())
			ReceiveDraggedFolder(fSpec);
		else
			ReceiveDraggedFile(fSpec);

	} while (false);
	
}//end ReceiveDragItem								  





#pragma mark -
//-----------------------------------------------
// DrawSelf  if there is a selection, then select it
//-----------------------------------------------
#include "MNewRegion.h"
void
PhotoPrintView::DrawSelf() {
	GrafPtr	curPort;
	GDHandle curDevice;
	::GetPort(&curPort);
	curDevice = ::GetGDevice();

	MRect visible;
	CalcRevealedRect();
	GetRevealedRect(visible);
	MNewRegion clip;
	clip = visible;

	if (mModel)
		mModel->Draw(0,
					(CGrafPtr)curPort,
					curDevice,
					clip);
		
	if (mController && mModel)
		mController->Select(mModel->GetSelection());
}//end DrawSelf


//-----------------------------------------------
// ClickSelf.  Handle 'dat event
//-----------------------------------------------
void		
PhotoPrintView::ClickSelf(const SMouseDownEvent &inMouseDown) {
	FocusDraw ();

	MRect rFrame;
	CalcPortFrameRect(rFrame);
	SDimension32	imageDimensions;
	GetImageSize(imageDimensions);
	rFrame.SetWidth(imageDimensions.width);
	rFrame.SetHeight(imageDimensions.height);

	mController->HandleClick(inMouseDown, rFrame);
}//end ClickSelf


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

