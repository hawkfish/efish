/*
	File:		ERect32.h

	Contains:	Interface to 32-bit rectangles.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		11 Jul 2000		drd		Added Rect constructor
		11 Jul 2000		drd		Created

	Notes:
		This draws on CRect32 (from PensŽeUtil) and MRect (from Toolbox++).

		This class is based on the PowerPlant type
			struct	SRect32  {
				SInt32	left;
				SInt32	top;
				SInt32	right;
				SInt32	bottom;
			};
		If we want to avoid PowerPlant, we'll have to define this ourself.
*/

#include "LPane.h"

class	ERect32 : public SRect32 {
public:
	enum {
		kInvalidCoordinate = 0x80000000L
	};

	// NOTE: The coordinates are not in the same order as a LongRect defined by WASTE.
	// This required a change in CMarquee.h, since the purpose is to be able
	// to cast an SRect32 as a ERect32.

				ERect32()						{ this->MakeEmpty(); }
				ERect32(const SInt32 t, const SInt32 l, const SInt32 b, const SInt32 r)
												{ top = t; left = l; bottom = b; right = r; }
				ERect32(const Rect& inRect)		{ top = inRect.top; left = inRect.left;
													bottom = inRect.bottom;
													right = inRect.right; }
				ERect32(const ERect32& inRect);

	SInt32		Height() const					{ return bottom - top; }
	SPoint32	MidPoint() const { SPoint32 p = {(top + bottom) / 2, (left + right) / 2}; return p; }
	SInt32		Width() const					{ return right - left; }

	bool		Contains(const Point inPt) const;
	bool		Contains(const SPoint32 inPt) const;
	bool		IsEmpty() const					{ return this->Height() <= 0 || this->Width() <= 0; }
	bool		IsInvalid() const				{ return top == kInvalidCoordinate; }
	bool		Overlaps(const Rect& inRect) const;

	void		MakeEmpty()						{ top = left = bottom = right = 0; }
	void 		MakeInvalid()					{ top = bottom = kInvalidCoordinate; }

	void		Offset(const long inDx, const long inDy);
	void		SetWidth(const SInt32 inWidth)	{ right = left + inWidth; }
	void		SetHeight(const SInt32 inHght)	{ bottom = top + inHght; }

	int			operator==(const ERect32& other) const;
								
	int			operator!=(const ERect32& other) const	{ return !(*this == other); }
};
