//-------------------------------------------------------------------------------
//
//	File:
//		PITypes.h
//
//	(c) Copyright 1991-1999, Adobe Systems Incorporated.
//	All Rights Reserved.
//
//	Distribution:
//		PUBLIC
//
//	Description:
//		This file describes the type definitions used in Photoshop's interface files.
//		
//	Use:
//		Include with all Photoshop plug-in projects.
//		
//
//	Version history:
//		Version 1.0.0	10/9/1997	Ace		Initial compilation.
//		
//-------------------------------------------------------------------------------
//  $Id: PITypes.h,v 1.1 2001/10/23 08:08:43 hawkfish Exp $ 

#ifndef __PITypes__
#define __PITypes__

#pragma once

#include "SPTypes.h"

/******************************************************************************/

/* Set up the preprocessor flags to indicate the target platform.	*/

#ifndef	MSWindows
#define	MSWindows	(MSDOS || WIN32)
#endif

#ifndef	Unix
#define	Unix		(__unix)
#endif

#ifndef Macintosh
#if (Unix || defined(macintosh) || defined(__powerc) || defined(__MC68K__) || defined (__POWERPC__) || defined(THINK_C)) && !MSWindows
#define Macintosh 1
#else
#define Macintosh 0
#endif
#endif /* #ifndef Macintosh */

#ifndef MacintoshOS
#define	MacintoshOS	(Macintosh && !Unix)
#endif

#ifndef Macintosh68K
#define	Macintosh68K	(MacintoshOS && m68k)
#endif

#ifndef MacintoshPPC
#define	MacintoshPPC	(MacintoshOS && __powerc)
#endif

#if defined (__BORLANDC__)
#pragma option -a-
#endif

/******************************************************************************/

/* Set up the flags for the Adobe Graphics Manager.	*/

#ifdef MAC_ENV
#undef MAC_ENV
#endif

#ifdef WIN_ENV
#undef WIN_ENV
#endif

#if Macintosh
#define MAC_ENV	1
#endif

#if MSWindows
#define WIN_ENV	1
#if WIN32
#define WINNT_ENV   1
#endif
#endif

/******************************************************************************/

/* Some utility defines for C/C++ compatiblity. */

#ifndef START_C_INCLUDE
#ifdef __cplusplus
#define START_C_INCLUDE extern "C" {
#define END_C_INCLUDE }
#else
#define START_C_INCLUDE
#define END_C_INCLUDE
#endif
#endif

#ifndef START_C_CODE
#ifdef __cplusplus
#define START_C_CODE extern "C" {
#define END_C_CODE }
#else
#define START_C_CODE
#define END_C_CODE
#endif
#endif

/******************************************************************************/

#if Macintosh

#ifndef __PSWorld__

#include <MacTypes.h>
#include <MacMemory.h>
#include <Resources.h>
#include <Quickdraw.h>
#include <Dialogs.h>
#include <OSUtils.h>
#include <ToolUtils.h>
#include <Packages.h>
#include <Errors.h>
#include <Files.h>
#include <Aliases.h>
#include <AppleEvents.h>

#endif

#ifndef TRUE
#define TRUE	true
#endif

#ifndef FALSE
#define FALSE	false
#endif

#ifndef MACPASCAL
#define MACPASCAL pascal
#endif

#ifndef WINPASCAL
#define WINPASCAL
#endif

#ifndef __MWERKS__
#ifndef far
#define far
#endif
#endif

#ifndef huge
#define huge
#endif

#ifndef __PSWorld__

#if defined(_MSC_VER)
	// MSVC++ is weird!
	typedef __int64 int64;
#else
	typedef long long int64;
#endif

typedef signed long  int32;
typedef signed short int16;
typedef signed char  int8;

typedef unsigned long  unsigned32;
typedef unsigned short unsigned16;
typedef unsigned char  unsigned8;

/* The following types are used by AGM and we arrange to define them so that */
/* we won't have conflicts when working with AGM. */

typedef unsigned8	uint8;
typedef unsigned16	uint16;
typedef unsigned32	uint32;

