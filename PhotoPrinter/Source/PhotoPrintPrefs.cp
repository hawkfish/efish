/*
	File:		PhotoPrintPrefs.cp

	Contains:	Implementation of application (Carbon-only) preferences.

	Written by:	David Dunham and Dav Lion

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		28 jun 2001		dml		added mWarnAlternate
		01 Dec 2000		drd		26 Added mBinderMargin
		03 Aug 2000		drd		Changed sort constants (and XML rep of sort_None)
		03 Aug 2000		drd		added mDateFormat, mTimeFormat
		02 aug 2000		dml		add copy ct, mApplyToOpenDocs, sort_nothing entry
		21 Jul 2000		drd		Added mAlternatePrinting, mBandedPrinting
		17 Jul 2000		drd		Added limit_Index
		13 Jul 2000		drd		Added mGutter
		12 Jul 2000		drd		Default minimum now limit_Slide; read font prefs
		11 Jul 2000		drd		Added mMinimumSize, mMaximumSize, gSizeLimitMap
		11 Jul 2000		drd		Added mCaptionStyle
		11 Jul 2000		drd		Created
*/

#include "PhotoPrintPrefs.h"

#include <CFString.h>
#include "EChrono.h"

// Globals
PhotoPrintPrefs*	PhotoPrintPrefs::gSingleton = nil;
SizeLimitMap		PhotoPrintPrefs::gSizeLimitMap;
SortingMap			PhotoPrintPrefs::gSortingMap;

/*
PhotoPrintPrefs
*/
PhotoPrintPrefs::PhotoPrintPrefs(CFStringRef inAppName)
	: EPrefs(inAppName)
	, mAlternatePrinting(false)
	, mApplyToOpenDocs(true)
	, mBandedPrinting(false)
	, mBinderMargin(0)
	, mCaptionStyle(caption_None)
	, mFontNumber(kPlatformDefaultGuiFontID)
	, mFontSize(12)		// !!! what is the system size
	, mGutter(kDefaultGutter)
	, mMaximumSize(limit_None)
	, mMinimumSize(limit_Slide)
	, mShowFileDates(false)
	, mShowFileNames(false)
	, mDateFormat(date_Short)
	, mTimeFormat(time_None)
	, mSorting(sort_Creation)
	, mSortAscending(true)
	, mWarnAlternate (true)
{
	// Enforce our singleton nature
	Assert_(gSingleton == nil);
	gSingleton = this;

	// Initialize our map the first time
	if (gSizeLimitMap.empty()) {
	    gSizeLimitMap[limit_None] = "none";
		gSizeLimitMap[limit_Index] = "index";
		gSizeLimitMap[limit_Slide] = "slide";
		gSizeLimitMap[limit_3by2] = "3*2";
		gSizeLimitMap[limit_5by3] = "5*3";
		gSizeLimitMap[limit_6by4] = "6*4";
		gSizeLimitMap[limit_7by5] = "7*5";
		gSizeLimitMap[limit_10by7half] = "10*7.5";
	}

	if (gSortingMap.empty()) {
		gSortingMap[sort_Creation] = "by Creation Date";
		gSortingMap[sort_Modification] = "by Modification Date";
		gSortingMap[sort_Name] = "by Name";
		gSortingMap[sort_None] = "no sort";
	}//endif need to initialize sorting map

	EChrono::InitializeDateFormatName();
	EChrono::InitializeTimeFormatName();

	// Load preferences from the file
	this->GetPref(CFSTR("alternatePrinting"), mAlternatePrinting);
	this->GetPref(CFSTR("ApplyToOpenDocs"), mApplyToOpenDocs);
	this->GetPref(CFSTR("bandedPrinting"), mBandedPrinting);
	this->GetPref(CFSTR("binderMargin"), mBinderMargin);

	mMaximumSize = (SizeLimitT)this->GetShortEnumPref(CFSTR("maximumSize"),
		gSizeLimitMap, limit_None);
	mMinimumSize = (SizeLimitT)this->GetShortEnumPref(CFSTR("minimumSize"),
		gSizeLimitMap, limit_Slide);

	SInt16		theInt16;
	this->GetPref(CFSTR("captionStyle"), theInt16);
	mCaptionStyle = (CaptionT)theInt16;

	this->GetPref(CFSTR("fontSize"), mFontSize);

	Str255		fontName;
	this->GetPref(CFSTR("fontName"), fontName);
	::GetFNum(fontName, &mFontNumber);
	// !!! handle missing font

	this->GetPref(CFSTR("gutter"), mGutter);

	this->GetPref(CFSTR("showFileDates"), mShowFileDates);
	this->GetPref(CFSTR("showFileNames"), mShowFileNames);
	mDateFormat = (DateFormatT)this->GetShortEnumPref(CFSTR("dateFormat"),
		EChrono::gDateFormatMap, mDateFormat);
	mTimeFormat = (TimeFormatT)this->GetShortEnumPref(CFSTR("timeFormat"),
		EChrono::gTimeFormatMap, mTimeFormat);

	mSorting = (SortingT)this->GetShortEnumPref(CFSTR("sorting"), gSortingMap, mSorting);
	this->GetPref(CFSTR("sortAscending"), mSortAscending);

	this->GetPref(CFSTR("warnAlternate"), mWarnAlternate);
} // PhotoPrintPrefs


