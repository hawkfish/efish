/*
	File:		EUtil.h

	Contains:	utility routines

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

	03 Jul 2000		drd		BestFit, FitRectInside default to kDontExpand
	29 jun 2000 	dml		add FitRectInside, pragma once
	26 Jun 2000		drd		Symbolic constant
	21 june 2000	dml		added 'okToExpand' to BestFit
*/

#pragma once

#include "MRect.h"

class EUtil {
public:
	enum {
		kCanExpand = true,
		kDontExpand = false
	};

	static	void 	BestFit				(SInt32&	outWidth, 
										 SInt32&	outHeight,
										 const	SInt32		boundingWidth,
										 const	SInt32		boundingHeight,
										 const	SInt32		objectWidth,
										 const	SInt32		objectHeight,
												bool		okToExpand = kDontExpand);

	static	void	FitRectInside(const MRect& child,
								  const MRect& bounding,
								  MRect& outDestRect,
								  bool okToExpand = kDontExpand);

}; //end class EUtil
