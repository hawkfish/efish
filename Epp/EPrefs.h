/*
	File:		EPrefs.h

	Contains:	Definition of generic (Carbon-only) preferences.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		02 aug 2000		dml		add copy ct
		12 Jul 2000		drd		GetPref for Str255
		11 Jul 2000		drd		Methods for enums
		11 Jul 2000		drd		Added a char* SetPref
		10 Jul 2000		drd		Created
*/

#pragma once

#include <CFBase.h>
#include <map>

typedef	std::map<SInt16, char*> ShortEnumMap;
typedef	std::map<SInt32, char*> LongEnumMap;

class EPrefs {
public:
					EPrefs(CFStringRef inAppName);
					EPrefs(const EPrefs& other);
	virtual			~EPrefs();

			
			SInt16	GetLongEnumPref(CFStringRef inKey, const LongEnumMap& inMap, const SInt32 inDefault = 0);
			void	GetPref(CFStringRef inKey, bool& outValue);
			void	GetPref(CFStringRef inKey, SInt16& outValue);
			void	GetPref(CFStringRef inKey, Str255& outValue);
			SInt16	GetShortEnumPref(CFStringRef inKey, const ShortEnumMap& inMap, const SInt16 inDefault = 0);

			void	SetPref(CFStringRef inKey, const bool inValue) const;
			void	SetPref(CFStringRef inKey, const char* inValue) const;
			void	SetPref(CFStringRef inKey, const SInt16 inValue) const;
			void	SetPref(CFStringRef inKey, ConstStr255Param inValue) const;

			bool	Write();

protected:
			bool	LookupEnum(CFStringRef inText, const ShortEnumMap& inMap, SInt16& outVal);

	// Instance data
	CFStringRef		mAppName;
};
