/*
	File:		PhotoUtility.cp

	Contains:	Handy functions and constants

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

	06 jul 2001		dml		move gNeedDoubleOrientationSetting here, default is True!
	28 Jun 2001		drd		75 101 DrawXformedRect handles kFillHilite
	11 Dec 2000		drd		Photo size is 3.5*5, not 3*5
	06 dec 2000		dml		add CalcLineHeight();
	22 Sep 2000		drd		Moved DrawXformedRect here (and added paint, inval)
	21 sep 2000		dml		add kHardwiredHeaderSize
	19 Sep 2000		drd		GetSize and support
	21 aug 2000		dml		lowered epsilon to 0.00001
*/

#include "PhotoUtility.h"

#include "MNewRegion.h"

const double PhotoUtility::sEpsilon = 0.00001;

PhotoUtility::SizeMap	PhotoUtility::gSizeMap;
const RGBColor			PhotoUtility::sNonReproBlue = {38912,57856, 57856};
const double		 	PhotoUtility::kRad2Degrees = 57.2958;
const double 			PhotoUtility::kHardwiredHeaderSize = 0.333; /*inches*/

bool PhotoUtility::gNeedDoubleOrientationSetting (true);

/*
*DrawXformedRect
*/
void
PhotoUtility::DrawXformedRect(
	const Rect&				inRect,
	MatrixRecord*			inMat,
	const RectOperationT	inWhat,
	const bool				inUseRegion)
{
	Point	vertices[4];
	
	vertices[0] = topLeft(inRect);
	vertices[2] = botRight(inRect);
	vertices[1].v = inRect.top;
	vertices[1].h = inRect.right;
	vertices[3].v = inRect.bottom;
	vertices[3].h = inRect.left;
	
	::TransformPoints(inMat, vertices, 4);

	MNewRegion	region;
	if (inUseRegion)
		region.Open();
	::MoveTo(vertices[0].h, vertices[0].v);
	::LineTo(vertices[1].h, vertices[1].v);
	::LineTo(vertices[2].h, vertices[2].v);
	::LineTo(vertices[3].h, vertices[3].v);
	::LineTo(vertices[0].h, vertices[0].v);
	if (inUseRegion) {
		region.Close();
		switch (inWhat) {
			case kFrame:
				region.Frame();
				break;

/*
			case kFill:
				region.Fill();	// ??? fill with what?
				break;
*/

			case kFillHilite:
				region.Hilite();
				break;

			case kInvalidate:
				::InvalWindowRgn(::GetWindowFromPort(::GetQDGlobalsThePort()), (RgnHandle)region);
				break;

			case kPaint:
				region.Paint();
				break;
		} // end switch
	}//endif
} // DrawXformedRect


SInt16	
PhotoUtility::GetLineHeight(short inFontNum, short inFontSize, Style inStyle) {
	StTextState		save;

	::TextFont(inFontNum);
	::TextSize(inFontSize);
	::TextFace(inStyle);

	FontInfo		fi;
	::GetFontInfo(&fi);

	return fi.ascent + fi.descent + fi.leading;	
}//end GetLineHeight



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
	gSizeMap['5*3 '] = DoubleSize(3.5, 5);
	gSizeMap['6*4 '] = DoubleSize(4, 6);
	gSizeMap['7*5 '] = DoubleSize(5, 7);
	gSizeMap['10*7'] = DoubleSize(7.5, 10);

	gSizeMap['2*3 '] = DoubleSize(3, 2);
	gSizeMap['3*5 '] = DoubleSize(5, 3.5);
	gSizeMap['4*6 '] = DoubleSize(6, 4);
	gSizeMap['5*7 '] = DoubleSize(7, 5);
	gSizeMap['7*10'] = DoubleSize(10, 7.5);
} // InitializeSizeMap