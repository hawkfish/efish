/*
	File:		PhotoUtility.h

	Contains:	Handy functions and constants

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000-2002 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		07 Jan 2002		drd		382 Added GetUnitsMap accessor (and changed UnitsMap definition)
		21 aug 2001		dml		add kClampToBounds, kDontClampToBounds.  add CalcCropValuesAsPercentages
		12 jul 2001		dml		added UnitsT
		06 jul 2001		dml		move gNeedDoubleOrientationSetting here
		28 Jun 2001		drd		75 101 kFillHilite
		5 dec 2000		dml		added AnnoyingLocationT
		22 Sep 2000		drd		Moved DrawXformedRect here, and added RectOperationT arg
		21 sep 2000		dml		add kHardwiredHeaderSize
		19 Sep 2000		drd		Added gSizeMap, GetSize, InitializeSizeMap; removed sWhiteRGB
		07 sep 2000		dml		numCursors = 8 (removed stupid ones)
		12 aug 2000		dml		added kCalcWithNaturalBounds, kCalcWithXforms
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

#include <map>
#include <math.h>
#include "ERect32.h"

enum {
	kLandscape = 'land',
	kPortrait = 'port',

	kDPI = 72,										// Nominal dots per inch (pixels/inch)

	kBiasRows = 'rows',
	kBiasCols = 'cols',
	
	kFirstSpinCursor = 5000,
	kNumCursors = 8,
	
	kCalcWithNaturalBounds = true,
	kCalcWithXforms = false,

	kDrawWithRegions = true,
	kDrawDirectly = false,
	
	kClampToBounds = true,
	kDontClampToBounds = false
};

typedef enum {
	kFrame,
	kFill,
	kInvalidate,
	kPaint,
	kFillHilite			// kHilite is taken
} RectOperationT;

typedef enum {
	sort_None = 1,		// correspond to first menu item!
	sort_Creation,
	sort_Modification,
	sort_Name
} SortingT;

typedef enum {
	annoy_header = 'head',
	annoy_footer = 'foot',
	annoy_diagonal = 'diag',
	annoy_none = 'none'
	} AnnoyLocationT;


typedef enum {
	unit_Inches = 1,
	unit_Centimeters,
	unit_Points
	} UnitsT;
	

class PhotoUtility {
	typedef std::pair<double, double>		DoubleSize;
	typedef	std::map<OSType, DoubleSize>	SizeMap;
	typedef std::map<SInt16, char*>			UnitsMap;
	
	static	SizeMap			gSizeMap;
	static	UnitsMap		gUnitsMap;
	static	void			InitializeSizeMap();
	static	void			InitializeUnitsMap();

public:
	// Globals
	static const double		sEpsilon;
	static const RGBColor	sNonReproBlue;	
	static const double 	kRad2Degrees;	
	static const double 	kHardwiredHeaderSize; /*inches*/
	static bool gNeedDoubleOrientationSetting;
	
	// Functions
	static Boolean	DoubleEqual(const double& a, const double& b) { return (fabs(a - b) < sEpsilon); }
	static void		DrawXformedRect(
									const Rect&				inRect,
									MatrixRecord*			inMat,
									const RectOperationT	inWhat,
									const bool				inUseRegion = kDrawWithRegions);
	static void		GetSize(const OSType inType, double& outWidth, double& outHeight);

	static SInt16	GetLineHeight(short inFontNum, short inFontSize, Style inStyle = normal);
	static const char*	GetUnitsString(const UnitsT& in_unit);

	static	void		CalcCropValuesAsPercentages(const ERect32& inCrop, const ERect32& inBounds, 
													double& outTopCrop, double& outLeftCrop, 
													double& outBottomCrop, double& outRightCrop, bool clampToBounds = kClampToBounds);


	static	UnitsMap&	GetUnitsMap();
};//end class PhotoUtility
