#ifndef __ADMTracker__
#define __ADMTracker__

/*
 *        Name:	ADMTracker.h
 *   $Revision: 1.1 $
 *      Author:	Dave Lazarony 
 *        Date:	3/25/96
 *     Purpose:	ADM Tracker Suite.
 *
 * Copyright (c) 1986-2001 Adobe Systems Incorporated, All Rights Reserved.
 *
 */


/*
 * Includes
 */
 
#ifndef __ADMTypes__
#include "ADMTypes.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#pragma PRAGMA_ALIGN_BEGIN
#pragma PRAGMA_IMPORT_BEGIN

/*
 * Constants
 */

#define kADMTrackerSuite		"ADM Tracker Suite"

#define kADMTrackerSuiteVersion1	1
#define kADMTrackerSuiteVersion		kADMTrackerSuiteVersion1

/*
 * Types
 */

typedef enum {
	kADMMouseNormal,
	kADMMouseCaptured,
	kADMMouseUncaptured
} ADMMouseState;

typedef unsigned long ADMTime;

/*
 * 			     ADM General						        Macintosh Specific			                   Windows Specific	
 *  -----------------------------------------   -----------------------------------------   ---------------------------------------------
 */
typedef enum 
{
	kADMMouseMoveAction			= 1,			// It is better to use the specific move down and up cases
	kADMMouseMovedDownAction	= 1,
	
	kADMButtonDownAction		= 2,														kWinLeftButtonDownAction		= 2,
																							kWinMiddleButtonDownAction		= 3,
																							kWinRightButtonDownAction		= 4,
	kADMShiftKeyDownAction		= 5,
	kADMMenuKeyDownAction		= 6,			kMacCommandKeyDownAction	= 6,			kWinControlKeyDownAction		= 6,
	kADMModKeyDownAction		= 7,			kMacOptionKeyDownAction		= 7,			kWinAltKeyDownAction			= 7,
												kMacControlKeyDownAction	= 8,	
	kADMSpaceKeyDownAction		= 9,	
	kADMTabKeyDownAction		= 10,
	kADMEnterAction				= 11,

	kADMMouseMovedUpAction		= -1,
	kADMButtonUpAction			= -2,														kWinLeftButtonUpAction			= -2,
																							kWinMiddleButtonUpAction		= -3,
																							kWinRightButtonUpAction			= -4,
	kADMShiftKeyUpAction		= -5,
	kADMMenuKeyUpAction			= -6,			kMacCommandKeyUpAction		= -6,			kWinControlKeyUpAction			= -6,
	kADMModKeyUpAction			= -7,			kMacOptionKeyUpAction		= -7,			kWinAltKeyUpAction				= -7,
												kMacControlKeyUpAction		= -8,	
	kADMSpaceKeyUpAction		= -9,	
	kADMTabKeyUpAction			= -10,
	kADMLeaveAction				= -11,
	kADMUncapturedButtonUpAction= -12,														kWinUncapturedLeftButtonUpAction	= -12,
																							kWinUncapturedMiddleButtonUpAction	= -13,
																							kWinUncapturedRightButtonUpAction	= -14,
	kADMKeyStrokeAction			= -15,

	kADMDummyAction				= 0x8FFFFFFF
} ADMAction;


enum {
	kADMNullMask				= 0x00000000,

	kADMUncapturedActionMask	= 0x00000001,

	kADMMouseMovedDownMask		= 0x00000002,
	
	kADMButtonDownMask			= 0x00000004,												kWinLeftButtonDownMask		= 0x00000004,
																							kWinMiddleButtonDownMask	= 0x00000008,
																							kWinRightButtonDownMask		= 0x00000010,
	kADMShiftKeyDownMask		= 0x00000020,
	kADMMenuKeyDownMask			= 0x00000040,	kMacCommandKeyDownMask		= 0x00000040,	kWinControlKeyDownMask		= 0x00000040,
	kADMModKeyDownMask			= 0x00000080,	kMacOptionKeyDownMask		= 0x00000080,	kWinAltKeyDownMask			= 0x00000080,
												kMacControlKeyDownMask		= 0x00000100,	
	kADMSpaceKeyDownMask		= 0x00000200,	
	kADMTabKeyDownMask			= 0x00000400,
	kADMEnterMask				= 0x00000800,

	kADMMouseMovedUpMask		= 0x00020000,
	kADMButtonUpMask			= 0x00040000,												kWinLeftButtonUpMask		= 0x00040000,
																							kWinMiddleButtonUpMask		= 0x00080000,
																							kWinRightButtonUpMask		= 0x00100000,
	kADMShiftKeyUpMask			= 0x00200000,
	kADMMenuKeyUpMask			= 0x00400000,	kMacCommandKeyUpMask		= 0x00400000,	kWinControlKeyUpMask		= 0x00400000,
	kADMModKeyUpMask			= 0x00800000,	kMacOptionKeyUpMask			= 0x00800000,	kWinAltKeyUpMask			= 0x00800000,
												kMacControlKeyUpMask		= 0x01000000,	
	kADMSpaceKeyUpMask			= 0x02000000,	
	kADMTabKeyUpMask			= 0x04000000,
	kADMLeaveMask				= 0x08000000,
	kADMUncapturedButtonUpMask	= 0x10000000,	/* Applies to all UncapturedButtonUpActions on Windows */
	
	kADMKeyStrokeMask			= 0x80000000
};
	
#define kADMAllActionsMask ((unsigned long) 0xFFFFFFFF)


