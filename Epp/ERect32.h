/*
	File:		ERect32.h

	Contains:	Interface to 32-bit rectangles.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		13 jul 2000		dml		add Intersect, Union, operator+, operator=, operator+=, operator*=, operator=, SPoint32 ct
		12 jul 2000		dml		pragma once
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
#pragma once
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
	ERect32&	operator=(const ERect32& other);							
				ERect32(const SPoint32& topleft, const SPoint32& bottomRight);

	SInt32		Height() const					{ return bottom - top; }
	SInt32		Width() const					{ return right - left; }
	SPoint32	MidPoint() const { SPoint32 p = {(top + bottom) / 2, (left + right) / 2}; return p; }
	SPoint32	TopLeft() const { SPoint32 p = {top, left}; return p;}
	SPoint32 	BotRight() const { SPoint32 p = {bottom, right}; return p;}

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

	static 		ERect32		Intersect(const ERect32& a, const ERect32& b);
	static		ERect32		Union(const ERect32& a, const ERect32& b);

	ERect32&	operator+=	(const	ERect32&	other)
								{*this = Union(*this, other); return *this;};				
	ERect32&	operator*=	(const	ERect32&	other)
								{*this = Intersect(*this, other); return *this;};				

	int			operator==(const ERect32& other) const;
	int			operator!=(const ERect32& other) const	{ return !(*this == other); }

	

};

inline ERect32 operator+ (const ERect32& a, const ERect32& b)
	{ERect32 result; result = ERect32::Union(a, b); return result;};
	
inline ERect32 operator* (const ERect32& a, const ERect32& b)
	{ERect32 result; result = ERect32::Intersect(a, b); return result;};
		

