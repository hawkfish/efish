/*
	File:		PhotoPrintPrefs.cp

	Contains:	Implementation of application (Carbon-only) preferences.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		02 aug 2000		dml		add copy ct, mApplyToOpenDocs
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
	, mCaptionStyle(caption_None)
	, mFontNumber(kPlatformDefaultGuiFontID)
	, mFontSize(12)		// !!! what is the system size
	, mGutter(kDefaultGutter)
	, mMaximumSize(limit_None)
	, mMinimumSize(limit_Slide)
	, mShowFileDates(false)
	, mShowFileNames(false)
	, mSorting(sort_creation)
	, mSortAscending(true)
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
		gSortingMap[sort_creation] = "by Creation Date";
		gSortingMap[sort_modification] = "by Modification Date";
		gSortingMap[sort_name] = "by Name";
		}//endif need to initialize sorting map

	// Load preferences from the file
	this->GetPref(CFSTR("alternatePrinting"), mAlternatePrinting);
	this->GetPref(CFSTR("ApplyToOpenDocs"), mApplyToOpenDocs);
	this->GetPref(CFSTR("bandedPrinting"), mBandedPrinting);

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

	mSorting = (SortingT)this->GetShortEnumPref(CFSTR("sorting"),
		gSortingMap, sort_creation);
		
	this->GetPref(CFSTR("sortAscending"), mSortAscending);
	
} // PhotoPrintPrefs



//--------------------------------------------
// PhotoPrintPrefs copy ct
//--------------------------------------------
PhotoPrintPrefs::PhotoPrintPrefs(const PhotoPrintPrefs& other)
	: EPrefs (other)
	, mAlternatePrinting (other.GetAlternatePrinting())
	, mApplyToOpenDocs (other.GetApplyToOpenDocs())
	, mBandedPrinting (other.GetBandedPrinting())
	, mCaptionStyle (other.GetCaptionStyle())
	, mFontNumber (other.GetFontNumber())
	, mFontSize (other.GetFontSize())
	, mGutter (other.GetGutter())
	, mMaximumSize (other.GetMaximumSize())
	, mMinimumSize (other.GetMinimumSize())
	, mShowFileDates (other.GetShowFileDates())
	, mShowFileNames (other.GetShowFileNames())
	, mSorting (other.GetSorting())
	, mSortAscending (other.GetSortAscending())
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
	this->SetPref(CFSTR("ApplyToOpenDocs"), inVal);
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