/*
	File:		AlignmentGizmo.cp

	Contains:	utility routines for alignment.  const->text, also rect placements

	Written by:	Dav Lion

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		22 aug 2001		dml		add ERect32 version of FitTransformedRectInside
		14 aug 2001		dml		add ERect32 version of MoveMidpointTo
		19 jul 2000		dml		floor removed from FitTransformedRectInside
		10 jul 2001		dml		147.  add more precision to FitTransformedRectInside
		02 mar 2001		dml		add MoveMidpointTo, cleanup debugging code
		01 mar 2001		dml		add FitTransformedRectInside
		05 Oct 2000		drd		Use std:: for strcmp
		14 sep 2000		dml		add ERect alignment ops
		11 jul 2000		dml		use more tmp vars inside FitAndAlignRectInside
		29 jun 2000		dml		AlignRectInside
		28 jun 2000		dml		fix AlignRectInside, add FitAndAlignRectInside
		16 jun 2000		dml		initial revision
*/
#include "AlignmentGizmo.h"
#include <math.h>
#include <algorithm>
#include "ERect32.h"
#include "PhotoUtility.h"
#include "EMatrix.h"

bool AlignmentGizmo::sInitialized = false;
AlignmentMap AlignmentGizmo::sAlignmentMap;


double PointDistance(Point& a, Point& b);
SInt16 FindTopIndex(Point inArray[]);
SInt32 FindTopIndex(SPoint32 inArray[]);

const char*
AlignmentGizmo::Find(AlignmentType key) {
	if (!sInitialized) {
		Initialize();
		}//endif need to construct

		AlignmentMap::const_iterator	i (sAlignmentMap.find (key));
		if (i != sAlignmentMap.end ()) 
			return (*i).second;
		else
			return 0;
	}//end Find


void
AlignmentGizmo::Initialize() {
    sAlignmentMap[kAlignNone] = "AlignNone";
    sAlignmentMap[kAlignVerticalCenter] = "AlignVerticalCenter";
    sAlignmentMap[kAlignTop] ="AlignTop";
    sAlignmentMap[kAlignBottom] ="AlignBottom";
    sAlignmentMap[kAlignHorizontalCenter] ="AlignHorizontalCenter";
    sAlignmentMap[kAlignAbsoluteCenter] ="AlignAbsoluteCenter";
    sAlignmentMap[kAlignCenterTop] ="AlignCenterTop";
    sAlignmentMap[kAlignCenterBottom] ="AlignCenterBottom";
    sAlignmentMap[kAlignLeft] ="AlignLeft";
    sAlignmentMap[kAlignCenterLeft] ="AlignCenterLeft";
    sAlignmentMap[kAlignTopLeft] ="AlignTopLeft";
    sAlignmentMap[kAlignBottomLeft] ="AlignBottomLeft";
    sAlignmentMap[kAlignRight] ="AlignRight";
    sAlignmentMap[kAlignCenterRight] = "AlignCenterRight";
    sAlignmentMap[kAlignTopRight] = "AlignTopRight";
    sAlignmentMap[kAlignBottomRight] = "AlignBottomRight";
	sInitialized = true;
	}//end Initialize


AlignmentType
AlignmentGizmo::Lookup(const char* text) {
	if (!sInitialized) {
		Initialize();
		}//endif need to construct

	for (AlignmentMap::const_iterator	i = sAlignmentMap.begin(); i != sAlignmentMap.end(); ++i) {
		if (std::strcmp((*i).second, text) == 0) {
			return (*i).first;
			}//endif
		}//end

	return kAlignNone;
}//end Lookup


void
AlignmentGizmo::AlignRectInside(const MRect& target,
								const MRect& bounding,
								AlignmentType alignment,
								MRect&	destRect) 
{
	MRect dst (target);  // we use a temp so that target and dst may be the same
	
	// move topleft of dst to that of bounding,
	// then clip dst to bounding;
	dst.Offset(bounding.left - dst.left, bounding.top - dst.top);
	dst *= bounding;
	
	if ((alignment & kAlignTop) == kAlignTop) // 0x02
		dst.Offset(0, dst.top - bounding.top);
	if ((alignment & kAlignBottom) == kAlignBottom) // 0x03
		dst.Offset(0, bounding.bottom - dst.bottom);

	if ((alignment & kAlignVerticalCenter) == kAlignVerticalCenter) // 0x01
		dst.Offset(0, ((bounding.Height() - dst.Height()) / 2) + dst.top - bounding.top);
	if ((alignment & kAlignHorizontalCenter) == kAlignHorizontalCenter) // 0x04
		dst.Offset(((bounding.Width() - dst.Width()) / 2) + dst.left - bounding.left, 0);

	if ((alignment & kAlignLeft) == kAlignLeft) // 0x08
		dst.Offset(dst.left - bounding.left, 0);
	if ((alignment & kAlignRight) == kAlignRight) // 0x0c
		dst.Offset(0, bounding.right - dst.right);
		
	destRect = dst;
}//end AlignRectInside
	


