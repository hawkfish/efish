// PhotoUtility.cp
// Copyright © 2000 Electric Fish, Inc.

/* Change Log (most recent first)
*
*	21 aug 2000		dml		lowered epsilon to 0.00001
*/


#include "PhotoUtility.h"

const double PhotoUtility::sEpsilon = 0.00001;

const RGBColor	PhotoUtility::sNonReproBlue = {38912,57856, 57856};
const RGBColor	PhotoUtility::sWhiteRGB = {65535, 65535, 65535};
const double 	PhotoUtility::kRad2Degrees = 57.2958;