/*
	File:		PrintProperties.h

	Contains:	???

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		26 Jun 2000		drd		Use double, not float
		21 jun 2000		dml		added mAlternate, GetAlternate, SetAlternate
		19 jun 2000		dml		removed #include "PhotoPrinter.h"
		19 jun 2000 	dml		added RotationBehavior, alphabetized
		16 june 2000 	dml		added mOverlap
*/

#pragma once

namespace XML {
	class Output;
	class Element;
	class Handler;
}

class PrintProperties {
	public:
		enum MarginType {
			kMinimalMargins = 0,
			kHorizontalSymmetric,
			kVerticalSymmetric,
			kFullSymmetric,
			kCustom,
			kFnordMargins};

	enum RotationType {
		kNoRotation = 0,
		k90CWRotation,
		k180Rotation,
		k270CWRotation,
		kFnordRotation };

	enum RotationBehavior {
		kNeverRotate = 0,
		kAutoRotate,
		kAlwaysRotate,
		kFnordRotateBehavior };

	protected:
		bool		mAlternate;
		bool		mCropMarks;
		bool 		mFitToPage;
		bool		mHiRes;
		MarginType	mMarginType;
		double		mTop;
		double		mLeft;
		double		mBottom;
		double		mRight;
		double		mOverlap;
		RotationType 	mRotation;
		RotationBehavior mRotationBehavior;

		static const char *const sMarginLabels[kFnordMargins];
		static const char *const sRotationLabels[kFnordRotation];
		static const char *const sRotationBehaviorLabels[kFnordRotateBehavior];


	public:
		PrintProperties();
		PrintProperties(bool inFit, RotationType inRot, RotationBehavior inBehavior,
						bool hiRes, bool cropMarks, MarginType inMargin,
						double inTop = 0.0, double inLeft = 0.0, 
						double inBottom = 0.0, double inRight = 0.0,
						double inOverlap = 0.0, bool inAlternate = false);
		PrintProperties(const PrintProperties& other);
						
		virtual ~PrintProperties();
		
		virtual bool				GetAlternate(void) const;
		virtual bool				GetCropMarks(void) const;
		virtual bool 				GetFit	(void) const;
		virtual bool				GetHiRes (void) const;
		virtual MarginType			GetMarginType(void) const;
		virtual void 				GetMargins(double& outTop, double& outLeft, 
												double& outBottom, double& outRight) const;
		virtual double				GetOverlap(void) const; 
		virtual RotationType		GetRotation(void) const;
		virtual RotationBehavior	GetRotationBehavior(void) const;
		
		virtual void	SetAlternate(bool inVal);
		virtual void	SetCropMarks(bool inVal);
		virtual void 	SetFit	(bool inVal);
		virtual void	SetHiRes (bool inVal);
		virtual void 	SetMarginType(MarginType inVal);
		virtual void 	SetMargins(double inTop, double inLeft, double inBottom, double inRight);
		virtual void	SetOverlap(double inOverlap);
		virtual void	SetRotation(RotationType inRotation);
		virtual void	SetRotationBehavior(RotationBehavior inBehavior);
// IO
				void 	Write	(XML::Output &out) const;
				void 	Read	(XML::Element &elem);			
		static	void	sParseProperties(XML::Element &elem, void *userData);


};//end class PrintProperties