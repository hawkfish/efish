/*
	File:		PrintProperties.h

	Contains:	???

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):
	
		22 mar 2001		dml		add mBinderHoles
		13 sep 2000		dml		add header/footer
		24 jul 2000		dml		remove Alternate (app-wide pref)
		21 jul 2000		dml		remove banded printing
		20 jul 2000		dml		add BandedPrinting
		14 Jul 2000		drd		Changed RotationBehavior constants, made it RotationBehaviorT
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

	enum RotationBehaviorT {
		kForceLandscape = 0,						// Starts at 0 for easy serialization
		kPickBestRotation,
		kForcePortrait,
		kFnordRotateBehavior
	};

	protected:
		bool		mBinderHoles;
		bool		mCropMarks;
		bool 		mFitToPage;
		bool		mHiRes;
		MarginType	mMarginType;
		double		mTop;
		double		mLeft;
		double		mBottom;
		double		mRight;
		double		mOverlap;
		double		mHeader;
		double		mFooter;
		RotationType 	mRotation;
		RotationBehaviorT	mRotationBehavior;

		static const char *const gMarginLabels[kFnordMargins];
		static const char *const gRotationLabels[kFnordRotation];
		static const char *const gRotationBehaviorLabels[kFnordRotateBehavior];


	public:
		PrintProperties();
		PrintProperties(bool inFit, RotationType inRot, RotationBehaviorT inBehavior,
						bool hiRes, bool cropMarks, MarginType inMargin,
						double inTop = 0.0, double inLeft = 0.0, 
						double inBottom = 0.0, double inRight = 0.0,
						double inOverlap = 0.0, double inHeader = 0.0, double inFooter = 0.0, bool inBinderHoles = false);
		PrintProperties(const PrintProperties& other);
						
		virtual ~PrintProperties();
		
		virtual bool				GetBinderHoles(void) const;
		virtual bool				GetCropMarks(void) const;
		virtual bool 				GetFit	(void) const;
		virtual double				GetFooter (void) const;
		virtual double				GetHeader(void) const;
		virtual bool				GetHiRes (void) const;
		virtual MarginType			GetMarginType(void) const;
		virtual void 				GetMargins(double& outTop, double& outLeft, 
												double& outBottom, double& outRight) const;
		virtual double				GetOverlap(void) const; 
		virtual RotationType		GetRotation(void) const;
		virtual RotationBehaviorT	GetRotationBehavior(void) const;
		
		virtual void	SetBinderHoles(bool inVal);
		virtual void	SetCropMarks(bool inVal);
		virtual void 	SetFit	(bool inVal);
		virtual void	SetFooter (double inVal);
		virtual void	SetHeader (double inVal);
		virtual void	SetHiRes (bool inVal);
		virtual void 	SetMarginType(MarginType inVal);
		virtual void 	SetMargins(double inTop, double inLeft, double inBottom, double inRight);
		virtual void	SetOverlap(double inOverlap);
		virtual void	SetRotation(RotationType inRotation);
		virtual void	SetRotationBehavior(RotationBehaviorT inBehavior);
// IO
				void 	Write	(XML::Output &out) const;
				void 	Read	(XML::Element &elem);			
		static	void	sParseProperties(XML::Element &elem, void *userData);


};//end class PrintProperties