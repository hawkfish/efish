/*
	File:		EPrefs.cp

	Contains:	Implementation of generic (Carbon-only) preferences.

	Written by:	David Dunham

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights reserved.
				Some changes courtesy A Sharp, LLC.

	Change History (most recent first):

		03 Dec 2001		drd		Methods for CFArray
		25 Oct 2001		drd		Use CFStringGetSystemEncoding() instead of assuming
		24 Oct 2001		drd		GetPref for string returns empty string if not present
		22 Oct 2001		drd		All methods are now const; GetPref, SetPref for Handle;
								PrefExists
		05 Oct 2000		drd		Use std:: with strcmp
		02 aug 2000		dml		add copy ct
		20 Jul 2000		drd		Use kCFBooleanTrue, kCFBooleanFalse in SetPref
		12 Jul 2000		drd		Stopped using NoCopy version, it didn't seem to work
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
	::CFRetain(mAppName);
} // EPrefs

/*
EPrefs
	Copy-constructor
*/
EPrefs::EPrefs(const EPrefs& other)
	: mAppName (other.mAppName)
{
	::CFRetain(mAppName);
}//end copy ct

/*
~EPrefs
*/
EPrefs::~EPrefs()
{
	// Note that we aren't calling Write. Possibly we should, though the base design was to
	// save for a Preferences dialog. Subclasses will likely want to call Write from their
	// destructor.

	::CFRelease(mAppName);
} // ~EPrefs

/*
CopyPref
	Return nil if inKey is not present
*/
void
EPrefs::CopyPref(CFStringRef inKey, CFArrayRef& outValue) const
{
	CFTypeRef	theValue = ::CFPreferencesCopyAppValue(inKey, mAppName);
	if (theValue != nil) {
		if (::CFGetTypeID(theValue) == ::CFArrayGetTypeID()) {
			outValue = static_cast<CFArrayRef>(theValue);
		} else {
			::CFRelease(theValue);
			outValue = nil;
		}
	}
} // CopyPref

/*
CopyPref
	Return nil if inKey is not present
*/
void
EPrefs::CopyPref(CFStringRef inKey, CFMutableArrayRef& outValue) const
{
	CFTypeRef	theValue = ::CFPreferencesCopyAppValue(inKey, mAppName);
	if (theValue != nil) {
		if (::CFGetTypeID(theValue) == ::CFArrayGetTypeID()) {
			outValue = (CFMutableArrayRef) (theValue);	// C cast handles both const and type
		} else {
			::CFRelease(theValue);
			outValue = nil;
		}
	}
} // CopyPref

/*
GetPref
	outValue is unchanged if inKey is not present
*/
void
EPrefs::GetPref(CFStringRef inKey, bool& outValue) const
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
	outValue is unchanged if inKey is not present
*/
void
EPrefs::GetPref(CFStringRef inKey, SInt16& outValue) const
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
GetPref
	outValue is set to empty string if inKey is not present
*/
void
EPrefs::GetPref(CFStringRef inKey, Str255& outValue) const
{
	CFTypeRef	theValue = ::CFPreferencesCopyAppValue(inKey, mAppName);
	if (theValue != nil) {
		if (::CFGetTypeID(theValue) == ::CFStringGetTypeID()) {
			::CFStringGetPascalString((CFStringRef)(theValue), outValue,
				sizeof(outValue), ::CFStringGetSystemEncoding());
		}
		::CFRelease(theValue);
	} else {
		outValue[0] = 0;						// Empty string
	}
} // GetPref

