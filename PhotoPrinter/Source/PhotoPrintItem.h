/*
	File:		PhotoPrintItem.h

	Contains:	Definition of an item (i.e. an image).

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		15 Aug 2001		rmgw	Change rectangle interfaces to return copies.
		02 Aug 2001		drd		Added quality arg to DrawImage
		01 aug 2001		dml		262, 225 CalcImageCaptionRects no longer const
		27 Jul 2001		rmgw	Be vewy careful when hunting proxies.  Bug #244.
		27 jul 2001		dml		GetTransformedBounds protected 212, 217, 224, 236
		26 Jul 2001		rmgw	Factor out XML parsing.  Bug #228.
		25 Jul 2001		drd		15 Removed ESpinCursor arg from Draw
		25 Jul 2001		drd		211 Added inCopyRotateAndSkew arg to CopyForTemplate
		24 jul 2001		dml		constification 
		19 Jul 2001		drd		194 ClearFileSpec
		18 Jul 2001		rmgw	Add ESpinCursor definition.
		12 jul 2001		dml		add Operator= 
		 7 Jul 2001		rmgw	Add full dest rect access.
		 7 Jul 2001		rmgw	Add full alias/file spec access.
		 7 Jul 2001		rmgw	Fix copy/assignment badness.
		06 jul 2001		dml		include EFileSpecProvider, not ESortedFileList.  include <vector>
		 2 Jul 2001		rmgw	Convert item list to vector representation.
		 2 Jul 2001		rmgw	Remove reference in GetName.
		27 Jun 2001		drd		56 added DrawEmpty arg for DrawMissing to use
		14 Jun 2001		rmgw	First pass at handling missing files.  Bug #56.
		16 mar 2001		dml		ResolveCropStuff takes worldSpace matrix
		12 mar 2001		dml		DrawCaption... needs to respect WorldSpace
		06 mar 2001		dml		made GetMaxBounds const, removed SetFile
		28 feb 2001		dml		add operator=
		27 feb 2001		dml		safer (and more correct) for ResolveCropStuff to figure out its own matrices
		19 feb 2001		dml		refactor for rmgw
		01 feb 2001		dml		add MakePict
		17 jan 2001		dml		bug 29.  pass DrawingProperties to DrawCaption, DrawCaptionText
		16 jan 2001		dml		added isTemplate to Write()
		21 sep 2000		dml		MakeProxy and GetProxy don't take any args
		19 Sep 2000		drd		CheckExact* take doubles; FitT
		18 Sep 2000		drd		Added ESpinCursor arg to Draw
		14 Sep 2000		drd		Added si_NaturalBounds; GetNaturalBounds is const
		14 Sep 2000		drd		CheckExactHeight, CheckExactWidth; added arg to GetDimensions
		07 Sep 2000		drd		si_DimensionsInParens
		05 Sep 2000		drd		MakeIcon
		31 aug 2000		dml		added bool arg to IsLandscape, IsPortrait
		29 Aug 2000		drd		GetProxy is back
		25 aug 2000		dml		change LGWorld to EGWorld
		23 aug 2000		dml		change proxy to LGWorld 
		23 aug 2000		dml		change storage of Crop percentages to double
		22 aug 2000		dml		added ReanimateQTI
		21 Aug 2000		drd		Renamed sParseBounds to ParseBounds (functions start with capital)
		18 aug 2000		dml		rewrite cropping (and crop/zoom) to be relative
		16 aug 2000		dml		GetFileSpec returns an HORef<MFileSpec>&
		14 Aug 2000		drd		Made IsEmpty public
		07 aug 2000		dml		add ConstPhotoIterator, ConstReversePhotoIterator
		04 aug 2000		dml		maintain aliases instead of mfilespec as primary source. (filespec still required)
		03 aug 2000		dml		add typdefs for vectors + iterators
		03 Aug 2000		drd		GetCreatedTime, GetModifiedTime; changed DrawCaptionText param
		03 aug 2000		dml		implement FileSpecProvider so that we can be sorted
		14 Jul 2000		drd		IsEmpty looks at spec
		10 jul 2000		dml		moved StQTImportComponent to new file
		10 Jul 2000		drd		DrawCaptionText
		07 Jul 2000		drd		AdjustRectangles, DrawCaption
		07 Jul 2000		drd		GetDimensions
		05 jul 2000		dml		add doScale argument to SetupDestMatrix
		03 Jul 2000		drd		Added GetProxy; MakeProxy is public
		03 Jul 2000		drd		MakeProxy no longer has clip arg
		03 Jul 2000		drd		Added DeleteProxy, DrawImage, gProxyBitDepth, gUseProxies; MakeProxy takes args
		29 jun	2000	dml		add proxy stubs
		27 jun	2000 	dml		added SetScreenBounds
		27 Jun 	2000	drd		IsLandscape, IsPortrait
		27 jun 	2000 	dml		ResolveCropStuff must take an HORef&
		26 Jun 	2000	drd		GetFile, SetFile
		19 june 2000	dml		added mCrop, GetCrop, SetCrop, alphabetized
*/

