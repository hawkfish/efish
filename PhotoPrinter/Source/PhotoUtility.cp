/*
	File:		PhotoUtility.cp

	Contains:	Handy functions and constants

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

	21 sep 2000		dml		add kHardwiredHeaderSize
	19 Sep 2000		drd		GetSize and support
	21 aug 2000		dml		lowered epsilon to 0.00001
*/

#include "PhotoUtility.h"

const double PhotoUtility::sEpsilon = 0.00001;

PhotoUtility::SizeMap	PhotoUtility::gSizeMap;
const RGBColor			PhotoUtility::sNonReproBlue = {38912,57856, 57856};
const double		 	PhotoUtility::kRad2Degrees = 57.2958;
const double 			PhotoUtility::kHardwiredHeaderSize = 0.333; /*inches*/

/*
GetSize
*/
void
PhotoUtility::GetSize(const OSType inType, double& outWidth, double& outHeight)
{
	if (gSizeMap.empty())
		InitializeSizeMap();

	DoubleSize	size = gSizeMap[inType];
	outWidth = size.first;
	outHeight = size.second;
} // GetSize

/*
InitializeSizeMap
*/
void
PhotoUtility::InitializeSizeMap()
{
	// Note that we are photo sizes (like a 3*5 card), not paper
	// Portrait
	gSizeMap['3*2 '] = DoubleSize(2, 3);
	gSizeMap['5*3 '] = DoubleSize(3, 5);
	gSizeMap['6*4 '] = DoubleSize(4, 6);
	gSizeMap['7*5 '] = DoubleSize(5, 7);
	gSizeMap['10*7'] = DoubleSize(7.5, 10);

	gSizeMap['2*3 '] = DoubleSize(3, 2);
	gSizeMap['3*5 '] = DoubleSize(5, 3);
	gSizeMap['4*6 '] = DoubleSize(6, 4);
	gSizeMap['5*7 '] = DoubleSize(7, 5);
	gSizeMap['7*10'] = DoubleSize(10, 7.5);
} // InitializeSizeMap