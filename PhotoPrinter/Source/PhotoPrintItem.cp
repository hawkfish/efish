/*
	File:		PhotoPrintItem.cp

	Contains:	Definition of an item (i.e. an image).

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

	18 sep 2000		dml		fixed bug 8 (placeholders + ImageOptions)
	18 Sep 2000		drd		Added ESpinCursor arg to Draw
	15 Sep 2000		drd		GetDimensions checks for si_NaturalBounds; fixed a test for proxy existence
	14 sep 2000		dml		bug 5:  DrawIntoNewPictureWithRotation must handle lack of proxy.  same with MakeIcon
	14 sep 2000		dml		fix HasCrop (bug 1)
	14 Sep 2000		drd		Add more codes (and a resolution arg) to GetDimensions
	11 sep 2000		dml		revert some of DrawCaptionText, fix some clip problems w/ captions, more double comparisons fixed
	08 sep 2000		dml		changes to DrawCaptionText
	07 sep 2000		dml		IsLandscape handles absence of imageRect
	07 Sep 2000		drd		MakeIcon uses Apple sample code; GetDimension is sloppier
	06 Sep 2000		drd		MakeIcon (not working yet)
	06 sep 2000		dml		implement caption_RightHorizontal style
	31 aug 2000		dml		fix proxies! (esp w/ cropping, rotation).  DrawIntoNewPicture compensates for existing rot
	31 Aug 2000		drd		DrawIntoNewPictureWithRotation uses dithering to try to improve looks
	30 aug 2000		dml		draw thumbnails at correct aspect ratio
	30 aug 2000		dml		thumbnails drawn with proxy again. fast + stable (thanks to proxy + StLockPixels)
	30 Aug 2000		drd		Fixed spelling of SetPurgeable
	30 aug 2000		dml		add a StLockPixels whenever we draw the proxy (doh!)
	29 Aug 2000		drd		SetFile copies mFileSpec too
	29 Aug 2000		drd		GetProxy; use GetProxy in Draw
	24 aug 2000		dml		added DrawProxyIntoNewPictureWithRotation
	23 aug 2000		dml		proxy should respect expand + offset.  
	23 aug 2000		dml		change storage of Crop percentages to double
	22 aug 2000		dml		SetupDestRect uses expanded only for rect mapping, placement rect for rotation midpoint
	22 aug 2000		dml		bottleneced QTI instantiation into ReanimateQTI.  removed QTI from SetupDestMatrix (!!).
	22 aug 2000		dml		Draw should pass workingCrop to DrawImage, also, ResolveCropStuff must handle xformed coordinates + crop rect
	22 aug 2000		dml		perhaps it is better to throw in the CT if we can't make the qti (fix iMac death)
	21 Aug 2000		drd		The class function is now called ParseProperties; removed ParseBounds
							and made ParseRect the class function
	21 aug 2000		dml		move qti instantiation in ct inside try-catch block, to allow unable-to-draw objects
	21 aug 2000		dml		make serialization respect cropping
	18 aug 2000		dml		make crop (and cropzoom) relative
	16 Aug 2000		drd		DrawEmpty doesn't normalize entire StColorPortState, and adjusts points
							for the way QuickDraw rectangles enclose stuff
	16 aug 2000		dml		remove eggregious casting in HORef tests
	16 aug 2000		dml		GetFileSpec returns an HORef&
	16 aug 2000		dml		GetFileSpec doesn't always install a new spec, only if needed or changed
	15 Aug 2000		drd		GetFileSpec makes sure we have an alias
	15 aug 2000		dml		fixed bug in SetupDestMatrix having to do w/ qti ownership
	15 aug 2000		dml		changes to copy ct (don't copy qti); clarifications of emptiness test casts
	14 Aug 2000		drd		DrawCaption doesn't draw file-related things if we're empty
	07 aug 2000		dml		if GetDestRect(recompute == true) make sure qti is valid
	07 Aug 2000		drd		Only use StQuicktimeRenderer if we rotate
	04 Aug 2000		drd		Fixed DrawCaptionText to handle multiple lines of rotated text
	04 aug 2000		dml		GetName handles Empty case correctly
	04 aug 2000		dml		change from mSpec to mAlias
	03 Aug 2000		drd		DrawCaption observes date/time format prefs
	03 Aug 2000		drd		Better caption_RightVertical (1-line); DrawCaption handles dates
	01 Aug 2000		drd		Started dealing with caption_RightVertical
	01 Aug 2000		drd		Back to 1-bit StQuicktimeRenderer
	31 jul 2000		dml		sure, it only needs a 1-bit gworld, but use 32 to reduce issues in debugging
	31 Jul 2000		dml		DrawCaptionText passes clip region to StQuicktimeRenderer
	25 Jul 2000		drd		DrawCaptionText only needs a 1-bit StQuicktimeRenderer
	20 Jul 2000		drd		AdjustRectangles handles caption_Inside
	17 Jul 2000		drd		MakeProxy makes sure we have a QTI
	14 jul 2000		dml		fix bug in Draw having to do w/ component opening (don't make if empty)
	13 jul 2000		dml		open/close component before each draw (free up memory!)
	12 Jul 2000		drd		AdjustRectangles, DrawCaption use GetCaptionLineHeight
	11 jul 2000		dml		adjustRect correctly sets up ImageRect via AlignmentGizmo
	10 jul 2000		dml		moved StQTImportComponent to separate file, Read now sets NaturalBounds,
							DrawCaption uses StQuicktimeRenderer
	10 Jul 2000		dml		SetDest must call AdjustRectangles, MapDestRect maps FontSize also
	10 Jul 2000		drd		DrawCaptionText, filename caption
	07 Jul 2000		drd		AdjustRectangles, DrawCaption (first stab)
	07 Jul 2000		drd		GetDimensions
	06 Jul 2000		drd		MapDestRect maps mImageRect
	05 jul 2000		dml		MakeProxy operates on a local matrix, not mMat
	05 jul 2000		dml		MakeProxy and DrawProxy use TransformedBounds, not mDest
	05 jul 2000		dml		SetupDestMatrix now optionally scales (sometimes want other xforms only)
	05 Jul 2000		drd		MakeProxy(nil) uses a default matrix
	03 jul 2000		dml		ResolveCropStuff should clip against transformed bounds
	03 Jul 2000		drd		MakeProxy doesn't clip as much
	03 Jul 2000		drd		SetFile sends DeleteProxy; added gUseProxies, DrawImage; redo MakeProxy
	30 Jun 2000		drd		SetFile copies QTI (very handy for SchoolLayout)
	29 jun 2000		dml		add proxy support
	29 jun 2000		dml		clean up ownership issues in ResolveCropStuff
	27 jun 2000		dml		added SetScreenBounds, removed CROP_BY_REGION ifdefs, fixed DrawEmpty
	27 Jun 2000		drd		IsLandscape, IsPortrait
	27 jun 2000		dml		doh!  pass reference to cropRgn to ResolveRegionStuff (cropping works again)
	26 Jun 2000		drd		SetFile; improved default constructor
	21 june 2000 	dml		initial crop should be EmptyRect, and we should special case it in Draw()
	20 june 2000	dml		work on cropping.  
	19 june 2000	dml		copy ct copies crop
	19 june 2000	dml		added cropping, alphabetized
*/

#include "PhotoPrintItem.h"

