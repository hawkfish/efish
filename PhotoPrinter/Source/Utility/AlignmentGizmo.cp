/*
	File:		AlignmentGizmo.cp

	Contains:	utility routines for alignment.  const->text, also rect placements

	Written by:	Dav Lion

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		29 jun 2000		dml		AlignRectInside
		28 jun 2000		dml		fix AlignRectInside, add FitAndAlignRectInside
		16 jun 2000		dml		initial revision
*/
#include "AlignmentGizmo.h"

bool AlignmentGizmo::sInitialized = false;
AlignmentMap AlignmentGizmo::sAlignmentMap;

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
		if (strcmp((*i).second, text) == 0) {
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
AlignmentGizmo::FitAndAlignRectInside(const MRect& target,
										const MRect& bounding,
										AlignmentType alignment,
										MRect&	outDestRect,
										bool okToExpand) {

	EUtil::FitRectInside(target, bounding, outDestRect, okToExpand);
	AlignmentGizmo::AlignRectInside(outDestRect, bounding, alignment, outDestRect);

}//end FitAndAlignRectInside										




