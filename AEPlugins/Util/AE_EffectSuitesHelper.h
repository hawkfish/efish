/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2001 Adobe Systems Incorporated                       */
/* All Rights Reserved.                                            */
/*                                                                 */
/* NOTICE:  All information contained herein is, and remains the   */
/* property of Adobe Systems Incorporated and its suppliers, if    */
/* any.  The intellectual and technical concepts contained         */
/* herein are proprietary to Adobe Systems Incorporated and its    */
/* suppliers and may be covered by U.S. and Foreign Patents,       */
/* patents in process, and are protected by trade secret or        */
/* copyright law.  Dissemination of this information or            */
/* reproduction of this material is strictly forbidden unless      */
/* prior written permission is obtained from Adobe Systems         */
/* Incorporated.                                                   */
/*                                                                 */
/* Adobe Premiere Device Control plug-in definitions			   */
/*																   */
/* AE_EffectSuitesHelper.h										   */
/*																   */ 
/* After Effects 5.0 PICA Suites (extended 3/8/00)				   */
/*																   */
/*																   */
/*******************************************************************/

#ifndef _H_AE_EffectSuitesHelper
#define _H_AE_EffectSuitesHelper

#include <AE_Effect.h>
#include <SPBasic.h>

#if PRAGMA_STRUCT_ALIGN
	#pragma options align=mac68k
#endif

#ifdef _WINDOWS
	#pragma pack( push, AEalign, 2 )
#endif

#ifdef __cplusplus
	extern "C" {
#endif


/** PF_HelperSuite1

	PF_GetCurrentTool()

		retrieves the type of the current tool palette tool selected					

**/

#define kPFHelperSuite				"AE Plugin Helper Suite"
#define kPFHelperSuiteVersion1		1
#define kPFHelperSuiteVersion		kPFHelperSuiteVersion1

typedef enum {
	PF_SuiteTool_NONE = 0, 				
	PF_SuiteTool_ARROW,
	PF_SuiteTool_ROTATE,
	PF_SuiteTool_RECTMASK,
	PF_SuiteTool_OVALMASK,
	PF_SuiteTool_PEN,
	PF_SuiteTool_PAN,
	PF_SuiteTool_HAND,
	PF_SuiteTool_MAGNIFY
} PF_SuiteTool;



typedef struct PF_HelperSuite1 {
	// obsolete, use PF_HelperSuite2
	SPAPI PF_Err 	(*PF_GetCurrentTool)( PF_SuiteTool 		*toolP );		/* << */
} PF_HelperSuite1;


/** PF_HelperSuite2

	PF_ParseClipboard()

		causes After Effects to parse the clipboard immediately
					
**/
typedef enum {
	PF_ExtendedSuiteTool_NONE = 0,
	PF_ExtendedSuiteTool_ARROW,
	PF_ExtendedSuiteTool_ROTATE,
	PF_ExtendedSuiteTool_PEN_NORMAL,
	PF_ExtendedSuiteTool_PEN_ADD_POINT,
	PF_ExtendedSuiteTool_PEN_DELETE_POINT,
	PF_ExtendedSuiteTool_PEN_CONVERT_POINT,
	PF_ExtendedSuiteTool_MASK_RECT,
	PF_ExtendedSuiteTool_MASK_OVAL,
	PF_ExtendedSuiteTool_CAMERA_ORBIT,
	PF_ExtendedSuiteTool_CAMERA_TRACK_XY,
	PF_ExtendedSuiteTool_CAMERA_TRACK_Z,
	PF_ExtendedSuiteTool_PAN_BEHIND,
	PF_ExtendedSuiteTool_HAND,
	PF_ExtendedSuiteTool_MAGNIFY

} PF_ExtendedSuiteTool;


#define kPFHelperSuite2				"AE Plugin Helper Suite2"
#define kPFHelperSuite2Version1		1
#define kPFHelperSuite2Version2		2
#define kPFHelperSuite2Version		kPFHelperSuite2Version2

typedef struct PF_HelperSuite2 {
	SPAPI PF_Err 	(*PF_ParseClipboard)( void );
	// Do not call PF_SetCurrentExtendedTool until the UI is built. i.e. Do not call it from
	// your plugin init function.
	SPAPI PF_Err	(*PF_SetCurrentExtendedTool)(PF_ExtendedSuiteTool tool);
	SPAPI PF_Err	(*PF_GetCurrentExtendedTool)(PF_ExtendedSuiteTool *tool);
} PF_HelperSuite2;

#ifdef __cplusplus
	}
#endif

#if PRAGMA_STRUCT_ALIGN
	#pragma options align=reset
#endif

#ifdef _WINDOWS
	#pragma pack( pop, AEalign)
#endif



#endif
