//	PrintProperties.cp
//		Copyright © 2000 Electric Fish, Inc. All rights reserved.

#include "PrintProperties.h"
#include "xmlinput.h"
#include "xmloutput.h"

PrintProperties::PrintProperties() 
	: mFitToPage (false)
	, mRotation (PhotoPrinter::kNoRotation)
	, mHiRes (true)
	, mCropMarks (false)
	, mMarginType (kMinimalMargins)
	, mTop (0.0)
	, mLeft (0.0)
	, mBottom (0.0)
	, mRight (0.0)	
{
}//end mt ct	


PrintProperties::PrintProperties(bool inFit, PhotoPrinter::RotationType inRot,
				bool inHiRes, bool inCrop, MarginType inMargin,
				float inTop, float inLeft, 
				float inBottom, float inRight)
	: mFitToPage (inFit)
	, mRotation (inRot)
	, mHiRes (inHiRes)
	, mCropMarks (inCrop)
	, mMarginType (inMargin)
	, mTop (inTop)
	, mLeft (inLeft)
	, mBottom (inBottom)
	, mRight (inRight)
{
}//end
				
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

bool 
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


const char	*const PrintProperties::sMarginLabels[kFnordMargins] =
{
	"Minimal", "HorizontalSym", "VerticalSym", "FullSym", "Custom"
};//end



void
PrintProperties::Write	(XML::Output &out) const {
	out.WriteElement("fitToPage", mFitToPage);
	out.WriteElement("rotation", PhotoPrinter::GetRotationLabels()[mRotation]);
	out.WriteElement("hiRes", mHiRes);
	out.WriteElement("cropMarks", mCropMarks);
	out.WriteElement("marginType", sMarginLabels[mMarginType]);
	out.WriteElement("top", mTop);
	out.WriteElement("left", mLeft);
	out.WriteElement("bottom", mBottom);
	out.WriteElement("right", mRight);
	}//end write


void
PrintProperties::Read	(XML::Element &elem) {
	float minVal (0.0);
	float maxVal (200000.0);

	XML::Handler handlers[] = {
		XML::Handler("fitToPage", &mFitToPage),
		XML::Handler("hiRes", &mHiRes),
		XML::Handler("cropMarks", &mCropMarks),
		XML::Handler("marginType", sMarginLabels, kFnordMargins, XML_OBJECT_MEMBER(PrintProperties, mMarginType)),
		XML::Handler("top", &mTop, minVal, maxVal),
		XML::Handler("left", &mLeft, minVal, maxVal),
		XML::Handler("bottom", &mBottom, minVal, maxVal),
		XML::Handler("right", &mRight, minVal, maxVal),
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

