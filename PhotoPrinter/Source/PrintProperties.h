//	PrintProperties.h
//		Copyright © 2000 Electric Fish, Inc. All rights reserved.

#pragma once
#include "PhotoPrinter.h"

class PrintProperties {
	public:
		enum MarginType {
			kMinimalMargins = 0,
			kHorizontalSymmetric,
			kVerticalSymmetric,
			kFullSymmetric,
			kCustom,
			kFnordMargins};

	protected:
		Boolean 	fitToPage;
		PhotoPrinter::RotationType rotation;
		Boolean		hiRes;
		Boolean		cropMarks;
		MarginType	marginType;
		float		top;
		float		left;
		float		bottom;
		float		right;

	public:
		PrintProperties();
		PrintProperties(Boolean inFit, PhotoPrinter::RotationType inRot,
						Boolean hiRes, Boolean cropMarks, MarginType inMargin,
						float inTop = 0.0, float inLeft = 0.0, 
						float inBottom = 0.0, float inRight = 0.0);
						
		virtual ~PrintProperties();
		
		virtual Boolean GetFit	(void) const;
		virtual Boolean	GetHiRes (void) const;
		virtual Boolean	GetCropMarks(void) const;
		virtual Boolean GetMarginType(void) const;
		virtual void 	GetMargins(float& outTop, float& outLeft, 
									float& outBottom, float& outRight) const;
		
		virtual void 	SetFit	(Boolean inVal);
		virtual void	SetHiRes (Boolean inVal);
		virtual void	SetCropMarks(Boolean inVal);
		virtual void 	SetMarginType(MarginType inVal);
		virtual void 	SetMargins(float inTop, float inLeft, float inBottom, float inRight);


};//end class PrintProperties