/*
	File:		PhotoPrintPrefs.cp

	Contains:	Implementation of application (Carbon-only) preferences.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		11 Jul 2000		drd		Added mCaptionStyle
		11 Jul 2000		drd		Created
*/

#include "PhotoPrintPrefs.h"

#include <CFString.h>

// Globals
PhotoPrintPrefs*	PhotoPrintPrefs::gSingleton = nil;

/*
PhotoPrintPrefs
*/
PhotoPrintPrefs::PhotoPrintPrefs(CFStringRef inAppName)
	: EPrefs(inAppName)
	, mCaptionStyle(caption_None)
	, mFontNumber(kPlatformDefaultGuiFontID)
	, mFontSize(12)		// !!! what is the system size
	, mShowFileDates(false)
	, mShowFileNames(false)
{
	Assert_(gSingleton == nil);
	gSingleton = this;

	// Load preferences
	SInt16		theInt16;
	this->GetPref(CFSTR("captionStyle"), theInt16);
	mCaptionStyle = (CaptionT)theInt16;
	this->GetPref(CFSTR("showFileDates"), mShowFileDates);
	this->GetPref(CFSTR("showFileNames"), mShowFileNames);
} // PhotoPrintPrefs

/*
~PhotoPrintPrefs
*/
PhotoPrintPrefs::~PhotoPrintPrefs()
{
	gSingleton = nil;
} // ~PhotoPrintPrefs

/*
SetCaptionStyle
*/
void
PhotoPrintPrefs::SetCaptionStyle(const CaptionT inStyle)
{
	mCaptionStyle = inStyle;
	this->SetPref(CFSTR("captionStyle"), (SInt16)inStyle);
} // SetCaptionStyle

/*
SetFontNumber
*/
void
PhotoPrintPrefs::SetFontNumber(const SInt16 inFont)
{
	mFontNumber = inFont;

	Str255			font;
	this->GetFontName(font);
	this->SetPref(CFSTR("fontName"), font);
} // SetFontNumber

/*
SetFontSize
*/
void
PhotoPrintPrefs::SetFontSize(const SInt16 inSize)
{
	mFontSize = inSize;
	this->SetPref(CFSTR("fontSize"), inSize);
} // SetFontSize

/*
SetShowFileDates
*/
void
PhotoPrintPrefs::SetShowFileDates(const bool inVal)
{
	mShowFileDates = inVal;
	this->SetPref(CFSTR("showFileDates"), inVal);
} // SetShowFileDates

/*
SetShowFileNames
*/
void
PhotoPrintPrefs::SetShowFileNames(const bool inVal)
{
	mShowFileNames = inVal;
	this->SetPref(CFSTR("showFileNames"), inVal);
} // SetShowFileNames
