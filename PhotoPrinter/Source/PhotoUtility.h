/*
	File:		PhotoUtility.h

	Contains:	Handy functions and constants

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		27 Jun 2000		drd		Added kLandscape, kPortrait
*/

#pragma once

#include <math.h>

enum {
	kLandscape = 'land',
	kPortrait = 'port'
};

class PhotoUtility {
public:
	// Globals
	static const double		sEpsilon;
	static const RGBColor	sNonReproBlue;	

	// Functions
	static Boolean	DoubleEqual(const double& a, const double& b) { return (fabs(a - b) < sEpsilon); }
};//end class PhotoUtility
