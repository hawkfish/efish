/*
	File:		PrintProperties.cp

	Contains:	???

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first)

		14 Jul 2000		drd		Changed RotationBehavior constants, made it RotationBehaviorT
		26 Jun 2000		drd		Use double, not float
		19 jun	2000	dml		added rotationBehavior, alphabecized
		16 june 2000	dml		read-in rotation
*/

#include "PrintProperties.h"

#include "xmlinput.h"
#include "xmloutput.h"

const char *const PrintProperties::gRotationLabels[kFnordRotation] =
{
	"None", "90CW", "180", "270CW"
};// end sRotationLabels

const char	*const PrintProperties::gMarginLabels[kFnordMargins] =
{
	"Minimal", "HorizontalSym", "VerticalSym", "FullSym", "Custom"
};//end sMarginLabels

const char *const PrintProperties::gRotationBehaviorLabels[kFnordRotateBehavior] =
{
	"portrait", "auto", "landscape"
};// end sRotationBehaviorLabels

PrintProperties::PrintProperties() 
	: mAlternate (false)
	, mCropMarks (false)
	, mFitToPage (false)
	, mHiRes (true)
	, mMarginType (kMinimalMargins)
	, mTop (0.0)
	, mLeft (0.0)
	, mBottom (0.0)
	, mRight (0.0)	
	, mOverlap (0.0)
	, mRotation (kNoRotation)
	, mRotationBehavior(kForcePortrait)			// Default because that's how it comes out of printer
{
}//end mt ct	


PrintProperties::PrintProperties(bool inFit, RotationType inRot, RotationBehaviorT inBehavior,
				bool inHiRes, bool inCrop, MarginType inMargin,
				double inTop, double inLeft, 
				double inBottom, double inRight,
				double inOverlap, bool inAlternate)
	: mAlternate (inAlternate)
	, mCropMarks (inCrop)
	, mFitToPage (inFit)
	, mHiRes (inHiRes)
	, mMarginType (inMargin)
	, mTop (inTop)
	, mLeft (inLeft)
	, mBottom (inBottom)
	, mRight (inRight)
	, mOverlap (inOverlap)
	, mRotation (inRot)
	, mRotationBehavior (inBehavior)
{
}//end
	
	
PrintProperties::PrintProperties(const PrintProperties& other) {
	SetCropMarks(other.GetCropMarks());
	SetFit(other.GetFit());
	SetHiRes(other.GetHiRes());
	SetMarginType(other.GetMarginType());
	other.GetMargins(mTop, mLeft, mBottom, mRight);
	SetOverlap(other.GetOverlap());
	SetRotation(other.GetRotation());
	SetRotationBehavior(other.GetRotationBehavior());
	}//end copy ct	

				
PrintProperties::~PrintProperties(){
}//end


bool
PrintProperties::GetAlternate(void) const
{
	return mAlternate;
	}//end GetAlternate


bool	
PrintProperties::GetCropMarks(void) const
{
	return mCropMarks;
}//end

bool 
PrintProperties::GetFit	(void) const
{
	return mFitToPage;
}//end


bool	
PrintProperties::GetHiRes (void) const
{
	return mHiRes;
}//end


PrintProperties::MarginType 
PrintProperties::GetMarginType(void) const
{
	return mMarginType;
}//end

void 	
PrintProperties::GetMargins(double& outTop, double& outLeft, 
					double& outBottom, double& outRight) const
{
	outTop = mTop;
	outLeft = mLeft;
	outBottom = mBottom;
	outRight = mRight;
}//end

double
PrintProperties::GetOverlap(void) const
{	
	return mOverlap;
	}//end GetOverlap


PrintProperties::RotationType
PrintProperties::GetRotation(void) const
{
	return mRotation;
}//end GetRotation


PrintProperties::RotationBehaviorT
PrintProperties::GetRotationBehavior(void) const {
 return mRotationBehavior;
}// end GetRotationBehavior

#pragma mark -

void
PrintProperties::SetAlternate(bool inVal)
{
	mAlternate = inVal;
}//end SetAlternate

void	
PrintProperties::SetCropMarks(bool inVal){
	mCropMarks = inVal;
}//end

void 	
PrintProperties::SetFit	(bool inVal){
	mFitToPage = inVal;
}//end

void	
PrintProperties::SetHiRes (bool inVal){
	mHiRes = inVal;
}//end



void 	
PrintProperties::SetMarginType(MarginType inVal){
	mMarginType = inVal;
}//end

void 	
PrintProperties::SetMargins(double inTop, double inLeft, double inBottom, double inRight){
	mTop = inTop;
	mLeft = inLeft;
	mBottom = inBottom;
	mRight = inRight;
}//end

void
PrintProperties::SetOverlap(double inOverlap) {
	mOverlap = inOverlap;
	}//end SetOverlap
	
void
PrintProperties::SetRotation(RotationType inRot) {
	mRotation = inRot;
}//end SetRotation


void
PrintProperties::SetRotationBehavior(RotationBehaviorT inBehavior) {
	mRotationBehavior = inBehavior;
}//end SetRotationBehavior

#pragma mark -

void
PrintProperties::Write	(XML::Output &out) const {
	out.WriteElement("alternate", mAlternate);
	out.WriteElement("cropMarks", mCropMarks);
	out.WriteElement("fitToPage", mFitToPage);
	out.WriteElement("hiRes", mHiRes);
	out.WriteElement("marginType", gMarginLabels[mMarginType]);
	out.WriteElement("top", mTop);
	out.WriteElement("left", mLeft);
	out.WriteElement("bottom", mBottom);
	out.WriteElement("right", mRight);
	out.WriteElement("overlap", mOverlap);
	out.WriteElement("rotation", gRotationLabels[mRotation]);
	out.WriteElement("rotationBehavior", gRotationBehaviorLabels[mRotationBehavior]);
	}//end write


void
PrintProperties::Read	(XML::Element &elem) {
	double minVal (0.0);
	double maxVal (200000.0);

	XML::Handler handlers[] = {
		XML::Handler("alternate", &mAlternate),
		XML::Handler("cropMarks", &mCropMarks),
		XML::Handler("fitToPage", &mFitToPage),
		XML::Handler("hiRes", &mHiRes),
		XML::Handler("marginType", gMarginLabels, kFnordMargins, XML_OBJECT_MEMBER(PrintProperties, mMarginType)),
		XML::Handler("top", &mTop, &minVal, &maxVal),
		XML::Handler("left", &mLeft, &minVal, &maxVal),
		XML::Handler("bottom", &mBottom, &minVal, &maxVal),
		XML::Handler("right", &mRight, &minVal, &maxVal),
		XML::Handler("overlap", &mOverlap, &minVal, &maxVal),
		XML::Handler("rotation", gRotationLabels, kFnordRotation, XML_OBJECT_MEMBER(PrintProperties, mRotation)),
		XML::Handler("rotationBehavior", gRotationBehaviorLabels, kFnordRotateBehavior,
										XML_OBJECT_MEMBER(PrintProperties, mRotationBehavior)),
		XML::Handler::END
		}; //handlers
	elem.Process(handlers, this);
	}//end Read


void	
PrintProperties::sParseProperties(XML::Element &elem, void *userData)
{
	PrintProperties* props = (PrintProperties*)userData;
	props->Read(elem);
}//end sParseProperties