#pragma once

//	PhotoPrint
#include "PhotoItemProperties.h"
#include "PhotoDrawingProperties.h"

//	Toolbox++
#include "HORef.h"
#include "MAlias.h"
#include "MFileSpec.h"
#include "MNewPicture.h"
#include "MRect.h"
#include "MRegion.h"

//	Epp
#include "EGWorld.h"
#include "EFileSpecProvider.h"
#include "StQTImportComponent.h"

//	STL
#include <vector>

#include <list>

typedef enum {
	kCrop,
	kFit,
	kStretch
} FitT;

// an Item is the fundamental visual-atom of PhotoPrint
// Items have-a
//				underlying quicktime object
//				natural bounds
//				destination bounds
//				rotation
//				(skew -- NOT YET IMPLEMENTED)
//
// Items can
//
//			derive a Matrix from all other geometry settings
//			Draw themselves

class PhotoPrintItem : public FileSpecProvider {


public:
	enum {
		str_ImageSize = 300,
			si_Inches = 1,
			si_Centimetres,
			si_Pixels,
			si_Dimensions,
			si_OtherDimensions,
			si_DimensionsInParens,
			si_NaturalBounds,

		kDimDelta = 36,
		kTinyDelta = 2,

		kEmpty = 'empt',
		kMissing = 'miss',

		// For CopyForTemplate
		kDontCopyRotateAndSkew = false,
		kCopyRotateAndSkew = true,

		// For use in DrawImage
		kPrintQuality = codecMaxQuality,
		kProxyQuality = codecLowQuality
	};
	
	typedef	EGWorld					Proxy;
	typedef	HORef<Proxy>			ProxyRef;
	
protected:
	MRect							mCaptionRect; 	// caption location
	MRect							mImageRect; 	// the image dest bounds
	MRect							mFrameRect; 	// the frame bounds
	MRect							mDest; 			// sum of image + caption + frame rects

	MRect							mMaxBounds; 	// transient -- not serialized.  if empty, this is receivable rect
	MRect							mImageMaxBounds; // transient -- not serialized.  MaxBounds minus CaptionRect etc	
	MRect							mNaturalBounds; // image's intrinsic size

	// cropping and cropzoom support
	double							mXScale; // for cropzoom
	double							mYScale; // for cropzoom
	double							mTopCrop;
	double							mLeftCrop;
	double							mBottomCrop;
	double							mRightCrop;
	double							mTopOffset;
	double							mLeftOffset;

	MatrixRecord					mMat;
	PhotoItemProperties				mProperties;

	double							mRot;
	double							mSkew;

	HORef<MAlias>					mAlias;
	Boolean							mCanResolveAlias;	//	e.g. not in an update event.
	mutable	HORef<MFileSpec>		mFileSpec; // UGH.  only for sorting + serialization.  use alias

	HORef<StQTImportComponent>		mQTI;
	ProxyRef						mProxy;

	// internal routine called by AdjustRectangles, SetMaxBounds.  Sets mImageMaxBounds
	virtual	void			CalcImageCaptionRects(MRect& oImageRect, MRect& oCaptionRect, const MRect& inDest,
													const PhotoDrawingProperties& drawProps) ;

	virtual	void	DrawCaption(MatrixRecord* inWorldSpace, RgnHandle inClip, const PhotoDrawingProperties& props);
	virtual	void	DrawCaptionText(MatrixRecord* inWorldSpace, ConstStr255Param inText, const SInt16 inVerticalOffset, 
									RgnHandle inClip, const PhotoDrawingProperties& props);
	virtual void	DrawEmpty(MatrixRecord* destinationSpace = nil,
							  CGrafPtr destPort = nil,
							  GDHandle destDevice = nil,
							  RgnHandle inClip = nil,
							  const OSType inKind = kEmpty); 
							  
