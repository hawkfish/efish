/*
	File:		ESpinCursor.h

	Contains:	cheezy spinning cursor (NOT based on 'acur' resources)

	Written by:	Dav Lion

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):
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
	
public:
	enum {
		kShow = true,
		kDontShow = false
	};
					ESpinCursor (const ResIDT inFirst, const SInt16 inNumFrames, const bool inStart = kShow);
	virtual			~ESpinCursor();
	
			void	Spin(void);
}; //end ESpinCursor
