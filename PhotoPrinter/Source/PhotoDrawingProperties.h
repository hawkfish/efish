// PhotoDrawingProperties.h
// Copyright © 2000 Electric Fish, Inc.  All Rights Reserved
#pragma once

class PhotoDrawingProperties {
	protected:
	
		bool	mPrinting;
		bool	mPreview;
		bool	mDraft;
	
	public:
	
				PhotoDrawingProperties(bool inPrinting = false,
										bool inPreview = false,
										bool inDraft = false) : mPrinting (inPrinting) \
																, mPreview (inPreview) \
																, mDraft (inDraft) {};
																
				PhotoDrawingProperties(const PhotoDrawingProperties& other) {\
						*this = other;};																
		virtual	~PhotoDrawingProperties(void) {};
		
		
		bool GetPrinting(void) const	{return mPrinting;};
		bool GetPreview(void) const 	{return mPreview;};
		bool GetDraft(void) const		{return mDraft;};
		
		void SetPrinting(bool inState) 	{mPrinting = inState;};
		void SetPreview(bool inState)	{mPreview = inState;};
		void SetDraft(bool inState)		{mDraft = inState;};
		
	PhotoDrawingProperties& operator=(const PhotoDrawingProperties& other) { \
		SetPrinting(other.GetPrinting());\
		SetPreview(other.GetPreview());\
		SetDraft(other.GetDraft());\
		return *this;};
			
		};//end PhotoDrawingProperties