/*
	File:		PhotoPrintItem.cp

	Contains:	Definition of an item (i.e. an image).

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

	 7 Jul 2001		rmgw	Add full dest rect access; fix alias/file access and construction. 
	 7 Jul 2001		rmgw	Add full alias/file spec access.
	 7 Jul 2001		rmgw	Fix copy/assignment badness.
	06 Jul 2001		drd		128 MakeProxy calls SetWatch
	06 jul 2001		dml		CalcImageCaptionRects must handle empty NaturalBounds (templates strike again!)
	05 jul 2001		dml		don't copy rotation in Operator= (see comments below)
	03 jul 2001		dml		104, 25.  Rotation of Caption controlled by ItemProperty
	02 jul 2001		dml		remove StValueChanger blocks in Draw concerning debugging of exceptions
	02 jul 2001		dml		17.  changed order of matrix concat in DrawCaptionText
	 2 Jul 2001		rmgw	Remove reference in GetName.
	27 Jun 2001		drd		Changed color of debug gDrawMaxBounds to chartreuse; 56 added DrawEmpty
							arg for DrawMissing, added try/catch to GetCreatedTime, GetModifiedTime
	26 Jun 2001		drd		88 GetFileSpec can return nil, test for this
	25 Jun 2001		drd		85 Default constructor initializes mCanResolveAlias
	14 Jun 2001		rmgw	First pass at handling missing files.  Bug #56.
	25 Apr 2001		drd		removed clipping hack in DrawEmpty
	06 Apr 2001		drd		Handle printing of empty item
	22 Mar 2001		drd		Hacks to try to get image under new PowerPlant
	15 mar 2001		dml		txtSize calcs done always.  copy ct copies maxbounds, ResolveCropStuff handles worldSpace
	14 mar 2001		dml		fix some crop handling
	12 mar 2001		dml		fix captions to respect WorldSpace
	09 mar 2001		dml		Draw must call ::TransformRgn w/ worldspace on resolveCropRgn
	07 mar 2001		dml		IsLandscape uses naturalbounds, not imageRect
	06 mar 2001		dml		bug 54.  GetDimensions more attentive to idealized sizes (gridlayout sets in MaxBounds)
	01 mar 2001		dml		mMaxBounds now transient, not serialized
	28 feb 2001		dml		operator=, changes to AdjustRectangles to support operator= when used w/ templates
	26 feb 2001		dml		ResolveCropStuff no longer accepts matrix
	19 feb 2001		dml		refactor for rmgw, fix bug 3
	01 feb 2001		dml		add MakePict
	17 jan 2001		dml		DrawCaption, DrawCaptionText attentive to on-screen resolution (zooming) bug 29
	16 jan 2001		dml		added isTemplate to Write()
	11 Dec 2000		drd		GetDimensions uses 3.5 inch
	05 Oct 2000		drd		Use std:: for sscanf
	21 sep 2000		dml		fix leak caused by MakeProxy calling DrawImage.  DrawImage now nils mQTI
	21 sep 2000		dml		MakeProxy (and GetProxy) no longer take matrix, since create w/o rotation
	20 sep 2000		dml		MakeProxy uses SilentExceptionEater
	19 Sep 2000		drd		DrawProxyIntoPicHandle is paranoid about there being a current port
	19 Sep 2000		drd		CheckExact* take doubles
	19 sep 2000		dml		DrawIntoNewPictureWithRotation makes minimally sized dest picture
	18 Sep 2000		drd		DrawCaptionText, rather than DrawCaption, erases for caption_Inside with no rotation
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
#include "PhotoExceptionHandler.h"
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
bool	PhotoPrintItem::gDrawMaxBounds = false;			// handy for debugging

// ---------------------------------------------------------------------------
// PhotoPrintItem empty constructor
// ---------------------------------------------------------------------------

PhotoPrintItem::PhotoPrintItem (void)

	: mXScale (1.0)
	, mYScale (1.0)
	, mTopCrop (0)
	, mLeftCrop (0)
	, mBottomCrop (0)
	, mRightCrop (0)
	, mTopOffset (0.0)
	, mLeftOffset (0.0)

	, mRot (0.0)
	, mSkew (0.0)

	, mCanResolveAlias (true)

{

	::SetIdentityMatrix (&mMat);

}//end empty ct


// ---------------------------------------------------------------------------
// PhotoPrintItem copy constructor
// ---------------------------------------------------------------------------

PhotoPrintItem::PhotoPrintItem(

	const PhotoPrintItem& other) 

	: mCaptionRect (other.GetCaptionRect())
	, mImageRect (other.GetImageRect())
	, mFrameRect (other.GetFrameRect())
	, mDest (other.GetDestRect())

	, mMaxBounds (other.GetMaxBounds())
	, mImageMaxBounds (other.GetImageMaxBounds())
	, mNaturalBounds (other.GetNaturalBounds())
	
	, mXScale (other.mXScale)
	, mYScale (other.mYScale)
	, mTopCrop (other.mTopCrop)
	, mLeftCrop (other.mLeftCrop)
	, mBottomCrop (other.mBottomCrop)
	, mRightCrop (other.mRightCrop)
	, mTopOffset (other.mTopOffset)
	, mLeftOffset (other.mLeftOffset)
	
	//, mMat (other.mMat)	//	No T++ object so done below
	, mProperties (other.GetProperties())

	, mRot (other.GetRotation())
	, mSkew (other.GetSkew())

	, mAlias (other.mAlias)
	, mCanResolveAlias (true)	// ??? why not copied? Because no one will clear it - rmgw
	, mFileSpec (other.mFileSpec)

	, mQTI (other.mQTI)

	, mProxy (other.mProxy)
	
{
	// could recompute, but hey, it's a copy constructor
	::CopyMatrix(&(other.mMat), &mMat);	

}//end copy ct


// ---------------------------------------------------------------------------
// PhotoPrintItem constructor
// ---------------------------------------------------------------------------
PhotoPrintItem::PhotoPrintItem(

	const MFileSpec& inSpec)
	
	: mXScale (1.0)
	, mYScale (1.0)
	, mTopCrop (0)
	, mLeftCrop (0)
	, mBottomCrop (0)
	, mRightCrop (0)
	, mTopOffset (0.0)
	, mLeftOffset (0.0)
	
	, mRot (0.0)
	, mSkew (0.0)
	
	, mAlias (new MDisposeAliasHandle(inSpec.MakeAlias ()))
	, mCanResolveAlias (true)
	, mFileSpec (new MFileSpec (inSpec, false))
	
{
	
	::SetIdentityMatrix (&mMat);

	ReanimateQTI(); // make it just for side effects
	mQTI = nil;		// throw it away
	//	What bloody "side effects"???

}//end ct
	

// ---------------------------------------------------------------------------
// ~PhotoPrintItem destructor
// ---------------------------------------------------------------------------
PhotoPrintItem::~PhotoPrintItem() {

	}//end dt



#pragma mark -

// ---------------------------------------------------------------------------
// AdjustRectangles
//	Adjust rectangles depending on caption.  
// ---------------------------------------------------------------------------
void
PhotoPrintItem::AdjustRectangles(const PhotoDrawingProperties& drawProps)
{
	MRect		oldImageRect(mImageRect);

	MRect bogusRect;
	// first pass gets the correct imageRect, based on Dest
	CalcImageCaptionRects(mImageRect, bogusRect, mDest, drawProps);

	// second pass gets the correct captionRect, based on Max
	CalcImageCaptionRects(bogusRect, mCaptionRect, mMaxBounds, drawProps);
	

	// IFF there was an old imageRect (not an empty template space)
	// AND Rectangles changed and are bigger, invalidate proxy
	if (oldImageRect &&
		(oldImageRect.Width() < mImageRect.Width() ||
		oldImageRect.Height() < mImageRect.Height()))
		this->DeleteProxy();
} // AdjustRectangles



// ---------------------------------------------------------------------------
// AdoptAlias
// ---------------------------------------------------------------------------
void
PhotoPrintItem::AdoptAlias(

	AliasHandle	inAlias)
	
{ // begin AdoptAlias
	
	mAlias = new MDisposeAliasHandle (inAlias);
	mFileSpec = 0;
	
	ReanimateQTI(); // make it just for side effects
	mQTI = nil;		// throw it away
	//	What bloody "side effects"???
	
} // end AdoptAlias
	

void
PhotoPrintItem::CalcImageCaptionRects(MRect& oImageRect, MRect& oCaptionRect,
										const MRect& inMax,
										const PhotoDrawingProperties& drawProps) const
{
	const PhotoItemProperties&	props(this->GetProperties());

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
		
		SInt16 txtLineHeight (props.GetCaptionLineHeight());
		txtLineHeight *= ((double)drawProps.GetScreenRes()) / 72.0;
		height = lines * txtLineHeight; // must scale to screen res!!

		oImageRect = inMax;

		// a template has no natural bounds, only max bounds
		MRect extentsBasis;
		if (this->GetNaturalBounds())
			extentsBasis = this->GetNaturalBounds();
		else
			extentsBasis = this->GetMaxBounds();
			
		switch (props.GetCaptionStyle()) {
			case caption_Bottom:
				oImageRect.SetHeight(oImageRect.Height() - height);

				AlignmentGizmo::FitAndAlignRectInside(extentsBasis,
													oImageRect,
													kAlignAbsoluteCenter,
													oImageRect,
													EUtil::kDontExpand);

				oCaptionRect = inMax;
				oCaptionRect.top = oCaptionRect.bottom - height;
				break;

			case caption_Inside:
				oCaptionRect = inMax;
				oCaptionRect.top = oCaptionRect.bottom - height;
				width = min(max((long)kNarrowestCaption, inMax.Width() / 3), inMax.Width());
				oCaptionRect.SetWidth(width);
				break;

			case caption_RightHorizontal:
				SInt16 captionWidth = inMax.Width() * kRightHorizontalCoefficient;;
				oImageRect.SetWidth(inMax.Width() - captionWidth);
				AlignmentGizmo::FitAndAlignRectInside(extentsBasis,
													oImageRect,
													kAlignAbsoluteCenter,
													oImageRect,
													EUtil::kDontExpand);
				oCaptionRect = oImageRect;
				oCaptionRect.SetWidth(captionWidth);
				oCaptionRect.Offset(oImageRect.Width(), 0); // place it next to
				// !!! should center vertically
				break;

			case caption_RightVertical:
				oImageRect.SetWidth(oImageRect.Width() - height);

				AlignmentGizmo::FitAndAlignRectInside(extentsBasis,
													oImageRect,
													kAlignAbsoluteCenter,
													oImageRect,
													EUtil::kDontExpand);

				oCaptionRect = inMax;
				oCaptionRect.left = oCaptionRect.right - height;
				break;

		}
	} else {
		oImageRect = inMax;
		oCaptionRect = MRect();		// Make it empty
	}

}//end CalcImageCaptionRects


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
// CopyForTemplate.  Note:  does NOT copy the Dest Rect
//		because its intended use it for replacement into an existing location (templates!)
//		AdjustRectangles is called, which derives all other rects from old destRect
//			with the new properties (caption, etc)
//
// 5 jul 2001.  also don't copy rotation/skew, so that template's values can override
// ---------------------------------------------------------------------------
void
PhotoPrintItem::CopyForTemplate	(

	const PhotoPrintItem&	other) 
	
{
	
	mAlias = other.mAlias;
	mFileSpec = other.mFileSpec;
	
	mNaturalBounds = other.GetNaturalBounds();
	mTopCrop = other.mTopCrop;
	mLeftCrop = other.mLeftCrop;
	mBottomCrop = other.mBottomCrop;
	mRightCrop = other.mRightCrop;
	mXScale = other.mXScale;
	mYScale = other.mYScale;
	mTopOffset = other.mTopOffset;
	mLeftOffset = other.mLeftOffset;
	mQTI = other.mQTI;
	mProperties = other.GetProperties();

	// hopefully this is a big speed-up
	mProxy = other.mProxy;

	

	//we don't copy the rectangles from the other object
	PhotoDrawingProperties defaultProps;
	AdjustRectangles(defaultProps);

}//end CopyForTemplate

// ---------------------------------------------------------------------------
// DeriveCropRect
//
// crop is kept as 4 doubles representing percentage offsets from each of 4 sides
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
	StValueChanger<Boolean>	saveCanResolveAlias (mCanResolveAlias, false);

	if (gDrawMaxBounds && mMaxBounds) {
		StColorPenState saveState;
		StColorPenState::Normalize();
		RGBColor		chartreuse = { 38912, 57856, 1000 };
		::RGBForeColor(&chartreuse);
		::PenSize(2, 2);
		MRect			copyMaxBounds (mMaxBounds);
		if (worldSpace)
			::TransformRect(worldSpace, &copyMaxBounds, NULL);
		::FrameRect(&copyMaxBounds);
		}//endif debugging the cell

	bool				useProxy (this->CanUseProxy(props) && (this->GetProxy() != nil));
	if (!useProxy) mProxy = nil; // ensure that if we aren't using proxy, it's deleted (mainly debugging issue)
	
	try {
		MatrixRecord	imageSpace;
		SetupDestMatrix(&imageSpace, kDoScale, kDoRotation);

		MatrixRecord	compositeSpace;
		if (useProxy) // if we're drawing proxy, matrix is slightly different (since proxy size differs)
			SetupProxyMatrix(&compositeSpace, kDoScale, kDoRotation);
		else // else start with the imageSpace matrix
			::CopyMatrix(&imageSpace, &compositeSpace);
		if (worldSpace) // composite in any worldspace xforms
			::ConcatMatrix(worldSpace, &compositeSpace);
					
		HORef<MRegion>	cropRgn;
		RgnHandle		workingCrop(this->ResolveCropStuff(cropRgn, inClip, worldSpace));

		do {
			if (this->IsEmpty()) {
				if (!props.GetPrinting()) {
					this->DrawEmpty(&compositeSpace, inDestPort, inDestDevice, workingCrop);
				}
				continue;
			} //endif empty

			if (useProxy) {
				this->DrawProxy(props, &compositeSpace, inDestPort, inDestDevice, workingCrop);
				continue;
				}

			// If we have no proxy (for whatever reason), spin the watch because drawing will take a while
			if (mProxy == nil && inCursor != nil)
				inCursor->Spin();
			//and draw the image
			this->DrawImage(&compositeSpace, inDestPort, inDestDevice, workingCrop);
		} while (false);

		if (this->GetProperties().HasCaption()) {
			this->DrawCaption(worldSpace, inClip, props); //caption should just deal with inner clip (cause image cropping could screw it up)
		}
	}//end try
	catch (...) {
		// An exception during drawing is pretty bad news.
		mQTI = nil;
		throw;
	}//end catch
	
	mQTI = nil; // if we've made it during this draw operation, make sure to free it here
} // Draw

void
PhotoPrintItem::DrawCaption(MatrixRecord* inWorldSpace, RgnHandle inPassthroughClip, const PhotoDrawingProperties& drawProps)
{
	PhotoItemProperties&	props(this->GetProperties());
	MPString			theCaption(props.GetCaption());

	StColorPenState		restorePen;
	StTextState			restoreText;
	::RGBForeColor(&Color_Black);

	SInt16				offset = 0;
	SInt16				captionLineHeight (props.GetCaptionLineHeight());
	captionLineHeight *= ((double)drawProps.GetScreenRes()) / 72.0;
	

	if (theCaption.Length() > 0) {
		this->DrawCaptionText(inWorldSpace, theCaption, offset, inPassthroughClip, drawProps);
		offset += captionLineHeight;
	}

	if (! this->IsEmpty() && props.GetShowName()) {
		Str255	fileName;
		this->GetName(fileName);
		this->DrawCaptionText(inWorldSpace, fileName, offset, inPassthroughClip, drawProps);
		offset += captionLineHeight;
	}

	if (! this->IsEmpty() && props.GetShowDate()) {
		LStr255			date;
		UInt32			modTime = this->GetModifiedTime();
		if (modTime != 0) {
			EChrono::GetDateTime(date, modTime,
				PhotoPrintPrefs::Singleton()->GetDateFormat(), PhotoPrintPrefs::Singleton()->GetTimeFormat());
			this->DrawCaptionText(inWorldSpace, date, offset, inPassthroughClip, drawProps);
		}
		offset += captionLineHeight;
	}
} // DrawCaption

/*
DrawCaptionText
*/
void
PhotoPrintItem::DrawCaptionText(MatrixRecord* inWorldSpace, ConstStr255Param inText, const SInt16 inVerticalOffset, 
								RgnHandle inClip, const PhotoDrawingProperties& drawProps)
{
	MRect				bounds(mCaptionRect);

	// and respect any worldspace xform that has come in
	if (inWorldSpace) // composite in any worldspace xforms
		::TransformRect(inWorldSpace, &bounds, NULL);


	// setup the matrix
	MatrixRecord		mat;
	::SetIdentityMatrix(&mat);

	MatrixRecord		rotator;
	bool				additionalRotation = false;
	if (this->GetProperties().GetCaptionStyle() == caption_RightVertical) {
		additionalRotation = true;
		Point			midPoint = bounds.MidPoint(); //rotate around center of full caption rect
		::SetIdentityMatrix(&rotator);
		::RotateMatrix(&rotator, ::Long2Fix(270), ::Long2Fix(midPoint.h), ::Long2Fix(midPoint.v));
		// and we have to change the rectangle
		::TransformRect(&rotator, &bounds, nil);
	}

	// Take the offset into account (this moves down to individual lines in the caption)
	bounds.top += inVerticalOffset;

	// start with a translate to topleft of caption rect
	::TranslateMatrix(&mat, ::FixRatio(bounds.left, 1), ::FixRatio(bounds.top, 1));

	// If the caption is in a rotated style, include that transformation
	if (additionalRotation) {
		::ConcatMatrix(&rotator, &mat);
	}

	// then any rotation happens around center of image rect
	if (mProperties.GetCaptionLinkedRotation() &&
		(!PhotoUtility::DoubleEqual(mRot, 0.0))) {
		MRect		dest (this->GetImageRect());
		if (inWorldSpace)
			::TransformRect(inWorldSpace, &dest, NULL);
		Point		midPoint = dest.MidPoint();
		::RotateMatrix(&mat, ::Long2Fix(static_cast<long>(mRot)),
			::Long2Fix(midPoint.h), ::Long2Fix(midPoint.v));
	}


	{
	// Use a StQuicktimeRenderer to draw rotated text (we only make one if we have to, both as an
	// optimization, and to work around a Mac OS X DP4 bug)
	HORef<StQuicktimeRenderer>		qtr;
	if (additionalRotation || !PhotoUtility::DoubleEqual(mRot, 0.0))
		qtr = new StQuicktimeRenderer(bounds, 1, useTempMem, &mat, inClip);
	else {
		// We're not rotating, so we won't be blitting white pixels from the GWorld. We may
		// want to draw our own white pixels.
		if (this->GetProperties().GetCaptionStyle() == caption_Inside) {
			::RGBBackColor(&Color_White);
			bounds.Erase();
		}
	}
	::TextFont(this->GetProperties().GetFontNumber());
	SInt16 txtSize (this->GetProperties().GetFontSize());
	txtSize *= ((double)drawProps.GetScreenRes()) / 72.0;
	::TextSize(txtSize);
	Ptr					text = (Ptr)(inText);	// I couldn't get this to work with 1 C++ cast
	UTextDrawing::DrawWithJustification(text + 1, ::StrLength(inText), bounds, teJustCenter, true);
	}//end QTRendering block
} // end DrawCaptionText

