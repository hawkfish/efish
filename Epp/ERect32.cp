/*
	File:		ERect32.cp

	Contains:	Implementation of 32-bit rectangles.

	Written by:	David Dunham

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		14 Aug 2001		rmgw	Enhance SPoint32 constructor.
		13 jul 2000		dml		add Intersect, Union
		11 Jul 2000		drd		Created
*/

#include "ERect32.h"
#include <algorithm.h>

/*
ERect32
	Copy-Constructor
*/
ERect32::ERect32(const ERect32& inRect)
{
	top = inRect.top;
	left = inRect.left;
	bottom = inRect.bottom;
	right = inRect.right;
} // ERect32


ERect32&
ERect32::operator=(const ERect32& other) {
	if (this != &other) {
		top = other.top;
		left = other.left;
		bottom = other.bottom;
		right = other.right;
		}//endif sane
	return *this;
	}//end operator=


/*
ERect32
	SPoint32 ct
*/
ERect32::ERect32(const SPoint32& pt1, const SPoint32& pt2) {
	top = std::min (pt1.v, pt2.v);
	left = std::min (pt1.h, pt2.h);
	bottom = std::max (pt1.v, pt2.v);
	right = std::max (pt1.h, pt2.h);
	}//end SPoint32 ct


/*
operator==
*/
int	ERect32::operator==(const ERect32& inOther) const
{
	return top == inOther.top && bottom == inOther.bottom && left == inOther.left &&
		right == inOther.right;
} // operator==

/*
Contains
*/
bool	ERect32::Contains(const Point inPt) const
{
	return inPt.v >= top && inPt.v <= bottom && inPt.h >= left && inPt.h <= right;
} // Contains

/*
Contains
*/
bool	ERect32::Contains(const SPoint32 inPt) const
{
	return inPt.v >= top && inPt.v <= bottom && inPt.h >= left && inPt.h <= right;
} // Contains



//---------------------------------------------------
// Intersect
//---------------------------------------------------
ERect32
ERect32::Intersect(const ERect32& a, const ERect32& b) {
	ERect32 result;
	
	result.top = max(a.top, b.top);
	result.left = max(a.left, b.left);
	
	result.right = min(a.right, b.right);
	result.bottom = min(a.bottom, b.bottom);
	
	result.right = max(result.right, result.left);
	result.bottom = max(result.bottom, result.top);
	
	if (result.IsEmpty())
		result.MakeEmpty();
	return result;
	}//end Intersect


/*
Offset
*/
void	ERect32::Offset(const long inDx, const long inDy)
{
	top += inDy;
	left += inDx;
	bottom += inDy;
	right += inDx;
} // Offset

/*
Overlaps
*/
bool	ERect32::Overlaps(const Rect& inRect) const
{
	// !!! This is lame
	Rect	temp;
	temp.left = left;
	temp.top = top;
	temp.right = right;
	temp.bottom = bottom;
	bool	r = ::SectRect(&temp, &inRect, &temp);
	return r;
} // Overlaps




void RectScale(Rect&	ioR, double scalar) {
	short width (ioR.right - ioR.left);
	short height (ioR.bottom - ioR.top);
	ioR.top *= scalar;
	ioR.left *= scalar;
	ioR.right = ioR.left + (width * scalar);
	ioR.bottom = ioR.top + (height * scalar);	
	}//



void RectScale(ERect32&	ioR, double scalar) {
	SInt32 width (ioR.right - ioR.left);
	SInt32 height (ioR.bottom - ioR.top);
	ioR.top *= scalar;
	ioR.left *= scalar;
	ioR.right = ioR.left + (width * scalar);
	ioR.bottom = ioR.top + (height * scalar);	
	}//






//---------------------------------------------------
// Union
//---------------------------------------------------
ERect32
ERect32::Union(const ERect32& a, const ERect32& b) {
	ERect32 result;
	
	result.top = min(a.top, b.top);
	result.left = min(a.left, b.left);
	result.bottom = max(a.bottom, b.bottom);
	result.right = max(a.right, b.right);
	
	return result;
	}//end Union
