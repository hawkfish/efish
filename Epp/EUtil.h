/*
	File:		EUtil.h

	Contains:	utility routines

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

	14 Aug 2000		drd		BringFinderToFront
	20 Jul 2000		drd		Added arg to GetMonitorRect
	18 Jul 2000		drd		AlignToScreen
	12 Jul 2000		drd		SizeFromMenu
	12 jul 2000		dml		added ERect32 version of FitRectInside
	03 Jul 2000		drd		BestFit, FitRectInside default to kDontExpand
	29 jun 2000 	dml		add FitRectInside, pragma once
	26 Jun 2000		drd		Symbolic constant
	21 june 2000	dml		added 'okToExpand' to BestFit
*/

#pragma once

#include "MRect.h"
#include "ERect32.h"

class EUtil {
public:
	enum {
		kCanExpand = true,
		kDontExpand = false,

		kSubtractMenuBar = true,
		kIncludeMenuBar = false,

		kDefaultScreenInset = 1
	};

	// Menus
	static	SInt16	SizeFromMenu(const SInt16 inMenuItem, MenuHandle inMenu);

	// Processes
	static	void	BringFinderToFront(void);

	// Rectangles
	static	void 	BestFit				(SInt32&	outWidth, 
										 SInt32&	outHeight,
										 const	SInt32		boundingWidth,
										 const	SInt32		boundingHeight,
										 const	SInt32		objectWidth,
										 const	SInt32		objectHeight,
										 const	bool		okToExpand = kDontExpand);

	static	void	FitRectInside(const MRect& child,
								  const MRect& bounding,
								  MRect& outDestRect,
								  const bool okToExpand = kDontExpand);

	static	void	FitRectInside(const ERect32& child,
								  const ERect32& bounding,
								  ERect32& outDestRect,
								  const bool okToExpand = kDontExpand);

	// Windows
	static	void	AlignToScreen(LWindow* inWindow, const AlignmentType inAlign = kAlignAbsoluteCenter);
	static	void	GetMonitorRect(WindowPtr inFrontWindow,
									Rect& outMonitorRect,
									const bool inMenuBar = kIncludeMenuBar);

	static	SInt16		gScreenInset;
}; //end class EUtil
