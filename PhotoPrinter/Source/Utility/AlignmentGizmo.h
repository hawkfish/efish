/*
	File:		AlignmentGizmo.h

	Contains:	utility routines for alignment.  const->text, also rect placements

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		14 sep 2000		dml		add ERect32 alignment ops
		03 Jul 2000		drd		Default arg to FitAndAlignRectInside is kDontExpand
		29 jun 2000		dml		add FitAndAlignRectInside
		16 jun 2000		dml		initial revision
*/

#pragma once

#include <map.h>
#include "EUtil.h"
#include "ERect32.h"

// support for the map between alignment type and text
typedef	map<AlignmentType, char*, less<AlignmentType> > AlignmentMap;

class AlignmentGizmo {
	protected:
		static	bool sInitialized;
		static	AlignmentMap	sAlignmentMap;
		static	void Initialize();
		
	public :	
		static const char*			Find(AlignmentType key);
		static AlignmentType		Lookup(const char* text);
		static	void	AlignRectInside(const MRect& target,
										const MRect& bounding,
										AlignmentType alignment,
										MRect&	outDestRect);
		static	void	AlignRectInside(const ERect32& target,
										const ERect32& bounding,
										AlignmentType alignment,
										ERect32&	outDestRect);
		static	void	FitAndAlignRectInside(const MRect& target,
											const MRect& bounding,
											AlignmentType alignment,
											MRect&	outDestRect,
											bool okToExpand = EUtil::kDontExpand);
		static	void	FitAndAlignRectInside(const ERect32& target,
											const ERect32& bounding,
											AlignmentType alignment,
											ERect32&	outDestRect,
											bool okToExpand = EUtil::kDontExpand);
	};//end class AlignmentGizmo