#include <algorithm.h>
#include "AlignmentGizmo.h"
#include "EChrono.h"
#include "ESpinCursor.h"
#include "IconUtil.h"
#include "MHandle.h"
#include "MNewRegion.h"
#include "MOpenPicture.h"
#include "PhotoPrintPrefs.h"
#include "PhotoUtility.h"
#include "StPixelState.h"
#include "StQuicktimeRenderer.h"
#include "xmlinput.h"
#include "xmloutput.h"

// Globals
SInt16	PhotoPrintItem::gProxyBitDepth = 16;
bool	PhotoPrintItem::gUseProxies = true;				// For debug purposes
double	kRightHorizontalCoefficient = 0.33; 			// how much of avail space allocated to image?
// ---------------------------------------------------------------------------
// PhotoPrintItem constructor
// ---------------------------------------------------------------------------
PhotoPrintItem::PhotoPrintItem(const MFileSpec& inSpec)
	: mAlias (new MDisposeAliasHandle((AliasHandle)inSpec))
	, mRot (0.0)
	, mSkew (0.0)
	, mXScale (1.0)
	, mYScale (1.0)
	, mTopCrop (0)
	, mLeftCrop (0)
	, mBottomCrop (0)
	, mRightCrop (0)
	, mTopOffset (0.0)
	, mLeftOffset (0.0)
{
	ReanimateQTI(); // make it just for side effects
	mQTI = nil;		// throw it away

	::SetIdentityMatrix(&mMat);
}//end ct
	

// ---------------------------------------------------------------------------
// PhotoPrintItem copy constructor
// ---------------------------------------------------------------------------
PhotoPrintItem::PhotoPrintItem(PhotoPrintItem& other) 
	: mAlias (other.mAlias)
	, mNaturalBounds (other.GetNaturalBounds())
	, mTopCrop (other.mTopCrop)
	, mLeftCrop (other.mLeftCrop)
	, mBottomCrop (other.mBottomCrop)
	, mRightCrop (other.mRightCrop)
	, mXScale (other.mXScale)
	, mYScale (other.mYScale)
	, mTopOffset (other.mTopOffset)
	, mLeftOffset (other.mLeftOffset)
	, mDest (other.GetDestRect())
	, mImageRect (other.GetImageRect())
	, mFrameRect (other.GetFrameRect())
	, mCaptionRect (other.GetCaptionRect())
	, mRot (other.GetRotation())
	, mSkew (other.GetSkew())
	, mQTI (other.mQTI)
	, mProperties (other.GetProperties())
{
	// could recompute, but hey, it's a copy constructor
	::CopyMatrix(&(other.mMat), &mMat);	
}//end copy ct


// ---------------------------------------------------------------------------
// PhotoPrintItem empty constructor
// ---------------------------------------------------------------------------
PhotoPrintItem::PhotoPrintItem()
	: mAlias (nil)
	, mRot (0.0)
	, mSkew (0.0)
	, mXScale (1.0)
	, mYScale (1.0)
	, mTopCrop (0)
	, mLeftCrop (0)
	, mBottomCrop (0)
	, mRightCrop (0)
	, mTopOffset (0.0)
	, mLeftOffset (0.0)
	, mQTI(nil)
{
}//end empty ct



// ---------------------------------------------------------------------------
// ~PhotoPrintItem destructor
// ---------------------------------------------------------------------------
PhotoPrintItem::~PhotoPrintItem() {

	}//end dt

// ---------------------------------------------------------------------------
// SetFile
//	This method copies everything needed to make the two items refer to the
//	same file.
// ---------------------------------------------------------------------------
void
PhotoPrintItem::SetFile(const PhotoPrintItem& inOther)
{
	mAlias = inOther.mAlias;
	mFileSpec = inOther.mFileSpec;
	mQTI = nil;
	mNaturalBounds = inOther.mNaturalBounds;	// Likewise, the bounds will be the same

	this->DeleteProxy();						// Picture has changed, get rid of cached low-res
} // SetFile

#pragma mark -

// ---------------------------------------------------------------------------
// AdjustRectangles
//	Adjust rectangles depending on caption.
// ---------------------------------------------------------------------------
void
PhotoPrintItem::AdjustRectangles()
{
	MRect		oldImageRect(mImageRect);

	PhotoItemProperties&	props(this->GetProperties());

	if (props.HasCaption()) {
		SInt16	height;
		SInt16	width;
		SInt16	lines = 0;
		if (props.GetCaption().Length() > 0) {
			lines++;
		}

		if (props.GetShowName()) {
			lines++;
		}
		if (props.GetShowDate()) {
			lines++;
		}
		height = lines * props.GetCaptionLineHeight();

		mImageRect = mDest;
		switch (props.GetCaptionStyle()) {
			case caption_Bottom:
				mImageRect.SetHeight(mImageRect.Height() - height);

				AlignmentGizmo::FitAndAlignRectInside(this->GetNaturalBounds(),
													mImageRect,
													kAlignAbsoluteCenter,
													mImageRect,
													EUtil::kDontExpand);

				mCaptionRect = mDest;
				mCaptionRect.top = mCaptionRect.bottom - height;
				break;

			case caption_Inside:
				mCaptionRect = mDest;
				mCaptionRect.top = mCaptionRect.bottom - height;
				width = min(max((long)kNarrowestCaption, mDest.Width() / 3), mDest.Width());
				mCaptionRect.SetWidth(width);
				break;

			case caption_RightHorizontal:
				SInt16 captionWidth = mDest.Width() * kRightHorizontalCoefficient;;
				mImageRect.SetWidth(mDest.Width() - captionWidth);
				AlignmentGizmo::FitAndAlignRectInside(this->GetNaturalBounds(),
													mImageRect,
													kAlignAbsoluteCenter,
													mImageRect,
													EUtil::kDontExpand);
				mCaptionRect = mImageRect;
				mCaptionRect.SetWidth(captionWidth);
				mCaptionRect.Offset(mImageRect.Width(), 0); // place it next to
				// !!! should center vertically
				break;

			case caption_RightVertical:
				mImageRect.SetWidth(mImageRect.Width() - height);

				AlignmentGizmo::FitAndAlignRectInside(this->GetNaturalBounds(),
													mImageRect,
													kAlignAbsoluteCenter,
													mImageRect,
													EUtil::kDontExpand);

				mCaptionRect = mDest;
				mCaptionRect.left = mCaptionRect.right - height;
				break;

		}
	} else {
		mImageRect = mDest;
		mCaptionRect = MRect();		// Make it empty
	}

	// IFF Rectangles changed, proxy is invalid
	if (oldImageRect.Width() != mImageRect.Width() ||
		oldImageRect.Height() != mImageRect.Height())
		this->DeleteProxy();
} // AdjustRectangles


// ---------------------------------------------------------------------------
// CanUseProxy
// ---------------------------------------------------------------------------
bool
PhotoPrintItem::CanUseProxy(const PhotoDrawingProperties& props) const
{
	bool happy (false);

	if (!props.GetPrinting() && gUseProxies)
//		if ((PhotoUtility::DoubleEqual(mRot, 0.0)) && (PhotoUtility::DoubleEqual(mSkew,0.0)))
			happy = true;

	return happy;
} // CanUseProxy


