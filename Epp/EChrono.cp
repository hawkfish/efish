/*
	File:		EChrono.cp

	Contains:	Utility routines dealing with dates & time.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Depends on:	PowerPlant (for LStr255)

	Change History (most recent first):

	03 Aug 2000		drd		Created (from
*/

#include "EChrono.h"

Intl0Hndl	EChrono::gItl0 = nil;		// Time and numeric date format info
Intl1Hndl	EChrono::gItl1 = nil;		// Alpha date format info
UInt8		EChrono::gTimeCycle;		// What sort of time (timeCycle12 = 12:00 - 11:59)

/*
GetDateTime
*/
void	EChrono::GetDateTime(
	LStr255&			outString,
	const UInt32		inDate,
	const DateFormatT	inDateFormat,
	const TimeFormatT	inTimeFormat,
	const char			inSeparator)
{
	DateForm		format;
	Handle			itl;

	// The first time, get the resources and make a non-purgeable copy so we can change them as needed
	if (gItl0 == nil) {
		gItl0 = (Intl0Hndl)::GetIntlResource(0);
		::DetachResource((Handle)gItl0);
		::HNoPurge((Handle)gItl0);
		gTimeCycle = (*gItl0)->timeCycle;
	}
	if (gItl1 == nil) {
		gItl1 = (Intl1Hndl)::GetIntlResource(1);
		::DetachResource((Handle)gItl1);
		::HNoPurge((Handle)gItl1);
	}

	if (inDateFormat != date_None) {
		switch (inDateFormat) {
			case date_Numeric:
				itl = (Handle)gItl0;
				format = shortDate;
				break;
				
			case date_Short:
				(*gItl1)->suppressDay = supWeek;
				itl = (Handle)gItl1;
				format = abbrevDate;
				break;

			case date_Long:
				(*gItl1)->suppressDay = supWeek;
				itl =(Handle) gItl1;
				format = longDate;
				break;
	
			case date_ShortDay:
				(*gItl1)->suppressDay = 0;
				itl = (Handle)gItl1;
				format = abbrevDate;
				break;

			case date_LongDay:
				(*gItl1)->suppressDay = 0;
				itl = (Handle)gItl1;
				format = longDate;
				break;
		}
		::DateString(inDate, format, outString, itl);
	} else {
		outString[0] = 0;
	}

	if (inTimeFormat != time_None) {
		LStr255		timeString;
		Boolean		wantSecs = (inTimeFormat == time_HMS || inTimeFormat == time_HMS24);
		if (inTimeFormat == time_HM24 || inTimeFormat == time_HMS24) {
			(*gItl0)->timeCycle = timeCycle24;
		} else if (inTimeFormat == time_HMsys || inTimeFormat == time_HMSsys) {
			(*gItl0)->timeCycle = gTimeCycle;
		} else {
			(*gItl0)->timeCycle = timeCycle12;
		}
		::TimeString(inDate, wantSecs, timeString, (Handle)gItl0);
		outString += inSeparator;
		outString += timeString;
	}
} // GetDateTime
