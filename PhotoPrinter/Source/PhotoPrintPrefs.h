/*
	File:		PhotoPrintPrefs.h

	Contains:	Definition of application (Carbon-only) preferences.

	Written by:	David Dunham

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		31 Jul 2001		drd		256 Added XMLDocFormatter and XMLDocParser as friends
		12 jul 2001		dml		add mDisplayUnits, Get/Set
		11 jul 2001		dml		add mWarnDirty
		28 jun 2001		dml		added mWarnAlternate
		23 Apr 2001		drd		GetMaximumSize, GetMinimumSize now protected to make
								sure people get it through document
		01 Dec 2000		drd		26 Added mBinderMargin
		05 Oct 2000		drd		Use std:: for map
		03 Aug 2000		drd		Added mDateFormat, mTimeFormat
		02 aug 2000		dml		add copy ct, mApplyToOpen
		26 jul 2000		dml		add mSorting
		21 Jul 2000		drd		Added mAlternatePrinting, mBandedPrinting
		13 Jul 2000		drd		Added mGutter, moved kDefaultGutter here
		11 Jul 2000		drd		Added mMinimumSize, mMaximumSize, gSizeLimitMap
		11 Jul 2000		drd		Added mCaptionStyle
		10 Jul 2000		drd		Created
*/

#pragma once

#include "EChrono.h"
#include "EPrefs.h"
#include "PhotoUtility.h"
#include "PhotoItemProperties.h"

// support for the map between size limits and text
typedef	std::map<SInt16, char*> SizeLimitMap;
typedef	std::map<SInt16, char*> SortingMap;


class PhotoPrintPrefs : public EPrefs
{
public:
	friend class PhotoPrintDoc;
	friend class PrefsDialog;
	friend class XMLDocFormatter;
	friend class XMLDocParser;

	enum {
		kDefaultGutter = 72 / 8					// 1/8 inch
	};

					PhotoPrintPrefs(CFStringRef inAppName);
					PhotoPrintPrefs(const PhotoPrintPrefs& other);
	virtual			~PhotoPrintPrefs();

	static PhotoPrintPrefs*	Singleton()					{ return gSingleton; }

	bool			GetAlternatePrinting() const		{ return mAlternatePrinting; }
	bool			GetApplyToOpenDocs() const			{ return mApplyToOpenDocs;}
	bool			GetBandedPrinting() const			{ return mBandedPrinting; }
	SInt16			GetBinderMargin() const				{ return mBinderMargin; }
	CaptionT		GetCaptionStyle() const				{ return mCaptionStyle; }
	DateFormatT		GetDateFormat() const				{ return mDateFormat; }
	UnitsT			GetDisplayUnits() const				{ return mDisplayUnits;}
	SInt16			GetFontNumber()	const				{ return mFontNumber; }
	SInt16			GetFontSize() const					{ return mFontSize; }
	void			GetFontName(Str255& outName)		{ ::GetFontName(mFontNumber, outName); }
	SInt16			GetGutter() const					{ return mGutter; }
protected:
	SizeLimitT		GetMaximumSize() const				{ return mMaximumSize; }
	SizeLimitT		GetMinimumSize() const				{ return mMinimumSize; }
public:
	bool			GetShowFileDates() const			{ return mShowFileDates; }
	bool			GetShowFileNames() const			{ return mShowFileNames; }
	SortingT		GetSorting() const					{ return mSorting;}
	bool			GetSortAscending() const			{ return mSortAscending;}
	TimeFormatT		GetTimeFormat() const				{ return mTimeFormat; }
	bool			GetWarnAlternate() const			{ return mWarnAlternate; }
	bool			GetWarnDirty() const				{ return mWarnDirty;};
	bool			GetWarnRename() const				{ return mWarnRename;};

	// Setters (set instance data and prefs structure in memory)
	void			SetAlternatePrinting(const bool inVal);
	void			SetApplyToOpenDocs(const bool inVal);
	void			SetBandedPrinting(const bool inVal);
	void			SetBinderMargin(const SInt16 inVal);
	void			SetCaptionStyle(const CaptionT inStyle);
	void			SetDateFormat(const DateFormatT inVal);
	void			SetDisplayUnits(const UnitsT inVal);
	void			SetFontNumber(const SInt16 inFont);
	void			SetFontSize(const SInt16 inSize);
	void			SetGutter(const SInt16 inVal);
	void			SetMaximumSize(const SizeLimitT inVal);
	void			SetMinimumSize(const SizeLimitT inVal);
	void			SetShowFileDates(const bool inVal);
	void			SetShowFileNames(const bool inVal);
	void			SetSorting(const SortingT inVal);
	void			SetSortAscending(const bool inVal);
	void			SetTimeFormat(const TimeFormatT inVal);
	void			SetWarnAlternate(const bool inVal);
	void			SetWarnDirty(const bool inVal);
	void			SetWarnRename(const bool inVal);
	
protected:
	// Application Preferences
	bool		mAlternatePrinting;
	bool		mApplyToOpenDocs;
	bool		mBandedPrinting;
	SInt16		mBinderMargin;				// Width in pixels
	CaptionT	mCaptionStyle;
	SInt16		mFontNumber;
	SInt16		mFontSize;
	SInt16		mGutter;
	SizeLimitT	mMaximumSize;
	SizeLimitT	mMinimumSize;
	bool		mShowFileDates;
	bool		mShowFileNames;
	DateFormatT	mDateFormat;
	TimeFormatT	mTimeFormat;
	SortingT	mSorting;
	bool		mSortAscending;

	bool		mWarnAlternate;
	bool		mWarnDirty;
	bool		mWarnRename;
	UnitsT		mDisplayUnits;

	static	PhotoPrintPrefs*	gSingleton;
	static	SizeLimitMap		gSizeLimitMap;
	static	SortingMap			gSortingMap;
};
