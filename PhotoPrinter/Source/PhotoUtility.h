/*
	File:		PhotoUtility.h

	Contains:	Handy functions and constants

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		11 Aug 2000		drd		Took curs_ and tool_ enums out (had already moved, thanks to concurrent development)
		11 aug 2000		dml		move cursor/tool enums stuff here (from app)
		07 aug 2000		dml		add kRadToDegrees
		03 Aug 2000		drd		Revised SortingT so sort_None is first in menu, tweaked names
		02 aug 2000		dml		added sort_nothing
		28 jul 2000		dml		sort codes start at 1 (to match menu!)
		26 jul 2000		dml		added kSort... codes as enum (SortingT)
		20 jul 2000		dml		added sWhiteRGB
		18 jul 2000		dml		added kFirstSpinCursor, kNumCursors
		13 jul 2000		dml		add kBiasRows, kBiasColumns (for grid layout)
		30 Jun 2000		drd		Added kDPI
		27 Jun 2000		drd		Added kLandscape, kPortrait
*/

#pragma once

#include <math.h>

enum {
	kLandscape = 'land',
	kPortrait = 'port',

	kDPI = 72,										// Nominal dots per inch (pixels/inch)

	kBiasRows = 'rows',
	kBiasCols = 'cols',
	
	kFirstSpinCursor = 5000,
	kNumCursors = 13
};

typedef enum {
	sort_None = 1,		// correspond to first menu item!
	sort_Creation,
	sort_Modification,
	sort_Name
} SortingT;


class PhotoUtility {
public:
	// Globals
	static const double		sEpsilon;
	static const RGBColor	sNonReproBlue;	
	static const RGBColor	sWhiteRGB;	
	static const double 	kRad2Degrees;	
	
	// Functions
	static Boolean	DoubleEqual(const double& a, const double& b) { return (fabs(a - b) < sEpsilon); }
};//end class PhotoUtility