// ---------------------------------------------------------------------------
// DeriveCropRect
//
// crop is kept as 4 UInt16's representing percentage offsets from each of 4 sides
// we take those percentages, and convert the image rect
// ---------------------------------------------------------------------------
void
PhotoPrintItem::DeriveCropRect(MRect& outRect) {
	outRect = GetImageRect();

	SInt16 width (GetImageRect().Width());
	SInt16 height (GetImageRect().Height());

	outRect.top += height * (mTopCrop / 100.0);
	outRect.left += width * (mLeftCrop / 100.0);
	outRect.bottom -= height * (mBottomCrop / 100.0);
	outRect.right -= width * (mRightCrop / 100.0);
	}//end DeriveCropRect


// ---------------------------------------------------------------------------
// PhotoPrintItem::Draw
//			set the qt matrix + have the component render
// ---------------------------------------------------------------------------
void
PhotoPrintItem::Draw(
	const PhotoDrawingProperties&	props,
	MatrixRecord*					worldSpace,
	CGrafPtr						inDestPort,
	GDHandle						inDestDevice,
	RgnHandle						inClip,
	HORef<ESpinCursor>				inCursor)
{
	try {
		MatrixRecord	localSpace;
		SetupDestMatrix(&mMat);
		
		::CopyMatrix(&mMat, &localSpace);
		if (worldSpace)
			::ConcatMatrix(worldSpace, &localSpace);
		
		HORef<MRegion>	cropRgn;
		RgnHandle		workingCrop(this->ResolveCropStuff(cropRgn, inClip));

		do {
			if (this->IsEmpty()) {
				if (!props.GetPrinting()) {
					this->DrawEmpty(props, &localSpace, inDestPort, inDestDevice, workingCrop);
				} //endif we're not printing
				break;
			} //endif empty

			// If we have no proxy (for whatever reason), spin the watch because drawing will take a while
			if (mProxy == nil && inCursor != nil)
				inCursor->Spin();

			if (this->CanUseProxy(props)) {
				// Be sure we have a proxy (we ignore the return value, it will set mProxy)
				this->GetProxy(&localSpace);
				
				if (mProxy != nil) {
					this->DrawProxy(props, worldSpace, inDestPort, inDestDevice, workingCrop);
					break;
				}
			}//endif can use proxies
			else
				mProxy = nil; //otherwise ensure we delete the proxy (since toggling intended only for debugging)
			{ //otherwise we can draw normally
				this->DrawImage(&localSpace, inDestPort, inDestDevice, workingCrop);
			} //end normal drawing block
		} while (false);

		if (this->GetProperties().HasCaption()) {
			this->DrawCaption(inClip); //caption should just deal with inner clip (cause image cropping could screw it up)
		}
	}//end try
	catch (...) {
		mQTI = nil;
		throw;
	}//end catch
	
	mQTI = nil; // if we've made it during this draw operation, make sure to free it here
} // Draw

void
PhotoPrintItem::DrawCaption(RgnHandle inPassthroughClip)
{
	PhotoItemProperties&	props(this->GetProperties());
	MPString			theCaption(props.GetCaption());

	StColorPenState		restorePen;
	StTextState			restoreText;
	::RGBForeColor(&Color_Black);

	SInt16				offset = 0;

	if (props.GetCaptionStyle() == caption_Inside) {
		::RGBBackColor(&Color_White);
		mCaptionRect.Erase();
	}

	if (theCaption.Length() > 0) {
		this->DrawCaptionText(theCaption, offset, inPassthroughClip);
		offset += props.GetCaptionLineHeight();
	}

	if (! this->IsEmpty() && props.GetShowName()) {
		Str255	fileName;
		this->GetName(fileName);
		this->DrawCaptionText(fileName, offset, inPassthroughClip);
		offset += props.GetCaptionLineHeight();
	}

	if (! this->IsEmpty() && props.GetShowDate()) {
		LStr255			date;
		EChrono::GetDateTime(date, this->GetModifiedTime(),
			PhotoPrintPrefs::Singleton()->GetDateFormat(), PhotoPrintPrefs::Singleton()->GetTimeFormat());
		this->DrawCaptionText(date, offset, inPassthroughClip);
		offset += props.GetCaptionLineHeight();
	}
} // DrawCaption

/*
DrawCaptionText
*/
void
PhotoPrintItem::DrawCaptionText(ConstStr255Param inText, const SInt16 inVerticalOffset, RgnHandle inClip)
{
	MRect				bounds(mCaptionRect);


	// setup the matrix
	MatrixRecord		mat;
	::SetIdentityMatrix(&mat);

	MatrixRecord		rotator;
	bool				additionalRotation = false;
	if (this->GetProperties().GetCaptionStyle() == caption_RightVertical) {
		additionalRotation = true;
		Point			midPoint = mCaptionRect.MidPoint(); //rotate around center of full caption rect
		::SetIdentityMatrix(&rotator);
		::RotateMatrix(&rotator, ::Long2Fix(270), ::Long2Fix(midPoint.h), ::Long2Fix(midPoint.v));
		// and we have to change the rectangle
		::TransformRect(&rotator, &bounds, nil);
	}


	// Take the offset into account (this moves down to individual lines in the caption)
	bounds.top += inVerticalOffset;

	// start with a translate to topleft of caption rect
	::TranslateMatrix(&mat, ::FixRatio(bounds.left, 1), ::FixRatio(bounds.top, 1));

	// then any rotation happens around center of image rect
	if (!PhotoUtility::DoubleEqual(mRot, 0.0)) {
		MRect		dest (this->GetImageRect());
		Point		midPoint = dest.MidPoint();
		::RotateMatrix(&mat, ::Long2Fix(static_cast<long>(mRot)),
			::Long2Fix(midPoint.h), ::Long2Fix(midPoint.v));
	}

	// If the caption is in a rotated style, include that transformation
	if (additionalRotation) {
		::ConcatMatrix(&rotator, &mat);
	}

	{
	// Use a StQuicktimeRenderer to draw rotated text (we only make one if we have to, both as an
	// optimization, and to work around a Mac OS X DP4 bug)
	HORef<StQuicktimeRenderer>		qtr;
	if (additionalRotation || !PhotoUtility::DoubleEqual(mRot, 0.0))
		qtr = new StQuicktimeRenderer(bounds, 1, useTempMem, &mat, inClip);
	::TextFont(this->GetProperties().GetFontNumber());
	::TextSize(this->GetProperties().GetFontSize());
	Ptr					text = (Ptr)(inText);	// I couldn't get this to work with 1 C++ cast
	UTextDrawing::DrawWithJustification(text + 1, ::StrLength(inText), bounds, teJustCenter, true);
	}//end QTRendering block
} // end DrawCaptionText

