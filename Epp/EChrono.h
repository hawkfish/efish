/*
	File:		EChrono.h

	Contains:	Definition of utility routines dealing with dates & time.
				Based on code by David Dunham & Scott Shwarts.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Depends on:	PowerPlant (for LStr255)

	Change History (most recent first):

		03 Aug 2000		drd		Created (from PenséeDate.h)
*/

typedef enum {
	date_None = 1,
	date_Numeric,
	date_Short,
	date_Long,
	date_ShortDay,
	date_LongDay
} DateFormatT;

typedef enum {
	time_None = 1,
	time_HM,
	time_HMS,
	time_HM24,
	time_HMS24,
	time_HMsys,				// Time with hours and minutes; system decides AM/PM or 24
	time_HMSsys				// Time with hours, minutes, and seconds; system decides AM/PM or 24
} TimeFormatT;

class	EChrono {
public:

	static	void		GetDateTime(
							LStr255&			outString,
							const UInt32		inDate,
							const DateFormatT	inDateFormat,
							const TimeFormatT	inTimeFormat,
							const char inSeparator = ' ');

protected:
	static	Intl0Hndl	gItl0;
	static	Intl1Hndl	gItl1;
	static	UInt8		gTimeCycle;		// What sort of time (timeCycle12 = 12:00 - 11:59)
};
