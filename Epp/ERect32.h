/*
	File:		ERect32.h

	Contains:	Interface to 32-bit rectangles.

	Written by:	David Dunham

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		22 aug 2001		dml		Fix MidPoint (SPoint32 is h,v even though Point is v,h)
		14 Aug 2001		rmgw	Fix TopLeft and make more like MRect.
		02 aug 2001		dml		add operator bool, boolean
		14 aug 2000		dml		add RectSwapOrientation
		14 jul 2000		dml		add RectScale
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

	explicit	ERect32()						{ this->MakeEmpty(); }
				ERect32(const SInt32 t, const SInt32 l, const SInt32 b, const SInt32 r)
												{ top = t; left = l; bottom = b; right = r; }
				ERect32(const Rect& inRect)		{ top = inRect.top; left = inRect.left;
													bottom = inRect.bottom;
													right = inRect.right; }
				ERect32(const SPoint32& pt1, const SPoint32& pt2);
				ERect32(const SPoint32& topleft, const SDimension32& inSize)
						{ top = topleft.v; left = topleft.h; 
						  bottom = top + inSize.height; right = left + inSize.width; }
	explicit	ERect32(const SDimension32& inSize)
						{ top = 0; left = 0; 
						  bottom = top + inSize.height; right = left + inSize.width; }
				ERect32(const ERect32& inRect);
	ERect32&	operator=(const ERect32& other);							

	SInt32		Height() const					{ return bottom - top; }
	SInt32		Width() const					{ return right - left; }
	SPoint32	MidPoint() const { SPoint32 p = {(left + right) / 2, (top + bottom) / 2}; return p; }

	const	SPoint32&	TopLeft() const { return *(const SPoint32*) &left; }
	const	SPoint32& 	BotRight() const { return *(const SPoint32*) &right; }
	SPoint32&			TopLeft() { return *(SPoint32*) &left; }
	SPoint32& 			BotRight() { return *(SPoint32*) &right; }

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
	operator	Boolean		(void) const
							{return !IsEmpty ();};
	operator	bool		(void) const
							{return !IsEmpty ();};
	
	

};

inline ERect32 operator+ (const ERect32& a, const ERect32& b)
	{ERect32 result; result = ERect32::Union(a, b); return result;};
	
inline ERect32 operator* (const ERect32& a, const ERect32& b)
	{ERect32 result; result = ERect32::Intersect(a, b); return result;};
		

template<class R1, class R2>
inline void RectCopy (R1& r1, const R2& r2)
	{r1.top = r2.top; r1.left = r2.left; r1.right = r2.right; r1.bottom = r2.bottom;};

template<class R1, class R2>
inline bool RectEquals (const R1& r1, const R2& r2)
	{return (r1.top == r2.top) && (r1.left == r2.left) && (r1.right == r2.right) && (r1.bottom == r2.bottom);};


void RectScale(Rect&	ioR, double scalar);
void RectScale(ERect32&	ioR, double scalar);



template<class R1>
inline void RectSwapOrientation(R1& r1) 
	{R1 temp; temp.right = r1.bottom - r1.top; r1.bottom = r1.top + (r1.right - r1.left); r1.right = r1.left + temp.right;};