typedef enum 
{
	kADMNullModifier			= 0x00000000,
	
	kADMButtonDownModifier		= 0x00000004,												kWinLeftButtonDownModifier		= 0x00000004,
																							kWinMiddleButtonDownModifier	= 0x00000008,
																							kWinRightButtonDownModifier		= 0x00000010,
	kADMShiftKeyDownModifier	= 0x00000020,
	kADMMenuKeyDownModifier		= 0x00000040,	kMacCommandKeyDownModifier	= 0x00000040,	kWinControlKeyDownModifier		= 0x00000040,
	kADMModKeyDownModifier		= 0x00000080,	kMacOptionKeyDownModifier	= 0x00000080,	kWinAltKeyDownModifier			= 0x00000080,
												kMacControlKeyDownModifier	= 0x00000100,	
	kADMSpaceKeyDownModifier	= 0x00000200,	
	kADMTabKeyDownModifier		= 0x00000400,
	kADMDoubleClickModifier		= 0x00000800,

	kADMCapsLockOnModifier		= 0x00001000,
	
	kADMDummyModifier			= 0xFFFFFFFF
} ADMModifiers;


/*
 * Virtual Keys
 */
enum {
	
	kADMUnknownKey			= 0x0000,
	kADMCancelKey			= 0x0001,
	kADMEnterKey			= 0x0003,
	kADMHomeKey				= 0x0004,
	kADMEndKey				= 0x0005,
	kADMPageUpKey			= 0x0006,
	kADMPageDownKey			= 0x0007,
	kADMBackspaceKey		= 0x0008,
	kADMTabKey				= 0x0009,
	kADMInsertKey			= 0x000A,
	kADMReturnKey			= 0x000D,
	kADMF1Key				= 0x000E,
	kADMF2Key				= 0x000F,
	kADMF3Key				= 0x0010,
	kADMF4Key				= 0x0011,
	kADMF5Key				= 0x0012,
	kADMF6Key				= 0x0013,
	kADMF7Key				= 0x0014,
	kADMF8Key				= 0x0015,
	kADMF9Key				= 0x0016,
	kADMF10Key				= 0x0017,
	kADMF11Key				= 0x0018,
	kADMF12Key				= 0x0019,
	kADMClearKey			= 0x001A,
	kADMEscapeKey			= 0x001B,
	kADMLeftKey				= 0x001C,
	kADMRightKey			= 0x001D,
	kADMUpKey				= 0x001E,
	kADMDownKey				= 0x001F,
	kADMSpaceKey			= 0x0020,
	
	// Virtual keys from 0x0020 through the period key (.) are their ASCII equivalents
	kADMApostropheKey		= 0x0027,	// '
	kADMCommaKey			= 0x002c,	// ,
	kADMMinusKey			= 0x002d,	// -
	kADMPeriodKey			= 0x002e,	// .

	// kADM0Key - kADM9Key are the same as ASCII '0' thru '9' (0x30 - 0x39)
	
	kADMSemicolonKey		= 0x003b,	// ;
	kADMEqualKey			= 0x003d,	// =

	// kADMAKey - kADMZKey are the same as ASCII '0' thru '9' (0x41 - 0x5A)

	kADMRightSqrBracketKey	= 0x005b,	// ]
	kADMBackSlashKey		= 0x005c,	// "\"
	kADMLeftSqrBracketKey	= 0x005d,	// [ (last ASCII value)

	kADMDeleteKey			= 0x007F,

	// key pad keys
	kADMKP0Key				= 0x00e0,
	kADMKP1Key				= 0x00e1,
	kADMKP2Key				= 0x00e2,
	kADMKP3Key				= 0x00e3,
	kADMKP4Key				= 0x00e4,
	kADMKP5Key				= 0x00e5,
	kADMKP6Key				= 0x00e6,
	kADMKP7Key				= 0x00e7,
	kADMKP8Key				= 0x00e8,
	kADMKP9Key 				= 0x00e9,
	kADMKPEqualKey 			= 0x00ea,
	kADMKPMultiplyKey		= 0x00eb,
	kADMKPMinusKey 			= 0x00ec,
	kADMKPPlusKey 			= 0x00ed,
	kADMKPDivideKey		 	= 0x00ee,
	kADMKPDecimalKey		= 0x00ef,

	kADMDummyKey			= 0xFFFF
	
};


/*
 *  Tracker Suite
 */
typedef struct ADMTrackerSuite1
{
	void ASAPI (*GetPoint)(ADMTrackerRef tracker, ASPoint *point);
	
	ADMAction ASAPI (*GetAction)(ADMTrackerRef tracker); 	
	ADMModifiers ASAPI (*GetModifiers)(ADMTrackerRef tracker);
	
	ASBoolean ASAPI (*TestAction)(ADMTrackerRef tracker, ADMAction action);
	ASBoolean ASAPI (*TestModifier)(ADMTrackerRef tracker, ADMModifiers modifier);
	
	ADMTime ASAPI (*GetTime)(ADMTrackerRef tracker);

	void ASAPI (*Abort)(ADMTrackerRef tracker);

	ADMMouseState ASAPI (*GetMouseState)(ADMTrackerRef tracker);
	void ASAPI (*ReleaseMouseCapture)(ADMTrackerRef tracker);
	
	ADMChar ASAPI (*GetVirtualKey)(ADMTrackerRef tracker);
	ADMChar ASAPI (*GetCharacter)(ADMTrackerRef tracker);
		
} ADMTrackerSuite1;

typedef ADMTrackerSuite1 ADMTrackerSuite;	// Current version


#pragma PRAGMA_IMPORT_END
#pragma PRAGMA_ALIGN_END

#ifdef __cplusplus
}
#endif


#endif








