/** AE_EffectSuites.h

	After Effects 4.0 SweetPea Suites, 11/25/98

	Copyright (c) 1998 Adobe Systems Inc, Seattle WA
	All Rights Reserved
 
**/

#ifndef _H_AE_EffectSuites
#define _H_AE_EffectSuites

#include <AE_Effect.h>
#include <AE_EffectUI.h>		// for PF_CursorType
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



#define kPFPathQuerySuite			"PF Path Query Suite"
#define kPFPathQuerySuiteVersion1	1


typedef struct PF_PathOutline *PF_PathOutlinePtr;
typedef struct PF_PathSegPrep *PF_PathSegPrepPtr;

typedef struct PF_PathQuerySuite1 {

	SPAPI PF_Err	(*PF_NumPaths)(	PF_ProgPtr			effect_ref,
									long				*num_pathsPL);	/* << */

	SPAPI PF_Err	(*PF_PathInfo)(	PF_ProgPtr			effect_ref,
									long				indexL,
									PF_PathID			*unique_idP);	/* << */

	SPAPI PF_Err	(*PF_CheckoutPath)(	PF_ProgPtr			effect_ref,
										PF_PathID			unique_id,
										long				what_time,
										long				time_step,
										unsigned long		time_scale,
										PF_PathOutlinePtr	*pathPP);	/* << */	// can return NULL ptr if path doesn't exist

	SPAPI PF_Err	(*PF_CheckinPath)(	PF_ProgPtr			effect_ref,
										PF_PathID			unique_id,
										PF_Boolean			changedB,
										PF_PathOutlinePtr	pathP);

} PF_PathQuerySuite1;



/* -------------------------------------------------------------------- */


#define kPFPathDataSuite			"PF Path Data Suite"
#define kPFPathDataSuiteVersion1	1


typedef struct {

	PF_FpLong				x, y;
	PF_FpLong				tan_in_x, tan_in_y;
	PF_FpLong				tan_out_x, tan_out_y;

} PF_PathVertex;

#define	PF_MAX_PATH_NAME_LEN		31


typedef struct PF_PathDataSuite1 {

	SPAPI PF_Err	(*PF_PathIsOpen)(		PF_ProgPtr			effect_ref,
											PF_PathOutlinePtr	pathP,
											PF_Boolean			*openPB);

	// N segments means there are segments [0..N-1]; segment J is defined by vertex J & J+1
	SPAPI PF_Err 	(*PF_PathNumSegments)(	PF_ProgPtr			effect_ref,
											PF_PathOutlinePtr	pathP,
											long				*num_segmentsPL);
	
	// which_pointL range: [0..num_segments]; for closed paths vertex[0] == vertex[num_segments]
	SPAPI PF_Err 	(*PF_PathVertexInfo)(	PF_ProgPtr			effect_ref,
											PF_PathOutlinePtr	pathP,
											long				which_pointL,
											PF_PathVertex		*vertexP);

	SPAPI PF_Err 	(*PF_PathPrepareSegLength)(
											PF_ProgPtr			effect_ref,
											PF_PathOutlinePtr	pathP,
											long				which_segL,
											long				frequencyL,
											PF_PathSegPrepPtr	*lengthPrepPP);

	SPAPI PF_Err 	(*PF_PathGetSegLength)(	PF_ProgPtr			effect_ref,
											PF_PathOutlinePtr	pathP,
											long				which_segL,
											PF_PathSegPrepPtr	*lengthPrepP0,
											PF_FpLong			*lengthPF);
											
	SPAPI PF_Err 	(*PF_PathEvalSegLength)(PF_ProgPtr			effect_ref,
											PF_PathOutlinePtr	pathP,
											PF_PathSegPrepPtr	*lengthPrepPP0,
											long				which_segL,
											PF_FpLong			lengthF,
											PF_FpLong			*x,
											PF_FpLong			*y);

	SPAPI PF_Err 	(*PF_PathEvalSegLengthDeriv1)(
											PF_ProgPtr			effect_ref,
											PF_PathOutlinePtr	pathP,
											PF_PathSegPrepPtr	*lengthPrepPP0,
											long				which_segL,
											PF_FpLong			lengthF,
											PF_FpLong			*x,
											PF_FpLong			*y,
											PF_FpLong			*deriv1x,
											PF_FpLong			*deriv1y);


	SPAPI PF_Err 	(*PF_PathCleanupSegLength)(
											PF_ProgPtr			effect_ref,
											PF_PathOutlinePtr	pathP,
											long				which_segL,
											PF_PathSegPrepPtr	*lengthPrepPP);


	SPAPI PF_Err 	(*PF_PathIsInverted)(	PF_ProgPtr			effect_ref,
											PF_PathID			unique_id,
											PF_Boolean			*invertedB);

	SPAPI PF_Err 	(*PF_PathGetMaskMode)(	PF_ProgPtr			effect_ref,
											PF_PathID			unique_id,
											PF_MaskMode			*modeP);

	SPAPI PF_Err	(*PF_PathGetName)(		PF_ProgPtr			effect_ref,
											PF_PathID			unique_id,
											char				*nameZ);		/* << can be up to PF_MAX_PATH_NAME_LEN+1 bytes long */


} PF_PathDataSuite1;


