/*
File:		ParamConstants.java

Contains:	Place to put constants so they don't have to be qualified

Written by:	David Dunham

Copyright:	Copyright ©2000-2001 by Applied Microsystems Corporation.  All Rights reserved.

Change History (most recent first):

	05 Apr 2001		drd	Put in constants package, and made public
	08 Mar 2001		drd	format_AllFormats
	07 Mar 2001		drd	Removed format_Control
	21 Dec 2000		drd	Got rid of radix-related formats (now just format_Number and format_Control)
	01 Dec 2000		drd	Added radix_Decimal, radix_Hex
	30 Nov 2000		drd	Removed type_Unsigned (Python doesn't support unsigned either) and
					changed type_Signed to type_Integer; added other subscript constants
	29 Nov 2000		drd	Added kCurrentValue
	15 Nov 2000		drd	Added kChildren, kNoChildren, kUnknown
	18 Oct 2000		drd	Added kImmutable, kMutable
	17 Oct 2000		drd	Created
*/

package amc.altostratus.constants;

public interface ParamConstants
{
	// Parameter display
	char	format_Number = '#';			// Radix determines display
	char	format_Name = 'N';
	char	format_Time = 'T';
	char	format_Bar = 'B';
	char	format_LinearMeter = 'M';
	char	format_ArcMeter = 'm';
	char	format_BitMap = '1';
	char	format_Slider = 'S';
	String	format_AllFormats = "#NTBMm1S";
	char	radix_Decimal = 'D';
	char	radix_Hex = 'H';

	// Parameter editing
	boolean	kImmutable = false;
	boolean	kMutable = true;

	// Parameter value
	char	type_None = ' ';
	char	type_Integer = 'I';	// 32 bits
	char	type_Long = 'L';		// 64 bits
	char	type_Float = 'F';	// double precision floating point
	char	type_String = 'C';	// Unicode
	char	type_CharArray = 'A';	// Array of unsigned characters

	// Subscripts
	int		kCurrentValue = 0;
	int		kMaxValue = 1;
	int		kMinValue = 2;
	int		kANDedValue = 3;	// For integers and long longs
	int		kORedValue = 4;		// For integers and long longs
	int		kAverageValue = 3;	// For doubles
	int		kVarianceValue = 4;	// For doubles

	// Tree structure
	char	kChildren = 'Y';
	char	kNoChildren = 'N';
	char	kUnknown = '?';
}
