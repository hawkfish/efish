// PhotoPrintItem.h
// Copyright © 2000 Electric Fish, Inc.  All Rights Reserved
#pragma once
#include "MFileSpec.h"
#include "MRect.h"
#include "HORef.h"

#include "PhotoItemProperties.h"

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
		StQTImportComponent(const MFileSpec& inSpec);
		virtual ~StQTImportComponent();

		operator GraphicsImportComponent (void)
			{return mGI;};
	}; //end class StQTImportComponent



class PhotoPrintItem {

	protected:
		MFileSpec		mSpec;
		MRect			mNaturalBounds;
		MRect			mDest;
		double			mRot;
		double			mSkew;
		MatrixRecord	mMat;
		StQTImportComponent	mQTI;
		PhotoItemProperties	mProperties;
		
		virtual void SetupDestMatrix(MatrixRecord* pMat);
	
	public:
	
								PhotoPrintItem(const MFileSpec& inSpec);
								PhotoPrintItem(PhotoPrintItem& other);
		virtual 				~PhotoPrintItem();
	
		// pieces of the geom. desc.
		virtual void 			SetRotation(double inRot) {mRot = inRot;};
		virtual double 			GetRotation() const {return mRot;};
		virtual void 			SetSkew(double inSkew) {mSkew = inSkew;};
		virtual double 			GetSkew() const {return mSkew;};
		
		// dest is orthagonal rect, in display (screen or printer) space
		virtual void 			SetDest(const MRect& inDest) {mDest = inDest;};
		virtual const MRect& 	GetDestRect(void) const {return mDest;};
		// the all important mapping (usually) from screen to printer
		virtual void			MapDestRect(const MRect& sourceRect, const MRect& destRect);
		
		// bounds as qt parses the file
		virtual const MRect&	GetNaturalBounds(void) {return mNaturalBounds;};

		// extents of fully transformed bounds (since rotated shape may have bigger bounds)
		virtual MRect			GetTransformedBounds(void);

		// various constraints on operations (not yet used)
		virtual	PhotoItemProperties& GetProperties(void) {return mProperties;};
		
		// the cumulative transform matrix
		virtual void			GetMatrix(MatrixRecord*	pDestMatrix,
											bool inForceRecompute = false);

		// may pass in a matrix for mapping local to dest space.  
		virtual void 			Draw(MatrixRecord* destinationSpace = 0,
									 CGrafPtr destPort = 0,
									 GDHandle destDevice = 0);
			
	}; // end class PhotoPrintItem
	
	
typedef PhotoPrintItem* PhotoItemRef;	
	