/*
	File:		PhotoPrintItem.h

	Contains:	Definition of an item (i.e. an image).

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

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

#include "HORef.h"
#include "MFileSpec.h"
#include "MNewPicture.h"
#include "MRect.h"
#include "MRegion.h"

#include "ESortedFileList.h"
#include "PhotoItemProperties.h"
#include "PhotoDrawingProperties.h"
#include "StQTImportComponent.h"
#include <UState.h>
#include <UGWorld.h>

namespace XML {
	class Output;
	class Element;
	class Handler;
}

#include <list>

typedef MDisposeHandle<AliasHandle> MDisposeAliasHandle;

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
			si_OtherDimensions
	};

protected:
	MRect							mCaptionRect; 	// caption location
	MRect							mImageRect; 	// the image dest bounds
	MRect							mFrameRect; 	// the frame bounds
	MRect							mDest; 			// sum of image + caption + frame rects

	MRect							mMaxBounds; 	// when empty, this is gross "receivable" 
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

	HORef<MDisposeAliasHandle>		mAlias;
	HORef<MFileSpec>				mFileSpec; // UGH.  only for sorting + serialization.  use alias

	HORef<StQTImportComponent>		mQTI;
	HORef<LGWorld>						mProxy;

	virtual	void	DrawCaption(RgnHandle inClip);
	virtual	void	DrawCaptionText(ConstStr255Param inText, const SInt16 inVerticalOffset, RgnHandle inClip);
	virtual void	DrawEmpty(const PhotoDrawingProperties& props,
							  MatrixRecord* destinationSpace = nil,
							  CGrafPtr destPort = nil,
							  GDHandle destDevice = nil,
							  RgnHandle inClip = nil); 
							  
	virtual	void	DrawImage(	 MatrixRecord*	inLocalSpace,
								 CGrafPtr		inDestPort,
								 GDHandle		inDestDevice,
								 RgnHandle		inClip) ;

	virtual void	DrawProxy(const PhotoDrawingProperties& props,
								MatrixRecord* destinationSpace = nil,
								CGrafPtr destPort = nil,
								GDHandle destDevice = nil,
								RgnHandle inClip = nil);

	static	void	ParseRect(XML::Element &elem, void *userData);
	static	void	WriteRect(XML::Output &out, const char* tagName, const MRect& rect);

	virtual void 			SetupDestMatrix(MatrixRecord* pMat, bool doScale = true);

	virtual bool			CanUseProxy(const PhotoDrawingProperties& props) const;

	// the qti is often not instantiated.  this bottleneck insures all derived fields are consistent
	virtual void			ReanimateQTI(void);
	
public:
							PhotoPrintItem(const MFileSpec& inSpec);
							PhotoPrintItem(PhotoPrintItem& other);
							PhotoPrintItem();
	virtual 				~PhotoPrintItem();

	virtual	UInt32			GetCreatedTime() ;
	virtual	UInt32			GetModifiedTime() ;

	virtual HORef<MFileSpec>&	GetFileSpec(void);
	virtual	void				SetFile(const PhotoPrintItem& inOther);

	// pieces of the geom. desc.
	virtual void 			SetRotation(double inRot) {mRot = inRot;};
	virtual double 			GetRotation() const {return mRot;};
	virtual void 			SetSkew(double inSkew) {mSkew = inSkew;};
	virtual double 			GetSkew() const {return mSkew;};
	
	// R E C T A N G L E M A N I A !!
			
	// max bounds are useful for templates which need placeholders.  
	// (since an item w/ no file/image has no intrinsic bounds)
	virtual const MRect&	GetMaxBounds(void) {return mMaxBounds;};
	virtual void			SetMaxBounds(const MRect& inMax) {mMaxBounds = inMax;};
	
	// dest is orthagonal rect, in untransformed space
	virtual void 			SetDest(const MRect& inDest);
	virtual const MRect 	GetDestRect(void) const;
	// it is also possible to set an explicit screen rect, which
	// results in setting the Dest for the that screen rect transformed
	// by the Inverse of the current matrix.
	virtual bool 			SetScreenDest(const MRect& inDest);

	virtual const MRect&	GetCaptionRect(void) const	{return mCaptionRect;};
	virtual void			SetCaptionRect(const MRect& inCaptionRect);

	virtual const MRect&	GetFrameRect(void)	const	{return mFrameRect;};
	virtual void			SetFrameRect(const MRect& inFrameRect);

	virtual const MRect&	GetImageRect(void)	const	{return mImageRect;};
	virtual void			SetImageRect(const MRect& inImageRect);

	virtual void			GetExpandedOffsetImageRect(MRect& outRect);

	// the all important mapping (usually) from screen to printer
	virtual void			MapDestRect(const MRect& sourceRect, const MRect& destRect);
	// the convoluted construction of cropping region is encapsulated here
	virtual RgnHandle		ResolveCropStuff(HORef<MRegion>& cropRgn, RgnHandle inClip);
	
	// bounds as qt parses the file (image bounds)
	virtual const MRect&	GetNaturalBounds(void) {return mNaturalBounds;};

	// extents of fully transformed bounds (since rotated shape may have bigger bounds)
	virtual MRect			GetTransformedBounds(void);

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
	
	virtual	void			AdjustRectangles();

	// various constraints on operations (not yet used)
	virtual	PhotoItemProperties& GetProperties(void) {return mProperties;};
	
	// the cumulative transform matrix
	virtual void			GetMatrix(MatrixRecord*	pDestMatrix,
										bool inForceRecompute = false);

	// may pass in a matrix for mapping local to dest space.  
	virtual void 			Draw(const PhotoDrawingProperties& props,
								 MatrixRecord* destinationSpace = 0,
								 CGrafPtr destPort = 0,
								 GDHandle destDevice = 0,
								 RgnHandle inClip = nil);

	virtual	void			DeleteProxy(void)		{ mProxy = nil; }
	virtual	OSType			GetDimensions(Str255 outDescriptor, const SInt16 inWhich = si_Dimensions) const;
	virtual void			GetName(Str255& outName);
	virtual bool			IsEmpty(void) const		{ return mAlias == nil; } // do we have contents?
	virtual	bool			IsLandscape() const;
	virtual	bool			IsPortrait() const {return !(IsLandscape());};
	virtual void			MakeProxy(MatrixRecord*	inLocalSpace);
		
// IO
					void 	Write(XML::Output &out) ;
					void 	Read(XML::Element &elem);

// Class globals
static	SInt16	gProxyBitDepth;
static	bool	gUseProxies;
}; // end class PhotoPrintItem
	

// Handy typedefs
typedef PhotoPrintItem*							PhotoItemRef;	
typedef std::list<PhotoItemRef>					PhotoItemList;
typedef	PhotoItemList::iterator					PhotoIterator;
typedef	PhotoItemList::const_iterator			ConstPhotoIterator;
typedef PhotoItemList::reverse_iterator			ReversePhotoIterator;
typedef PhotoItemList::const_reverse_iterator	ConstReversePhotoIterator;