/*
DrawEmpty
*/
void
PhotoPrintItem::DrawEmpty(const PhotoDrawingProperties& /*props*/,
						 MatrixRecord* localSpace, // already composited and ready to use
						 CGrafPtr inDestPort,
						 GDHandle inDestDevice,
						 RgnHandle inClip) {

	MRect	bounds (GetDestRect());
	if (!bounds) // if no bounds, use the maxBounds
		bounds = GetMaxBounds();

	enum cornerType {
		kTopLeft = 0,
		kTopRight,
		kBotRight,
		kBotLeft,
		kFnordCorner};
		
	Point	corners[kFnordCorner];
	corners[kTopLeft] = bounds.TopLeft();
	corners[kBotRight] = bounds.BotRight();
	corners[kBotRight].h--;						// Take into account the way QuickDraw rects work
	corners[kBotRight].v--;						// Take into account the way QuickDraw rects work
	corners[kTopRight].h = corners[kBotRight].h;
	corners[kTopRight].v = corners[kTopLeft].v;
	corners[kBotLeft].h = corners[kTopLeft].h;
	corners[kBotLeft].v = corners[kBotRight].v;

	// transform those corners by composite matrix
	::TransformPoints(localSpace, corners, kFnordCorner);

	HORef<StGDeviceSaver> saveDevice;
	CGrafPtr	destPort;
	if (inDestPort != nil)
		destPort = inDestPort;
	else
		::GetPort((GrafPtr*)&destPort);
		
	StColorPortState		saveState ((GrafPtr)destPort);
	if (inDestDevice != nil) {
		saveDevice = new StGDeviceSaver;
		::SetGWorld(destPort, inDestDevice);
		}//endif device specified 

	// Just initialize the stuff we care about (don't normalize entire StColorPortState)
	StColorPenState::Normalize();
	StClipRgnState::Normalize();

	::RGBForeColor(&PhotoUtility::sNonReproBlue);
	
	HORef<StClipRgnState>	saveClip;
	if (inClip != nil) {
		saveClip = new StClipRgnState (inClip);
		}//endif clipping to do

	::MoveTo(corners[kTopLeft].h, corners[kTopLeft].v);
	::LineTo(corners[kTopRight].h, corners[kTopRight].v);
	::LineTo(corners[kBotRight].h, corners[kBotRight].v);
	::LineTo(corners[kBotLeft].h, corners[kBotLeft].v);
	::LineTo(corners[kTopLeft].h, corners[kTopLeft].v);
	::LineTo(corners[kBotRight].h, corners[kBotRight].v);
	::MoveTo(corners[kBotLeft].h, corners[kBotLeft].v);
	::LineTo(corners[kTopRight].h, corners[kTopRight].v);
}//end DrawEmpty

// ---------------------------------------------------------------------------
// DrawImage
//	Does the basic drawing. Called by Draw and MakeProxy.
// ---------------------------------------------------------------------------
void
PhotoPrintItem::DrawImage(
	 MatrixRecord*	inLocalSpace, // already composited and ready to use
	 CGrafPtr		inDestPort,
	 GDHandle		inDestDevice,
	 RgnHandle		inClip) 
{
	OSErr			e;

	if ((mQTI == nil) && (mAlias != nil)) {
		ReanimateQTI();		
		}//endif

	ThrowIfOSErr_(::GraphicsImportSetMatrix (*mQTI, inLocalSpace));

	if (inDestPort && inDestDevice) {
		e =::GraphicsImportSetGWorld(*mQTI, inDestPort, inDestDevice);
		ThrowIfOSErr_(e);
	}//endif
		
	e = ::GraphicsImportSetClip(*mQTI, inClip);
	ThrowIfOSErr_(e);
	e = ::GraphicsImportSetQuality (*mQTI, codecMaxQuality);
	ThrowIfOSErr_(e);
	e = ::GraphicsImportDraw(*mQTI);
	ThrowIfOSErr_(e);
} // DrawImage

// ---------------------------------------------------------------------------
// DrawProxy
// ---------------------------------------------------------------------------
void
PhotoPrintItem::DrawProxy(const PhotoDrawingProperties& props,
						 MatrixRecord* worldSpace, // already composited and ready to use
						 CGrafPtr inDestPort,
						 GDHandle inDestDevice,
						 RgnHandle inClip)
{
	StClipRgnState saveClip (inClip);
	HORef<StGDeviceSaver> saveDevice;
	CGrafPtr	destPort;
	if (inDestPort != nil)
		destPort = inDestPort;
	else
		::GetPort((GrafPtr*)&destPort);
		
	StColorPortState	saveState ((GrafPtr)destPort);
	if (inDestDevice != nil) {
		saveDevice = new StGDeviceSaver;
		::SetGWorld(destPort, inDestDevice);
		}//endif device specified 
	
	MRect proxyBounds;
	mProxy->GetBounds(proxyBounds);

	MRect imageRect = GetImageRect();
	MatrixRecord localSpace;
	MatrixRecord xformMat;
	::SetIdentityMatrix(&localSpace);
	::TranslateMatrix(&localSpace, Long2Fix(imageRect.left + (mLeftOffset * proxyBounds.Width())),
									 Long2Fix(imageRect.top  + (mTopOffset * proxyBounds.Height())));
	SetupDestMatrix(&xformMat, false);
	::ConcatMatrix(&xformMat, &localSpace);
	if (worldSpace)
		::ConcatMatrix(worldSpace, &localSpace);



	ImageDescriptionHandle	sourceDesc;
	PixMapHandle			sourcePixels = ::GetGWorldPixMap(mProxy->GetMacGWorld());
	StLockPixels locker (sourcePixels);
	OSErr					err = ::MakeImageDescriptionForPixMap(sourcePixels, &sourceDesc);
	ThrowIfOSErr_(err);


	UInt32 quality;
	ComponentRecord* codec;
	if (props.GetDraft()) {
		quality = codecLowQuality;
		codec = bestSpeedCodec;
		}//endif draft mode
	else {
		quality = codecMaxQuality;
		codec = bestFidelityCodec;
		}//else


	err = ::FDecompressImage(::GetPixBaseAddr(sourcePixels),
						sourceDesc,
						::GetPortPixMap(destPort),
						nil,					// Decompress entire source
						&localSpace,
						srcCopy,
						inClip,					// mask
						nil,					// matte
						nil,					// matteRect
						quality,				// accuracy
						codec,					// codec
						0,						// dataSize not needed with no dataProc
						nil,					// dataProc
						nil);					// progressProc
	ThrowIfOSErr_(err);

	::DisposeHandle((Handle)sourceDesc);
}//end DrawProxy				

/*
DrawIntoNewPictureWithRotation
*/
void
PhotoPrintItem::DrawIntoNewPictureWithRotation(double inRot, const MRect& destBounds, MNewPicture& destPict)
{
	MRect			imageBounds;
	
	// MakeRotatedThumbnails has already tried to make the proxy (if needed) and lock it down (if avail)
	if (mProxy != nil) {
		mProxy->GetBounds(imageBounds);
		}//endif proxy is avail, lock it down for this duration!!
	else {
		if (IsEmpty()) {
			imageBounds = GetDestRect();
			if (!imageBounds)
				imageBounds = GetMaxBounds();
			}// endif empty placeholder
		else
			imageBounds = GetNaturalBounds();
		}//else no proxy
	
	MRect			aspectDest;
	AlignmentGizmo::FitAndAlignRectInside(imageBounds, destBounds, kAlignAbsoluteCenter, aspectDest, EUtil::kDontExpand);

	MatrixRecord mat;
	::SetIdentityMatrix(&mat);
	::RectMatrix(&mat, &imageBounds, &aspectDest);
	::RotateMatrix(&mat, Long2Fix(inRot - mRot/*subtract since proxy will composite back in*/), 
						Long2Fix(destBounds.MidPoint().h), Long2Fix(destBounds.MidPoint().v));
	// be a good citizen and setup clipping
	MNewRegion clip;
	clip = destBounds; 

	EGWorld			offscreen(destBounds, gProxyBitDepth);
	GDHandle		offscreenDevice (::GetGWorldDevice(offscreen.GetMacGWorld()));

	// render the rotated proxy
	PhotoDrawingProperties	props (kNotPrinting, kPreview, kDraft);
	if (mProxy != nil)
		DrawProxy(props, &mat, offscreen.GetMacGWorld(), offscreenDevice, clip);
	else {
		if (IsEmpty())
			DrawEmpty(props, &mat, offscreen.GetMacGWorld(), offscreenDevice, clip);
		else
			DrawImage(&mat, offscreen.GetMacGWorld(), offscreenDevice, clip);
		}//else no proxy
	// now we need to blit that offscreen into another, while a Picture is open to capture it into a pict
	LGWorld			offscreen2(destBounds, gProxyBitDepth);
	if (offscreen2.BeginDrawing()) {
		MNewPicture			pict;			// Creates a PICT and destroys it
		{
			MOpenPicture	openPicture(pict, destBounds);
			offscreen.CopyImage(UQDGlobals::GetCurrentPort(), destBounds, ditherCopy);
		}
		offscreen2.EndDrawing();

		destPict.Attach(pict.Detach());		// Transfer ownership of the PICT
		} // if
}//end DrawProxyIntoPicHandle