/* -------------------------------------------------------------------- */

#define kPFParamUtilsSuite			"PF Param Utils Suite"
#define kPFParamUtilsSuiteVersion1	1



typedef struct {
	long	reservedAL[4];
} PF_State;


#define PF_ParamIndex_NONE							(-1L)
#define PF_ParamIndex_CHECK_ALL						(-2L)
#define PF_ParamIndex_CHECK_ALL_EXCEPT_LAYER_PARAMS	(-3L)

#define PF_KeyIndex_NONE							(-1L)


enum {
	PF_TimeDir_GREATER_THAN				= 0x0000,
	PF_TimeDir_LESS_THAN				= 0x0001,
	PF_TimeDir_GREATER_THAN_OR_EQUAL	= 0x1000,
	PF_TimeDir_LESS_THAN_OR_EQUAL		= 0x1001
};
typedef long	PF_TimeDir;

typedef long	PF_KeyIndex;


/** PF_ParamUtilsSuite1

	PF_UpdateParamUI()

  		You can call this function for each param whose UI settings you
		want to change when handling a PF_Cmd_USER_CHANGED_PARAM or
		PF_Cmd_UPDATE_PARAMS_UI.  These	changes are cosmetic only, and don't
		go into the undo buffer.
		
		The ONLY fields that can be changed in this way are:

			PF_ParamDef
				ui_flags: PF_PUI_ECW_SEPARATOR, PF_PUI_DISABLED only
				ui_width
				ui_height
				name
				flags: PF_ParamFlag_COLLAPSE_TWIRLY only

				PF_ParamDefUnion:
					slider_min, slider_max, precision, display_flags of any slider type

				AND, for PF_PUI_STD_CONTROL_ONLY params, you are also allowed to change
				the value field for the param, since that is considered cosmetic; you
				can also change the value of a PF_PUI_STD_CONTROL_ONLY by setting
				the PF_ChangeFlag_CHANGED_VALUE before returning.

**/

typedef struct PF_ParamUtilsSuite1 {

	SPAPI PF_Err	(*PF_UpdateParamUI)(
								PF_ProgPtr			effect_ref,
								PF_ParamIndex		param_index,
								const PF_ParamDef	*defP);


	SPAPI PF_Err	(*PF_GetCurrentState)(
								PF_ProgPtr			effect_ref,
								PF_State			*stateP);		/* << */

	SPAPI PF_Err	(*PF_HasParamChanged)(
								PF_ProgPtr			effect_ref,
								const PF_State		*stateP,		// has param changed since this state was grabbed
								PF_ParamIndex		param_index,	// pass PF_ParamIndex_CHECK_ALL to check to see if _any_ params
																	//	have changed including layer param[0];
																	// pass PF_ParamIndex_CHECK_ALL_EXCEPT_LAYER_PARAMS to see
																	//	if any non-layer param values have changed
								PF_Boolean			*changedPB);	/* << */


	SPAPI PF_Err	(*PF_IsIdenticalCheckout)(
								PF_ProgPtr			effect_ref,
								PF_ParamIndex		param_index,
								long				what_time1,
								long				time_step1,
								unsigned long		time_scale1,
								long				what_time2,
								long				time_step2,
								unsigned long		time_scale2,
								PF_Boolean			*identicalPB);		/* << */


	SPAPI PF_Err	(*PF_FindKeyframeTime)(
								PF_ProgPtr			effect_ref,
								PF_ParamIndex		param_index,
								long				what_time,
								unsigned long		time_scale,
								PF_TimeDir			time_dir,
								PF_Boolean			*foundPB,			/* << */
								PF_KeyIndex			*key_indexP0,		/* << */
								long				*key_timeP0,		/* << */	// you can ask for either:
								unsigned long		*key_timescaleP0);	/* << */	// time&timescale OR neither

	SPAPI PF_Err	(*PF_GetKeyframeCount)(
								PF_ProgPtr			effect_ref,
								PF_ParamIndex		param_index,
								PF_KeyIndex			*key_countP);		/* << */	// returns PF_KeyIndex_NONE for constant

	SPAPI PF_Err	(*PF_CheckoutKeyframe)(
								PF_ProgPtr			effect_ref,
								PF_ParamIndex		param_index,
								PF_KeyIndex			key_index,			// zero-based
								long				*key_timeP0,		/* << */	// you can ask for either:
								unsigned long		*key_timescaleP0,	/* << */	// time&timescale OR neither
								PF_ParamDef			*paramP0);			/* << */

	SPAPI PF_Err	(*PF_CheckinKeyframe)(
								PF_ProgPtr			effect_ref,
								PF_ParamDef			*paramP);

	SPAPI PF_Err	(*PF_KeyIndexToTime)(
								PF_ProgPtr			effect_ref,
								PF_ParamIndex		param_index,
								PF_KeyIndex			key_indexP,			/* >> */
								long				*key_timeP,			/* >> */
								unsigned long		*key_timescaleP);	/* << */

} PF_ParamUtilsSuite1;





