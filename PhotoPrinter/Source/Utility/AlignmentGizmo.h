/*
	File:		AlignmentGizmo.h

	Contains:	utility routines for alignment.  const->text, also rect placements

	Written by:	Dav Lion

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		16 jun 2000		dml		initial revision
*/
#pragma once

#include <map.h>
#include <MRect.h>

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
	};//end class AlignmentGizmo