#pragma mark -

/*
CheckExactHeight
*/
void
PhotoPrintItem::CheckExactHeight(
	long&			ioWidth,
	long&			ioHeight,
	OSType&			outCode, 
	SInt16&			outUnits,
	const long		inTestWidth,
	const long		inTestHeight,
	const OSType	inCode) const
{
	if (abs((long)(ioWidth - inTestWidth * kDPI)) < kDimDelta && abs((long)(ioHeight - inTestHeight * kDPI)) < kTinyDelta) {
		outUnits = si_Inches;
		ioWidth = inTestWidth;
		ioHeight = inTestHeight;
		outCode = inCode;
	}
} // CheckExactHeight

/*
CheckExactWidth
*/
void
PhotoPrintItem::CheckExactWidth(
	long&			ioWidth,
	long&			ioHeight,
	OSType&			outCode, 
	SInt16&			outUnits,
	const long		inTestWidth,
	const long		inTestHeight,
	const OSType	inCode) const
{
	if (abs((long)(ioWidth - inTestWidth * kDPI)) < kTinyDelta && abs((long)(ioHeight - inTestHeight * kDPI)) < kDimDelta) {
		outUnits = si_Inches;
		ioWidth = inTestWidth;
		ioHeight = inTestHeight;
		outCode = inCode;
	}
} // CheckExactWidth

// ---------------------------------------------------------------------------
// GetCrop
// 
void
PhotoPrintItem::GetCrop(double& outTopCrop, double& outLeftCrop, double& outBottomCrop, double& outRightCrop) const {
	outTopCrop = mTopCrop;
	outLeftCrop = mLeftCrop;
	outBottomCrop = mBottomCrop;
	outRightCrop = mRightCrop;
}//end GetCrop

// ---------------------------------------------------------------------------
// GetCropZoomOffset
// 
void
PhotoPrintItem::GetCropZoomOffset(double& outTopOffset, double& outLeftOffset) {
	outTopOffset = mTopOffset;
	outLeftOffset = mLeftOffset;
	}//end GetCropZoomOffset

// ---------------------------------------------------------------------------
// GetCropZoomScales
// 
void
PhotoPrintItem::GetCropZoomScales(double& outZoomScaleX, double& outZoomScaleY) const {	
	outZoomScaleX = mXScale;
	outZoomScaleY = mYScale;
}//end GetCropZoomScales


// ---------------------------------------------------------------------------
// GetDestRect
// 
const MRect
PhotoPrintItem::GetDestRect (void) const {
	return mDest;
}//end GetDestRect 

// ---------------------------------------------------------------------------
// GetDimensions 
// 	Returns both a 4-letter code and a human-readable version H x W
// ---------------------------------------------------------------------------
OSType
PhotoPrintItem::GetDimensions(Str255 outDescriptor, const SInt16 inResolution, const SInt16 inWhich) const
{
	OSType		code = 'cust';
	long		width;
	long		height;
	SInt16		unitIndex = si_Pixels;

	if (inWhich == si_NaturalBounds) {
		// Get the natural bounds of the image
		width = this->GetNaturalBounds().Width();
		height = this->GetNaturalBounds().Height();
	} else {
		// Get the size in printed points
		width = this->GetImageRect().Width() * ((double)kDPI / (double)inResolution);
		height = this->GetImageRect().Height() * ((double)kDPI / (double)inResolution);
		this->CheckExactHeight(width, height, code, unitIndex, 2, 3, '3*2 ');
		this->CheckExactWidth(width, height, code, unitIndex, 3, 2, '2*3 ');
		this->CheckExactHeight(width, height, code, unitIndex, 3, 5, '5*3 ');
		this->CheckExactWidth(width, height, code, unitIndex, 5, 3, '3*5 ');
		this->CheckExactHeight(width, height, code, unitIndex, 4, 6, '6*4 ');
		this->CheckExactWidth(width, height, code, unitIndex, 6, 4, '4*6 ');
		this->CheckExactHeight(width, height, code, unitIndex, 5, 7, '7*5 ');
		this->CheckExactWidth(width, height, code, unitIndex, 7, 5, '5*7 ');
	}

	LStr255		w(width);
	LStr255		h(height);
	MPString	text(str_ImageSize, inWhich);
	MPString	units(str_ImageSize, unitIndex);
	text.Replace(h, "\p^0");
	text.Replace(w, "\p^1");
	text.Replace(units, "\p^2");
	::BlockMoveData(text, outDescriptor, text.Length() + 1);

	return code;
} // GetDimensions

// ---------------------------------------------------------------------------
//GetExpandedOffsetImageRect
//	take the image rect.  scale it by our cropzoom factors, and offset it over so
// cropped section lines up correctly.
// ---------------------------------------------------------------------------
void
PhotoPrintItem::GetExpandedOffsetImageRect(MRect& outRect)
{
	outRect = GetImageRect();
	outRect.SetWidth(outRect.Width() * mXScale);
	outRect.SetHeight(outRect.Height() * mYScale);

	outRect.Offset(mLeftOffset * outRect.Width(), mTopOffset * outRect.Height());	
}//end GetExpandedOffsetImageaRect


// ---------------------------------------------------------------------------
// GetFileSpec:  forces resolution of the alias.  
// ---------------------------------------------------------------------------
HORef<MFileSpec>&
PhotoPrintItem::GetFileSpec()
{
	if (mAlias != nil) {
		Boolean outChanged;
		MFileSpec* newSpec =  new MFileSpec(outChanged, *mAlias);
		if ((outChanged) || (mFileSpec == nil))
			mFileSpec = newSpec;
		else
			delete newSpec;
	}
	return mFileSpec;
} // GetFileSpec


// ---------------------------------------------------------------------------
// GetMatrix 
// (might be stale but faster if don't force recompute)
// ---------------------------------------------------------------------------
void
PhotoPrintItem::GetMatrix(MatrixRecord* pDestMatrix,
							bool inForceRecompute)
{
	if (inForceRecompute)
		this->SetupDestMatrix(&mMat);

	::CopyMatrix(&mMat, pDestMatrix);
}//end GetMatrix	

