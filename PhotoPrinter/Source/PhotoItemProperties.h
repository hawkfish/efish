/*
	File:		PhotoItemProperties.h

	Contains:	properties that an item might use, but which aren't intrinsic to an Item

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		28 Jun 2001		drd		24 107 caption_COUNT, gCaptionStyles
		21 Aug 2000		drd		Got rid of the method version of ParseAlignment
		20 Jul 2000		drd		Added caption_Inside
		17 Jul 2000		drd		Added limit_Index
		12 Jul 2000		drd		GetCaptionLineHeight
		12 jul 2000		dml 	add SizeLimitToInches
		11 Jul 2000		drd		Added limit_Slide
		11 Jul 2000		drd		Added SizeLimitT
		06 Jul 2000		drd		Added caption, shape, frame stuff
		16 jun 2000 	dml		factored alignment stuff into AlignmentGizmo
		14 Jun 2000		dml		Added AlignmentType (and associated map classes), removed mCenter
*/

#pragma once

#include "MPString.h"

namespace XML {
	class Output;
	class Element;
	class Handler;
}

typedef enum {
	shape_Rectangle = 1,
	shape_Oval,
	shape_Diamond
} ShapeT;

typedef enum {
	frame_None = 0,
	frame_Hairline,
	frame_Thin,
	frame_Thick
} FrameT;

typedef enum {
	caption_None = 0,
	caption_Bottom,
	caption_RightHorizontal,
	caption_RightVertical,
	caption_LeftHorizontal,
	caption_LeftVertical,
	caption_Top,
	caption_Inside,
	caption_COUNT
} CaptionT;

const SInt16	kNarrowestCaption = 64;

typedef enum {
	limit_None = 1,									// Corresponds directly to menu items
	limit_Index = 3,
	limit_Slide,
	limit_3by2,
	limit_5by3,
	limit_6by4,
	limit_7by5,
	limit_10by7half
} SizeLimitT;

class PhotoItemProperties {
protected:
	
	// what are we allowed to do
	bool			mCanMove;
	bool			mCanResize;
	bool			mCanRotate;
	
	// what do we want to do
	bool			mMaintainAspect;
	bool			mMaximize;
	
	// how do we want to do it?
	AlignmentType	mAlignment;
	bool 			mFullSize;
		
	// Text caption
	MPString		mCaption;
	CaptionT		mCaptionStyle;
	bool			mShowDate;
	bool			mShowName;
	SInt16			mFontNumber;
	SInt16			mFontSize;

	// Image shape
	ShapeT			mImageShape;
	bool			mShadow;
	RGBColor		mShadowColor;
	bool			mBlurEdges;

	// Decorative frame
	RGBColor		mFrameColor;
	FrameT			mFrameStyle;

	static 	void	ParseAlignment(XML::Element &elem, void *userData);


public:
			PhotoItemProperties();
			PhotoItemProperties(bool inRotate, bool inResize, bool inMove,
								bool inAspect, bool inMax, 
								bool inFullSize, AlignmentType inAlign);
			PhotoItemProperties(const PhotoItemProperties& other);
	virtual ~PhotoItemProperties();
	
	
		AlignmentType	GetAlignment() const;
		bool 			GetAspect() const;
		bool			GetBlurEdges() const			{ return mBlurEdges; }
		const MPString&	GetCaption() const				{ return mCaption; }
		SInt16			GetCaptionLineHeight() const;
		CaptionT		GetCaptionStyle() const			{ return mCaptionStyle; }
		SInt16			GetFontNumber() const			{ return mFontNumber; }
		SInt16			GetFontSize() const				{ return mFontSize; }
		const RGBColor&	GetFrameColor() const			{ return mFrameColor; }
		FrameT			GetFrameStyle() const			{ return mFrameStyle; }
		bool 			GetFullSize() const;
		ShapeT			GetImageShape() const			{ return mImageShape; }
		bool			GetMaximize() const;
		bool			GetMove() const;
		bool			GetResize() const;
		bool			GetRotate() const;
		bool			GetShadow() const				{ return mShadow; }
		const RGBColor&	GetShadowColor() const			{ return mShadowColor; }
		bool			GetShowDate() const				{ return mShowDate; }
		bool			GetShowName() const				{ return mShowName; }
		bool			HasCaption() const;

		void	SetAlignment(AlignmentType inAlign);
		void 	SetAspect(bool inVal);
		void	SetBlurEdges(const bool inVal)			{ mBlurEdges = inVal; }
		void	SetCaption(const MPString& inStr)		{ mCaption = inStr; }
		void	SetCaptionStyle(const CaptionT inVal)	{ mCaptionStyle = inVal; }
		void	SetFontName(ConstStr255Param inName);
		void	SetFontNumber(const SInt16 inFont)		{ mFontNumber = inFont; }
		void	SetFontSize(const SInt16 inSize)		{ mFontSize = inSize; }
		void	SetFrameColor(const RGBColor& inColor)	{ mFrameColor = inColor; }
		void	SetFrameStyle(const FrameT inStyle)		{ mFrameStyle = inStyle; }
		void	SetFullSize(bool inVal);
		void	SetImageShape(const ShapeT inStyle)		{ mImageShape = inStyle; }
		void	SetMaximize(bool inVal);
		void	SetMove	(bool inVal);
		void	SetResize(bool inVal);
		void 	SetRotate(bool inVal);
		void	SetShadow(const bool inVal)				{ mShadow = inVal; }
		void	SetShadowColor(const RGBColor& inColor)	{ mShadowColor = inColor; }
		void	SetShowDate(const bool inVal)			{ mShowDate = inVal; }
		void	SetShowName(const bool inVal)			{ mShowName = inVal; }
		static void 	SizeLimitToInches(SizeLimitT limit, double& hInches, double& vInches);

// IO
		void 	Read(XML::Element &elem);
		void 	Write(XML::Output &out) const;
		static	void	ParseProperties(XML::Element &elem, void *userData);

protected:
	static	const char	*const gCaptionStyles[caption_COUNT];

};//end class PhotoItemProperties