/*
DrawEmpty
*/
void
PhotoPrintItem::DrawEmpty(MatrixRecord* localSpace, // already composited and ready to use
						 CGrafPtr inDestPort,
						 GDHandle inDestDevice,
						 RgnHandle inClip,
						 const OSType inKind)
{
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
// !?!?	StClipRgnState::Normalize();

	if (inKind == kMissing) {
		RGBColor	fireEngine = { 65000, 0, 0 };
		::RGBForeColor(&fireEngine);
	} else {
		::RGBForeColor(&PhotoUtility::sNonReproBlue);
	}
	
	HORef<StClipRgnState>	saveClip;
	if (inClip != nil) {
		saveClip = new StClipRgnState (inClip);
		}//endif clipping to do

	::MoveTo(corners[kTopLeft].h, corners[kTopLeft].v);
	::LineTo(corners[kTopRight].h, corners[kTopRight].v);
	::LineTo(corners[kBotRight].h, corners[kBotRight].v);
	::LineTo(corners[kBotLeft].h, corners[kBotLeft].v);
	::LineTo(corners[kTopLeft].h, corners[kTopLeft].v);
	if (inKind == kMissing) {
		::PenSize(2, 2);
	}
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
	try {
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
		} // try
		
	catch (...) {
		DrawMissing (inLocalSpace, inDestPort, inDestDevice, inClip);
		} // catch
		
	// free 'dat QT memory!!
	mQTI = nil; // if we've made it during this draw operation, make sure to free it here
		
} // DrawImage

