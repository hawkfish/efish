/*
	File:		ImageActions.cp

	Contains:	Implementation of various undoable actions on images.

	Written by:	David Dunham and Dav Lion

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		11 Jul 2001		rmgw	kDragFlavor => kXMLFlavor.
		20 jul 2001		dml		144 DeleteAction::RedoSelf calls SetImageCount to re-install placeholders iff FixedLayout
		03 jul 2001		dml		add PhotoDrawingProperties to SetMaxBounds, SetDest
		02 Jul 2001		rmgw	AdoptNewItem now takes a PhotoIterator.
		14 Jun 2001		drd		69 Finally finished PasteAction
		23 May 2001		drd		69 PasteAction
		15 Feb 2001		rmgw	10 DeleteAll => RemoveAllItems
		21 Sep 2000		drd		Exception handling class renamed to ExceptionHandler
		20 sep 2000		dml		use PhotoExceptionHandler
		15 sep 2000		dml		fix multiple-crop-zoom 
		15 Sep 2000		drd		LayoutImages, called by DeleteAction
		23 aug 2000		dml		crops are now stored as doubles (percentages)
		22 aug 2000		dml		add offset to CropAction
		21 aug 2000		dml		added a pragma mark - near rotationaction
		21 aug 2000		dml		CalcCropValuesAsRect must respect EmptyRect flag == noCrop, CropZoom ct also
		18 aug 2000		dml		rewrite cropping as relative
		17 aug 2000		dml		perform cropZoom at origin, offset afterwords (!)
		16 aug 2000		dml		tweaks on CropZoomAction
		15 aug 2000		dml		add RotateAction
		15 Aug 2000		drd		CropZoomAction
		14 Aug 2000		drd		CropAction, ImageAction
		04 Aug 2000		drd		Created
*/

#include "ImageActions.h"
#include "Layout.h"
#include "PhotoPrintConstants.h"
#include "PhotoPrintDoc.h"
#include "PhotoPrintResources.h"
#include "PhotoPrintView.h"
#include "AlignmentGizmo.h"
#include "PhotoExceptionHandler.h"
#include "XMLHandleStream.h"
#include "xmlinput.h"
#include "FixedLayout.h"

/*
PhotoPrintAction
*/
PhotoPrintAction::PhotoPrintAction(
	PhotoPrintDoc*	inDoc,
	const SInt16	inStringIndex,
	const Boolean	inAlreadyDone)
	: LAction(str_Redo, inStringIndex, inAlreadyDone)
{
	mDoc = inDoc;
	mView = inDoc->GetView();
	mModel = mView->GetModel();
} // PhotoPrintAction

/*
~PhotoPrintAction
*/
PhotoPrintAction::~PhotoPrintAction()
{
} // ~PhotoPrintAction

/*
CanRedo {OVERRIDE}
*/
Boolean
PhotoPrintAction::CanRedo() const
{
	return !this->IsDone() && mDoc->IsOnDuty();
} // CanRedo

/*
CanUndo {OVERRIDE}
*/
Boolean
PhotoPrintAction::CanUndo() const
{
	return this->IsDone() && mDoc->IsOnDuty();
} // CanUndo


/*
LayoutImages
*/
void
PhotoPrintAction::LayoutImages()
{
	mView->Refresh();								// Doc orientation may change, so refresh before AND after
	mView->GetLayout()->LayoutImages();
	mView->Refresh();
} // LayoutImages

/*
Redo {OVERRIDE}
*/
void
PhotoPrintAction::Redo()
{
	try {
		if (this->CanRedo()) {
			this->RedoSelf();

			// Mark the model as dirty
			mModel->SetDirty();
			}
		mIsDone = true;
		}//end try

	catch (LException e) {
		// Mark the model as dirty
		mModel->SetDirty();
		if (!ExceptionHandler::HandleKnownExceptions(e))
			throw;
		}//end catch
} // Redo

/*
Undo {OVERRIDE}
*/
void
PhotoPrintAction::Undo()
{
	try {
		if (this->CanUndo()) {
			this->UndoSelf();

			// Mark the model as dirty
			mModel->SetDirty();
		}

		mIsDone = false;
		}//end try

	catch (LException e) {
		// Mark the model as dirty
		mModel->SetDirty();
		if (!ExceptionHandler::HandleKnownExceptions(e))
			throw;		
		}//end catch
} // Undo

#pragma mark -

