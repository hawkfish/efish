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
		bool 	fitToPage;
		PhotoPrinter::RotationType rotation;
		bool		hiRes;
		bool		cropMarks;
		MarginType	marginType;
		float		top;
		float		left;
		float		bottom;
		float		right;

	public:
		PrintProperties();
		PrintProperties(bool inFit, PhotoPrinter::RotationType inRot,
						bool hiRes, bool cropMarks, MarginType inMargin,
						float inTop = 0.0, float inLeft = 0.0, 
						float inBottom = 0.0, float inRight = 0.0);
						
		virtual ~PrintProperties();
		
		virtual bool GetFit	(void) const;
		virtual bool	GetHiRes (void) const;
		virtual bool	GetCropMarks(void) const;
		virtual bool GetMarginType(void) const;
		virtual void 	GetMargins(float& outTop, float& outLeft, 
									float& outBottom, float& outRight) const;
		
		virtual void 	SetFit	(bool inVal);
		virtual void	SetHiRes (bool inVal);
		virtual void	SetCropMarks(bool inVal);
		virtual void 	SetMarginType(MarginType inVal);
		virtual void 	SetMargins(float inTop, float inLeft, float inBottom, float inRight);


};//end class PrintProperties