/* -------------------------------------------------------------------- */


#define kPFAppSuite			"PF AE App Suite"
#define kPFAppSuiteVersion1	1
#define kPFAppSuiteVersion2	2

enum {
	PF_App_Color_NONE = -1, 

	PF_App_Color_FRAME, 
	PF_App_Color_FILL, 
	PF_App_Color_TEXT, 
	PF_App_Color_LIGHT_TINGE, 
	PF_App_Color_DARK_TINGE, 
	PF_App_Color_HILITE, 
	PF_App_Color_SHADOW, 
	
	PF_App_Color_BUTTON_FRAME, 
	PF_App_Color_BUTTON_FILL, 
	PF_App_Color_BUTTON_TEXT, 
	PF_App_Color_BUTTON_LIGHT_TINGE, 
	PF_App_Color_BUTTON_DARK_TINGE, 
	PF_App_Color_BUTTON_HILITE, 
	PF_App_Color_BUTTON_SHADOW, 
	
	PF_App_Color_BUTTON_PRESSED_FRAME, 
	PF_App_Color_BUTTON_PRESSED_FILL, 
	PF_App_Color_BUTTON_PRESSED_TEXT, 
	PF_App_Color_BUTTON_PRESSED_LIGHT_TINGE, 
	PF_App_Color_BUTTON_PRESSED_DARK_TINGE, 
	PF_App_Color_BUTTON_PRESSED_HILITE, 
	PF_App_Color_BUTTON_PRESSED_SHADOW, 

	/********************************/

	PF_App_Color_FRAME_DISABLED, 
	PF_App_Color_FILL_DISABLED, 
	PF_App_Color_TEXT_DISABLED, 
	PF_App_Color_LIGHT_TINGE_DISABLED, 
	PF_App_Color_DARK_TINGE_DISABLED, 
	PF_App_Color_HILITE_DISABLED, 
	PF_App_Color_SHADOW_DISABLED, 
	
	PF_App_Color_BUTTON_FRAME_DISABLED, 
	PF_App_Color_BUTTON_FILL_DISABLED, 
	PF_App_Color_BUTTON_TEXT_DISABLED, 
	PF_App_Color_BUTTON_LIGHT_TINGE_DISABLED, 
	PF_App_Color_BUTTON_DARK_TINGE_DISABLED, 
	PF_App_Color_BUTTON_HILITE_DISABLED, 
	PF_App_Color_BUTTON_SHADOW_DISABLED, 
	
	PF_App_Color_BUTTON_PRESSED_FRAME_DISABLED, 
	PF_App_Color_BUTTON_PRESSED_FILL_DISABLED, 
	PF_App_Color_BUTTON_PRESSED_TEXT_DISABLED, 
	PF_App_Color_BUTTON_PRESSED_LIGHT_TINGE_DISABLED, 
	PF_App_Color_BUTTON_PRESSED_DARK_TINGE_DISABLED, 
	PF_App_Color_BUTTON_PRESSED_HILITE_DISABLED, 
	PF_App_Color_BUTTON_PRESSED_SHADOW_DISABLED, 
	
	/********************************/
	PF_App_Color_BLACK, 
	PF_App_Color_WHITE, 
	PF_App_Color_GRAY, 
	PF_App_Color_RED, 
	PF_App_Color_YELLOW, 
	PF_App_Color_GREEN, 
	PF_App_Color_CYAN,