	virtual	void	DrawImage(	 MatrixRecord*	inLocalSpace,
								 CGrafPtr		inDestPort,
								 GDHandle		inDestDevice,
								 RgnHandle		inClip,
								 const CodecQ	inQuality);

	virtual void	DrawMissing(MatrixRecord* destinationSpace = nil,
							  CGrafPtr destPort = nil,
							  GDHandle destDevice = nil,
							  RgnHandle inClip = nil); 

	virtual void	DrawProxy(ProxyRef inProxy, 
								const PhotoDrawingProperties& props,
								MatrixRecord* inLocalSpace,
								CGrafPtr destPort = nil,
								GDHandle destDevice = nil,
								RgnHandle inClip = nil);

	virtual void	DrawIntoNewPictureWithRotation(ProxyRef localProxy, double inRot, const MRect& destBounds, MNewPicture& destPict);	


	// extents of fully transformed bounds (since rotated shape may have bigger bounds)
	virtual MRect			GetTransformedBounds(void);

	virtual void 			SetupDestMatrix(MatrixRecord* pMat, bool doScale = true, bool doRotation = true) const;
	virtual void 			SetupProxyMatrix(ProxyRef inProxy, MatrixRecord* pMat, bool doScale = true, bool doRotation = true) ;

	virtual bool			CanUseProxy(const PhotoDrawingProperties& props) const;

	// the qti is often not instantiated.  this bottleneck insures all derived fields are consistent
	virtual void			ReanimateQTI(void);
	
public:
	
	typedef	MRect			ItemRect;
	
	explicit				PhotoPrintItem (const	Rect&					inCaptionRect,
											const	Rect&					inImageRect,
											const	Rect&					inFrameRect,
											const	Rect&					inDest,

											double							inXScale,
											double							inYScale,
											double							inTopCrop,
											double							inLeftCrop,
											double							inBottomCrop,
											double							inRightCrop,
											double							inTopOffset,
											double							inLeftOffset,

											const	PhotoItemProperties&	inProperties,

											double							inRot,
											double							inSkew,

											HORef<MFileSpec>				inFileSpec);

	explicit				PhotoPrintItem	(const 	FSSpec& inSpec);
							PhotoPrintItem	(const	PhotoPrintItem& other);
							PhotoPrintItem	(void);
	virtual 				~PhotoPrintItem	(void);
	PhotoPrintItem	&operator=	(const PhotoPrintItem&	other);	


	virtual	UInt32			GetCreatedTime() ;
	virtual	UInt32			GetModifiedTime() ;
	
	virtual	AliasHandle		GetAlias (void) const {return *mAlias;};
	virtual	void			AdoptAlias (AliasHandle	inAlias);

	virtual	void			ClearFileSpec()		{ mFileSpec = nil; }
	virtual HORef<MFileSpec>GetFileSpec(void) const;
	virtual void			SetFileSpec(const	FSSpec&	inSpec);

	// pieces of the geom. desc.
	virtual void 			SetRotation(double inRot) {mRot = inRot;};
	virtual double 			GetRotation() const {return mRot;};
	virtual void 			SetSkew(double inSkew) {mSkew = inSkew;};
	virtual double 			GetSkew() const {return mSkew;};
	
	// R E C T A N G L E M A N I A !!
			
	// max bounds are useful for templates which need placeholders.  
	// (since an item w/ no file/image has no intrinsic bounds)
	virtual ItemRect		GetMaxBounds(void) const;
	virtual void			SetMaxBounds(const ItemRect& inMax, const PhotoDrawingProperties& drawProps); 
	virtual ItemRect		GetImageMaxBounds(void) const;
	
	// dest is orthagonal rect, in untransformed space
	virtual void 			SetDest(const ItemRect& inDest);	//	Serialization
	virtual void 			SetDest(const ItemRect& inDest, const PhotoDrawingProperties& drawProps);
	virtual ItemRect 		GetDestRect(void) const;
	// it is also possible to set an explicit screen rect, which
	// results in setting the Dest for the that screen rect transformed
	// by the Inverse of the current matrix.
	virtual bool 			SetScreenDest(const ItemRect& inDest, const PhotoDrawingProperties& drawProps);

	virtual ItemRect		GetCaptionRect(void) const;
	virtual void			SetCaptionRect(const ItemRect& inCaptionRect);

	virtual ItemRect		GetFrameRect(void)	const;
	virtual void			SetFrameRect(const ItemRect& inFrameRect);

	virtual ItemRect		GetImageRect(void)	const;
	virtual void			SetImageRect(const ItemRect& inImageRect);