// ---------------------------------------------------------------------------
// DrawMissing
//	Draws a missing image.  for now it just looks like an empty image.
// ---------------------------------------------------------------------------
void
PhotoPrintItem::DrawMissing(
	MatrixRecord*	inLocalSpace, // already composited and ready to use
	 CGrafPtr		inDestPort,
	 GDHandle		inDestDevice,
	 RgnHandle		inClip) 
{
	this->DrawEmpty(inLocalSpace, inDestPort, inDestDevice, inClip, kMissing);
} // DrawMissing

// ---------------------------------------------------------------------------
// DrawProxy
// ---------------------------------------------------------------------------
void
PhotoPrintItem::DrawProxy(const PhotoDrawingProperties& props,
						 MatrixRecord* inLocalSpace, // already composited and ready to use
						 CGrafPtr inDestPort,
						 GDHandle inDestDevice,
						 RgnHandle inClip)
{
	HORef<StClipRgnState> saveClip;
	if (inClip != nil)
		saveClip = new StClipRgnState(inClip);
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
						inLocalSpace,
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


//-------------------------------------------------------------------------------------
//	DrawIntoNewPictureWithRotation
//-------------------------------------------------------------------------------------

void
PhotoPrintItem::DrawIntoNewPictureWithRotation(double inRot, const MRect& destBounds, MNewPicture& destPict)
{
	MRect			imageBounds;
	
	// MakeRotatedThumbnails has already tried to make the proxy (if needed) and lock it down (if avail)
	if (GetProxy() != nil) {
		GetProxy()->GetBounds(imageBounds);
		imageBounds.Offset(-imageBounds.left, -imageBounds.top);
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
	::RotateMatrix(&mat, ::Long2Fix(inRot ), 
						::Long2Fix(destBounds.MidPoint().h), ::Long2Fix(destBounds.MidPoint().v));
	// be a good citizen and setup clipping
	MNewRegion clip;
	clip = destBounds; 


	// rotate the aspectDest rect so we can make a minimal sized picture
	MatrixRecord rotOnly;
	::SetIdentityMatrix(&rotOnly);
	::RotateMatrix(&rotOnly, ::Long2Fix(inRot ), 
					::Long2Fix(destBounds.MidPoint().h), ::Long2Fix(destBounds.MidPoint().v));		
	MRect rotAspectDest (aspectDest);
	::TransformRect(&rotOnly, &rotAspectDest, nil);


	EGWorld			offscreen(rotAspectDest, gProxyBitDepth);
	GWorldPtr		theGWorld = offscreen.GetMacGWorld();
	GDHandle		offscreenDevice(::GetGWorldDevice(theGWorld));
	StGrafPortSaver	savePort(theGWorld);		// Out of paranoia, make sure we've got a port

	// render the rotated proxy
	PhotoDrawingProperties	props (kNotPrinting, kPreview, kDraft);
	if (GetProxy() != nil)
		this->DrawProxy(props, &mat, theGWorld, offscreenDevice, clip);
	else {
		if (this->IsEmpty())
			this->DrawEmpty(&mat, theGWorld, offscreenDevice, clip);
		else
			this->DrawImage(&mat, theGWorld, offscreenDevice, clip);
		}//else no proxy
	// now we need to blit that offscreen into another, while a Picture is open to capture it into a pict
	LGWorld			offscreen2(rotAspectDest, gProxyBitDepth);
	if (offscreen2.BeginDrawing()) {
		MNewPicture			pict;			// Creates a PICT and destroys it
		{
			MOpenPicture	openPicture(pict, rotAspectDest);
			offscreen.CopyImage(UQDGlobals::GetCurrentPort(), rotAspectDest, ditherCopy);
		}
		offscreen2.EndDrawing();

		destPict.Attach(pict.Detach());		// Transfer ownership of the PICT
	} // if
}//end DrawIntoNewPictureWithRotation

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
	const double	inTestWidth,
	const double	inTestHeight,
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
	const double	inTestWidth,
	const double	inTestHeight,
	const OSType	inCode) const
{
	if (abs((long)(ioWidth - (inTestWidth * kDPI))) < kTinyDelta && 
		abs((long)(ioHeight - (inTestHeight * kDPI))) < kDimDelta) {
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

		// if maxBounds is set, then use that, otherwise use actual bounds
		MRect referenceBounds;
		if (this->GetMaxBounds()) // boolean operator returns empty or not
			referenceBounds = GetMaxBounds();
		else
			referenceBounds = GetDestRect();

		width = referenceBounds.Width() * ((double)kDPI / (double)inResolution);
		height = referenceBounds.Height() * ((double)kDPI / (double)inResolution);
		this->CheckExactHeight(width, height, code, unitIndex, 2, 3, '3*2 ');
		this->CheckExactWidth(width, height, code, unitIndex, 3, 2, '2*3 ');
		this->CheckExactHeight(width, height, code, unitIndex, 3.5, 5, '5*3 ');
		this->CheckExactWidth(width, height, code, unitIndex, 5, 3.5, '3*5 ');
		this->CheckExactHeight(width, height, code, unitIndex, 4, 6, '6*4 ');
		this->CheckExactWidth(width, height, code, unitIndex, 6, 4, '4*6 ');
		this->CheckExactHeight(width, height, code, unitIndex, 5, 7, '7*5 ');
		this->CheckExactWidth(width, height, code, unitIndex, 7, 5, '5*7 ');
		this->CheckExactHeight(width, height, code, unitIndex, 7.5, 10, '10*7');
		this->CheckExactWidth(width, height, code, unitIndex, 10, 7.5, '7*10');
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
//	GetExpandedOffsetImageRect  
//		returns the size of the entire image at current resolution offset to appear correcty in cropwindow
//		take the image rect.  scale it by our cropzoom factors, 
//		and offset in case we dragged the image within the crop'd window
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
//		Note that we may return nil (especially in the case of a placeholder)
// ---------------------------------------------------------------------------
HORef<MFileSpec>
PhotoPrintItem::GetFileSpec() const
{
	if (mCanResolveAlias && (mAlias != nil)) {
		Boolean outChanged;
		HORef<MFileSpec>	newSpec (new MFileSpec(outChanged, *mAlias));
		if (outChanged || (mFileSpec == nil))
			mFileSpec = newSpec;
	}
		
	return mFileSpec;
} // GetFileSpec


// ---------------------------------------------------------------------------
// GetMatrix 
// (might be stale but faster if don't force recompute)
// ---------------------------------------------------------------------------
void
PhotoPrintItem::GetMatrix(MatrixRecord* pDestMatrix,
							bool inDoScale,
							bool inDoRotate)
{
	this->SetupDestMatrix(pDestMatrix, inDoScale, inDoRotate);
}//end GetMatrix	

// ---------------------------------------------------------------------------
// GetCreatedTime.  useful for 
// ---------------------------------------------------------------------------
UInt32
PhotoPrintItem::GetCreatedTime() 
{
	UInt32		theTime = 0;

	if (!this->IsEmpty()) {
		// Don't put up error dialogs in the middle of this
		StDisableDebugThrow_();
		StDisableDebugSignal_();
		try {
			CInfoPBRec		info;
			this->GetFileSpec()->GetCatInfo(info);
			theTime = info.hFileInfo.ioFlCrDat;
		} catch (...) {
			// Eat exceptions; the most likely one here is a file no longer being present,
			// and there are other places we should notice that
		}
	}

	return theTime;
} // GetCreatedTime

// ---------------------------------------------------------------------------
// GetModifiedTime
// ---------------------------------------------------------------------------
UInt32
PhotoPrintItem::GetModifiedTime() 
{
	UInt32		theTime = 0;

	if (!this->IsEmpty()) {
		// Don't put up error dialogs in the middle of this
		StDisableDebugThrow_();
		StDisableDebugSignal_();
		try {
			CInfoPBRec		info;
			this->GetFileSpec()->GetCatInfo(info);
			theTime =  info.hFileInfo.ioFlMdDat;
		} catch (...) {
			// Eat exceptions; the most likely one here is a file no longer being present,
			// and there are other places we should notice that
		}
	}

	return theTime;
} // GetModifiedTime


// ---------------------------------------------------------------------------
// GetName.  not unicode ready
// ---------------------------------------------------------------------------
void
PhotoPrintItem::GetName(Str255 outName)
{
	if (this->IsEmpty())
		outName[0] = 0;
	else
		::memcpy(outName, this->GetFileSpec()->Name(), sizeof(Str255));
}//end GetName


// ---------------------------------------------------------------------------
//	GetProxy
//		tries hard to return a valid proxy.  will make new if purged or non-existant
// ---------------------------------------------------------------------------
HORef<EGWorld>
PhotoPrintItem::GetProxy() 
{
	// If the proxy pixels were purged, delete the proxy (and start afresh)
	if ((mProxy != nil) && (mProxy->IsPurged()))
		mProxy = nil;

	if (mProxy == nil)
		this->MakeProxy();

	return mProxy;
} // GetProxy


// ---------------------------------------------------------------------------
// GetTransformedBounds 
// 		once we are rotated, our bounding box changes
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
	
	MatrixRecord m;
	SetupDestMatrix(&m, kIgnoreScale, kDoRotation);
	::TransformPoints(&m, corners, 4);
	
	MRect bounds;
	bounds.left = min(min(corners[0].h, corners[1].h), min(corners[2].h, corners[3].h));
	bounds.right = max(max(corners[0].h, corners[1].h), max(corners[2].h, corners[3].h));
	bounds.top = min(min(corners[0].v, corners[1].v), min(corners[2].v, corners[3].v));
	bounds.bottom = max(max(corners[0].v, corners[1].v), max(corners[2].v, corners[3].v));

	return bounds;
	}//end	 


// ---------------------------------------------------------------------------
// HasCrop
// ---------------------------------------------------------------------------
bool
PhotoPrintItem::HasCrop() const {
	return (!(PhotoUtility::DoubleEqual(mTopCrop,0.0) &&
			PhotoUtility::DoubleEqual(mLeftCrop,0.0) &&
			PhotoUtility::DoubleEqual(mRightCrop,0.0) &&
			PhotoUtility::DoubleEqual(mBottomCrop,0.0)));
	}//end HasCrop


// ---------------------------------------------------------------------------
// HasZoom
// ---------------------------------------------------------------------------
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
	MRect bounds (GetNaturalBounds()); 

	//handle case where we're empty placeholder and don't have imageRect yet
	if (!bounds)
		bounds = GetMaxBounds();
	
	if (!useNaturalBounds) {
		MatrixRecord m;
		SetupDestMatrix(&m, kDoScale, kDoRotation);
		::TransformRect(&m, &bounds, nil);
		}//endif

	return bounds.Width() >= bounds.Height();

} // IsLandscape


// ---------------------------------------------------------------------------
// IconTypeToPixelSpec
// ---------------------------------------------------------------------------
void
PhotoPrintItem::IconTypeToPixelSpec(ResType inType, SInt16& iconSize, SInt16& pixelDepth) {

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
	}//switch

}//end IconTypeToPixelSpec