typedef unsigned char	Bool8;
typedef unsigned long	Bool32;

#ifndef	OTHER_AGM_BASIC_H
#define OTHER_AGM_BASIC_H "PITypes.h"
#endif

#endif /* __PSWorld__ */

typedef struct PlatformData {

	short dummy;                    /* dummy variable required for some compilers. */

	} PlatformData;

#endif /* Macintosh */

/******************************************************************************/

#if MSWindows

#define MACPASCAL
#define WINPASCAL pascal 

#ifndef __PSWorld__
#include <windows.h>

/* Error codes. */
#define noErr 0

#define userCanceledErr (-128)
#define coercedParamErr 2

#define readErr			(-19)
#define writErr			(-20)
#define openErr			(-23)
#define dskFulErr			(-34)
#define ioErr				(-36)
#define eofErr				(-39)		// Also - end of descriptor error.
#define fnfErr				(-43)
#define vLckdErr			(-46)
#define fLckdErr			(-45)
#define paramErr			(-50)
#define memFullErr		(-108)
#define nilHandleErr		(-109)
#define memWZErr			(-111)


#define nil		   NULL

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef true
#define true       TRUE
#endif

#ifndef false
#define false      FALSE
#endif

#if _MSC_VER
typedef __int64 int64;
typedef unsigned __int64 unsigned64;
#else
typedef signed long long int64;
typedef unsigned long long unsigned64;
#endif

typedef signed long  int32;
typedef signed short int16;
typedef signed char  int8;

typedef unsigned long	uint32;
typedef unsigned short	uint16;
typedef unsigned char	uint8;

typedef unsigned long  unsigned32;
typedef unsigned short unsigned16;
typedef unsigned char  unsigned8;

typedef long Fixed;
typedef long Fract;
typedef long (*ProcPtr)();

#ifndef __TYPES__
#ifndef __GEOMETRY__
typedef struct Point
   {
	  short v;
	  short h;
   } Point;

typedef struct Rect
   {
	  short top;
	  short left;
	  short bottom;
	  short right;
   } Rect;
#endif
#endif

#ifndef __QUICKDRAW__
typedef struct
   {
	  WORD red;        /* Magnitude of red   component, 16 bit significant.*/
	  WORD green;      /* Magnitude of green component, 16 bit significant.*/
	  WORD blue;       /* Magnitude of blue  component, 16 bit significant.*/
   } RGBColor;
#endif

#ifndef __OCE__
typedef unsigned long DescType;
#endif

typedef LPSTR Ptr;
typedef LPSTR *Handle;
typedef BYTE  Boolean;
typedef DWORD OSType;
typedef short OSErr;
typedef unsigned long ResType;

typedef unsigned char Str255[256];  /*  first byte length of string. The string is zero terminated. */
typedef const unsigned char *ConstStr255Param;

typedef struct
	{
	short vRefNum;
	long parID;
	Str255 name;	/* Pascal String, for Windows files. */
	} FSSpec;

#endif

typedef struct PlatformData {

	long hwnd;        /* window to own dialogs. */

	} PlatformData;
	
short ShowAlert (short stringID); /* Developer implements this  */
	
#endif    /* Windows */

/******************************************************************************/

// Unicode character type.
typedef uint16 Character;

/******************************************************************************/

// HACK (SRP): The definition of LookUpTable is done this way because PITypes.h
// is included in PSWorld.h prior to LookUpTable being declared.

typedef unsigned8 PILookUpTable [256];

#ifndef __PSWorld__

/* Common datatypes defined in PSWorld */

typedef PILookUpTable LookUpTable;

typedef unsigned16 LookUpTable8x16 [256];

typedef struct
	{
	LookUpTable R;
	LookUpTable G;
	LookUpTable B;
	} RGBLookUpTable;
	
typedef struct
	{
	RGBLookUpTable clut;
	int32 count;
	int32 transparentIndex;
	} ColorTableInfo;
	
/* Structures to hold colors in various spaces. */

#ifndef __GEOMETRY__

typedef Rect CRect;

#endif

