// ===========================================================================
//	File:						UWindowStagger.h
// Version:					1.0 - May 7, 1996
//	Author:					Mike Shields (mshields@inconnect.com)
//							
//	Copyright ©1996 Mike Shields. All rights reserved.
//
//	20 Sep 2000		drd		Removed GetMonitorRect (now part of E++); moved constants into class
//	30 Sep 1996		drd		Made GetMonitorRect public
// ===========================================================================
//	UWindowStagger.cp		<- double-click + Command-D to see class declaration
//
// Static class which allows you to stagger windows on the desktop in an intelligent
// manner.

#pragma once 

#ifndef __TYPES__
#include <Types.h>
#endif

#ifndef __QUICKDRAW__
#include <Quickdraw.h>
#endif

class LWindow;


class UWindowStagger
{
public:
	enum {
		kHorizWindowStaggerAmount	= 8,
		kVertWindowStaggerAmount	= 20
	};

	static	SInt16		gSpaceAtLeft;
	static	SInt16		gSpaceBelowMenu;

	static void			StaggerFromPosition(LWindow* inWindow, Point inPosition, 
														short inDeltaH = kHorizWindowStaggerAmount, 
														short inDeltaV = kVertWindowStaggerAmount);
	static void			Stagger(LWindow* inWindow, 
										short inDeltaH = kHorizWindowStaggerAmount, 
										short inDeltaV = kVertWindowStaggerAmount);

	static void			ForceOnScreen(LWindow* inWindow);
	
protected:	
	static Boolean		IsPositionUnique(Point inPosition);
	static void			MakePositionUnique(LWindow* inWindow, Point &inNewPosition, 
														Point inTopLeft, Point inDelta, 
														const Rect& inMonitorRect);
	static Boolean		WillWindowFit(LWindow* inWindow, Point inCurrentPosition, 
												const Rect& inMonitorRect);
};
