/*
	File:		PhotoPrintPrefs.h

	Contains:	Definition of application (Carbon-only) preferences.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		11 Jul 2000		drd		Added mCaptionStyle
		10 Jul 2000		drd		Created
*/

#pragma once

#include "EPrefs.h"

#include "PhotoItemProperties.h"

class PhotoPrintPrefs : public EPrefs
{
public:
					PhotoPrintPrefs(CFStringRef inAppName);
	virtual			~PhotoPrintPrefs();

	static PhotoPrintPrefs*	Singleton()					{ return gSingleton; }

	CaptionT		GetCaptionStyle() const				{ return mCaptionStyle; }
	SInt16			GetFontNumber()						{ return mFontNumber; }
	SInt16			GetFontSize()						{ return mFontSize; }
	void			GetFontName(Str255& outName)		{ ::GetFontName(mFontNumber, outName); }
	bool			GetShowFileDates()					{ return mShowFileDates; }
	bool			GetShowFileNames()					{ return mShowFileNames; }

	// Setters (set instance data and prefs structure in memory)
	void			SetCaptionStyle(const CaptionT inStyle);
	void			SetFontNumber(const SInt16 inFont);
	void			SetFontSize(const SInt16 inSize);
	void			SetShowFileDates(const bool inVal);
	void			SetShowFileNames(const bool inVal);

protected:
	// Application Preferences
	CaptionT	mCaptionStyle;
	SInt16		mFontNumber;
	SInt16		mFontSize;
	bool		mShowFileDates;
	bool		mShowFileNames;

	static	PhotoPrintPrefs*	gSingleton;
};
