#pragma once
/*
	File:		ESpinCursor.h

	Contains:	cheezy spinning cursor

	Written by:	Dav Lion 
	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):
*/

#include "MCursor.h"
#include "HORef.h"

class ESpinCursor {
	protected:
		HORef<MCursor>	mCursor;
		ResIDT	mFirst;
		SInt16	mNum;
		SInt16 	mIndex;
		
	public:
	
		ESpinCursor (ResIDT inFirst, SInt16 inNumFrames);
		virtual ~ESpinCursor();
		
		void	Spin(void);
		
		}; //end ESpinCursor