// ---------------------------------------------------------------------------
//	MakeIcon
//	Creates and returns a handle. Caller owns the handle. 
// ---------------------------------------------------------------------------
Handle
PhotoPrintItem::MakeIcon(const ResType inType)
{
	this->GetProxy();											// Be certain our proxy is present

	SInt16		iconSize;
	SInt16		pixelDepth;

	IconTypeToPixelSpec(inType, iconSize, pixelDepth);
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
// 	the proxy is an EGWorld which is cheaper to draw than whatever QTI we have.
// we need to create the proxy w/o rotation, since we will draw it later with rotation
// ---------------------------------------------------------------------------
void
PhotoPrintItem::MakeProxy()
{
	if (this->IsEmpty())
		return;

	UCursor::SetWatch();

	try {
		SilentExceptionEater silence;
		StDisableDebugThrow_();
		
		if (mQTI == nil && mAlias) 
			ReanimateQTI();

		StGrafPortSaver				savePort;		// Be sure we're in the right port even if there's a throw

		MatrixRecord rectOnlyMatrix;
		SetupDestMatrix(&rectOnlyMatrix, kDoScale, kIgnoreRotation); // proxy is made w/o rotation
	
		//	Create the offscreen GWorld
		MRect					bounds;
		GetExpandedOffsetImageRect(bounds);	
		// make the proxy as unpurgable at first, since we need to draw into it
		mProxy = new EGWorld (bounds, gProxyBitDepth, 0, 0, 0, EGWorld::kNeverTryTempMem, kNoPurge);

		//	Draw into it
		if (mProxy->BeginDrawing ()) {
			this->DrawImage(&rectOnlyMatrix, mProxy->GetMacGWorld(), ::GetGDevice(), nil);
			mProxy->EndDrawing();
			} //endif able to lock + draw
		mProxy->SetPurgeable(true);

	} catch (...) {
		// Swallow the exception, and set mProxy to nil (also mQTI in case DrawImage failed)
		mProxy = nil;
		mQTI = nil;
	}

} // MakeProxy


// ---------------------------------------------------------------------------
//	MakeRotatedThumbnails
// ---------------------------------------------------------------------------
void
PhotoPrintItem::MakeRotatedThumbnails(MNewPicture& io0Rotation, MNewPicture& io90Rotation, 
										MNewPicture& io180Rotation, MNewPicture& io270Rotation, 
										const MRect& bounds)
{
	if (IsEmpty()) return;
	
	HORef<StPixelState> possibleProxyLocker = nil;
	//try to ensure there is a proxy to draw
	if (GetProxy() != nil) {
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
//MakePreview.  Caller owns returned Handle
// ---------------------------------------------------------------------------
PicHandle
PhotoPrintItem::MakePict(const MRect& bounds) 
{
	MNewPicture preview;

	HORef<StPixelState> possibleProxyLocker = nil;
	//try to ensure there is a proxy to draw
	if (GetProxy() != nil) {
		possibleProxyLocker = new StPixelState(mProxy->GetMacGWorld());
		mProxy->SetPurgeable(false);
		}//endif there is a proxy, lock it down for the duration of this function
			
	DrawIntoNewPictureWithRotation(0.0, bounds, preview);
	return preview.Detach();
	}//end MakePreview



// ---------------------------------------------------------------------------
// ReanimateQTI
// attempt to make a QTI from the filespec corresponding to our alias
// Throw if nil, or if unable to get bounds
// ---------------------------------------------------------------------------
void
PhotoPrintItem::ReanimateQTI() {
	if ((mAlias != nil) && (mQTI == nil)){
		Assert_(this->GetFileSpec() != nil);
		mQTI  = new StQTImportComponent(GetFileSpec());
		ThrowIfNil_(mQTI);
		ComponentResult res;
		res = ::GraphicsImportGetNaturalBounds (*mQTI, &mNaturalBounds);
		ThrowIfOSErr_(res);				
	}//endif enough knowledge to attempt reanimation
}//end ReandimateQTI


// ---------------------------------------------------------------------------
// ResolveCropStuff
//
// takes our crop region + transforms it by an incoming matrix
// then, clips that against an incoming clip region
// return a RgnHandle that actually points to the HORef<MRegion> which has been passed in
// caller in effect has always owned any region which gets created here
// ---------------------------------------------------------------------------
RgnHandle
PhotoPrintItem::ResolveCropStuff(HORef<MRegion>& cropRgn, RgnHandle inClip, MatrixRecord* inWorldSpace)
{
	MRect xformDest (GetTransformedBounds());
	
	// do we have intrinsic cropping?
	if (HasCrop()) {
		cropRgn = new MNewRegion;
		MRect derivedRect;
		DeriveCropRect(derivedRect);
	
		// now, xform those points (rotation, scale, skew, etc)
		Point corners[4];
		corners[0] = derivedRect.TopLeft();
		corners[2] = derivedRect.BotRight();
		corners[1].v = corners[0].v; // topRight
		corners[1].h = corners[2].h;
		corners[3].v = corners[2].v; // bottomLeft
		corners[3].h = corners[0].h;		

		MatrixRecord m;
		SetupDestMatrix(&m, kIgnoreScale, kDoRotation); // we want to know about rotation, but not scale or offset
		::TransformPoints(&m/*inXform*/, corners, 4);
	
		cropRgn->Open();
		::MoveTo(corners[0].h, corners[0].v);
		::LineTo(corners[1].h, corners[1].v);
		::LineTo(corners[2].h, corners[2].v);
		::LineTo(corners[3].h, corners[3].v);
		::LineTo(corners[0].h, corners[0].v);
		cropRgn->Close();
		
		// fake out clip bug present in some QTImporters.  (Perhaps this is confusing some print drivers?)
		// by creating a tiny region on the topline, union'ed with the rest
		MNewRegion fakey;
		fakey = MRect (xformDest.top, xformDest.left, xformDest.top + 1, xformDest.left + 1);

		cropRgn->Union(*cropRgn, fakey);
		if (inWorldSpace)
			::TransformRgn(inWorldSpace, *cropRgn);
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
		MRect doubleXformDest (xformDest);
		if (inWorldSpace)
			::TransformRect(inWorldSpace, &doubleXformDest, NULL);
		destRgn = doubleXformDest;
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



// ---------------------------------------------------------------------------
// SetCropZoomOffset
//
// ---------------------------------------------------------------------------
void
PhotoPrintItem::SetCropZoomOffset(double inTopOffset, double inLeftOffset) {
	mTopOffset = inTopOffset;
	mLeftOffset = inLeftOffset;
	}//end SetCropZoomOffset


// ---------------------------------------------------------------------------
// SetCropZoomScales
//
// ---------------------------------------------------------------------------
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
//	For serialization clients
void 			
PhotoPrintItem::SetDest(const MRect& inDest)
{
	mDest = inDest;
}//end SetDest


// ---------------------------------------------------------------------------
// SetDest
//
// ---------------------------------------------------------------------------
void 			
PhotoPrintItem::SetDest(const MRect& inDest, const PhotoDrawingProperties& drawProps)
{
	SetDest (inDest);
	AdjustRectangles(drawProps);
}//end SetDest


// ---------------------------------------------------------------------------
// SetFileSpec
//
// ---------------------------------------------------------------------------
void			
PhotoPrintItem::SetFileSpec(const FSSpec& inSpec)
{
	AdoptAlias (MFileSpec (inSpec).MakeAlias ());
	
	GetFileSpec ();		//	Force a resolve
	
}//end SetFrameRect


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


void
PhotoPrintItem::SetMaxBounds(const MRect& inMax, const PhotoDrawingProperties& drawProps) {
	if (inMax != mMaxBounds) {
		mMaxBounds = inMax;
		
		MRect bogusCaption;
		CalcImageCaptionRects(mImageMaxBounds, bogusCaption, mMaxBounds, drawProps);
		}//endif need to change
}//end

// ---------------------------------------------------------------------------
// SetScreenDest
//
// ---------------------------------------------------------------------------
bool
PhotoPrintItem::SetScreenDest(const MRect& inDest, const PhotoDrawingProperties& drawProps)
{
	MatrixRecord	inverse;
	
	MatrixRecord m;
	SetupDestMatrix(&m);
	Boolean happy (::InverseMatrix (&m, 
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
		SetDest(xformed, drawProps);
		
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
PhotoPrintItem::SetupDestMatrix(MatrixRecord* pMat, bool doScale, bool doRotation) {
	MRect dest;
	if (!this->IsEmpty() && doScale) {
		GetExpandedOffsetImageRect(dest);
		::RectMatrix(pMat, &GetNaturalBounds(), &dest);
		}//endif know about a file and supposed to scale it into a dest rect
	else {
		::SetIdentityMatrix(pMat);
		}//else just start off w/ identity matrix

	if (doRotation) {
		dest =	GetImageRect(); // calculate rotation around midpoint of placement on screen, not expanded offset mess
		Point midPoint (dest.MidPoint());
		::RotateMatrix (pMat, Long2Fix((long)mRot), Long2Fix(midPoint.h), Long2Fix(midPoint.v));
		}//endif doing rotation

	}//end SetupDestMatrix


// ---------------------------------------------------------------------------
// SetupProxyMatrix uses proxy rects to compute scale/rotate matrix
// ---------------------------------------------------------------------------
void
PhotoPrintItem::SetupProxyMatrix(MatrixRecord* pMat, bool doScale, bool doRotation) {
	if ((GetProxy() == nil) || (!doScale && !doRotation)){
		::SetIdentityMatrix(pMat);
		}//endif nothing can be done
	else {
		MRect imageRect = GetImageRect();
		if (doScale) {
			MRect proxyBounds;
			GetProxy()->GetBounds(proxyBounds);
			proxyBounds.Offset(-proxyBounds.left, -proxyBounds.top); //ensure at origin for RectMatrix to Xlate
			
			MRect dest;
			GetExpandedOffsetImageRect(dest);
			::RectMatrix(pMat, &proxyBounds, &dest);			
			}//end doScale

		if (doRotation) {
			Point midPoint (imageRect.MidPoint());
			::RotateMatrix (pMat, Long2Fix((long)mRot), Long2Fix(midPoint.h), Long2Fix(midPoint.v));
			}//endif doing rotation
		}//else

}//end SetupProxyMatrix

#pragma mark -

// ---------------------------------------------------------------------------
// ParseRect [static]
// ---------------------------------------------------------------------------
void 	
PhotoPrintItem::ParseRect(XML::Element &elem, void *userData) {
	XML::Char tmp[64];
	size_t len = elem.ReadData(tmp, sizeof(tmp));
	tmp[len] = 0;
	
	Rect*	pRect ((Rect*)userData);

	SInt16 howMany = std::sscanf(tmp, "%hd,%hd,%hd,%hd", &pRect->top, &pRect->left, &pRect->bottom, &pRect->right);
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
//		XML::Handler("maxBounds", ParseRect, (void*)&mMaxBounds),
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
PhotoPrintItem::Write(XML::Output &out, bool isTemplate) 
{
	if ((!isTemplate) && (!this->IsEmpty())) {
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
//	WriteRect(out, "maxBounds", mMaxBounds);

	out.WriteElement("rotation", mRot);
	out.WriteElement("skew", mSkew);

	out.BeginElement("properties", XML::Output::indent);
	mProperties.Write(out);
	out.EndElement(XML::Output::indent);
}//end Write