/*
GetCreatedTime
*/
UInt32
PhotoPrintItem::GetCreatedTime() 
{
	CInfoPBRec		info;

	this->GetFileSpec()->GetCatInfo(info);

	return info.hFileInfo.ioFlCrDat;
} // GetCreatedTime

/*
GetModifiedTime
*/
UInt32
PhotoPrintItem::GetModifiedTime() 
{
	CInfoPBRec		info;

	this->GetFileSpec()->GetCatInfo(info);
	return info.hFileInfo.ioFlMdDat;
} // GetModifiedTime

// ---------------------------------------------------------------------------
// GetName
// ---------------------------------------------------------------------------
void
PhotoPrintItem::GetName(Str255& outName)
{
	if (IsEmpty())
		outName[0] = 0;
	else
		::memcpy(outName, GetFileSpec()->Name(), sizeof(Str255));
}//end GetName

/*
GetProxy
*/
HORef<EGWorld>
PhotoPrintItem::GetProxy(MatrixRecord* inWorldSpace) 
{
	// If the proxy pixels were purged, delete the proxy (and start afresh)
	if ((mProxy != nil) && (mProxy->IsPurged()))
		mProxy = nil;

	if (mProxy == nil)
		this->MakeProxy(inWorldSpace);

	return mProxy;
} // GetProxy

// ---------------------------------------------------------------------------
// GetTransformedBounds 
// 
// ---------------------------------------------------------------------------
MRect
PhotoPrintItem::GetTransformedBounds() {
	Point	corners[4];
	MRect	dest (GetDestRect());
	corners[0] = dest.TopLeft();
	corners[3] = dest.BotRight();
	corners[1].v = corners[0].v; // topRight
	corners[1].h = corners[3].h;
	corners[2].v = corners[3].v; // bottomLeft
	corners[2].h = corners[0].h;
	
	SetupDestMatrix(&mMat, false);
	::TransformPoints(&mMat, corners, 4);
	
	MRect bounds;
	bounds.left = min(min(corners[0].h, corners[1].h), min(corners[2].h, corners[3].h));
	bounds.right = max(max(corners[0].h, corners[1].h), max(corners[2].h, corners[3].h));
	bounds.top = min(min(corners[0].v, corners[1].v), min(corners[2].v, corners[3].v));
	bounds.bottom = max(max(corners[0].v, corners[1].v), max(corners[2].v, corners[3].v));

	return bounds;
	}//end	 


bool
PhotoPrintItem::HasCrop() const {
	return (!(PhotoUtility::DoubleEqual(mTopCrop,0.0) &&
			PhotoUtility::DoubleEqual(mLeftCrop,0.0) &&
			PhotoUtility::DoubleEqual(mRightCrop,0.0) &&
			PhotoUtility::DoubleEqual(mBottomCrop,0.0)));
	}//end HasCrop


bool
PhotoPrintItem::HasZoom() const {
	return (! (PhotoUtility::DoubleEqual(mXScale, 1.0) &&
				PhotoUtility::DoubleEqual(mYScale, 1.0)));
		
	}//end HasZoom


// ---------------------------------------------------------------------------
// IsLandscape
// ---------------------------------------------------------------------------
bool
PhotoPrintItem::IsLandscape(bool useNaturalBounds) 
{
	MRect bounds (GetImageRect()); 

	//handle case where we're empty placeholder and don't have imageRect yet
	if (!bounds)
		bounds = GetMaxBounds();
	
	if (!useNaturalBounds) {
		SetupDestMatrix(&mMat);
		::TransformRect(&mMat, &bounds, nil);
		}//endif

	return bounds.Width() >= bounds.Height();

} // IsLandscape

/*
MakeIcon
	Creates and returns a handle. Caller owns the handle. 
*/
Handle
PhotoPrintItem::MakeIcon(const ResType inType)
{
	this->GetProxy();											// Be certain our proxy is present

	SInt16		iconSize;
	SInt16		pixelDepth;

	switch (inType) {
		case 'ICN#':
			iconSize = 32;
			pixelDepth = 1;
			break;

		case 'ics#':
			iconSize = 16;
			pixelDepth = 1;
			break;

		case 'icl4':
			iconSize = 32;
			pixelDepth = 4;
			break;

		case 'icl8':
			iconSize = 32;
			pixelDepth = 8;
			break;

		case 'ics4':
			iconSize = 16;
			pixelDepth = 4;
			break;

		case 'ics8':
			iconSize = 16;
			pixelDepth = 8;
			break;
	}

	MRect			iconBounds(0, 0, iconSize, iconSize);

	MRect			imageBounds;
	if (GetProxy() != nil)
		mProxy->GetBounds(imageBounds);
	else
		imageBounds = GetNaturalBounds();
		
	MRect			aspectDest;
	AlignmentGizmo::FitAndAlignRectInside(imageBounds, iconBounds, kAlignAbsoluteCenter, aspectDest, EUtil::kDontExpand);

	// Make a small copy of the proxy (or image)
	EGWorld			offscreen(iconBounds, pixelDepth);
	if (mProxy != nil)
		mProxy->CopyImage(offscreen.GetMacGWorld(), aspectDest, ditherCopy);
	else {
		MatrixRecord localSpace;
		GDHandle	offscreenDevice (::GetGWorldDevice(offscreen.GetMacGWorld()));
		::RectMatrix(&localSpace,&GetNaturalBounds(), &aspectDest);
		DrawImage(&localSpace, offscreen.GetMacGWorld(), offscreenDevice, nil);
		}//else have to draw the image
	// And draw a box around it
	offscreen.BeginDrawing();
	aspectDest.Frame();
	offscreen.EndDrawing();

	Handle			theHandle;
	if (inType == 'ICN#' || inType == 'ics#')
		theHandle = ::MakeICN_pound(offscreen.GetMacGWorld(), &iconBounds, iconSize);
	else
		theHandle = ::MakeIcon(offscreen.GetMacGWorld(), &iconBounds, pixelDepth, iconSize);

	return theHandle;
} // MakeIcon

// ---------------------------------------------------------------------------
// MakeProxy
// ---------------------------------------------------------------------------
void
PhotoPrintItem::MakeProxy(
	 MatrixRecord*	inLocalSpace)				// already composited and ready to use
{
	if (IsEmpty())
		return;

	if (mQTI == nil && mAlias) {
		ReanimateQTI();
	}

	StDisableDebugThrow_();
	StGrafPortSaver				savePort;		// Be sure we're in the right port even if there's a throw

	if (inLocalSpace == nil) {
		MatrixRecord defaultMatrix;
		this->SetupDestMatrix(&defaultMatrix);
		inLocalSpace = &defaultMatrix;
	}

	HORef<MRegion>				cropRgn;
	RgnHandle					workingCrop(this->ResolveCropStuff(cropRgn, nil));

	try {
		//	Create the offscreen GWorld
//		MRect					bounds(GetTransformedBounds());	// !!! Assume rectangular
		MRect					bounds;
		GetExpandedOffsetImageRect(bounds);	
		// make the proxy as unpurgable at first, since we need to draw into it
		mProxy = new EGWorld (bounds, gProxyBitDepth, 0, 0, 0, EGWorld::kNeverTryTempMem, kNoPurge);

		//	Draw into it
		if (mProxy->BeginDrawing ()) {
			this->DrawImage(inLocalSpace, mProxy->GetMacGWorld(), ::GetGDevice(), /*workingCrop*/ nil);
			mProxy->EndDrawing();
			} //endif able to lock + draw
		mProxy->SetPurgeable(true);

	} catch (...) {
		// Swallow the exception, and set mProxy to nil
		mProxy = nil;
	}

	// Note that we keep the importer (since if we made it, we were probably called from
	// ImageOptionsDialog::SetupImage, which will share the QTI among the 4 rotation thumbnails
} // MakeProxy

