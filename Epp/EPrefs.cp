/*
	File:		EPrefs.cp

	Contains:	Implementation of generic (Carbon-only) preferences.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		11 Jul 2000		drd		GetShortEnumPref, LookupEnum
		11 Jul 2000		drd		Added a char* SetPref
		10 Jul 2000		drd		Created
*/

#include "EPrefs.h"

#include <CFNumber.h>
#include <CFPreferences.h>
#include <CFString.h>

/*
EPrefs
*/
EPrefs::EPrefs(CFStringRef inAppName)
	: mAppName(inAppName)
{
} // EPrefs

/*
~EPrefs
*/
EPrefs::~EPrefs()
{
} // ~EPrefs

/*
GetPref
*/
void
EPrefs::GetPref(CFStringRef inKey, bool& outValue)
{
	CFTypeRef	theValue = ::CFPreferencesCopyAppValue(inKey, mAppName);
	if (theValue != nil) {
		if (::CFGetTypeID(theValue) == ::CFBooleanGetTypeID()) {
			Boolean	theBool = ::CFBooleanGetValue((CFBooleanRef)theValue);
			outValue = theBool;
		} else if (::CFGetTypeID(theValue) == ::CFNumberGetTypeID()) {
			SInt16	theInt;
			::CFNumberGetValue((CFNumberRef)theValue, kCFNumberSInt16Type, &theInt);
			outValue = theInt;
		}
		::CFRelease(theValue);
	}
} // GetPref

/*
GetPref
*/
void
EPrefs::GetPref(CFStringRef inKey, SInt16& outValue)
{
	CFTypeRef	theValue = ::CFPreferencesCopyAppValue(inKey, mAppName);
	if (theValue != nil) {
		if (::CFGetTypeID(theValue) == ::CFNumberGetTypeID()) {
			SInt16	theInt;
			::CFNumberGetValue((CFNumberRef)theValue, kCFNumberSInt16Type, &theInt);
			outValue = theInt;
		}
		::CFRelease(theValue);
	}
} // GetPref

/*
GetShortEnumPref
	Read an enumerated value (-32768..32767) saved as a string
*/
SInt16
EPrefs::GetShortEnumPref(
	CFStringRef			inKey,
	const ShortEnumMap& inMap,
	const SInt16		inDefault)
{
	SInt16		retVal = inDefault;

	CFTypeRef	theValue = ::CFPreferencesCopyAppValue(inKey, mAppName);
	if (theValue != nil) {
		if (::CFGetTypeID(theValue) == ::CFStringGetTypeID()) {
			SInt16		theEnum;
			if (this->LookupEnum((CFStringRef)(theValue), inMap, theEnum))
				retVal = theEnum;
		}
		::CFRelease(theValue);
	}

	return retVal;
} // GetShortEnumPref

/*
LookupEnum
	Returns TRUE if we found it, FALSE otherwise
*/
bool
EPrefs::LookupEnum(
	CFStringRef inText,
	const ShortEnumMap& inMap,
	SInt16&		outVal)
{
	Assert_(!inMap.empty());

	char		bufr[256];
	CFIndex		len = sizeof(bufr);
	if (::CFStringGetCString(inText, bufr, len, kCFStringEncodingMacRoman)) {
		ShortEnumMap::const_iterator	i;
		for (i = inMap.begin(); i != inMap.end(); ++i) {
			if (strcmp((*i).second, bufr) == 0) {
				outVal = (*i).first;
				return true;
			} //endif
		} // end for
	} // endif able to get C string

	return false;
} // LookupEnum

/*
SetPref
*/
void
EPrefs::SetPref(CFStringRef inKey, const bool inValue) const
{
	// Just save the bool as a short integer
	SInt16		theValue = inValue;
	this->SetPref(inKey, theValue);
} // SetPref

/*
SetPref
*/
void
EPrefs::SetPref(CFStringRef inKey, const char* inValue) const
{
	// Use the NoCopy variant here, since this CFString is really just a wrapper so we can
	// pass it to CFPreferences (no need to do any allocation OR deallocation)
	CFStringRef	theValue = ::CFStringCreateWithCStringNoCopy(nil, inValue,
					kCFStringEncodingMacRoman, nil);

	::CFPreferencesSetAppValue(inKey, theValue, mAppName);

	::CFRelease(theValue);
} // SetPref

/*
SetPref
*/
void
EPrefs::SetPref(CFStringRef inKey, const SInt16 inValue) const
{
	CFNumberRef	theValue = ::CFNumberCreate(nil, kCFNumberSInt16Type, &inValue);

	::CFPreferencesSetAppValue(inKey, theValue, mAppName);

	::CFRelease(theValue);
} // SetPref

/*
SetPref
*/
void
EPrefs::SetPref(CFStringRef inKey, const ConstStr255Param inValue) const
{
	// Use the NoCopy variant here, since this CFString is really just a wrapper so we can
	// pass it to CFPreferences (no need to do any allocation OR deallocation)
	CFStringRef	theValue = ::CFStringCreateWithPascalStringNoCopy(nil, inValue,
					kCFStringEncodingMacRoman, nil);

	::CFPreferencesSetAppValue(inKey, theValue, mAppName);

	::CFRelease(theValue);
} // SetPref

/*
Write
	Write all changes in all sources of application defaults. Returns success or failure.
*/
bool
EPrefs::Write()
{
	return ::CFPreferencesAppSynchronize(mAppName);
} // Write
