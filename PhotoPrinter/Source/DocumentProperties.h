/*
	File:		DocumentProperties.h

	Contains:	Document-specific properties. (Why not just part of a document I'll never know.) 

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		14 sep 2000		dml		add header/footer support
		21 Aug 2000		drd		Removed gTitleLabels (we're using TitlePositionMapper)
		21 Aug 2000		drd		Added title-related data
		11 Aug 2000		drd		Removed mFull, mEmpty
*/

#pragma once

#include "MPString.h"

namespace XML {
	class Output;
	class Element;
	class Handler;
}

enum TitlePositionT {
		kHeader = 'head',
		kFooter = 'foot',
		kNoTitle = 'none'
};

class DocumentProperties {
public:
	enum DisplayStateT {
		kNormalState = 0,
		kPreviewState,
		kPrintingState,
		kFnordState
	};

protected:
	bool			mDirty;
	DisplayStateT 	mState;
	TitlePositionT	mTitlePosition;
	MPString		mHeader;
	MPString		mFooter;
	SInt16			mFontNumber;
	SInt16			mFontSize;

		static const char *const gTitleLabels[];
	
public:
				DocumentProperties(bool inDirty, DisplayStateT inState, TitlePositionT inPosition,
									const MPString inHeader, const MPString inFooter);					
				DocumentProperties(DocumentProperties& other);
				DocumentProperties();
	virtual		~DocumentProperties();


				bool		 	GetDirty(void) const;
				SInt16			GetFontNumber() const			{ return mFontNumber; }
				SInt16			GetFontSize() const				{ return mFontSize; }
				DisplayStateT 	GetState(void) const;
				MPString&		GetHeader()						{ return mHeader; }
				MPString& 		GetFooter()						{ return mFooter; }
				TitlePositionT	GetTitlePosition() const		{ return mTitlePosition; }

				void		SetDirty(const bool inVal);
				void		SetFontName(ConstStr255Param inName);
				void		SetFontNumber(const SInt16 inFont)	{ mFontNumber = inFont; }
				void		SetFontSize(const SInt16 inSize)	{ mFontSize = inSize; }
				void		SetState(const DisplayStateT inVal);
				void		SetHeader(const MPString inString)	{ mHeader = inString; }
				void		SetFooter(const MPString inString)	{ mFooter = inString; }
				void		SetTitlePosition(const TitlePositionT inPos)	{ mTitlePosition = inPos; }

// IO
		static	void		ParseProperties(XML::Element &elem, void *userData);
		static 	void		ParseTitlePosition(XML::Element &elem, void *userData);
				void 		Read(XML::Element &elem);
				void 		Write(XML::Output &out) const;
}; //end class DocumentProperties
