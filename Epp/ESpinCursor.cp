/*
	File:		ESpinCursor.cp

	Contains:	cheezy spinning cursor

	Written by:	Dav Lion

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):
	
	25 Jul 2001		drd		15 Added SpinWatch (and gWatchStep)
	18 Sep 2000		drd		Added arg to constructor
	18 jul 2000 	dml		added MCurResFile around sets, seems to fix epson print glitchiness
*/

#include "ESpinCursor.h"
#include "MResFile.h"
#include "UCursor.h"

// Globals
UInt32	ESpinCursor::gWatchStep = 0;

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

/*
SpinWatch [static]
	Shows the next stage of an animated cursor. The normal use is with kThemeWatchCursor, but it
	can also be used with kThemeCountingUpHandCursor, kThemeSpinningCursor, etc. Yes, there is
	only one gWatchStep, but this is not likely to be a problem. The whole point is to have a
	single call that people can make, and not have to pass objects all the way down a call chain.
*/
void
ESpinCursor::SpinWatch(const ThemeCursor inCursor)
{
	OSStatus	err = ::SetAnimatedThemeCursor(inCursor, gWatchStep++);
} // SpinWatch