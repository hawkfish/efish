/*
 * Name:
 *	ASTypes.h
 *
 * Copyright 1986-2001 Adobe Systems Incorporated.
 * All Rights Reserved.
 *
 * Purpose:
 *	Adobe Standard Types.
 *
 * Distribution:
 *	PUBLIC
 *
 * Version history:
 *	1.0.0 1/26/1996	DL	First version.
 *		Created by Dave Lazarony.
 */

#ifndef __ASTypes__
#define __ASTypes__

/*
 * Includes
 */
 
#ifndef __ASConfig__
#include "ASConfig.h"
#endif

#ifndef __ASPragma__
#include "ASPragma.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif

#pragma PRAGMA_ALIGN_BEGIN
#pragma PRAGMA_IMPORT_BEGIN


/*
 * Constants
 */

// true and false

#ifndef true
#define true	1
#endif

#ifndef false
#define false	0
#endif

#ifndef TRUE
#define TRUE	true
#endif

#ifndef FALSE
#define FALSE	false
#endif

// error codes
#define kNoErr					0
#define kOutOfMemoryErr			'!MEM'
#define kBadParameterErr		'PARM'
#define kNotImplementedErr		'!IMP'
#define kCantHappenErr			'CANT'


// NULL and nil

#ifndef NULL

#ifdef MAC_ENV
#if !defined(__cplusplus) && (defined(__SC__) || defined(THINK_C))
#define NULL	((void *) 0)
#else
#define NULL	0
#endif
#endif

#ifdef WIN_ENV
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

#endif

#ifndef nil
#define nil NULL
#endif


// AMPAPI  Adobe Standard Plugin API calling convention.

#ifndef AMPAPI
#ifdef MAC_ENV
#define ASAPI pascal
#endif
#ifdef WIN_ENV
#define ASAPI
#endif
#endif

// C calling convention for those places that need it.
// This doesn't really do anything, but is  more for
// an explicity declaration when it matters.
#define ASCAPI	


/*
 * Types
 */

// Integer Types

#ifndef _H_CoreExpT
typedef signed char ASInt8;
typedef signed short ASInt16;
typedef signed long ASInt32;
#endif

typedef unsigned char ASUInt8;
typedef unsigned short ASUInt16;
typedef unsigned long ASUInt32;

typedef long ASErr;

// Storage Types

typedef unsigned char ASByte;
typedef ASByte *ASBytePtr;

// Pointer Types

typedef void *ASPtr, **ASHandle;

// Fixed Types

#ifndef _H_ASExpT
typedef	long ASFixed;
#endif
typedef long ASFract;
typedef float ASReal;

#ifndef _H_ASExpT
typedef struct _t_ASFixedPoint {
	ASFixed h, v;
} ASFixedPoint;

typedef struct _t_ASFixedRect {
	ASFixed left, top, right, bottom;
} ASFixedRect;

typedef struct _t_ASFixedMatrix {
	ASFixed a, b, c, d, tx, ty;
} ASFixedMatrix;
#endif

typedef struct _t_ASRealPoint {
	ASReal h, v;
} ASRealPoint;

typedef struct _t_ASRealRect {
	ASReal left, top, right, bottom;
} ASRealRect;

typedef struct _t_ASRealMatrix {
	ASReal a, b, c, d, tx, ty;
} ASRealMatrix;


// Platform Structures

#ifdef MAC_ENV


// ASBoolean is the same a Macintosh Boolean.
typedef unsigned char ASBoolean; 

// ASPortRef is the same as a Windows HDC.
typedef struct GrafPort *ASPortRef;				

// ASWindowRef is the same as a Windows HWND.
typedef struct GrafPort *ASWindowRef;			

// ASRect is the same size and layout as a Macintosh Rect.
typedef struct _t_ASRect {
	short top, left, bottom, right;
} ASRect;

// ASPoint is the same size and layout as a Macintosh Point.
typedef struct _t_ASPoint {
	short v, h;
} ASPoint;

#endif


#ifdef WIN_ENV

// ASBoolean is the same a Windows BOOL.
typedef int ASBoolean;

// ASPortRef is the same as a Windows HDC.
typedef void * ASPortRef;				

// ASWindowRef is the same as a Windows HWND.
typedef void * ASWindowRef;			

// ASRect is the same size and layout as a Windows RECT.
typedef struct _t_ASRect {
	long left, top, right, bottom;
} ASRect;

// ASPoint is the same size and layout as a Windows POINT.
typedef struct _t_ASPoint  {
	long h, v;
} ASPoint;

#endif


// ASRGBColor is the same as a Macintosh RGBColor on Macintosh and Windows.
typedef struct _t_ASRGBColor {
	unsigned short red, green, blue;
} ASRGBColor;


// AIEvent is the same as a Macintosh EventRecord on Macintosh and Windows.
typedef struct _t_ASEvent {
	unsigned short	what;
	unsigned long	message;
	unsigned long	when;
	ASPoint			where;
	unsigned short	modifiers;
} ASEvent;

// This is a generic reference to a resource/plugin file.  If not otherwise stated,
// it is assumed to be equivalent to an SPPluginRef (see "SPPlugs.h")
typedef struct ASAccess *ASAccessRef;



#pragma PRAGMA_IMPORT_END
#pragma PRAGMA_ALIGN_END

#ifdef __cplusplus
}
#endif


#endif
