/*
	File:		ESpinCursor.h

	Contains:	cheezy spinning cursor (NOT based on 'acur' resources)

	Written by:	Dav Lion

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

	25 Jul 2001		drd		15 Added SpinWatch (and gWatchStep)
	18 Sep 2000		drd		Added arg to constructor
*/

#pragma once

#include "MCursor.h"
#include "HORef.h"

class ESpinCursor {
protected:
	HORef<MCursor>	mCursor;
	ResIDT			mFirst;
	SInt16			mNum;
	SInt16 			mIndex;

	static	UInt32	gWatchStep;
	
public:
	enum {
		kShow = true,
		kDontShow = false
	};
					ESpinCursor (const ResIDT inFirst, const SInt16 inNumFrames, const bool inStart = kShow);
	virtual			~ESpinCursor();
	
			void	Spin(void);

	static	void	SpinWatch(const ThemeCursor inCursor = kThemeWatchCursor);
}; //end ESpinCursor
