/*
	File:		LayoutMapper.h

	Contains:	Interface to the Layout type/XML name mapper.

	Written by:	Dav Lion

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):
	
		26 Jul 2001		rmgw	Factored from PhotoPrintDoc.

	Description:
	
		support for the map between alignment type and text
*/

#pragma once

#include <map>

typedef	std::map<OSType, char*, std::less<OSType> > LayoutMap;

class LayoutMapper {
	protected:
		static	bool sInitialized;
		static	LayoutMap	sMap;
		static	void Initialize();
		
	public :	
		static const char*			Find(OSType key);
		static OSType		Lookup(const char* text);
	};//end class LayoutMapper
	
