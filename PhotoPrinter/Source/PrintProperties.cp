//	PrintProperties.cp
//		Copyright © 2000 Electric Fish, Inc. All rights reserved.

#include "PrintProperties.h"


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