#ifndef __UGEOMETRY__
typedef struct
	{
	int32 top;
	int32 left;
	int32 bottom;
	int32 right;
	} VRect;
	
typedef struct VPoint
	{
	int32 v;
	int32 h;
	} VPoint;
#endif /* __UGEOMETRY__ */

#if MacintoshOS
typedef char TVolumeName [32];
#else
typedef char TVolumeName [256];
#endif /* MacintoshOS */
	
/******************************************************************************/

/*
 * Display Ordering Values.  Passed to certain bottlenecks to tell them what
 * the data looks like, and used (on non-Macs) in the pmReserved field of a
 * pixmap to tag that data.  For compatibility, the values are designed to
 * match up with some Latitude defined values we created for the Unix 3.0.
 * - SLB
 */

enum DisplayByteOrder {			/* Data is... */
	displayOrderIndexed = 0,	/* 8-bit, or otherwise unspecified. */
	displayOrderURGB = 1,		/* 32-bit, high 8 unused, Mac uses this. */
	displayOrderUBGR = 2,		/* 32-bit, high 8 unused, Sun, SGI like this. */
	displayOrderRGBU = 3,		/* 32-bit, low 8 unused, for completeness. */
	displayOrderBGRU = 4,		/* 32-bit, good for little-endian (PC). */
	displayOrderRGBPacked = 5,	/* 24-bit, unaligned, currently unsupported!! */
	displayOrderBGRPacked = 6	/* 24-bit, unaligned, currently unsupported!! */
};

#endif	/* __PSWorld__ */

/******************************************************************************/

typedef struct
	{
    unsigned8   c;
    unsigned8   m;
    unsigned8   y;
    unsigned8   k;
	} CMYKtuple;

typedef struct
	{
    unsigned8   alpha;
    unsigned8   r;
    unsigned8   g;
    unsigned8   b;
	} RGBtuple;

typedef struct
	{
    unsigned8   alpha;
    unsigned8   h;
    unsigned8   s;
    unsigned8   l;
	} HSLtuple;

typedef struct
	{
    unsigned8   alpha;
    unsigned8   h;
    unsigned8   s;
    unsigned8   b;
	} HSBtuple;

typedef struct
	{
    unsigned8   alpha;
    unsigned8   L;
    unsigned8   a;
    unsigned8   b;
	} LABtuple;

typedef struct
	{
    unsigned16   alpha;
    unsigned16   r;
    unsigned16   g;
    unsigned16   b;
	} RGB16tuple;

typedef struct
	{
    unsigned16   c;
    unsigned16   m;
    unsigned16   y;
    unsigned16   k;
	} CMYK16tuple;

typedef struct
	{
    unsigned16   alpha;
    unsigned16   L;
    unsigned16   a;
    unsigned16   b;
	} LAB16tuple;
	
typedef struct
	{
	unsigned8	alpha;
	unsigned8	padding;
	unsigned16	X;
	unsigned16	Y;
	unsigned16	Z;
	} XYZ16tuple;

typedef unsigned8   inverseCLUT [32][32][32];
typedef unsigned32  histogram3 [][32][32];

typedef int16            HueSatTable [1536];
typedef PILookUpTable    HueSatMaps[4];     /* general form of RGBLookUpTable */

/* hack for BNTLocal.c et al */
#ifndef __UMonitor__
typedef int16 Short3by3 [3] [3];
#endif /* __UMonitor__ */

typedef struct PIFloatPoint
	{
	double		y;
	double		x;
	} PIFloatPoint;

typedef struct PIFloatRect
	{
	double		top;
	double		left;
	double		bottom;
	double		right;
	} PIFloatRect;


/* 
PIAffineMatrix represents 3x3 matrix that has the following structure:

|	xx		xy		0	|
|	yx		yy		0	|
|	tx		ty		1	|

*/

typedef struct PIAffineMatrix
	{
	double xx, xy, yx, yy, tx, ty;
	} PIAffineMatrix;




/******************************************************************************/
#if defined (__BORLANDC__)   
#pragma option -a.
#endif

#endif /* __PITypes_h__ */
