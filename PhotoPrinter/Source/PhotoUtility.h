// PhotoUtility.h
// Copyright © 2000 Electric Fish, Inc.


#pragma once
#include <math.h>

class PhotoUtility {
	public:
		static const double sEpsilon;
		static Boolean DoubleEqual(const double& a, const double& b) {return (fabs(a - b) < sEpsilon);};
		static const RGBColor	sNonReproBlue;	
	};//end class PhotoUtility
	
	
