/*
	File:		ERect32.cp

	Contains:	Implementation of 32-bit rectangles.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		11 Jul 2000		drd		Created
*/

#include "ERect32.h"

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
