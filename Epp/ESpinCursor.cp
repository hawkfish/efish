/*
	File:		ESpinCursor.cp

	Contains:	cheezy spinning cursor

	Written by:	Dav Lion

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):
	
	18 Sep 2000		drd		Added arg to constructor
	18 jul 2000 	dml		added MCurResFile around sets, seems to fix epson print glitchiness
*/

#include "ESpinCursor.h"
#include "MResFile.h"
#include "UCursor.h"

/*
ESpinCursor
*/
ESpinCursor::ESpinCursor(const ResIDT inFirst, const SInt16 inNum, const bool inStart)
	: mIndex (0)
 {
	mFirst = inFirst;
	mNum = inNum;
	
	MCurResFile		restoreResFile;
	if (inStart == kShow) {
		UCursor::SetTheCursor(inFirst);
	}
}//end ct

/*
~ESpinCursor
*/
ESpinCursor::~ESpinCursor() {
}//end dt
	
/*
Spin
*/
void
ESpinCursor::Spin() {
	++mIndex %= mNum;
	try {
		MCurResFile		restoreResFile;
		mCursor = new MCursor(mFirst + mIndex);
		mCursor->Use();
	} // try
	catch (...) {
		// if we are unable to make a cursor, silently fail
	} // catch
}//end Spin