/*
	File:		LayoutMapper.cp

	Contains:	Implementation of the Layout type/XML name mapper.

	Written by:	Dav Lion

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):
	
		26 Jul 2001		rmgw	Factored from PhotoPrintDoc.

	Description:
	
		support for the map between alignment type and text
*/

#include "LayoutMapper.h"

#include "Layout.h"

bool 
LayoutMapper::sInitialized = false;

LayoutMap	
LayoutMapper::sMap;

void
LayoutMapper::Initialize() {
	sMap[Layout::kUnspecified] = "Unspecified";
	sMap[Layout::kCollage] = "Collage";
	sMap[Layout::kFixed] = "Fixed";
	sMap[Layout::kGrid] = "Grid";
	sMap[Layout::kMultiple] = "Multiple";
	sMap[Layout::kSchool] = "School";
	sMap[Layout::kSingle] = "Single";
	sInitialized = true;
	}//end
	
const char*
LayoutMapper::Find(OSType key) {
	if (!sInitialized) {
		Initialize();
		}//endif need to construct

		LayoutMap::const_iterator	i (sMap.find (key));
		if (i != sMap.end ()) 
			return (*i).second;
		else
			return 0;
	}//end Find
	
OSType
LayoutMapper::Lookup(const char* text) {
	if (!sInitialized) {
		Initialize();
		}//endif need to construct

	for (LayoutMap::const_iterator	i = sMap.begin(); i != sMap.end(); ++i) {
		if (strcmp((*i).second, text) == 0) {
			return (*i).first;
			}//endif
		}//end

	return Layout::kUnspecified;
}//end Lookup

