#ifndef __ADMTypes__
#define __ADMTypes__

/*
 *        Name:	ADMTypes.h
 *   $Revision: 1.1.1.1 $
 *      Author:	Dave Lazarony 
 *        Date:	3/10/96
 *     Purpose:	ADM Types.
 *
 * Copyright (c) 1986-2001 Adobe Systems Incorporated, All Rights Reserved.
 *
 */


/*
 * Includes
 */
 
#ifndef __ASTypes__
#include "ASTypes.h"
#endif

#ifndef __SPPlugs__
#include "SPPlugs.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#pragma PRAGMA_ALIGN_BEGIN
#pragma PRAGMA_IMPORT_BEGIN


/*
 * Types
 */

typedef struct ADMDialog *ADMDialogRef;
typedef struct ADMDrawer *ADMDrawerRef;
typedef struct ADMItem *ADMItemRef;
typedef struct ADMList *ADMListRef;
typedef struct ADMHierarchyList *ADMHierarchyListRef;
typedef struct ADMEntry *ADMEntryRef;
typedef struct ADMListEntry *ADMListEntryRef;
typedef struct ADMNotifier *ADMNotifierRef;
typedef struct ADMTracker *ADMTrackerRef;
typedef struct ADMIcon *ADMIconRef;
typedef struct ADMImage *ADMImageRef;
typedef void *ADMUserData;
typedef unsigned long ADMTimerRef;
typedef unsigned long ADMActionMask;
typedef unsigned short ADMChar;

typedef void *ADMRsrcInfo;

typedef SPPluginRef ADMPluginRef;	// Unless otherwise specified, ADM assumes it is using SP

#ifndef _t_AGMPortPtr
typedef struct _t_AGMPort *_t_AGMPortPtr;
#endif

/*
 * Units
 */
typedef enum
{
	kADMNoUnits 									= 0,
	kADMPointUnits 									= 1,
	kADMInchUnits									= 2,
	kADMMillimeterUnits								= 3,
	kADMCentimeterUnits								= 4,
	kADMPicaUnits									= 5,
	kADMPercentUnits								= 6,
	kADMDegreeUnits									= 7,
	kADMQUnits										= 8,
	kADMMinNormalUnits								= kADMPointUnits,
	kADMMaxNormalUnits								= kADMQUnits,
	
	kADMAppUnits1									= 101,
	kADMAppUnits2									= 102,
	kADMAppUnits3									= 103,
	kADMAppUnits4									= 104,
	kADMAppUnits5									= 105,
	kADMAppUnits6									= 106,
	kADMAppUnits7									= 107,
	kADMAppUnits8									= 108,
	kADMAppUnits9									= 109,
	kADMAppUnits10									= 110,
	kADMMaxAppUnits									= kADMAppUnits10,
	
	kADMDummyUnits									= 0xFFFFFFFF
} ADMUnits;


/*
 * Fonts
 */
typedef enum
{
	kADMDefaultFont = 0,
	kADMDialogFont,
	kADMPaletteFont,
	kADMItalicDialogFont,
	kADMItalicPaletteFont,
	kADMBoldDialogFont,
	kADMBoldPaletteFont,
	kADMBoldItalicDialogFont,
	kADMBoldItalicPaletteFont,
	kADMDummyFont = 0xFFFFFFFF
} ADMFont;


/*
 * Justify
 */
typedef enum
{
	kADMLeftJustify 								= 0,
	kADMCenterJustify								= 1,
	kADMRightJustify								= 2,
	kADMDummyJustify 								= 0xFFFFFFFF
} 
ADMJustify;


/*
 * Color
 */
typedef enum 
{
	kADMBlackColor = 0,
	kADMWhiteColor,
	kADMHiliteColor,
	kADMHiliteTextColor,
	kADMLightColor,
	kADMBackgroundColor,
	kADMShadowColor,
	kADMDisabledColor,
	kADMButtonUpColor,
	kADMButtonDownColor,
	kADMButtonDownShadowColor,
	kADMToolTipBackgroundColor,
	kADMToolTipForegroundColor,
	kADMWindowColor,
	kADMForegroundColor,
	kADMTextColor,
	kADMRedColor,
	kADMDummyColor = 0xFFFFFFFF
} ADMColor;


#pragma PRAGMA_IMPORT_END
#pragma PRAGMA_ALIGN_END

#ifdef __cplusplus
}
#endif


#endif