/*
CropAction
*/
CropAction::CropAction(
	PhotoPrintDoc*	inDoc,
	const SInt16	inStringIndex,
	const MRect&	inNewCrop,
	const double	inTopOffset,
	const double	inLeftOffset)
	: ImageAction(inDoc, inStringIndex)
	, mNewTopOffset (inTopOffset)
	, mNewLeftOffset (inLeftOffset)
{
	mImage->GetCrop(mOldTopCrop, mOldLeftCrop, mOldBottomCrop, mOldRightCrop);
	mImage->GetCropZoomOffset(mOldTopOffset, mOldLeftOffset);
	ERect32	newCrop32		(inNewCrop);
	ERect32 image			(mImage->GetImageRect());
	
	CalcCropValuesAsPercentages(newCrop32, image, mNewTopCrop, mNewLeftCrop, mNewBottomCrop, mNewRightCrop);	
} // CropAction


CropAction::~CropAction()
{
} // ~CropAction


/*
RedoSelf {OVERRIDE}
*/
void
CropAction::RedoSelf()
{
	mImage->SetCrop(mNewTopCrop, mNewLeftCrop, mNewBottomCrop, mNewRightCrop);
	mImage->SetCropZoomOffset(mNewTopOffset, mNewLeftOffset);
	mModel->SetDirty();		// !!! need to be more precise
} // RedoSelf


void		
CropAction::CalcCropValuesAsPercentages(const ERect32& inCrop, const ERect32& inBounds, 
										double& outTopCrop, double& outLeftCrop, 
										double& outBottomCrop, double& outRightCrop)
{	
	if (inCrop.IsEmpty()) {// if incoming crop is empty rect
		outTopCrop = outLeftCrop = outBottomCrop = outRightCrop = 0.0;
		}//endif
	else {		
		double height	(inBounds.Height() / 100.);
		double width 	(inBounds.Width() / 100.);
		
		ERect32	clampedCrop (inCrop);
		clampedCrop *= inBounds;
		
		outTopCrop = (clampedCrop.top - inBounds.top) / height;
		outLeftCrop = (clampedCrop.left - inBounds.left) / width;
		outBottomCrop = (inBounds.bottom - clampedCrop.bottom) / height;
		outRightCrop = (inBounds.right - clampedCrop.right) / width;
	}//else must calculate
}//end CalcCropValuesAsPercentages


/*
UndoSelf {OVERRIDE}
*/
void
CropAction::UndoSelf()
{
	mImage->SetCrop(mOldTopCrop, mOldLeftCrop, mOldBottomCrop, mOldRightCrop);
	mImage->SetCropZoomOffset(mOldTopOffset, mOldLeftOffset);
	mModel->SetDirty();		// !!! need to be more precise
} // UndoSelf

#pragma mark -

/*
CropZoomAction
*/
CropZoomAction::CropZoomAction(
	PhotoPrintDoc*	inDoc,
	const SInt16	inStringIndex,
	const MRect&	inNewCrop)
	: CropAction(inDoc, inStringIndex, inNewCrop)
{

	mImage->GetCropZoomScales(mOldXScale, mOldYScale);
	mImage->GetCropZoomOffset(mOldTopOffset, mOldLeftOffset);

	if (!inNewCrop) {
		mNewXScale = mNewYScale = 1.0;
		mNewTopOffset = mNewLeftOffset = 0.0;
		}//endif empty crop rect means turn cropzoom off
	else {
		MRect tempImage;
		mImage->GetExpandedOffsetImageRect(tempImage);
		ERect32 image (tempImage); // image will become too big to fit in a normal Rect

		// pretend the image is at origin for all intermediate calculations
		SInt32	offsetLeft (image.left);
		SInt32 	offsetTop	(image.top);
		image.Offset(-offsetLeft, -offsetTop);
		// which means we must adjust the crop rect also
		ERect32 newCrop (inNewCrop);
		newCrop.Offset(-offsetLeft, -offsetTop);
		// and work with an offset copy of the original bounds
		ERect32 oldBoundsAtOrigin 	(tempImage);
		oldBoundsAtOrigin.Offset(-offsetLeft, -offsetTop);

		newCrop *= image; // intersect crop rect with imageRect (controller doesn't know about caption/image separation)
		
		// derive various measurements of the crop and image rects
		SInt32	zw = newCrop.Width();
		SInt32	zh = newCrop.Height();
		SInt32	cw = image.Width();
		SInt32	ch = image.Height();
		// we get to expand by the smaller of the ratios
		double	ratio = std::min((double)cw / zw, (double)ch / zh);

		// this is the full size of the expanded image
		image.SetWidth(cw * ratio);
		image.SetHeight(ch * ratio);

		// now make just the expanded cropped rect
		// start with the full expanded rect
		ERect32 croppedExpanded (image);
		// offset over to the start of the cropped section
		croppedExpanded.Offset(newCrop.left * ratio, newCrop.top * ratio);
		// set the width and height
		croppedExpanded.SetWidth(newCrop.Width() * ratio);
		croppedExpanded.SetHeight(newCrop.Height() * ratio);

		ERect32 croppedExpandedCentered;
		//center the cropped + expanded rect inside the original bounds
		AlignmentGizmo::AlignRectInside(croppedExpanded, oldBoundsAtOrigin, kAlignAbsoluteCenter, croppedExpandedCentered);

		// offset the full expanded image so that the cropped expanded portion lines up with previous calculation
		image.Offset(-((newCrop.left * ratio) - croppedExpandedCentered.left),
					-((newCrop.top * ratio) - croppedExpandedCentered.top));

		
		// return from origin to previous location
		image.Offset(offsetLeft, offsetTop);
		
		
		// support for the new relative cropping
		mNewTopOffset = (-((newCrop.top * ratio) - croppedExpandedCentered.top)) / (double)image.Height();
		mNewLeftOffset = -((newCrop.left * ratio) - croppedExpandedCentered.left) / (double)image.Width();
		mNewXScale = ratio;
		mNewYScale = ratio;

		CalcCropValuesAsPercentages(croppedExpandedCentered, oldBoundsAtOrigin, 
									mNewTopCrop, mNewLeftCrop, mNewBottomCrop, mNewRightCrop);
		}//else actual crop, need to calculate

} // CropZoomAction