/*
MakeRotatedThumbnails
*/
void
PhotoPrintItem::MakeRotatedThumbnails(
	MNewPicture& io0Rotation, MNewPicture& io90Rotation, 
	MNewPicture& io180Rotation, MNewPicture& io270Rotation, const MRect& bounds)
{
	HORef<StPixelState> possibleProxyLocker = nil;
	//try to ensure there is a proxy to draw
	if (GetProxy(&mMat) != nil) {
		possibleProxyLocker = new StPixelState(mProxy->GetMacGWorld());
		mProxy->SetPurgeable(false);
		}//endif there is a proxy, lock it down for the duration of this function
			
	DrawIntoNewPictureWithRotation(0.0, bounds, io0Rotation);
	DrawIntoNewPictureWithRotation(90.0, bounds, io90Rotation);
	DrawIntoNewPictureWithRotation(180.0, bounds, io180Rotation);
	DrawIntoNewPictureWithRotation(270.0, bounds, io270Rotation);

}//end MakeRotatedThumbnails


// ---------------------------------------------------------------------------
// MapDestRect.  Used to map mDest (and associated rects) from one space to another
// so that rotation/skewing won't be affected by the transform 
// otherwise we could accomplish this with a matrix operation
//
// ---------------------------------------------------------------------------
void
PhotoPrintItem::MapDestRect(const MRect& sourceRect, const MRect& destRect)
{
	::MapRect(&mDest, &sourceRect, &destRect);
	::MapRect(&mImageRect, &sourceRect, &destRect);
	::MapRect(&mFrameRect, &sourceRect, &destRect);
	::MapRect(&mCaptionRect, &sourceRect, &destRect);
	
	// map the caption point size!
	GetProperties().SetFontSize(GetProperties().GetFontSize() * (destRect.Width() / sourceRect.Width()));
}//end MapDestRect


// ---------------------------------------------------------------------------
// ReanimateQTI
// attempt to make a QTI from the filespec corresponding to our alias
// Throw if nil, or if unable to get bounds
// ---------------------------------------------------------------------------
void
PhotoPrintItem::ReanimateQTI() {
	if ((mAlias != nil) && (mQTI == nil)){
		mQTI  = new StQTImportComponent(GetFileSpec());
		ThrowIfNil_(mQTI);
		ComponentResult res;
		res = ::GraphicsImportGetNaturalBounds (*mQTI, &mNaturalBounds);
		ThrowIfOSErr_(res);				
		}//endif enough knowledge to attempt reanimation
	}//end ReandimateQTI




// ---------------------------------------------------------------------------
// ResolveCropStuff
// ---------------------------------------------------------------------------
RgnHandle
PhotoPrintItem::ResolveCropStuff(HORef<MRegion>& cropRgn, RgnHandle inClip)
{
	MRect xformDest (GetTransformedBounds());
	
	// do we have intrinsic cropping?
	if (HasCrop()) {
		cropRgn = new MNewRegion;
		MRect derivedRect;
		DeriveCropRect(derivedRect);
	
		Point corners[4];
		corners[0] = derivedRect.TopLeft();
		corners[2] = derivedRect.BotRight();
		corners[1].v = corners[0].v; // topRight
		corners[1].h = corners[2].h;
		corners[3].v = corners[2].v; // bottomLeft
		corners[3].h = corners[0].h;		
		::TransformPoints(&mMat, corners, 4);
	
		cropRgn->Open();
		::MoveTo(corners[0].h, corners[0].v);
		::LineTo(corners[1].h, corners[1].v);
		::LineTo(corners[2].h, corners[2].v);
		::LineTo(corners[3].h, corners[3].v);
		::LineTo(corners[0].h, corners[0].v);
		cropRgn->Close();
		
		// fake out clip bug
		// by creating a tiny region on the topline, union'ed with the rest
		MNewRegion fakey;
		fakey = MRect (xformDest.top, xformDest.left, xformDest.top + 1, xformDest.left + 1);

		cropRgn->Union(*cropRgn, fakey);
		}//endif we have some intrinsic cropping

	// combine it with any incoming clipping
	if (inClip != nil) {
		if (cropRgn) 
			cropRgn->Intersect(*cropRgn, inClip);
		else {
			cropRgn = new MNewRegion ();
			cropRgn->SetRegion(inClip);
			}//else make a copy of the incoming clip region 

		// QTI for SGI crashes if clip outside of dest
		// crop to destRect (which by definition is within NaturalBounds rect)
		MNewRegion destRgn;
		destRgn = xformDest;
		cropRgn->Intersect(*cropRgn, destRgn);
		}//endif there is some incoming clipping

	// we hold an HORef<MRegion>
	// bool(HORef<>) may be false if nothing is stored
	// and bool(*HORef) may be false if a nil crop region is present
	// here's the two step process in figuring that out.
	RgnHandle workingCrop ((RgnHandle)nil);
	if (cropRgn) 
		workingCrop =  *cropRgn ? *cropRgn : (RgnHandle)nil;

	return workingCrop;
} // ResolveCropStuff



#pragma mark -
// R E C T A N G L E M A N I A !!



// ---------------------------------------------------------------------------
// SetCaptionRect
//
// ---------------------------------------------------------------------------
void
PhotoPrintItem::SetCaptionRect(const MRect& inCaptionRect)
{
	mCaptionRect = inCaptionRect;
}//end SetCaptionRect


// ---------------------------------------------------------------------------
// SetCrop
//
//		new crop percentages
// 				measurements are IN from appropriate edge (i.e. 0 == no crop from that edge)
// ---------------------------------------------------------------------------
void			
PhotoPrintItem::SetCrop(double inTopCrop, double inLeftCrop, double inBottomCrop, double inRightCrop)
{
	Assert_(inTopCrop <= 100.0  && inTopCrop >= 0.0);
	Assert_(inLeftCrop <= 100.0 && inLeftCrop >= 0.0);
	Assert_(inBottomCrop <= 100.0 && inBottomCrop >= 0.0);
	Assert_(inRightCrop <= 100.0 && inRightCrop >= 0.0);
	
	mTopCrop = inTopCrop;
	mLeftCrop = inLeftCrop;
	mBottomCrop = inBottomCrop;
	mRightCrop = inRightCrop;
} // SetCrop



void
PhotoPrintItem::SetCropZoomOffset(double inTopOffset, double inLeftOffset) {
	mTopOffset = inTopOffset;
	mLeftOffset = inLeftOffset;
	}//end SetCropZoomOffset


void
PhotoPrintItem::SetCropZoomScales(double inZoomScaleX, double inZoomScaleY) {	
	Assert_(inZoomScaleX > 0);
	Assert_(inZoomScaleY > 0);

	bool needsNewProxy (false);
	if ((inZoomScaleX > mXScale) || (inZoomScaleY > mYScale))
		needsNewProxy = true;
	
	mXScale = inZoomScaleX;
	mYScale = inZoomScaleY;

	if (needsNewProxy)
		DeleteProxy(); // proxy is made while drawing
}//end SetCropZoomScales



