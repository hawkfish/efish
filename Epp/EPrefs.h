/*
	File:		EPrefs.h

	Contains:	Definition of generic (Carbon-only) preferences.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		11 Jul 2000		drd		Added a char* SetPref
		10 Jul 2000		drd		Created
*/

#pragma once

#include <CFBase.h>

class EPrefs {
public:
					EPrefs(CFStringRef inAppName);
	virtual			~EPrefs();

			
			void	GetPref(CFStringRef inKey, bool& outValue);
			void	GetPref(CFStringRef inKey, SInt16& outValue);

			void	SetPref(CFStringRef inKey, const bool inValue) const;
			void	SetPref(CFStringRef inKey, const char* inValue) const;
			void	SetPref(CFStringRef inKey, const SInt16 inValue) const;
			void	SetPref(CFStringRef inKey, ConstStr255Param inValue) const;

			bool	Write();

protected:
	CFStringRef		mAppName;
};
