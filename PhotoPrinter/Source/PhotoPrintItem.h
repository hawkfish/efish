/*
	File:		PhotoPrintItem.h

	Contains:	Definition of the application class.

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

	27 jun	2000 	dml		added SetScreenBounds
	27 Jun 2000		drd		IsLandscape, IsPortrait
	27 jun 2000 	dml		ResolveCropStuff must take an HORef&
	26 Jun 2000		drd		GetFile, SetFile
	19 june 2000	dml		added mCrop, GetCrop, SetCrop, alphabetized
*/

#pragma once
#include "MFileSpec.h"
#include "MRect.h"
#include "HORef.h"
#include "MRegion.h"

#include "PhotoItemProperties.h"
#include "PhotoDrawingProperties.h"

namespace XML {
	class Output;
	class Element;
	class Handler;
}


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




/*******************************
* StQTImportComponent
*	utility class to open/close an import component for a file
*	usage ensures that the QT component is closed if object destructs (even if user's ct fails)
********************************/
class StQTImportComponent {

	GraphicsImportComponent	mGI;

	public:
		StQTImportComponent(const MFileSpec* inSpec);
		virtual ~StQTImportComponent();

		operator GraphicsImportComponent (void)
			{ return mGI; };
	}; //end class StQTImportComponent



class PhotoPrintItem {

	protected:
		MRect							mCrop;
		MRect							mDest;
		MatrixRecord					mMat;
		MRect							mMaxBounds; // when empty, this is gross "receivable" 
		MRect							mNaturalBounds;
		PhotoItemProperties				mProperties;
		double							mRot;
		double							mSkew;
		HORef<MFileSpec>				mSpec;
		HORef<StQTImportComponent>		mQTI;
		
		virtual void	DrawEmpty(const PhotoDrawingProperties& props,
								  MatrixRecord* destinationSpace = nil,
								  CGrafPtr destPort = nil,
								  GDHandle destDevice = nil,
								  RgnHandle inClip = nil); // and what do we look like when empty?
		virtual bool	IsEmpty(void) const { return !mQTI; } // do we have contents?

				void 	ParseBounds(XML::Element &elem, void *userData);
		static	void	sParseBounds(XML::Element &elem, void *userData);

		virtual void 	SetupDestMatrix(MatrixRecord* pMat);
	
	public:
	
								PhotoPrintItem(const MFileSpec& inSpec);
								PhotoPrintItem(PhotoPrintItem& other);
								PhotoPrintItem();
		virtual 				~PhotoPrintItem();


		virtual	MFileSpec*		GetFile() const		{ return (MFileSpec*)mSpec; }
		virtual	void			SetFile(const PhotoPrintItem& inOther);

		// pieces of the geom. desc.
		virtual void 			SetRotation(double inRot) {mRot = inRot;};
		virtual double 			GetRotation() const {return mRot;};
		virtual void 			SetSkew(double inSkew) {mSkew = inSkew;};
		virtual double 			GetSkew() const {return mSkew;};
		
		// max bounds are useful for templates which need placeholders.  
		virtual const MRect&	GetMaxBounds(void) {return mMaxBounds;};
		virtual void			SetMaxBounds(const MRect& inMax) {mMaxBounds = inMax;};
		
		// dest is orthagonal rect, in display (screen or printer) space
		virtual void 			SetDest(const MRect& inDest);
		virtual const MRect& 	GetDestRect(void) const {return mDest;};
		virtual bool 			SetScreenDest(const MRect& inDest);

		// the all important mapping (usually) from screen to printer
		virtual void			MapDestRect(const MRect& sourceRect, const MRect& destRect);
		// the convoluted construction of cropping region is encapsulated here
		virtual RgnHandle		ResolveCropStuff(HORef<MRegion>& cropRgn, RgnHandle inClip);
		
		// bounds as qt parses the file
		virtual const MRect&	GetNaturalBounds(void) {return mNaturalBounds;};

		// extents of fully transformed bounds (since rotated shape may have bigger bounds)
		virtual MRect			GetTransformedBounds(void);

		//Cropping
		virtual void			SetCrop(const MRect& inCrop);
		virtual const MRect&	GetCrop(void) const {return mCrop;};
		

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

		virtual ConstStr255Param	GetName();
		virtual	bool			IsLandscape() const;
		virtual	bool			IsPortrait() const;
			
// IO
					void 	Write(XML::Output &out) const;
					void 	Read(XML::Element &elem);

	}; // end class PhotoPrintItem
	
	
typedef PhotoPrintItem* PhotoItemRef;	
	