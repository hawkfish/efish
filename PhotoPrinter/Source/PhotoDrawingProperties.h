/* PhotoDrawingProperties.h
 Copyright © 2000 Electric Fish, Inc.  All Rights Reserved

 20 feb 2001		dml			add kForImage, kForProxy
 19 feb 2001		dml			added kDoRotation, kIgnoreRotation, kDoScale, kIgnoreScale
 17 jan 2001		dml			added mScreenRes, accessors
*/

#pragma once


enum {
	kPreview = true, // constants used in construction of PhotoDrawingProperties
	kNotPreview = false,
	
	kPrinting = true,
	kNotPrinting = false,
	
	kDraft = true,
	kNotDraft = false,
	
	kDoRotation = true,
	kIgnoreRotation = false,
	
	kDoScale = true,
	kIgnoreScale = false,
	
	kForImage = 1,
	kForProxy = 2	
	
	}; // useful constants


class PhotoDrawingProperties {
	protected:
	
		bool	mPrinting;
		bool	mPreview;
		bool	mDraft;
		SInt16	mScreenRes;
	
	public:
	
				PhotoDrawingProperties(bool inPrinting = false,
										bool inPreview = false,
										bool inDraft = false,
										SInt16 inRes = 72) : mPrinting (inPrinting) \
																, mPreview (inPreview) \
																, mDraft (inDraft)  \
																, mScreenRes (inRes) {};
																
				PhotoDrawingProperties(const PhotoDrawingProperties& other) {\
						*this = other;};																
		virtual	~PhotoDrawingProperties(void) {};
		
		
		bool GetPrinting(void) const	{return mPrinting;};
		bool GetPreview(void) const 	{return mPreview;};
		bool GetDraft(void) const		{return mDraft;};
		SInt16	GetScreenRes(void) const {return mScreenRes;};
		
		void SetPrinting(bool inState) 	{mPrinting = inState;};
		void SetPreview(bool inState)	{mPreview = inState;};
		void SetDraft(bool inState)		{mDraft = inState;};
		void SetScreenRes(SInt16 inRes) {mScreenRes = inRes;};
		
	PhotoDrawingProperties& operator=(const PhotoDrawingProperties& other) { \
		SetPrinting(other.GetPrinting());\
		SetPreview(other.GetPreview());\
		SetDraft(other.GetDraft());\
		SetScreenRes(other.GetScreenRes());\
		return *this;};
			
		};//end PhotoDrawingProperties