// ---------------------------------------------------------------------------
// SetDest
//
// ---------------------------------------------------------------------------
void 			
PhotoPrintItem::SetDest(const MRect& inDest)
{
	mDest = inDest;
	AdjustRectangles();
}//end SetDest


// ---------------------------------------------------------------------------
// SetFrameRect
//
// ---------------------------------------------------------------------------
void			
PhotoPrintItem::SetFrameRect(const MRect& inFrameRect)
{
	mFrameRect = inFrameRect;
}//end SetFrameRect


// ---------------------------------------------------------------------------
// SetImageRect
//
// ---------------------------------------------------------------------------
void			
PhotoPrintItem::SetImageRect(const MRect& inImageRect)
{
	mImageRect = inImageRect;
}//end SetImageRect


// ---------------------------------------------------------------------------
// SetScreenDest
//
// ---------------------------------------------------------------------------
bool
PhotoPrintItem::SetScreenDest(const MRect& inDest)
{
	MatrixRecord	inverse;
	
	SetupDestMatrix(&mMat);
	Boolean happy (::InverseMatrix (&mMat, 
                                    &inverse));
    if (happy) {
		Point	corners[2];
		corners[0] = inDest.TopLeft();
		corners[1] = inDest.BotRight();
		::TransformPoints(&inverse, corners, 2);
		
		MRect xformed (corners[0], corners[1]);

		// now:  move xformed so it's midpoint is same as original midpoint
		
		xformed.Offset(inDest.MidPoint().h - xformed.MidPoint().h,
						inDest.MidPoint().v - xformed.MidPoint().v);
		SetDest(xformed);
		
		return true;
    	}
	else
		return false;                    
}//end SetScreenDest

// ---------------------------------------------------------------------------
// PhotoPrintItem::SetupDestMatrix 
// create the matrix based on destbounds + rotation (SKEW NOT YET IMPLEMENTED)
// ---------------------------------------------------------------------------
void 
PhotoPrintItem::SetupDestMatrix(MatrixRecord* pMat, bool doScale) {
	MRect dest;
	if (!this->IsEmpty() && doScale) {
		GetExpandedOffsetImageRect(dest);
		::RectMatrix(pMat, &GetNaturalBounds(), &dest);
		}//endif know about a file and supposed to scale it into a dest rect
	else {
		::SetIdentityMatrix(pMat);
		}//else just start off w/ identity matrix

	dest =	GetImageRect(); // calculate rotation around midpoint of placement on screen, not expanded offset mess
	Point midPoint (dest.MidPoint());
	::RotateMatrix (pMat, Long2Fix((long)mRot), Long2Fix(midPoint.h), Long2Fix(midPoint.v));
	}//end


#pragma mark -

void 	


// ---------------------------------------------------------------------------
// ParseRect [static]
// ---------------------------------------------------------------------------
PhotoPrintItem::ParseRect(XML::Element &elem, void *userData) {
	XML::Char tmp[64];
	size_t len = elem.ReadData(tmp, sizeof(tmp));
	tmp[len] = 0;
	
	Rect*	pRect ((Rect*)userData);

	SInt16 howMany = sscanf(tmp, "%hd,%hd,%hd,%hd", &pRect->top, &pRect->left, &pRect->bottom, &pRect->right);
	if (howMany != 4) {
		int line = elem.GetInput().GetLine();
		int col = elem.GetInput().GetColumn();
		throw new XML::InvalidValue(elem.GetInput(), line, col);
	}//endif unhappy		
}//end ParseRect


// ---------------------------------------------------------------------------
//Read
// ---------------------------------------------------------------------------
void PhotoPrintItem::Read(XML::Element &elem)
{
	char	filename[256];
	filename[0] = 0;
	double	minVal (-360.0);
	double	maxVal (360.0);
	
	double 	scaleMin (0.0);
	double	scaleMax (100.0);
	
	XML::Handler handlers[] = {
		XML::Handler("bounds", ParseRect, (void*)&mDest),
		//crop stuff
		XML::Handler("topCrop", &mTopCrop),
		XML::Handler("leftCrop", &mLeftCrop),
		XML::Handler("bottomCrop", &mBottomCrop),
		XML::Handler("rightCrop", &mRightCrop),
		XML::Handler("topOffset", &mTopOffset),
		XML::Handler("leftOffset", &mLeftOffset),
		XML::Handler("xScale", &mXScale, &scaleMin, &scaleMax),
		XML::Handler("yScale", &mYScale, &scaleMin, &scaleMax),
		XML::Handler("imageRect", ParseRect, (void*)&mImageRect),
		XML::Handler("captionRect", ParseRect, (void*)&mCaptionRect),
		XML::Handler("frameRect", ParseRect, (void*)&mFrameRect),		
		XML::Handler("maxBounds", ParseRect, (void*)&mMaxBounds),
		XML::Handler("filename", filename, sizeof(filename)),
		XML::Handler("rotation", &mRot, &minVal, &maxVal),
		XML::Handler("skew", &mSkew, &minVal, &maxVal),
		XML::Handler("properties", PhotoItemProperties::ParseProperties, (void*)&mProperties),
		XML::Handler::END
	};
	elem.Process(handlers, this);

	if (strlen(filename)) {
		HORef<MFileSpec> reconstructedSpec = new MFileSpec(filename);	
		mAlias = new MDisposeAliasHandle(reconstructedSpec->MakeAlias());
		ReanimateQTI();
		mQTI = nil;
		}//endif a file was specified (empty means template/placeholder)
}//end Read


// ---------------------------------------------------------------------------
//WriteRect
// ---------------------------------------------------------------------------
void
PhotoPrintItem::WriteRect(XML::Output &out, const char* tagName, const MRect& rect) {
	out.BeginElement(tagName, XML::Output::terse);
	out << rect.top << "," << rect.left << "," << rect.bottom << "," << rect.right;
	out.EndElement(XML::Output::terse);
	}//end WriteRect


// ---------------------------------------------------------------------------
//Write
// ---------------------------------------------------------------------------
void 
PhotoPrintItem::Write(XML::Output &out) 
{
	if (!this->IsEmpty()) {
		HORef<char> path (GetFileSpec()->MakePath());
		out.WriteElement("filename", path);
		}//endif

	WriteRect(out, "bounds", mDest);
	
	//crop stuff
	out.WriteElement("topCrop", mTopCrop);
	out.WriteElement("leftCrop", mLeftCrop);
	out.WriteElement("bottomCrop", mBottomCrop);
	out.WriteElement("rightCrop", mRightCrop);
	out.WriteElement("topOffset", mTopOffset);
	out.WriteElement("leftOffset", mLeftOffset);
	out.WriteElement("xScale", mXScale);
	out.WriteElement("yScale", mYScale);
	
	WriteRect(out, "imageRect", mImageRect);
	WriteRect(out, "captionRect", mCaptionRect);
	WriteRect(out, "frameRect", mFrameRect);
	WriteRect(out, "maxBounds", mMaxBounds);

	out.WriteElement("rotation", mRot);
	out.WriteElement("skew", mSkew);

	out.BeginElement("properties", XML::Output::indent);
	mProperties.Write(out);
	out.EndElement(XML::Output::indent);
}//end Write
