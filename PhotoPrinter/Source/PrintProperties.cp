//	PrintProperties.cp
//		Copyright © 2000 Electric Fish, Inc. All rights reserved.

#include "PrintProperties.h"
#include "xmlinput.h"
#include "xmloutput.h"

PrintProperties::PrintProperties() 
	: fitToPage (false)
	, rotation (PhotoPrinter::kNoRotation)
	, hiRes (true)
	, cropMarks (false)
	, marginType (kMinimalMargins)
	, top (0.0)
	, left (0.0)
	, bottom (0.0)
	, right (0.0)	
{
}//end mt ct	


PrintProperties::PrintProperties(bool inFit, PhotoPrinter::RotationType inRot,
				bool inHiRes, bool inCrop, MarginType inMargin,
				float inTop, float inLeft, 
				float inBottom, float inRight)
	: fitToPage (inFit)
	, rotation (inRot)
	, hiRes (inHiRes)
	, cropMarks (inCrop)
	, marginType (inMargin)
	, top (inTop)
	, left (inLeft)
	, bottom (inBottom)
	, right (inRight)
{
}//end
				
PrintProperties::~PrintProperties(){
}//end


bool 
PrintProperties::GetFit	(void) const
{
	return fitToPage;
}//end


bool	
PrintProperties::GetHiRes (void) const
{
	return hiRes;
}//end

bool	
PrintProperties::GetCropMarks(void) const
{
	return cropMarks;
}//end

bool 
PrintProperties::GetMarginType(void) const
{
	return marginType;
}//end

void 	
PrintProperties::GetMargins(float& outTop, float& outLeft, 
					float& outBottom, float& outRight) const
{
	outTop = top;
	outLeft = left;
	outBottom = bottom;
	outRight = right;
}//end


void 	
PrintProperties::SetFit	(bool inVal){
	fitToPage = inVal;
}//end

void	
PrintProperties::SetHiRes (bool inVal){
	hiRes = inVal;
}//end

void	
PrintProperties::SetCropMarks(bool inVal){
	cropMarks = inVal;
}//end

void 	
PrintProperties::SetMarginType(MarginType inVal){
	marginType = inVal;
}//end

void 	
PrintProperties::SetMargins(float inTop, float inLeft, float inBottom, float inRight){
	top = inTop;
	left = inLeft;
	bottom = inBottom;
	right = inRight;
}//end


const char	*const PrintProperties::sMarginLabels[kFnordMargins] =
{
	"Minimal", "HorizontalSym", "VerticalSym", "FullSym", "Custom"
};//end



void
PrintProperties::Write	(XML::Output &out) const {
	out.WriteElement("fitToPage", fitToPage);
	out.WriteElement("rotation", PhotoPrinter::GetRotationLabels()[rotation]);
	out.WriteElement("hiRes", hiRes);
	out.WriteElement("cropMarks", cropMarks);
	out.WriteElement("marginType", sMarginLabels[marginType]);
	out.WriteElement("top", top);
	out.WriteElement("left", left);
	out.WriteElement("bottom", bottom);
	out.WriteElement("right", right);
	}//end write


void
PrintProperties::Read	(XML::Element &elem) {
	float minVal (0.0);
	float maxVal (200000.0);

	XML::Handler handlers[] = {
		XML::Handler("fitToPage", &fitToPage),
		XML::Handler("hiRes", &hiRes),
		XML::Handler("cropMarks", &cropMarks),
		XML::Handler("marginType", sMarginLabels, kFnordMargins, XML_OBJECT_MEMBER(PrintProperties, marginType)),
		XML::Handler("top", &top, minVal, maxVal),
		XML::Handler("left", &left, minVal, maxVal),
		XML::Handler("bottom", &bottom, minVal, maxVal),
		XML::Handler("right", &right, minVal, maxVal),
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

