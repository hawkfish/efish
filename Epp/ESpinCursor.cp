/*
	File:		ESpinCursor.cp

	Contains:	cheezy spinning cursor

	Written by:	Dav Lion 
	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):
*/
#include "ESpinCursor.h"
#include "UCursor.h"

ESpinCursor::ESpinCursor(ResIDT inFirst, SInt16 inNum)
	: mIndex (0)
 {
	mFirst = inFirst;
	mNum = inNum;
	
	UCursor::SetTheCursor(inFirst);
	}//end ct
	


ESpinCursor::~ESpinCursor() {
	}//end dt
	

void
ESpinCursor::Spin() {
	++mIndex %= mNum;
	try {
		mCursor = new MCursor (mFirst + mIndex);
		mCursor->Use();
		} // try
	
	catch (...) {
		// if we are unable to make a cursor, silently fail
		} // catch
			
	}//end Spin