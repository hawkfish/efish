/* PF_Masks.h */

#pragma once

#ifndef H_PF_MASKS
#define H_PF_MASKS

#include <A.h>
#include <AE_Effect.h>
#include <SPBasic.h>
#include <AE_EffectSuites.h>


#if PRAGMA_STRUCT_ALIGN
#pragma options align=mac68k
#endif

#ifdef _WINDOWS
	#pragma pack( push, AEalign, 2 )
#endif

#define PF_MASKS_MAJOR_VERSION			1
#define PF_MASKS_MINOR_VERSION			0

#ifdef __cplusplus
	extern "C" {
#endif


#define kPF_MaskSuite				"AEGP Mask Suite"
#define kPF_MaskSuiteVersion1		1

typedef struct PF_MaskSuite1 {

	SPAPI A_Err (*PF_MaskWorldWithPath)(
			PF_ProgPtr			effect_ref,
			PF_PathOutlinePtr	*mask,			/* >> */
			PF_FpLong			feather_x,		/* >> */
			PF_FpLong			feather_y,		/* >> */	
			PF_Boolean			invert,			/* >> */
			PF_FpLong			opacity,		/* >> 0...1 */
			PF_Quality			quality,		/* >> */
			PF_World			*worldP,		/* <> */
			PF_Rect				*bboxPR0);		/* >> */
				
} PF_MaskSuite1;

#ifdef __cplusplus
}		// end extern "C"
#endif


#if PRAGMA_STRUCT_ALIGN
	#pragma options align=reset
#endif

#ifdef _WINDOWS
	#pragma pack( pop, AEalign)
#endif


#endif /* H_PH_MASKS */