	/********************************/
	PF_App_Color_TLW_NEEDLE,
	PF_App_Color_TLW_CACHE_MARK,
	PF_App_Color_FILL_LIGHT, 
	PF_App_Color_HOT_TEXT,
	PF_App_Color_HOT_TEXT_DISABLED,

	/********************************/
	PF_App_Color_LABEL_0, 
	PF_App_Color_LABEL_1, 
	PF_App_Color_LABEL_2, 
	PF_App_Color_LABEL_3, 
	PF_App_Color_LABEL_4, 
	PF_App_Color_LABEL_5, 
	PF_App_Color_LABEL_6, 
	PF_App_Color_LABEL_7,

	PF_App_Color_NUMTYPES
};
typedef short	PF_App_ColorType;


typedef	struct PF_App_Color {
		unsigned short red;		
		unsigned short green;	
		unsigned short blue;	
} PF_App_Color;


#define	PF_APP_MAX_PERS_LEN				63

typedef struct PF_AppPersonalTextInfo {
	char		name[PF_APP_MAX_PERS_LEN + 1];
	char		org[PF_APP_MAX_PERS_LEN + 1];
	char		serial_str[PF_APP_MAX_PERS_LEN + 1];
} PF_AppPersonalTextInfo;


typedef enum {
	PF_FontStyle_NONE		= -1,	// sentinel
	PF_FontStyle_SYS		= 0,	// system font, system size, system style (0, 0, 0)
	PF_FontStyle_SMALL,				// usually small annotation text
	PF_FontStyle_SMALL_BOLD,		// more important small annotations
	PF_FontStyle_SMALL_ITALIC,		// missing things, etc.
	PF_FontStyle_MED,				// times in in/out panels
	PF_FontStyle_MED_BOLD,			// 
	PF_FontStyle_APP,				// 
	PF_FontStyle_APP_BOLD,			// time in TL window
	PF_FontStyle_APP_ITALIC			// 
} PF_FontStyleSheet;



#define	PF_FONT_NAME_LEN				255

typedef struct PF_FontName {
	char		font_nameAC[PF_FONT_NAME_LEN+1];
} PF_FontName;

typedef struct PFAppSuite1 {

	SPAPI PF_Err 	(*PF_AppGetBgColor)(	PF_App_Color			*bg_colorP);		/* << */

	SPAPI PF_Err 	(*PF_AppGetColor)(		PF_App_ColorType		color_type,			/* >> */
											PF_App_Color			*app_colorP);		/* << */

	SPAPI PF_Err 	(*PF_GetPersonalInfo)(	PF_AppPersonalTextInfo	*ptiP);				/* << */

	SPAPI PF_Err 	(*PF_GetFontStyleSheet)(PF_FontStyleSheet		sheet,				/* >> */
											PF_FontName				*font_nameP0,		/* << */			
											short					*font_numPS0,		/* << */
											short					*sizePS0,			/* << */
											short					*stylePS0);			/* << */

	// normally the effect should respond to PF_Event_ADJUST_CURSOR, but for changing
	//	the cursor during modal situations, you can use this API
	SPAPI PF_Err	(*PF_SetCursor)(		PF_CursorType		cursor);				/* >> */

} PFAppSuite1;

typedef struct PFAppSuite2 {
	SPAPI PF_Err 	(*PF_AppGetBgColor)(	PF_App_Color			*bg_colorP);		/* << */

	SPAPI PF_Err 	(*PF_AppGetColor)(		PF_App_ColorType		color_type,			/* >> */
											PF_App_Color			*app_colorP);		/* << */

	SPAPI PF_Err 	(*PF_GetPersonalInfo)(	PF_AppPersonalTextInfo	*ptiP);				/* << */

	SPAPI PF_Err 	(*PF_GetFontStyleSheet)(PF_FontStyleSheet		sheet,				/* >> */
											PF_FontName				*font_nameP0,		/* << */			
											short					*font_numPS0,		/* << */
											short					*sizePS0,			/* << */
											short					*stylePS0);			/* << */

	// normally the effect should respond to PF_Event_ADJUST_CURSOR, but for changing
	//	the cursor during modal situations, you can use this API
	SPAPI PF_Err	(*PF_SetCursor)(		PF_CursorType		cursor);				/* >> */

	// New to version 2
	SPAPI PF_Err	(*PF_IsRenderEngine)(	PF_Boolean				*render_enginePB);		/* >> */

} PFAppSuite2;


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