CropZoomAction::~CropZoomAction()
{
} // ~CropZoomAction

/*
RedoSelf {OVERRIDE}
*/
void
CropZoomAction::RedoSelf()
{
//	mImage->SetImageRect(mNewImage);
	mImage->SetCrop(mNewTopCrop, mNewLeftCrop, mNewBottomCrop, mNewRightCrop);
	mImage->SetCropZoomScales(mNewXScale, mNewYScale);
	mImage->SetCropZoomOffset(mNewTopOffset, mNewLeftOffset);
	mImage->DeleteProxy();
	mModel->SetDirty();		// !!! need to be more precise
} // RedoSelf

/*
UndoSelf {OVERRIDE}
*/
void
CropZoomAction::UndoSelf()
{
	mImage->SetCrop(mOldTopCrop, mOldLeftCrop, mOldBottomCrop, mOldRightCrop);
	mImage->SetCropZoomScales(mOldXScale, mOldYScale);
	mImage->SetCropZoomOffset(mOldTopOffset, mOldLeftOffset);
	mImage->DeleteProxy();
	mModel->SetDirty();		// !!! need to be more precise
} // UndoSelf

#pragma mark -

/*
DeleteAction
*/
DeleteAction::DeleteAction(
	PhotoPrintDoc*	inDoc,
	const SInt16	inStringIndex)
	: MultiImageAction(inDoc, inStringIndex)
{
	mAllImages.assign(mModel->begin(), mModel->end());
} // DeleteAction

/*
~DeleteAction
*/
DeleteAction::~DeleteAction()
{
} // ~DeleteAction

/*
RedoSelf {OVERRIDE}
*/
void
DeleteAction::RedoSelf()
{	
	// if we're operating on a FixedLayout, we have to replace deleted items with empties
	FixedLayout* fixed (dynamic_cast<FixedLayout*>(mView->GetLayout()));
	bool patchFixedLayout (fixed != nil);

	// take them all away
	mView->RemoveFromSelection(mImages);

	if (patchFixedLayout) {
		HORef<PhotoPrintItem> newItem = fixed->MakeNewImage();	
		for (PhotoIterator i = mImages.begin(); i != mImages.end(); ++i) {
			**i = *newItem;
			}//for
		}//endif replacing fixed items marked for deletion with placeholders		
	else {
		mModel->RemoveItems(mImages, PhotoPrintModel::kRemove);
		}//else
		
	this->LayoutImages();

	mOwnsImages = false;
} // RedoSelf

/*
UndoSelf {OVERRIDE}
*/
void
DeleteAction::UndoSelf()
{
	PhotoIterator	i;

	// There's no API to insert stuff at a particular point in the list, so start with
	// a clean slate and add all the ones that used to be there back
	mModel->RemoveAllItems();					// Gets rid of its items, but not the image data
	for (i = mAllImages.begin(); i != mAllImages.end(); ++i) {
		mModel->AdoptNewItem(*i, mModel->end ());
	}
	mView->AddToSelection(mImages);

	this->LayoutImages();

	mOwnsImages = false;
} // UndoSelf

#pragma mark -

/*
ImageAction
*/
ImageAction::ImageAction(
	PhotoPrintDoc*	inDoc,
	const SInt16	inStringIndex)
	: PhotoPrintAction(inDoc, inStringIndex, kNotAlreadyDone)
{
	mImage = mView->GetPrimarySelection();
} // ImageAction

/*
~ImageAction
*/
ImageAction::~ImageAction()
{
} // ~ImageAction

#pragma mark -

/*
MultiImageAction
*/
MultiImageAction::MultiImageAction(
	PhotoPrintDoc*	inDoc,
	const SInt16	inStringIndex)
	: PhotoPrintAction(inDoc, inStringIndex, kNotAlreadyDone)
{
	// Copy the selection
	mImages = mView->Selection();

	mOwnsImages = false;
} // MultiImageAction

