// PhotoUtility.h
// Copyright © 2000 Electric Fish, Inc.
#pragma once
#include <math.h>

class PhotoUtility {
	public:
		static const double epsilon;
		static Boolean DoubleEqual(const double& a, const double& b) {return (fabs(a - b) < epsilon);};
	
	};//end class PhotoUtility
	
	