void
AlignmentGizmo::AlignRectInside(const ERect32& target,
								const ERect32& bounding,
								AlignmentType alignment,
								ERect32&	destRect) 
{
	ERect32 dst (target);  // we use a temp so that target and dst may be the same
	
	// move topleft of dst to that of bounding,
	// then clip dst to bounding;
	dst.Offset(bounding.left - dst.left, bounding.top - dst.top);
	dst *= bounding;
	
	if ((alignment & kAlignTop) == kAlignTop) // 0x02
		dst.Offset(0, dst.top - bounding.top);
	if ((alignment & kAlignBottom) == kAlignBottom) // 0x03
		dst.Offset(0, bounding.bottom - dst.bottom);

	if ((alignment & kAlignVerticalCenter) == kAlignVerticalCenter) // 0x01
		dst.Offset(0, ((bounding.Height() - dst.Height()) / 2) + dst.top - bounding.top);
	if ((alignment & kAlignHorizontalCenter) == kAlignHorizontalCenter) // 0x04
		dst.Offset(((bounding.Width() - dst.Width()) / 2) + dst.left - bounding.left, 0);

	if ((alignment & kAlignLeft) == kAlignLeft) // 0x08
		dst.Offset(dst.left - bounding.left, 0);
	if ((alignment & kAlignRight) == kAlignRight) // 0x0c
		dst.Offset(0, bounding.right - dst.right);
		
	destRect = dst;
}//end AlignRectInside



void	
AlignmentGizmo::FitAndAlignRectInside(const MRect& target,
										const MRect& bounding,
										AlignmentType alignment,
										MRect&	outDestRect,
										bool okToExpand) {
	MRect originalBounds (bounding);

	EUtil::FitRectInside(target, bounding, outDestRect, okToExpand);
	AlignmentGizmo::AlignRectInside(outDestRect, originalBounds, alignment, outDestRect);

}//end FitAndAlignRectInside										



void	
AlignmentGizmo::FitAndAlignRectInside(const ERect32& target,
										const ERect32& bounding,
										AlignmentType alignment,
										ERect32&	outDestRect,
										bool okToExpand) {
	ERect32 originalBounds (bounding);

	EUtil::FitRectInside(target, bounding, outDestRect, okToExpand);
	AlignmentGizmo::AlignRectInside(outDestRect, originalBounds, alignment, outDestRect);

}//end FitAndAlignRectInside										



double
PointDistance(Point& a, Point& b) {
 	return sqrt(((a.v - b.v) * (a.v -b.v)) + ((a.h - b.h) * (a.h - b.h)));
}//end PointDistance


SInt16
FindTopIndex(Point inArray[]) {
	bool zeroLessThanOne (inArray[0].v < inArray[1].v);
	bool twoLessThanThree (inArray[2].v < inArray[3].v);
	
	if (zeroLessThanOne) {
		if (twoLessThanThree)
			return (inArray[0].v < inArray[2].v ? 0 : 2);
		else
			return (inArray[0].v < inArray[3].v ? 0 : 3);
		}//endif zero bigger than one
	else {
		if (twoLessThanThree)
			return (inArray[1].v < inArray[2].v ? 1 : 2);
		else
			return (inArray[1].v < inArray[3].v ? 1 : 3);
		}//else
	}//end FindTopIndex


SInt32
FindTopIndex(SPoint32 inArray[]) {
	bool zeroLessThanOne (inArray[0].v < inArray[1].v);
	bool twoLessThanThree (inArray[2].v < inArray[3].v);
	
	if (zeroLessThanOne) {
		if (twoLessThanThree)
			return (inArray[0].v < inArray[2].v ? 0 : 2);
		else
			return (inArray[0].v < inArray[3].v ? 0 : 3);
		}//endif zero bigger than one
	else {
		if (twoLessThanThree)
			return (inArray[1].v < inArray[2].v ? 1 : 2);
		else
			return (inArray[1].v < inArray[3].v ? 1 : 3);
		}//else
	}//end FindTopIndex