/*
~MultiImageAction
*/
MultiImageAction::~MultiImageAction()
{
	// Delete images if they have been deleted
	if (mOwnsImages) {
		PhotoIterator	i;
		for (i = mImages.begin(); i != mImages.end(); i++) {
			delete (*i);
		}
	}
} // ~MultiImageAction

#pragma mark -

/*
PasteAction
	This works for both Paste and Drop
	We assume ownership of the handle
*/
PasteAction::PasteAction(
	PhotoPrintDoc*	inDoc,
	const SInt16	inStringIndex,
	const ScrapFlavorType	inType,
	Handle			inData)
	: PhotoPrintAction(inDoc, inStringIndex, kNotAlreadyDone)
{
	if (inType == kXMLFlavor) {
		XMLHandleStream		stream(inData);		// LHandleStream assumes ownership of the handle
		XML::Input			in(stream);

		try { //PhotoItem will throw if it can't find a QTImporter
			StDisableDebugThrow_();
			StDisableDebugSignal_();

			XML::Handler handlers[] = {
				XML::Handler("Objects", ObjectsHandler),
				XML::Handler::END
			};

			in.Process(handlers, static_cast<void*>(this));
		} catch (...) {
			//silently fail. !!! should put up an alert or log
		}//catch
	} else {
		// !!! Need to do something with PICT; for now, just prevent leak
		::DisposeHandle(inData);
	}
} // PasteAction

/*
~PasteAction
*/
PasteAction::~PasteAction()
{
	// Delete images if they have not been inserted
	if (mOwnsImages) {
		PhotoIterator	i;
		for (i = mInsertedImages.begin(); i != mInsertedImages.end(); i++) {
			delete (*i);
		}
	}
} // ~PasteAction

/*
ObjectsHandler
	This function handles the "Objects" tag in our XML file, which represents a collection
	of images
*/
void
PasteAction::ObjectsHandler(XML::Element &elem, void* userData) {
	
	XML::Handler handlers[] = {
		XML::Handler("photo", PhotoHandler),
		XML::Handler::END
		};
		
	elem.Process(handlers, userData);
} // ObjectsHandler

/*
PhotoHandler
	This function handles the "photo" tag in our XML file, which represents a single image
*/
void
PasteAction::PhotoHandler(XML::Element &elem, void* userData) {
	PasteAction*		action = static_cast<PasteAction*>(userData);

	PhotoPrintItem*		item = new PhotoPrintItem();
	item->Read(elem);
	action->mView->SetupDraggedItem(item);
	action->mInsertedImages.push_back(item);
} // PhotoHandler

/*
RedoSelf {OVERRIDE}
*/
void
PasteAction::RedoSelf()
{
	PhotoIterator	i;
	for (i = mInsertedImages.begin(); i != mInsertedImages.end(); i++) {
		mView->GetLayout()->AddItem(*i, mModel->end ());		// It will be adopted
	}

	mOwnsImages = false;						// They have all been adopted

	this->LayoutImages();
	LCommander::SetUpdateCommandStatus(true);	// Menu may change due to paste
} // RedoSelf

/*
UndoSelf {OVERRIDE}
*/
void
PasteAction::UndoSelf()
{
	// take them all away
	mView->RemoveFromSelection(mInsertedImages);
	mModel->RemoveItems(mInsertedImages, PhotoPrintModel::kRemove);

	this->LayoutImages();

	mOwnsImages = true;
} // UndoSelf

#pragma mark -

RotateAction::RotateAction(PhotoPrintDoc*	inDoc,
							const SInt16	inStringIndex,
							double inRot,
							const Rect* inDest) 
	: ImageAction(inDoc, inStringIndex)
	, mOldRot(mImage->GetRotation())
	, mOldDest(mImage->GetDestRect())
	, mNewRot(inRot)
{
	if (inDest != nil)
		mNewDest = *inDest;
	else
		mNewDest = mOldDest;
	}//end ct


RotateAction::~RotateAction() {
	}//end dt


void
RotateAction::RedoSelf() {
	mImage->SetRotation(mNewRot);
	PhotoDrawingProperties	drawProps (false, false, false, mDoc->GetResolution());
	mImage->SetDest(mNewDest, drawProps);
	mModel->SetDirty();		// !!! need to be more precise
	}//end RedoSelf
	
void
RotateAction::UndoSelf() {
	mImage->SetRotation(mOldRot);
	PhotoDrawingProperties	drawProps (false, false, false, mDoc->GetResolution());
	mImage->SetDest(mOldDest, drawProps);
	mModel->SetDirty();		// !!! need to be more precise
	}//end UndoSelf	
	