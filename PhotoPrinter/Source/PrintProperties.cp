/*	PrintProperties.cp
		Copyright © 2000 Electric Fish, Inc. All rights reserved.

	Change History (most recent first)

	19 jun	2000		dml		added rotationBehavior, alphabecized
	16 june 2000		dml		read-in rotation

*/

#include "PrintProperties.h"
#include "xmlinput.h"
#include "xmloutput.h"


const char *const PrintProperties::sRotationLabels[kFnordRotation] =
{
	"None", "90CW", "180", "270CW"
};// end sRotationLabels

const char	*const PrintProperties::sMarginLabels[kFnordMargins] =
{
	"Minimal", "HorizontalSym", "VerticalSym", "FullSym", "Custom"
};//end sMarginLabels

const char *const PrintProperties::sRotationBehaviorLabels[kFnordRotateBehavior] =
{
	"Never", "Auto", "Always"
};// end sRotationBehaviorLabels



PrintProperties::PrintProperties() 
	: mFitToPage (false)
	, mRotation (kNoRotation)
	, mRotationBehavior (kNeverRotate)
	, mHiRes (true)
	, mCropMarks (false)
	, mMarginType (kMinimalMargins)
	, mTop (0.0)
	, mLeft (0.0)
	, mBottom (0.0)
	, mRight (0.0)	
	, mOverlap (0.0)
{
}//end mt ct	


PrintProperties::PrintProperties(bool inFit, RotationType inRot, RotationBehavior inBehavior,
				bool inHiRes, bool inCrop, MarginType inMargin,
				float inTop, float inLeft, 
				float inBottom, float inRight,
				float inOverlap)
	: mFitToPage (inFit)
	, mRotation (inRot)
	, mRotationBehavior (inBehavior)
	, mHiRes (inHiRes)
	, mCropMarks (inCrop)
	, mMarginType (inMargin)
	, mTop (inTop)
	, mLeft (inLeft)
	, mBottom (inBottom)
	, mRight (inRight)
	, mOverlap (inOverlap)
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
PrintProperties::GetFit	(void) const
{
	return mFitToPage;
}//end


bool	
PrintProperties::GetHiRes (void) const
{
	return mHiRes;
}//end

bool	
PrintProperties::GetCropMarks(void) const
{
	return mCropMarks;
}//end

PrintProperties::MarginType 
PrintProperties::GetMarginType(void) const
{
	return mMarginType;
}//end

void 	
PrintProperties::GetMargins(float& outTop, float& outLeft, 
					float& outBottom, float& outRight) const
{
	outTop = mTop;
	outLeft = mLeft;
	outBottom = mBottom;
	outRight = mRight;
}//end

float
PrintProperties::GetOverlap(void) const
{	
	return mOverlap;
	}//end GetOverlap


PrintProperties::RotationType
PrintProperties::GetRotation(void) const
{
	return mRotation;
	}//end GetRotation


PrintProperties::RotationBehavior
PrintProperties::GetRotationBehavior(void) const {
 return mRotationBehavior;
 }// end GetRotationBehavior


void 	
PrintProperties::SetFit	(bool inVal){
	mFitToPage = inVal;
}//end

void	
PrintProperties::SetHiRes (bool inVal){
	mHiRes = inVal;
}//end

void	
PrintProperties::SetCropMarks(bool inVal){
	mCropMarks = inVal;
}//end

void 	
PrintProperties::SetMarginType(MarginType inVal){
	mMarginType = inVal;
}//end

void 	
PrintProperties::SetMargins(float inTop, float inLeft, float inBottom, float inRight){
	mTop = inTop;
	mLeft = inLeft;
	mBottom = inBottom;
	mRight = inRight;
}//end

void
PrintProperties::SetOverlap(float inOverlap) {
	mOverlap = inOverlap;
	}//end SetOverlap
	
void
PrintProperties::SetRotation(RotationType inRot) {
	mRotation = inRot;
}//end SetRotation


void
PrintProperties::SetRotationBehavior(RotationBehavior inBehavior) {
	mRotationBehavior = inBehavior;
	}//end SetRotationBehavior





void
PrintProperties::Write	(XML::Output &out) const {
	out.WriteElement("cropMarks", mCropMarks);
	out.WriteElement("fitToPage", mFitToPage);
	out.WriteElement("hiRes", mHiRes);
	out.WriteElement("marginType", sMarginLabels[mMarginType]);
	out.WriteElement("top", mTop);
	out.WriteElement("left", mLeft);
	out.WriteElement("bottom", mBottom);
	out.WriteElement("right", mRight);
	out.WriteElement("overlap", mOverlap);
	out.WriteElement("rotation", sRotationLabels[mRotation]);
	out.WriteElement("rotationBehavior", sRotationBehaviorLabels[mRotationBehavior]);
	}//end write


void
PrintProperties::Read	(XML::Element &elem) {
	float minVal (0.0);
	float maxVal (200000.0);

	XML::Handler handlers[] = {
		XML::Handler("cropMarks", &mCropMarks),
		XML::Handler("fitToPage", &mFitToPage),
		XML::Handler("hiRes", &mHiRes),
		XML::Handler("marginType", sMarginLabels, kFnordMargins, XML_OBJECT_MEMBER(PrintProperties, mMarginType)),
		XML::Handler("top", &mTop, minVal, maxVal),
		XML::Handler("left", &mLeft, minVal, maxVal),
		XML::Handler("bottom", &mBottom, minVal, maxVal),
		XML::Handler("right", &mRight, minVal, maxVal),
		XML::Handler("overlap", &mOverlap, minVal, maxVal),
		XML::Handler("rotation", sRotationLabels, kFnordRotation, XML_OBJECT_MEMBER(PrintProperties, mRotation)),
		XML::Handler("rotationBehavior", sRotationBehaviorLabels, kFnordRotateBehavior,
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