// handle rotated rects w/ odd bounding rects while preserving original aspect
void
AlignmentGizmo::FitTransformedRectInside(const MRect& inRect,
											MatrixRecord* pMat,
											const MRect& bounding,
											MRect&	outDestRect)
{
	// first, xform the rect
	Point	corners[4];
	corners[0] = inRect.TopLeft();
	corners[3] = inRect.BotRight();
	corners[1].v = corners[0].v; // topRight
	corners[1].h = corners[3].h;
	corners[2].v = corners[3].v; // bottomLeft
	corners[2].h = corners[0].h;
	::TransformPoints(pMat, corners, 4);

	double heightScale;
	double widthScale;

	SInt32 topIndex (FindTopIndex(corners)); // find the smallest y value of the xformed corners
	switch (topIndex) {
		case 0:
		case 3: // use corner[0] to corner[3] for height
			heightScale = ((double)bounding.Height()) / (double)fabs(corners[0].v - corners[3].v);
			widthScale = ((double)bounding.Width()) / (double)fabs(corners[1].h - corners[2].h);
			break;
		case 1:
		case 2: // use corner[1] to corner[2] for height
			heightScale = ((double)bounding.Height()) / (double)fabs(corners[1].v - corners[2].v);
			widthScale = ((double)bounding.Width()) / (double)fabs(corners[0].h - corners[3].h);
			break;
	}//switch

	// use the minimum scale
	double	scaleToUse (std::min(heightScale, widthScale));	
	//clamp scale to hundredths
//	scaleToUse = floor(scaleToUse * 1000.0) / 1000.0;

	outDestRect	= inRect;
	RectScale(outDestRect, scaleToUse);
	
}//end FitTransformedRectInside



void	
AlignmentGizmo::FitTransformedRectInside(const ERect32& inRect,
										 MatrixRecord* pMat,
									 	const ERect32& bounding,
										ERect32&	outDestRect) {

	SPoint32	corners[4];
	corners[0] = inRect.TopLeft();
	corners[3] = inRect.BotRight();
	corners[1].v = corners[0].v; // topRight
	corners[1].h = corners[3].h;
	corners[2].v = corners[3].v; // bottomLeft
	corners[2].h = corners[0].h;

	EMatrix mat (*pMat);
	mat.TransformPoints(corners, 4);


	double heightScale;
	double widthScale;

	SInt16 topIndex (FindTopIndex(corners)); // find the smallest y value of the xformed corners
	switch (topIndex) {
		case 0:
		case 3: // use corner[0] to corner[3] for height
			heightScale = ((double)bounding.Height()) / (double)fabs(corners[0].v - corners[3].v);
			widthScale = ((double)bounding.Width()) / (double)fabs(corners[1].h - corners[2].h);
			break;
		case 1:
		case 2: // use corner[1] to corner[2] for height
			heightScale = ((double)bounding.Height()) / (double)fabs(corners[1].v - corners[2].v);
			widthScale = ((double)bounding.Width()) / (double)fabs(corners[0].h - corners[3].h);
			break;
	}//switch

	// use the minimum scale
	double	scaleToUse (std::min(heightScale, widthScale));	
	//clamp scale to hundredths
//	scaleToUse = floor(scaleToUse * 1000.0) / 1000.0;

	outDestRect	= inRect;
	RectScale(outDestRect, scaleToUse);
	
}//end 32bit FitTransformedRectInside



void	
AlignmentGizmo::MoveMidpointTo(const MRect& inRect,
								const MRect& bounding,
								MRect& outRect) {

	outRect = inRect;
	outRect.Offset( bounding.MidPoint().h - inRect.MidPoint().h, bounding.MidPoint().v - inRect.MidPoint().v);

}//end MoveMidpointTo

void	
AlignmentGizmo::MoveMidpointTo(const ERect32& inRect,
								const ERect32& bounding,
								ERect32& outRect) {

	outRect = inRect;
	outRect.Offset( bounding.MidPoint().h - inRect.MidPoint().h, bounding.MidPoint().v - inRect.MidPoint().v);

}//end MoveMidpointTo