/*
GetPref
	Must be an existing Handle, which will be resized. It will be set to a size of 0 if
	the key doesn't exist.
*/
void
EPrefs::GetPref(CFStringRef inKey, Handle outValue) const
{
	CFTypeRef	theValue = ::CFPreferencesCopyAppValue(inKey, mAppName);
	if (theValue != nil) {
		if (::CFGetTypeID(theValue) == ::CFDataGetTypeID()) {
			CFRange			range;
			range.location = 0;
			range.length = ::CFDataGetLength((CFDataRef) theValue);
			::SetHandleSize(outValue, range.length);
			StHandleLocker	lock(outValue);
			::CFDataGetBytes((CFDataRef) theValue, range, (UInt8 *) *outValue);
		}
		::CFRelease(theValue);
	} else {
		::SetHandleSize(outValue, 0L);			// Indicate failure
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
	const SInt16		inDefault) const
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
	SInt16&		outVal) const
{
	Assert_(!inMap.empty());

	char		bufr[256];
	CFIndex		len = sizeof(bufr);
	if (::CFStringGetCString(inText, bufr, len, ::CFStringGetSystemEncoding())) {
		ShortEnumMap::const_iterator	i;
		for (i = inMap.begin(); i != inMap.end(); ++i) {
			if (std::strcmp((*i).second, bufr) == 0) {
				outVal = (*i).first;
				return true;
			} //endif
		} // end for
	} // endif able to get C string

	return false;
} // LookupEnum

bool
EPrefs::PrefExists(CFStringRef inKey) const
{
	// This seems a bit inefficient, since we allocate space for the data (and then deallocate) ???
	CFTypeRef	theValue = ::CFPreferencesCopyAppValue(inKey, mAppName);
	if (theValue != nil) {
		::CFRelease(theValue);
		return true;
	} else {
		return false;
	}
} // PrefExists

/*
SetPref
*/
void
EPrefs::SetPref(CFStringRef inKey, const bool inValue) const
{
	CFBooleanRef	theValue;
	if (inValue)
		theValue = kCFBooleanTrue;
	else
		theValue = kCFBooleanFalse;
		
	::CFPreferencesSetAppValue(inKey, theValue, mAppName);
} // SetPref

/*
SetPref
*/
void
EPrefs::SetPref(CFStringRef inKey, const CFArrayRef inValue) const
{
	::CFPreferencesSetAppValue(inKey, nil, mAppName);	// Remove first
	::CFPreferencesSetAppValue(inKey, inValue, mAppName);
} // SetPref

/*
SetPref
*/
void
EPrefs::SetPref(CFStringRef inKey, const char* inValue) const
{
	// Use the NoCopy variant here, since this CFString is really just a wrapper so we can
	// pass it to CFPreferences (no need to do any allocation OR deallocation)
//	CFStringRef	theValue = ::CFStringCreateWithCStringNoCopy(nil, inValue,
//					kCFStringEncodingMacRoman, nil);
	CFStringRef	theValue = ::CFStringCreateWithCString(nil, inValue,
					::CFStringGetSystemEncoding());

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
EPrefs::SetPref(CFStringRef inKey, ConstStr255Param inValue) const
{
	// Use the NoCopy variant here, since this CFString is really just a wrapper so we can
	// pass it to CFPreferences (no need to do any allocation OR deallocation)
//	CFStringRef	theValue = ::CFStringCreateWithPascalStringNoCopy(nil, inValue,
//					kCFStringEncodingMacRoman, nil);
	CFStringRef	theValue = ::CFStringCreateWithPascalString(nil, inValue,
					::CFStringGetSystemEncoding());

	::CFPreferencesSetAppValue(inKey, theValue, mAppName);

	::CFRelease(theValue);
} // SetPref

/*
SetPref
	A Handle is generic data, so use CFData
*/
void
EPrefs::SetPref(CFStringRef inKey, const Handle inValue) const
{
	StHandleLocker	lock(inValue);
	CFDataRef		theValue = ::CFDataCreateWithBytesNoCopy(nil, (UInt8 *) *inValue,
		::GetHandleSize(inValue), kCFAllocatorNull);

	::CFPreferencesSetAppValue(inKey, theValue, mAppName);

	::CFRelease(theValue);
} // SetPref

/*
Write
	Write all changes in all sources of application defaults. Returns success or failure.
*/
bool
EPrefs::Write() const
{
	return ::CFPreferencesAppSynchronize(mAppName);
} // Write
