/***********************************************************************/

/*                                                                     */

/* SPPiPL.h                                                            */

/*                                                                     */

/* Copyright 1995-1999 Adobe Systems Incorporated.                     */

/* All Rights Reserved.                                                */

/*                                                                     */

/* Patents Pending                                                     */

/*                                                                     */

/* NOTICE: All information contained herein is the property of Adobe   */

/* Systems Incorporated. Many of the intellectual and technical        */

/* concepts contained herein are proprietary to Adobe, are protected   */

/* as trade secrets, and are made available only to Adobe licensees    */

/* for their internal use. Any reproduction or dissemination of this   */

/* software is strictly forbidden unless prior written permission is   */

/* obtained from Adobe.                                                */

/*                                                                     */

/***********************************************************************/



#ifndef __SPPiPL__

#define __SPPiPL__





/*******************************************************************************

 **

 **	Imports and alignment

 **

 **/



#include "SPTypes.h"





#if PRAGMA_STRUCT_ALIGN

#pragma options align=mac68k

#endif





/*******************************************************************************

 **

 **	Constants

 **

 ** Note: 4-character long IDs must be defined in hexidecimal, not long

 ** literal ('long'), due to Windows RC compilers not understanding what

 ** to do with them.

 **

 **/



// 'ADBE' Vendor ID for SP-generic apps:

#define PIAdobeVendorID				0x41444245L



// 'ivrs' SP plug-in version:

#define PISPVersionProperty			0x69767273L



// 'impt' Suite import info:

#define PIImportsProperty			0x696D7074L



// 'expt' Suite export info:

#define PIExportsProperty			0x65787074L



// 'ppcR' Power PC Resource info:

#define PIPowerPCCodeResProperty	0x70706352L



// 'heap' Mac PPC-only. If non-existant, loads in host default heap.

// If 0, loads in system heap:

#define PIDestinationHeapProperty 	0x68656170L

#define PILoadInAppHeap				(1L<<0)



// 'adpt' Adapter version property (a long):

#define PIAdapterProperty			0x61647074L



// 'pinm' Internal plug-in name:

#define PIPluginNameProperty		0x70696E6DL



// 'AcpM' Message flags:

#define PIConditionalMessages		0x4163704DL

#define	PIAcceptsPropertyMessage	(1L<<0)

#define	PISkipShutDownMessage		(1L<<1)

#define PIAcceptsPurgeCachesMessage	(1L<<2)

#define PISkipStartupMessage		(1L<<3)



/*******************************************************************************

 **

 **	General PiPL properties and structures

 **

 ** Many hosts multiply define these properties, so we bracket them

 ** so they are only defined once.

 **

 **/



#ifndef kGeneralPiPLPropertiesDefined // Already defined?

#define kGeneralPiPLPropertiesDefined // Only define once.



typedef unsigned long PIType;



#define kCurrentPiPLVersion 0



// 'kind' <PIType> giving the plug-in's kind:

#define PIKindProperty			0x6b696e64L



// 'm68k' <PI68KCodeDesc> 68k code descriptor. See struct below.

#define PI68KCodeProperty		0x6d36386bL



// 'pwpc' <PICFMCodeDesc> PowerPC code descriptor. See struct below.

#define PIPowerPCCodeProperty	0x70777063L



// 'wx86' <PIWin32X86CodeDesc> Win32 Intel code descriptor. See struct below.

#define PIWin32X86CodeProperty  0x77783836L



// 'fx86' <PIWin16X86CodeDesc> Win16 Intel code descriptor. See struct below.

#define PIWin16X86CodeProperty  0x66783836L



typedef struct PIProperty {

	PIType vendorID;		/* Vendor specific identifier. */

	PIType propertyKey;		/* Identification key for this property type. */

	long propertyID;		/* Index within this property type. Must be unique for properties of a given type in a PiPL. */

	long propertyLength;	/* Length of following data array. Will be rounded to a multiple of 4. */

	char propertyData[1];	/* propertyLength bytes long. Must be a multiple of 4. Can be zero? */

} PIProperty;



typedef struct PIPropertyList {

	long version;				/* Properties datastructure version number. */

	long count;					/* Number of properties contained. */

	PIProperty properties[1];	/* The actual properties. */

} PIPropertyList;



/* Below are structures describing types used in plug-in property lists.

 * These structures obey Macintosh 68k alignment and padding rules though

 * generally they are laid out so fields have natural alignment and any

 * needed padding is explicit in the structure.

 */



typedef struct PI68KCodeDesc {

	PIType fType;

	short fID;

} PI68KCodeDesc;



typedef struct PICFMCodeDesc {

	long fContainerOffset;

	long fContainerLength;

	unsigned char fEntryName[1];

} PICFMCodeDesc;





typedef struct PIWin32X86CodeDesc {

	char fEntryName[1];

} PIWin32X86CodeDesc;



typedef struct PIWin16X86CodeDesc {

	char fEntryName[1];

} PIWin16X86CodeDesc;



#endif // kGeneralPiPLPropertiesDefined





/*******************************************************************************

 **

 **	Additional Types

 **

 **/

	

typedef struct PICFMCodeResourceDesc {

	long fContainerOffset;			/* Currently unused, reserved. */

	long fContainerLength;			/* Set to 0 to use resource size */

	PIType fType;

	short fID;

	unsigned char fEntryName[1];

} PICFMCodeResourceDesc;

	

/*******************************************************************************/



#if PRAGMA_STRUCT_ALIGN

#pragma options align=reset

#endif

	

#endif // SPPiPL