	virtual void			GetExpandedOffsetImageRect(ItemRect& outRect) const;

	// the all important mapping (usually) from screen to printer
	virtual void			MapDestRect(const ItemRect& sourceRect, const ItemRect& destRect);
	// the convoluted construction of cropping region is encapsulated here
	virtual RgnHandle		ResolveCropStuff(HORef<MRegion>& cropRgn, RgnHandle inClip, MatrixRecord* inWorldSpace);
	
	// bounds as qt parses the file (image bounds)
	virtual ItemRect		GetNaturalBounds(void) const;

	//Cropping (and CropZoom)
	virtual void			SetCrop(double inTopCrop, double inLeftCrop, double inBottomCrop, double inRightCrop);
	virtual void			GetCrop(double& outTopCrop, double& outLeftCrop, double& outBottomCrop, double& outRightCrop) const;
	virtual void			SetCropZoomScales(double inZoomScaleX, double inZoomScaleY);
	virtual void			GetCropZoomScales(double& outZoomScaleX, double& outZoomScaleY) const;
	virtual	void			SetCropZoomOffset(double inTopOffset, double inLeftOffset);
	virtual void			GetCropZoomOffset(double& outTopOffset, double& outLeftOffset);
	virtual bool			HasCrop(void) const;
	virtual bool			HasZoom(void) const;
	virtual void			DeriveCropRect(MRect& outRect);
	
	virtual	void			AdjustRectangles(const PhotoDrawingProperties& drawProps);

	// various constraints on operations (not yet used)
	virtual	PhotoItemProperties& GetProperties(void) {return mProperties;};
	virtual	const PhotoItemProperties& GetProperties(void) const {return mProperties;};
	
	// the cumulative transform matrix
	virtual void			GetMatrix(MatrixRecord*	pDestMatrix,
										bool inDoScale = true,
										bool inDoRotate = true);

	// may pass in a matrix for mapping local to dest space.  
	virtual void 			Draw(const PhotoDrawingProperties& props,
								 MatrixRecord* destinationSpace = 0,
								 CGrafPtr destPort = 0,
								 GDHandle destDevice = 0,
								 RgnHandle inClip = nil);

	virtual	void			CheckExactHeight(
								long& ioWidth, long& ioHeight,
								OSType& outCode, SInt16& outUnits,
								const double inTestWidth, const double inTestHeight,
								const OSType inCode) const;
	virtual	void			CheckExactWidth(
								long& ioWidth, long& ioHeight,
								OSType& outCode, SInt16& outUnits,
								const double inTestWidth, const double inTestHeight,
								const OSType inCode) const;
	virtual	void			DeleteProxy(void)		{ mProxy = nil; }
	virtual	OSType			GetDimensions(Str255 outDescriptor, const SInt16 inResolution, const SInt16 inWhich) const;
	virtual void			GetName(Str255 outName);
	virtual	ProxyRef		GetProxy();
	virtual bool			IsEmpty(void) const		{ return mAlias == nil; } // do we have contents?
	virtual	bool			IsLandscape(bool useNaturalBounds = true) ;
	virtual	bool			IsPortrait(bool useNaturalBounds = true)  {return !(IsLandscape(useNaturalBounds));};

	virtual void			IconTypeToPixelSpec(ResType inType, SInt16& iconSize, SInt16& pixelDepth);
	virtual	Handle			MakeIcon(const ResType inType);
	virtual void			MakeProxy();
	virtual PicHandle		MakePict(const MRect& bounds);
	virtual void			MakeRotatedThumbnails(MNewPicture& io0Rotation, MNewPicture& io90Rotation, 
												MNewPicture& io180Rotation, MNewPicture& io270Rotation,
												const MRect& bounds);
		 
	virtual	void			CopyForTemplate(const PhotoPrintItem& other, const bool inCopyRotateAndSkew = false);

// Class globals
static	SInt16	gProxyBitDepth;
static	bool	gUseProxies;
static 	bool	gDrawMaxBounds;
}; // end class PhotoPrintItem
	

// Handy typedefs
typedef PhotoPrintItem*							PhotoItemRef;	
typedef std::vector<PhotoItemRef>				PhotoItemList;
typedef	PhotoItemList::iterator					PhotoIterator;
typedef	PhotoItemList::const_iterator			ConstPhotoIterator;
typedef PhotoItemList::reverse_iterator			ReversePhotoIterator;
typedef PhotoItemList::const_reverse_iterator	ConstReversePhotoIterator;