//--------------------------------------------
// PhotoPrintPrefs copy ct
//--------------------------------------------
PhotoPrintPrefs::PhotoPrintPrefs(const PhotoPrintPrefs& other)
	: EPrefs (other)
	, mAlternatePrinting (other.GetAlternatePrinting())
	, mApplyToOpenDocs (other.GetApplyToOpenDocs())
	, mBandedPrinting (other.GetBandedPrinting())
	, mBinderMargin (other.GetBinderMargin())
	, mCaptionStyle (other.GetCaptionStyle())
	, mFontNumber (other.GetFontNumber())
	, mFontSize (other.GetFontSize())
	, mGutter (other.GetGutter())
	, mMaximumSize (other.GetMaximumSize())
	, mMinimumSize (other.GetMinimumSize())
	, mShowFileDates (other.GetShowFileDates())
	, mShowFileNames (other.GetShowFileNames())
	, mDateFormat(other.mDateFormat)
	, mTimeFormat(other.mTimeFormat)
	, mSorting (other.GetSorting())
	, mSortAscending (other.GetSortAscending())
	, mWarnAlternate (other.GetWarnAlternate())
{
}//end copy ct


/*
~PhotoPrintPrefs
*/
PhotoPrintPrefs::~PhotoPrintPrefs()
{
	if (gSingleton == this) {
		// Be sure all changes are flushed
		this->Write();

		// Realistically, after we're destructed, there will never be another, but keep
		// track of our singleton nature anyway
		gSingleton = nil;
		}//endif we're the singleton
} // ~PhotoPrintPrefs

/*
SetAlternatePrinting
*/
void
PhotoPrintPrefs::SetAlternatePrinting(const bool inVal)
{
	mAlternatePrinting = inVal;
	this->SetPref(CFSTR("alternatePrinting"), inVal);
} // SetAlternatePrinting


void
PhotoPrintPrefs::SetApplyToOpenDocs(const bool inVal)
{
	mApplyToOpenDocs = inVal;
	this->SetPref(CFSTR("applyToOpenDocs"), inVal);
}//end 

/*
SetBandedPrinting
*/
void
PhotoPrintPrefs::SetBandedPrinting(const bool inVal)
{
	mBandedPrinting = inVal;
	this->SetPref(CFSTR("bandedPrinting"), inVal);
} // SetBandedPrinting

/*
SetBinderMargin
*/
void
PhotoPrintPrefs::SetBinderMargin(const SInt16 inVal)
{
	mBinderMargin = inVal;
	this->SetPref(CFSTR("binderMargin"), inVal);
} // SetBinderMargin

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
SetDateFormat
*/
void
PhotoPrintPrefs::SetDateFormat(const DateFormatT inVal)
{
	mDateFormat = inVal;
	this->SetPref(CFSTR("dateFormat"), EChrono::gDateFormatMap[inVal]);
} // SetDateFormat

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
SetGutter
*/
void
PhotoPrintPrefs::SetGutter(const SInt16 inVal)
{
	mGutter = inVal;
	this->SetPref(CFSTR("gutter"), inVal);
} // SetGutter

/*
SetMaximumSize
*/
void
PhotoPrintPrefs::SetMaximumSize(const SizeLimitT inVal)
{
	mMaximumSize = inVal;
	this->SetPref(CFSTR("maximumSize"), gSizeLimitMap[inVal]);
} // SetMaximumSize

/*
SetMinimumSize
*/
void
PhotoPrintPrefs::SetMinimumSize(const SizeLimitT inVal)
{
	mMinimumSize = inVal;
	this->SetPref(CFSTR("minimumSize"), gSizeLimitMap[inVal]);
} // SetMinimumSize

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


//------------------------------------------------
//
//------------------------------------------------
void
PhotoPrintPrefs::SetSorting(const SortingT inVal)
{
	mSorting = inVal;
	this->SetPref(CFSTR("sorting"), gSortingMap[inVal]);
}//end SetSorting



void
PhotoPrintPrefs::SetSortAscending(const bool inVal)
{
	mSortAscending = inVal;
	this->SetPref(CFSTR("sortAscending"), inVal);
}//end SetSortAscending

/*
SetTimeFormat
*/
void
PhotoPrintPrefs::SetTimeFormat(const TimeFormatT inVal)
{
	mTimeFormat = inVal;
	this->SetPref(CFSTR("timeFormat"), EChrono::gTimeFormatMap[inVal]);
} // SetTimeFormat


/*
SetWarnAlternate
*/
void
PhotoPrintPrefs::SetWarnAlternate(const bool inVal)
{
	mWarnAlternate = inVal;
	this->SetPref(CFSTR("warnAlternate"), inVal);
} // SetWarnAlternate
