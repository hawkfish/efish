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
/*******************************************************************/

#ifndef _H_AE_PlugGeneral
#define _H_AE_PlugGeneral

#pragma once

#include <A.h>
#include <FIEL_Public.h>
#include <AE_Effect.h>
#include <AE_EffectCB.h>
#include <SPBasic.h>

#include <PR_Public.h>
#include <PF_Masks.h>
#include <AE_EffectSuites.h>
#include <AE_IO.h>


#if PRAGMA_STRUCT_ALIGN
	#pragma options align=power
#endif

#ifdef _WINDOWS
	#pragma pack( push, AEalign4, 4 )
#endif



#ifdef __cplusplus
	extern "C" {
#endif


#define AEGP_PLUGIN_TYPE			'AEgx'

#define AEGP_INITFUNC_MAJOR_VERSION			1
#define AEGP_INITFUNC_MINOR_VERSION			8

#ifdef AEGP_INTERNAL
	#include <AE_GeneralPlug_Private.h>
#else 
	typedef struct _AEGP_Project		**AEGP_ProjectH;
	typedef struct _AEGP_Item 			**AEGP_ItemH;
	typedef struct _AEGP_Comp			**AEGP_CompH;
	typedef struct _AEGP_Footage		**AEGP_FootageH;
	typedef struct _AEGP_Layer			**AEGP_LayerH;
	typedef struct _AEGP_Effect			**AEGP_EffectRefH;
	typedef struct _AEGP_Mask			**AEGP_MaskRefH;
	typedef struct _AEGPp_Stream		**AEGP_StreamRefH;
	typedef struct _AEGP_LayerContext   **AEGP_RenderLayerContextH;
	typedef struct _AEGP_PersistentBlob	**AEGP_PersistentBlobH;
	typedef struct _AEGP_MaskOutline	**AEGP_MaskOutlineValH;
	typedef struct _AEGP_Collection		**AEGP_CollectionH;
	typedef struct _AEGP_SoundData		**AEGP_SoundDataH;

#endif


typedef A_long	AEGP_PluginID;
// define a _AEGP_Refcon1 struct to use a typesafe refcon.
typedef struct _AEGP_GlobalRefcon		*AEGP_GlobalRefcon;
typedef struct _AEGP_CommandRefcon	*AEGP_CommandRefcon;
typedef struct _AEGP_UpdateMenuRefcon	*AEGP_UpdateMenuRefcon;
typedef struct _AEGP_DeathRefcon		*AEGP_DeathRefcon;
typedef struct _AEGP_VersionRefcon		*AEGP_VersionRefcon;
typedef struct _AEGP_AboutStringRefcon	*AEGP_AboutStringRefcon;
typedef struct _AEGP_AboutRefcon		*AEGP_AboutRefcon;
typedef struct _AEGP_IdleRefcon			*AEGP_IdleRefcon;
typedef struct _AEGP_IORefcon			*AEGP_IORefcon;


// _SIZE constants include space for the terminating null. -1 for string length.
#define		AEGP_MAX_PATH_SIZE				260
#define		AEGP_MAX_ABOUT_STRING_SIZE		256
#define		AEGP_MAX_TYPE_NAME_SIZE			32
#define		AEGP_MAX_ITEM_NAME_SIZE			32
#define		AEGP_MAX_LAYER_NAME_SIZE		32
#define		AEGP_MAX_MASK_NAME_SIZE			32
#define		AEGP_MAX_EFFECT_NAME_SIZE			(PF_MAX_EFFECT_NAME_LEN + 17)
#define		AEGP_MAX_EFFECT_MATCH_NAME_SIZE		(PF_MAX_EFFECT_NAME_LEN + 17)
#define		AEGP_MAX_EFFECT_CATEGORY_NAME_SIZE	(PF_MAX_EFFECT_CATEGORY_NAME_LEN + 1)
#define		AEGP_MAX_STREAM_NAME_SIZE		(PF_MAX_EFFECT_PARAM_NAME_LEN + 1)
#define		AEGP_MAX_STREAM_UNITS_SIZE		(PF_MAX_EFFECT_PARAM_NAME_LEN + 1)
#define		AEGP_MAX_PROJ_NAME_SIZE			48
#define		AEGP_MAX_PLUGIN_NAME_SIZE		32

#define		AEGP_MAX_MARKER_NAME_SIZE			64
#define		AEGP_MAX_MARKER_URL_SIZE			1024
#define		AEGP_MAX_MARKER_TARGET_SIZE			128
#define		AEGP_MAX_MARKER_CHAPTER_SIZE		128


#define AEGP_Index_NONE			((A_long)0x80000000)
#define AEGP_Index_START		((A_long)0)
#define AEGP_Index_END			((A_long)-1)

typedef A_long 	AEGP_Index;

#define AEGP_LayerIDVal_NONE			(0L)

typedef A_long			AEGP_LayerIDVal;

#define AEGP_MaskIDVal_NONE			(0L)

typedef A_long			AEGP_MaskIDVal;

typedef struct {
	A_FpLong		alphaF, redF, greenF, blueF;  // in the range [0.0 - 1.0]
} AEGP_ColorVal;


enum {
	AEGP_CameraType_NONE = -1,

	AEGP_CameraType_PERSPECTIVE,
	AEGP_CameraType_ORTHOGRAPHIC,

	AEGP_CameraType_NUM_TYPES
};

typedef A_u_long AEGP_CameraType; 


enum {
	AEGP_LightType_NONE = -1,

	AEGP_LightType_PARALLEL,
	AEGP_LightType_SPOT,
	AEGP_LightType_POINT,
	AEGP_LightType_AMBIENT,
	
	AEGP_LightType_NUM_TYPES
};

typedef A_u_long AEGP_LightType;


/* -------------------------------------------------------------------- */


#define kAEGPProjSuite				"AEGP Proj Suite"
#define kAEGPProjSuiteVersion1		2					// version 1 is AE 4


typedef struct AEGP_ProjSuite1 {

	SPAPI A_Err	(*AEGP_GetNumProjects)(									// will always (in 5.0) return 1 if project is open
						A_long				*num_projPL);				/* << */

	SPAPI A_Err	(*AEGP_GetProjectByIndex)(
						A_long				proj_indexL,				/* >> */
						AEGP_ProjectH		*projPH);					/* << */

	SPAPI A_Err	(*AEGP_GetProjectName)(
						AEGP_ProjectH		projH,						/* >> */
						A_char 				*nameZ);					/* << space for A_char[AEGP_MAX_PROJ_NAME_SIZE] */

	SPAPI A_Err	(*AEGP_GetProjectPath)(
						AEGP_ProjectH		projH,						/* >> */
						A_char 				*pathZ);					/* << empty string if no file. space for A_char[AEGP_MAX_PATH_SIZE] */

	SPAPI A_Err	(*AEGP_GetProjectRootFolder)(
						AEGP_ProjectH		projH,						/* >> */
						AEGP_ItemH			*root_folderPH);			/* << */
						

} AEGP_ProjSuite1;


/* -------------------------------------------------------------------- */

enum {
	AEGP_SoundEncoding_UNSIGNED_PCM = 3,
	AEGP_SoundEncoding_SIGNED_PCM,
	AEGP_SoundEncoding_FLOAT,

	AEGP_SoundEncoding_END,
	AEGP_SoundEncoding_BEGIN = AEGP_SoundEncoding_UNSIGNED_PCM
};
typedef A_long AEGP_SoundEncoding;

typedef struct AEGP_SoundDataFormat {
	A_FpLong				sample_rateF;			
	AEGP_SoundEncoding		encoding;				
	A_long					bytes_per_sampleL;		// 1, 2, or 4 only (ignored if encoding == float)
	A_long					num_channelsL;			// 1 for mono, 2 for stereo
}AEGP_SoundDataFormat;


enum {
	AEGP_ItemType_NONE,
	
	AEGP_ItemType_FOLDER,
	AEGP_ItemType_COMP,
	AEGP_ItemType_SOLID,
	AEGP_ItemType_FOOTAGE,
	
	AEGP_ItemType_NUM_TYPES1
};
typedef A_short AEGP_ItemType;


enum {
	AEGP_ItemFlag_MISSING			= 0x1,			// footage property: here for convenience
	AEGP_ItemFlag_HAS_PROXY			= 0x2,
	AEGP_ItemFlag_USING_PROXY		= 0x4,			// is using the proxy as source
	AEGP_ItemFlag_MISSING_PROXY		= 0x8,			// footage property: here for convenience
	AEGP_ItemFlag_HAS_VIDEO			= 0x10,			// is there a video track?
	AEGP_ItemFlag_HAS_AUDIO			= 0x20,			// is there an audio track?
	AEGP_ItemFlag_STILL				= 0x40			// are all frames exactly the same
};
typedef A_long AEGP_ItemFlags;


enum {
	AEGP_Label_NONE = -1,			// undefined sentinel value
	AEGP_Label_NO_LABEL = 0,
	AEGP_Label_1,
	AEGP_Label_2,
	AEGP_Label_3,
	AEGP_Label_4,
	AEGP_Label_5,
	AEGP_Label_6,
	AEGP_Label_7
};
typedef A_char AEGP_LabelID;


#define kAEGPItemSuite				"AEGP Item Suite"
#define kAEGPItemSuiteVersion1		3

typedef struct AEGP_ItemSuite1 {

	SPAPI A_Err	(*AEGP_GetNextItem)(
						AEGP_ItemH			itemH,					/* >> */
						AEGP_ItemH			*next_itemPH);			/* << NULL after last item */

	SPAPI A_Err	(*AEGP_GetActiveItem)(
						AEGP_ItemH			*itemPH);				/* << could be NULL if none is active */

	SPAPI A_Err	(*AEGP_IsItemSelected)(
						AEGP_ItemH			itemH,					/* >> */
						A_Boolean			*selectedPB);			/* << */

	SPAPI A_Err	(*AEGP_SelectItem)(
						AEGP_ItemH			itemH,					/* >> */
						A_Boolean			selectB,				// >>	allows to select or deselect the item
						A_Boolean			deselect_othersB);		/* >> */

	SPAPI A_Err	(*AEGP_GetItemType)(
						AEGP_ItemH 			itemH,					/* >> */
						AEGP_ItemType		*item_typeP);			/* << */
						
	SPAPI A_Err	(*AEGP_GetTypeName)(
						AEGP_ItemType		item_type,				/* << */
						A_char 				*nameZ);				/* << space for A_char[AEGP_MAX_TYPE_NAME_SIZE] */

	SPAPI A_Err	(*AEGP_GetItemName)(
						AEGP_ItemH 			itemH,					/* >> */	
						A_char				*nameZ);				/* << space for A_char[AEGP_MAX_ITEM_NAME_SIZE] */

	SPAPI A_Err	(*AEGP_GetItemID)(
						AEGP_ItemH 			itemH,					/* >> */	
						A_long				*item_idPL);			/* << */

	SPAPI A_Err	(*AEGP_GetItemFlags)(
						AEGP_ItemH 			itemH,					/* >> */
						AEGP_ItemFlags		*item_flagsP);			/* << */

	SPAPI A_Err	(*AEGP_SetItemUseProxy)(							/* UNDOABLE */
						AEGP_ItemH 			itemH,					/* >> error if has_proxy is FALSE! */
						A_Boolean			use_proxyB);			/* >> */

	SPAPI A_Err	(*AEGP_GetItemParentFolder)(
						AEGP_ItemH			itemH,						/* >> */
						AEGP_ItemH			*parent_folder_itemPH);		/* << */

	SPAPI A_Err	(*AEGP_GetItemDuration)(
						AEGP_ItemH 			itemH,					/* >> */	
						A_Time 				*durationPT);			/* << */

	SPAPI A_Err	(*AEGP_GetItemCurrentTime)(							// not updated while rendering
						AEGP_ItemH 			itemH,					/* >> */	
						A_Time 				*curr_timePT);			/* << */

	SPAPI A_Err	(*AEGP_GetItemDimensions)(
						AEGP_ItemH 			itemH,					/* >> */	
						A_long	 			*widthPL,				/* << */
						A_long				*heightPL);				/* << */

	SPAPI A_Err	(*AEGP_GetItemPixelAspectRatio)(
						AEGP_ItemH 			itemH,					/* >> */	
						A_Ratio 			*pix_aspect_ratioPRt);	/* << */

	SPAPI A_Err	(*AEGP_DeleteItem)(									/* UNDOABLE */
						AEGP_ItemH 			itemH);					/* >> removes item from all comps */

	SPAPI A_Err	(*AEGP_GetItemSolidColor)(							// error if item isn't AEGP_ItemType_SOLID!
						AEGP_ItemH 			itemH,					/* >> */
						AEGP_ColorVal		*colorP);				/* << */

	SPAPI A_Err (*AEGP_CreateNewFolder)(
						const A_char		*nameZ,					/* >> */
						AEGP_ItemH			parent_folderH0,		/* >> */
						AEGP_ItemH			*new_folderPH);			/* << */  /* allocated and owned by project (AE) */

	SPAPI A_Err (*AEGP_SetItemCurrentTime)(							// Undoable
						AEGP_ItemH 			itemH,					/* >> */
						const A_Time		*new_timePT);			// >>


	//  work on Comps and Footage items.
	SPAPI A_Err (*AEGP_RenderNewItemSoundData)( // AEGP_SoundDataH must be disposed.
					AEGP_ItemH	itemH,								// >>
					const A_Time			*start_timePT,			// >>
					const A_Time			*durationPT,			// >>
					const AEGP_SoundDataFormat* sound_formatP,		// >>
					AEGP_SoundDataH			*new_sound_dataPH);		// << can return NULL if no audio

} AEGP_ItemSuite1;


/* -------------------------------------------------------------------- */

#define kAEGPSoundDataSuite				"AEGP Sound Data Suite"
#define kAEGPSoundDataVersion1			1


typedef struct AEGP_SoundDataSuite1 {
	SPAPI A_Err (*AEGP_NewSoundData)(			// Must be disposed with DisposeSoundData
					const AEGP_SoundDataFormat*	sound_formatP,
					AEGP_SoundDataH			*new_sound_dataPH);		// << can return NULL if no audio					
	
	SPAPI A_Err (*AEGP_DisposeSoundData)(
					AEGP_SoundDataH	sound_dataH); // >>

	SPAPI A_Err (*AEGP_GetSoundDataFormat)(
					AEGP_SoundDataH			soundH,			// >>
					AEGP_SoundDataFormat	*sound_formatP);	// <<


	/*
		If the sound format has two channels, the data is interleaved
		left (0), right(1), left(0), right(1), ...

		AEGP_SoundEncoding_FLOAT has a type of FpShort

		For bytes_per_sample == 1
		AEGP_SoundEncoding_UNSIGNED_PCM == A_u_char
		AEGP_SoundEncoding_SIGNED_PCM	== A_char

		For bytes_per_sample == 2
		AEGP_SoundEncoding_UNSIGNED_PCM == A_u_short
		AEGP_SoundEncoding_SIGNED_PCM	== A_short

		For bytes_per_sample == 4
		AEGP_SoundEncoding_UNSIGNED_PCM == A_u_long
		AEGP_SoundEncoding_SIGNED_PCM	== A_long

		usage:
		void * sound_dataP;
		sds->AEGP_LockSoundDataSamples( soundH, &sound_dataP);
		A_u_long* correct_samples = (A_u_long*)sound_dataP; // for AEGP_SoundEncoding_UNSIGNED_PCM
	*/

	SPAPI A_Err (*AEGP_LockSoundDataSamples)(
					AEGP_SoundDataH			soundH,			// >>
					void					**samples);		// << use the correct combination of unsigned/signed/float and bytes_per_sample to determine type

	SPAPI A_Err (*AEGP_UnlockSoundDataSamples)(
					AEGP_SoundDataH			soundH);		// >>

	SPAPI A_Err (*AEGP_GetNumSamples)(
					AEGP_SoundDataH			soundH,			// >> 
					A_long					*num_samplesPL);// << 

} AEGP_SoundDataSuite1 ;



/* -------------------------------------------------------------------- */


typedef struct {
	A_short	xS, yS;
} AEGP_DownsampleFactor;

enum {
	AEGP_CompFlag_SHOW_ALL_SHY			= 0x1,
	AEGP_CompFlag_RESERVED_1			= 0x2,
	AEGP_CompFlag_RESERVED_2			= 0x4,
	AEGP_CompFlag_ENABLE_MOTION_BLUR	= 0x8,
	AEGP_CompFlag_ENABLE_TIME_FILTER	= 0x10,
	AEGP_CompFlag_GRID_TO_FRAMES		= 0x20,
	AEGP_CompFlag_GRID_TO_FIELDS		= 0x40,
	AEGP_CompFlag_USE_LOCAL_DSF			= 0x80,		// If on, use the dsf in the comp, not the RO
	AEGP_CompFlag_DRAFT_3D				= 0x100
	
};
typedef A_long	AEGP_CompFlags;




#define kAEGPCompSuite				"AEGP Comp Suite"
#define kAEGPCompSuiteVersion1		4

typedef struct AEGP_CompSuite1 {

	SPAPI A_Err	(*AEGP_GetCompFromItem)(							// error if item isn't AEGP_ItemType_COMP!
						AEGP_ItemH 			itemH,					/* >> */
						AEGP_CompH			*compPH);				/* << */

	SPAPI A_Err	(*AEGP_GetItemFromComp)(
						AEGP_CompH			compH,					/* >> */
						AEGP_ItemH 			*itemPH);				/* << */

	SPAPI A_Err	(*AEGP_GetCompDownsampleFactor)(
						AEGP_CompH				compH,				/* >> */
						AEGP_DownsampleFactor	*dsfP);				/* << */
						
	SPAPI A_Err	(*AEGP_GetCompBGColor)(
						AEGP_CompH			compH,					/* >> */
						AEGP_ColorVal		*bg_colorP);			/* << */
						
	SPAPI A_Err	(*AEGP_GetCompFlags)(
						AEGP_CompH			compH,					/* >> */
						AEGP_CompFlags		*comp_flagsP);			/* << */

	SPAPI A_Err	(*AEGP_GetCompFramerate)(
						AEGP_CompH			compH,					/* >> */
						A_FpLong			*fpsPF);				/* << */

	SPAPI A_Err	(*AEGP_GetCompShutterAnglePhase)(
						AEGP_CompH			compH,					/* >> */
						A_Ratio				*angle,					/* << */
						A_Ratio				*phase);				/* << */

	SPAPI A_Err	(*AEGP_GetCompShutterFrameRange)(
						AEGP_CompH			compH,					/* >> */
						const A_Time		*comp_timeP,			/* >> */
						A_Time				*start,					/* << */
						A_Time				*duration);				/* << */

	SPAPI A_Err	(*AEGP_GetCompWorkAreaStart)(
						AEGP_CompH			compH,					/* >> */
						A_Time 				*work_area_startPT);	/* << */

	SPAPI A_Err	(*AEGP_GetCompWorkAreaDuration)(
						AEGP_CompH			compH,					/* >> */
						A_Time 				*work_area_durationPT);	/* << */

	SPAPI A_Err	(*AEGP_SetCompWorkAreaStartAndDuration)(			/* UNDOABLE */
						AEGP_CompH			compH,					/* >> */
						A_Time 				*work_area_startPT,		/* >> */
						A_Time 				*work_area_durationPT);	/* >> */

	SPAPI A_Err (*AEGP_CreateSolidInComp)(
						const A_char		*nameZ,					/* >> */
						A_long				width,					/* >> */
						A_long				height,					/* >> */
						const AEGP_ColorVal	*color,					/* >> */
						AEGP_ItemH			parent_compH,			/* >> */
						AEGP_LayerH			*new_solidPH);			/* << */

	SPAPI A_Err (*AEGP_CreateComp)(
						AEGP_ItemH			parent_folderH0,		/* >> */
						const A_char		*nameZ,					/* >> */
						A_long				widthL,					/* >> */
						A_long				heightL,				/* >> */
						const A_Ratio		*pixel_aspect_ratioPRt,	/* >> */
						const A_Time		*durationPT,			/* >> */
						const A_Ratio		*frameratePRt,			/* >> */
						AEGP_ItemH			*new_compPH);			/* << */

	SPAPI A_Err	(*AEGP_GetNewCollectionFromCompSelection)(
						AEGP_PluginID		plugin_id,				/* >> */
						AEGP_CompH			compH,					/* >> */
						AEGP_CollectionH	*collectionPH);			/* << */

	SPAPI A_Err (*AEGP_SetSelection)(
						AEGP_CompH	compH,							/* >> */
						AEGP_CollectionH collectionH);				/* >> not adopted */

	SPAPI A_Err (*AEGP_SetCompDisplayStartTime)(					/*	NOT Undoable! */
						AEGP_CompH			compH,					/* >> */
						const A_Time		*start_timePT);			/* >> */

	SPAPI A_Err (*AEGP_SetCompDuration)(							
						AEGP_CompH			compH,					/* >> */
						const A_Time		*durationPT);			/* >> */
} AEGP_CompSuite1;


/* -------------------------------------------------------------------- */


#define kAEGPMemorySuite				"AEGP Memory Suite"
#define kAEGPMemorySuiteVersion1		1

typedef struct _Up_OpaqueMem **AEGP_MemHandle;

enum {
	AEGP_MemFlag_NONE = 0x0,
	AEGP_MemFlag_CLEAR = 0x01,
	AEGP_MemFlag_QUIET = 0x02
};
typedef A_long	AEGP_MemFlag;

typedef A_u_long	AEGP_MemSize;


typedef struct AEGP_MemorySuite1 {


	SPAPI A_Err	 (*AEGP_NewMemHandle)( AEGP_PluginID	plugin_id,			/* >> */
									   const A_char	*whatZ,			/* >> */
									   AEGP_MemSize	size,			/* >> */
									   AEGP_MemFlag	flags,			/* >> */
									   AEGP_MemHandle	*memPH);	/* << */

	SPAPI A_Err	 (*AEGP_FreeMemHandle)(		AEGP_MemHandle memH);

	SPAPI A_Err	 (*AEGP_LockMemHandle)(		AEGP_MemHandle memH,			// nestable
											void			**ptr_to_ptr);	/* << */

	SPAPI A_Err	 (*AEGP_UnlockMemHandle)(	AEGP_MemHandle memH);

	SPAPI A_Err	 (*AEGP_GetMemHandleSize)(	AEGP_MemHandle	memH,
											AEGP_MemSize	*sizeP);		/* << */

	SPAPI A_Err	 (*AEGP_ResizeMemHandle)(	const char			*whatZ,		/* >> */
											AEGP_MemSize		new_size,	/* >> */
											AEGP_MemHandle		memH);		/* <> */

	SPAPI A_Err	 (*AEGP_SetMemReportingOn)(A_Boolean turn_OnB);

	SPAPI A_Err	 (*AEGP_GetMemStats)(	AEGP_PluginID	plugin_id,				/* >> */
										A_long			*countPL,			/* << */
										A_long			*sizePL);			/* << */
} AEGP_MemorySuite1;

/* -------------------------------------------------------------------- */


enum {
	AEGP_TransferFlag_PRESERVE_ALPHA		= 0x1,
	AEGP_TransferFlag_RANDOMIZE_DISSOLVE	= 0x2
};
typedef A_long	AEGP_TransferFlags;

enum {
	AEGP_TrackMatte_NO_TRACK_MATTE,
	AEGP_TrackMatte_ALPHA,
	AEGP_TrackMatte_NOT_ALPHA,
	AEGP_TrackMatte_LUMA,
	AEGP_TrackMatte_NOT_LUMA
};
typedef A_long	AEGP_TrackMatte;

typedef struct {
	PF_TransferMode			mode;				// defined in AE_EffectCB.h
	AEGP_TransferFlags		flags;
	AEGP_TrackMatte			track_matte;
} AEGP_LayerTransferMode;



enum {
	AEGP_LayerQual_NONE = -1,					// sentinel
	AEGP_LayerQual_WIREFRAME,					// wire-frames only
	AEGP_LayerQual_DRAFT,						// LO-qual filters, LO-qual geom
	AEGP_LayerQual_BEST							// HI-qual filters, HI-qual geom
};
typedef A_short AEGP_LayerQuality;			

enum {
	AEGP_LayerFlag_NONE				 		= 0,
	AEGP_LayerFlag_VIDEO_ACTIVE		 		= 0x1,
	AEGP_LayerFlag_AUDIO_ACTIVE 			= 0x2,
	AEGP_LayerFlag_EFFECTS_ACTIVE	 		= 0x4,
	AEGP_LayerFlag_MOTION_BLUR 				= 0x8,
	AEGP_LayerFlag_FRAME_BLENDING	 		= 0x10,
	AEGP_LayerFlag_LOCKED 					= 0x20,
	AEGP_LayerFlag_SHY 						= 0x40,
	AEGP_LayerFlag_COLLAPSE					= 0x80,
	AEGP_LayerFlag_AUTO_ORIENT_ROTATION		= 0x100,
	AEGP_LayerFlag_ADJUSTMENT_LAYER			= 0x200,
	AEGP_LayerFlag_TIME_REMAPPING			= 0x400,
  	AEGP_LayerFlag_RESERVED_1				= 0x800,
  	AEGP_LayerFlag_RESERVED_2				= 0x1000,
  	AEGP_LayerFlag_RESERVED_3				= 0x2000,
	AEGP_LayerFlag_SOLO						= 0x4000
};
typedef A_long				AEGP_LayerFlags;


// Layers are always one of the following types.

enum {
	AEGP_ObjectType_NONE = -1,			
	AEGP_ObjectType_AV,			// Includes all layers types previous to AE 5.0
								// (audio and/or video source, including adjustment layers)
	AEGP_ObjectType_LIGHT,				
	AEGP_ObjectType_CAMERA,				
	AEGP_ObjectType_NUM_TYPES
};
typedef A_long AEGP_ObjectType;

enum {
	AEGP_LTimeMode_LayerTime,
	AEGP_LTimeMode_CompTime
};
typedef A_short				AEGP_LTimeMode;

#define	AEGP_REORDER_LAYER_TO_END	-1

#define kAEGPLayerSuite				"AEGP Layer Suite"
#define kAEGPLayerSuiteVersion1		5								// version 1 is AE 4

typedef struct AEGP_LayerSuite1 {

	SPAPI A_Err	(*AEGP_GetCompNumLayers)(
						AEGP_CompH			compH,					/* >> */
						A_long				*num_layersPL);			/* << */
						
	SPAPI A_Err	(*AEGP_GetCompLayerByIndex)(
						AEGP_CompH			compH,					/* >> */
						A_long				layer_indexL,			/* >> */
						AEGP_LayerH			*layerPH);				/* << */

	SPAPI A_Err	(*AEGP_GetActiveLayer)(
						AEGP_LayerH			*layerPH);				/* << only if one layer is selected */

	SPAPI A_Err	(*AEGP_GetLayerIndex)(
						AEGP_LayerH			layerH,					/* >> */
						A_long				*layer_indexPL);		/* << */

	SPAPI A_Err	(*AEGP_GetLayerSourceItem)(
						AEGP_LayerH			layerH,					/* >> */
						AEGP_ItemH			*source_itemPH);		/* << */

	SPAPI A_Err	(*AEGP_GetLayerParentComp)(
						AEGP_LayerH			layerH,					/* >> */
						AEGP_CompH			*compPH);				/* << */

	SPAPI A_Err	(*AEGP_GetLayerName)(
						AEGP_LayerH 		layerH,					/* >> */	
						A_char				*layer_nameZ0,			/* << space for A_char[AEGP_MAX_LAYER_NAME_SIZE] */
						A_char				*source_nameZ0);		/* << space for A_char[AEGP_MAX_LAYER_NAME_SIZE] */

	SPAPI A_Err	(*AEGP_GetLayerQuality)(
						AEGP_LayerH			layerH,					/* >> */
						AEGP_LayerQuality	*qualityP);				/* << */
														
	SPAPI A_Err	(*AEGP_SetLayerQuality)(							/* UNDOABLE */
						AEGP_LayerH			layerH,					/* >> */
						AEGP_LayerQuality	quality);				/* >> */

	SPAPI A_Err	(*AEGP_GetLayerFlags)(
						AEGP_LayerH			layerH,					/* >> */
						AEGP_LayerFlags		*layer_flagsP);			/* << */
						
	SPAPI A_Err (*AEGP_SetLayerFlag)(
						AEGP_LayerH			layerH,					// >>
						AEGP_LayerFlags		single_flag,			// >>
						A_Boolean			valueB);				// >>

	SPAPI A_Err	(*AEGP_IsLayerVideoReallyOn)(						// accounts for solo status of other layers in comp
						AEGP_LayerH			layerH,					/* >> */
						A_Boolean			*onPB);					/* << */

	SPAPI A_Err	(*AEGP_IsLayerAudioReallyOn)(						// accounts for solo status of other layers in comp
						AEGP_LayerH			layerH,					/* >> */
						A_Boolean			*onPB);					/* << */

	SPAPI A_Err	(*AEGP_GetLayerCurrentTime)(						// not updated while rendering
						AEGP_LayerH 		layerH,					/* >> */	
						AEGP_LTimeMode		time_mode,				/* >> */				
						A_Time 				*curr_timePT);			/* << */

	SPAPI A_Err	(*AEGP_GetLayerInPoint)(
						AEGP_LayerH 		layerH,					/* >> */
						AEGP_LTimeMode		time_mode,				/* >> */				
						A_Time 				*in_pointPT);			/* << */

	SPAPI A_Err	(*AEGP_GetLayerDuration)(
						AEGP_LayerH 		layerH,					/* >> */
						AEGP_LTimeMode		time_mode,				/* >> */				
						A_Time 				*durationPT);			/* << */

	SPAPI A_Err	(*AEGP_SetLayerInPointAndDuration)(					/* UNDOABLE */
						AEGP_LayerH 		layerH,					/* >> */
						AEGP_LTimeMode		time_mode,				/* >> */				
						const A_Time 				*in_pointPT,	/* >> */
						const A_Time 				*durationPT);	/* >> */

	SPAPI A_Err	(*AEGP_GetLayerOffset)(
						AEGP_LayerH 		layerH,					/* >> */
						A_Time 				*offsetPT);				/* << always in comp time */

	SPAPI A_Err	(*AEGP_SetLayerOffset)(								/* UNDOABLE */
						AEGP_LayerH 		layerH,					/* >> */
						const A_Time 				*offsetPT);		/* >> always in comp time */

	SPAPI A_Err	(*AEGP_GetLayerStretch)(
						AEGP_LayerH 		layerH,					/* >> */
						A_Ratio 			*stretchPRt);			/* << */

	SPAPI A_Err	(*AEGP_SetLayerStretch)(							/* UNDOABLE */
						AEGP_LayerH 		layerH,					/* >> */
						const A_Ratio 			*stretchPRt);			/* >> */

	SPAPI A_Err	(*AEGP_GetLayerTransferMode)(
						AEGP_LayerH 			layerH,				/* >> */	
						AEGP_LayerTransferMode	*transfer_modeP);	/* << */

	SPAPI A_Err	(*AEGP_SetLayerTransferMode)(						/* UNDOABLE */
						AEGP_LayerH 			layerH,				/* >> */	
						const AEGP_LayerTransferMode	*transfer_modeP);	/* >> */

	SPAPI A_Err	(*AEGP_IsAddLayerValid)(
						AEGP_ItemH 			item_to_addH,			/* >> */
						AEGP_CompH 			into_compH,				/* >> */	
						A_Boolean			*validPB);				/* << */

	SPAPI A_Err	(*AEGP_AddLayer)(									/* UNDOABLE */
						AEGP_ItemH 			item_to_addH,			/* >> check AEGP_IsAddLayerValid() before using */
						AEGP_CompH 			into_compH,				/* >> */	
						AEGP_LayerH			*added_layerPH0);		/* << */

	SPAPI A_Err	(*AEGP_ReorderLayer)(								/* UNDOABLE */
						AEGP_LayerH 		layerH,					/* >> */	
						A_long 				layer_indexL);			/* >> */

	SPAPI A_Err	(*AEGP_GetLayerMaskedBounds)(
						AEGP_LayerH 		layerH,				/* >> */	
						const A_Time	 	*comp_timePT,		/* >> */
						A_FloatRect			*boundsPR);			/* << */

	SPAPI A_Err	(*AEGP_GetLayerObjectType)(						
						AEGP_LayerH 		layerH,				/* >> */	
						AEGP_ObjectType 	*object_type);		/* << */

	SPAPI A_Err	(*AEGP_IsLayer3D)(						
						AEGP_LayerH 		layerH,				/* >> */	
						A_Boolean 			*is_3DPB);			/* << */

	SPAPI A_Err	(*AEGP_IsLayer2D)(						
						AEGP_LayerH 		layerH,				/* >> */	
						A_Boolean 			*is_2DPB);			/* << */

	SPAPI A_Err	(*AEGP_IsVideoActive)(						
						AEGP_LayerH 		layerH,				/* >> */	
						const A_Time	 	*comp_timePT,		/* >> */
						A_Boolean			*is_activePB);		/* << */

	SPAPI A_Err	(*AEGP_IsLayerUsedAsTrackMatte)(
						AEGP_LayerH			layerH,					/* >> */
						A_Boolean			fill_must_be_activeB,	/* >> */
						A_Boolean			*is_track_mattePB);		/* << */

	SPAPI A_Err	(*AEGP_DoesLayerHaveTrackMatte)(
						AEGP_LayerH			layerH,					/* >> */
						A_Boolean			*has_track_mattePB);	/* << */

	SPAPI A_Err	(*AEGP_ConvertCompToLayerTime)(
					AEGP_LayerH		layerH,							/* >> */
					const A_Time	*comp_timeP,					/* >> */
					A_Time			*layer_timeP); 					/* << */

	SPAPI A_Err	(*AEGP_GetLayerDancingRandValue)(
					AEGP_LayerH		layerH,							/* >> */
					const A_Time	*comp_timePT,					/* >> */
					A_long			*rand_valuePL);					/* << */

	SPAPI A_Err	(*AEGP_GetLayerID)(
						AEGP_LayerH				layerH,				/* >> */
						AEGP_LayerIDVal				*id_valP);			/* << */

	SPAPI A_Err	(*AEGP_GetLayerToWorldXform)(						
				AEGP_LayerH				aegp_layerH,		/* >> */
				const A_Time			*comp_timeP,		/* >> */
				A_Matrix4				*tranform);			/* >> */

	SPAPI A_Err	(*AEGP_GetLayerToWorldXformFromView)(						
				AEGP_LayerH				aegp_layerH,		/* >> */
				const A_Time			*view_timeP,		/* >> */
				const A_Time			*comp_timeP,		/* >> */
				A_Matrix4				*tranform);			/* >> */

	SPAPI A_Err (*AEGP_SetLayerName)(
				AEGP_LayerH				aegp_layerH,		/* >> */
				const A_char			*new_nameZ);		/* >> */

} AEGP_LayerSuite1;

/* -------------------------------------------------------------------- */


enum {
	AEGP_LayerStream_NONE = -1,
	AEGP_LayerStream_ANCHORPOINT,
	AEGP_LayerStream_POSITION,
	AEGP_LayerStream_SCALE,
	AEGP_LayerStream_ROTATION,
	AEGP_LayerStream_ROTATE_Z = AEGP_LayerStream_ROTATION, // for 3d streams
	AEGP_LayerStream_OPACITY,
	AEGP_LayerStream_AUDIO,
	AEGP_LayerStream_MARKER,
	AEGP_LayerStream_TIME_REMAP,
	AEGP_LayerStream_ROTATE_X,
	AEGP_LayerStream_ROTATE_Y,
	AEGP_LayerStream_ORIENTATION,

	// only valid for AEGP_ObjectType == AEGP_ObjectType_CAMERA
	AEGP_LayerStream_ZOOM,			
	AEGP_LayerStream_DEPTH_OF_FIELD,
	AEGP_LayerStream_FOCUS_DISTANCE,
	AEGP_LayerStream_APERTURE,
	AEGP_LayerStream_BLUR_LEVEL,	

	// only valid for AEGP_ObjectType == AEGP_ObjectType_LIGHT	
	AEGP_LayerStream_INTENSITY,
	AEGP_LayerStream_COLOR,
	AEGP_LayerStream_CONE_ANGLE,
	AEGP_LayerStream_CONE_FEATHER,
	AEGP_LayerStream_SHADOW_DARKNESS,
	AEGP_LayerStream_SHADOW_DIFFUSION,

	// only valid for AEGP_ObjectType == AEGP_ObjectType_AV
	AEGP_LayerStream_ACCEPTS_SHADOWS,	
	AEGP_LayerStream_ACCEPTS_LIGHTS,
	AEGP_LayerStream_AMBIENT_COEFF,
	AEGP_LayerStream_DIFFUSE_COEFF,
	AEGP_LayerStream_SPECULAR_COEFF,
	AEGP_LayerStream_SHININESS_COEFF,

	AEGP_LayerStream_CASTS_SHADOWS, 	// LIGHT, and AV only, no CAMERA


	AEGP_LayerStream_BEGIN = AEGP_LayerStream_ANCHORPOINT,
	AEGP_LayerStream_END = AEGP_LayerStream_CASTS_SHADOWS+1
		
};
typedef A_long				AEGP_LayerStream;

enum {
	AEGP_MaskStream_OUTLINE = 400,
	AEGP_MaskStream_OPACITY,
	AEGP_MaskStream_FEATHER, 
	AEGP_MaskStream_EXPANSION,

	// useful for iteration
	AEGP_MaskStream_BEGIN = AEGP_MaskStream_OUTLINE,
	AEGP_MaskStream_END =	AEGP_MaskStream_EXPANSION+1
};
typedef A_long				AEGP_MaskStream;


enum {
	AEGP_StreamFlag_NONE			= 0,
	AEGP_StreamFlag_HAS_MIN			= 0x01,
	AEGP_StreamFlag_HAS_MAX			= 0x02,
	AEGP_StreamFlag_IS_SPATIAL		= 0x04
};
typedef A_long				AEGP_StreamFlags;


typedef A_FpLong		AEGP_FourDVal[4];
typedef A_FloatPoint3	AEGP_ThreeDVal;

typedef A_FloatPoint	AEGP_TwoDVal;			// if audio, rt is x, left is y

typedef A_FpLong		AEGP_OneDVal;


typedef A_Handle		AEGP_ArbBlockVal;

typedef struct {
	A_char				nameAC[AEGP_MAX_MARKER_NAME_SIZE];
	A_char				urlAC[AEGP_MAX_MARKER_URL_SIZE];
	A_char				targetAC[AEGP_MAX_MARKER_TARGET_SIZE];
	A_char				chapterAC[AEGP_MAX_MARKER_CHAPTER_SIZE];
} AEGP_MarkerVal;

typedef AEGP_MarkerVal** AEGP_MarkerValH;



enum {
	AEGP_KeyInterp_NONE = 0,
	AEGP_KeyInterp_LINEAR,
	AEGP_KeyInterp_BEZIER,
	AEGP_KeyInterp_HOLD,

	AEGP_Interp_NUM_VALUES
};
typedef long	AEGP_KeyframeInterpolationType;


typedef struct {
	A_FpLong	speedF;
	A_FpLong	influenceF;
} AEGP_KeyframeEase;


typedef union {
	AEGP_FourDVal		four_d;
	AEGP_ThreeDVal		three_d;
	AEGP_TwoDVal		two_d;
	AEGP_OneDVal		one_d;
	AEGP_ColorVal		color;
	AEGP_ArbBlockVal	arbH;
	AEGP_MarkerValH		markerH;
	AEGP_LayerIDVal		layer_id;
	AEGP_MaskIDVal		mask_id;
	AEGP_MaskOutlineValH	mask;
} AEGP_StreamVal;

typedef struct {
	AEGP_StreamRefH 	streamH;
	AEGP_StreamVal		val;
} AEGP_StreamValue;


enum {
	AEGP_StreamType_NO_DATA,
	AEGP_StreamType_ThreeD_SPATIAL,
	AEGP_StreamType_ThreeD,
	AEGP_StreamType_TwoD_SPATIAL,
	AEGP_StreamType_TwoD,
	AEGP_StreamType_OneD,
	AEGP_StreamType_COLOR,
	AEGP_StreamType_ARB,
	AEGP_StreamType_MARKER,
	AEGP_StreamType_LAYER_ID,
	AEGP_StreamType_MASK_ID,
	AEGP_StreamType_MASK	
};
typedef A_long				AEGP_StreamType;


#define kAEGPStreamSuite				"AEGP Stream Suite"
#define kAEGPStreamSuiteVersion1		4

typedef struct AEGP_StreamSuite1 {

	SPAPI A_Err (*AEGP_IsStreamLegal)( 
						AEGP_LayerH			layerH,					/* >> */
						AEGP_LayerStream	which_stream,			/* >> */
						A_Boolean*			is_legalP);				/* << */


	SPAPI A_Err (*AEGP_CanVaryOverTime)(						
						AEGP_StreamRefH streamH,					/* >> */
						A_Boolean* can_varyPB);						/* << */


	SPAPI A_Err	(*AEGP_GetNewLayerStream)(
						AEGP_PluginID		aegp_plugin_id,			/* >> */
						AEGP_LayerH			layerH,					/* >> */
						AEGP_LayerStream	which_stream,			/* >> */
						AEGP_StreamRefH 		*streamPH);				/* << must be disposed! */

	SPAPI A_Err	(*AEGP_GetEffectNumParamStreams)(
						AEGP_EffectRefH		effect_refH,				/* >> */
						A_long				*num_paramsPL);			/* << */

	SPAPI A_Err	(*AEGP_GetNewEffectStreamByIndex)(
						AEGP_PluginID		aegp_plugin_id,			/* >> */
						AEGP_EffectRefH		effect_refH,			/* >> */
						PF_ParamIndex		param_index,			/* >> valid in range [1..AEGP_GetEffectNumParamStreams]*/
						AEGP_StreamRefH 		*streamPH);				/* << must be disposed! */

	SPAPI A_Err	(*AEGP_GetNewMaskStream)(
						AEGP_PluginID		aegp_plugin_id,			/* >> */
						AEGP_MaskRefH		mask_refH,					/* >> */
						AEGP_MaskStream		which_stream,			/* >> */
						AEGP_StreamRefH 		*mask_streamPH);	/* << must be disposed! */

	SPAPI A_Err	(*AEGP_DisposeStream)(
						AEGP_StreamRefH 		streamH);				/* >> */	

	SPAPI A_Err	(*AEGP_GetStreamName)(
						AEGP_StreamRefH 		streamH,				/* >> */	
						A_Boolean			force_englishB,			/* >> */
						A_char				*nameZ);				/* << space for A_char[AEGP_MAX_STREAM_NAME_SIZE] */

	SPAPI A_Err	(*AEGP_GetStreamUnitsText)(
						AEGP_StreamRefH 		streamH,				/* >> */
						A_Boolean			force_englishB,			/* >> */
						A_char				*unitsZ);				/* << space for A_char[AEGP_MAX_STREAM_UNITS_SIZE] */

	SPAPI A_Err	(*AEGP_GetStreamProperties)(
						AEGP_StreamRefH 		streamH,				/* >> */
						AEGP_StreamFlags 	*flagsP,				/* << */
						A_FpLong 			*minP0,					/* << */
						A_FpLong 			*maxP0);				/* << */

	SPAPI A_Err	(*AEGP_IsStreamTimevarying)(						// takes expressions into account
						AEGP_StreamRefH 		streamH,			/* >> */	
						A_Boolean			*is_timevaryingPB);		/* << */

	SPAPI A_Err	(*AEGP_GetStreamType)(
						AEGP_StreamRefH 		streamH,			/* >> */	
						AEGP_StreamType		*stream_typeP);			/* << */

	SPAPI A_Err	(*AEGP_GetNewStreamValue)(
						AEGP_PluginID		aegp_plugin_id,			/* >> */
						AEGP_StreamRefH 	streamH,				/* >> */
						AEGP_LTimeMode		time_mode,				/* >> */
						const A_Time		*timePT,				/* >> */
						A_Boolean			pre_expressionB,		/* >> sample the stream before evaluating the expression */
						AEGP_StreamValue	*valueP);				/* << must be disposed */

	SPAPI A_Err	(*AEGP_DisposeStreamValue)(
						AEGP_StreamValue	*valueP);				/* <> */


	SPAPI A_Err	(*AEGP_SetStreamValue)(								// only legal to call when AEGP_GetStreamNumKFs==0 or NO_DATA
						AEGP_PluginID		aegp_plugin_id,			/* >> */
						AEGP_StreamRefH 		streamH,			/* >> */
						AEGP_StreamValue	*valueP);				/* << */

	// this is only valid on streams with primitive types. It is illegal on
	// AEGP_ArbBlockVal || AEGP_MarkerValH || AEGP_MaskOutlineValH
	SPAPI A_Err	(*AEGP_GetLayerStreamValue)(
						AEGP_LayerH			layerH,					/* >> */
						AEGP_LayerStream	which_stream,			/* >> */
						AEGP_LTimeMode		time_mode,				/* >> */
						const A_Time		*timePT,				/* >> */
						A_Boolean			pre_expressionB,		/* >> sample the stream before evaluating the expression */
						AEGP_StreamVal	*stream_valP,				/* << */
						AEGP_StreamType *stream_typeP0);			/* << */
	
	SPAPI A_Err	(*AEGP_GetExpressionState)(							// expressions can be disabled automatically by the parser on playback	
						AEGP_PluginID		aegp_plugin_id,			/* >> */
						AEGP_StreamRefH 		streamH,				/* >> */
						A_Boolean			*enabledPB);			/* >> */


	
	SPAPI A_Err	(*AEGP_SetExpressionState)(							// expressions can be disabled automatically by the parser on playback
						AEGP_PluginID		aegp_plugin_id,			/* >> */
						AEGP_StreamRefH 		streamH,				/* >> */
						A_Boolean			enabledB);				/* >> */

	SPAPI A_Err	(*AEGP_GetExpression)(
						AEGP_PluginID		aegp_plugin_id,			/* >> */
						AEGP_StreamRefH 		streamH,				/* >> */
						AEGP_MemHandle		*expressionHZ);			/* >> must be disposed with AEGP_FreeMemHandle */

	SPAPI A_Err	(*AEGP_SetExpression)(
						AEGP_PluginID		aegp_plugin_id,			/* >> */
						AEGP_StreamRefH 		streamH,				/* >> */
						const char*			expressionP);			/* >> not adopted */


} AEGP_StreamSuite1;


/* -------------------------------------------------------------------- */

typedef A_long AEGP_KeyframeIndex;

enum {
	AEGP_KeyframeFlag_NONE					= 0x00,
	AEGP_KeyframeFlag_TEMPORAL_CONTINUOUS	= 0x01,
	AEGP_KeyframeFlag_TEMPORAL_AUTOBEZIER	= 0x02,
	AEGP_KeyframeFlag_SPATIAL_CONTINUOUS	= 0x04,
	AEGP_KeyframeFlag_SPATIAL_AUTOBEZIER	= 0x08,
	AEGP_KeyframeFlag_ROVING				= 0x10
};
typedef A_long	AEGP_KeyframeFlags;


enum {
	AEGP_NumKF_NO_DATA		= -1
};

#define kAEGPKeyframeSuite				"AEGP Keyframe Suite"
#define kAEGPKeyframeSuiteVersion1		1


typedef struct AEGP_KeyframeSuite1 {

	// returns AEGP_NumKF_NO_DATA if it's a AEGP_StreamType_NO_DATA, and you can't retrieve any values
	// returns zero if no keyframes (but might have an expression, so not necessarily constant)

	
	SPAPI A_Err	(*AEGP_GetStreamNumKFs)(
						AEGP_StreamRefH 			streamH,				/* >> */
						A_long					*num_kfsPL);			/* << */


	SPAPI A_Err (*AEGP_GetKeyframeTime)(
						AEGP_StreamRefH			streamH,				/* >> */
						AEGP_KeyframeIndex		key_index,				/* >> */						
						AEGP_LTimeMode			time_mode,				/* >> */
						A_Time					*timePT);				/* << */

	// leaves stream unchanged if a keyframe already exists at specified time
	SPAPI A_Err (*AEGP_InsertKeyframe)(									/* UNDOABLE */
						AEGP_StreamRefH			streamH,				/* <> */
						AEGP_LTimeMode			time_mode,				/* >> */
						const A_Time			*timePT,				/* >> */
						AEGP_KeyframeIndex		*key_indexP);			/* << */

	SPAPI A_Err (*AEGP_DeleteKeyframe)(									/* UNDOABLE */
						AEGP_StreamRefH			streamH,				/* <> */
						AEGP_KeyframeIndex		key_index);				/* >> */

	SPAPI A_Err (*AEGP_GetNewKeyframeValue)(							// dispose using AEGP_DisposeStreamValue()
						AEGP_PluginID			aegp_plugin_id,			/* >> */
						AEGP_StreamRefH			streamH,				/* >> */
						AEGP_KeyframeIndex		key_index,				/* >> */
						AEGP_StreamValue		*valueP);				/* << */

	SPAPI A_Err (*AEGP_SetKeyframeValue)(								/* UNDOABLE */
						AEGP_StreamRefH			streamH,				/* >> */
						AEGP_KeyframeIndex		key_index,				/* >> */
						const AEGP_StreamValue	*valueP);				/* >> */	// not adopted

	SPAPI A_Err (*AEGP_GetStreamValueDimensionality)(
						AEGP_StreamRefH			streamH,				/* >> */
						short					*value_dimPS);			/* << */
					
	SPAPI A_Err (*AEGP_GetStreamTemporalDimensionality)(
						AEGP_StreamRefH			streamH,				/* >> */
						short					*temporal_dimPS);		/* << */
					
	SPAPI A_Err (*AEGP_GetNewKeyframeSpatialTangents)(					// dispose using AEGP_DisposeStreamValue()
						AEGP_PluginID			aegp_plugin_id,			/* >> */
						AEGP_StreamRefH			streamH,				/* >> */
						AEGP_KeyframeIndex		key_index,				/* >> */
						AEGP_StreamValue		*in_tanP0,				/* << */
						AEGP_StreamValue		*out_tanP0);			/* << */
				
	SPAPI A_Err (*AEGP_SetKeyframeSpatialTangents)(						/* UNDOABLE */
						AEGP_StreamRefH			streamH,				/* >> */
						AEGP_KeyframeIndex		key_index,				/* >> */
						const AEGP_StreamValue	*in_tanP0,				/* >> */	// not adopted
						const AEGP_StreamValue	*out_tanP0);			/* >> */	// not adopted
				
	SPAPI A_Err (*AEGP_GetKeyframeTemporalEase)(
						AEGP_StreamRefH			streamH,				/* >> */
						AEGP_KeyframeIndex		key_index,				/* >> */
						A_long					dimensionL,				/* >> */	// ranges from 0..TemporalDimensionality-1
						AEGP_KeyframeEase		*in_easeP0,				/* << */
						AEGP_KeyframeEase		*out_easeP0);			/* << */
	
	SPAPI A_Err (*AEGP_SetKeyframeTemporalEase)(						/* UNDOABLE */
						AEGP_StreamRefH			streamH,				/* >> */
						AEGP_KeyframeIndex		key_index,				/* >> */
						A_long					dimensionL,				/* >> */	// ranges from 0..TemporalDimensionality-1
						const AEGP_KeyframeEase	*in_easeP0,				/* >> */	// not adopted
						const AEGP_KeyframeEase	*out_easeP0);			/* >> */	// not adopted
	
	SPAPI A_Err (*AEGP_GetKeyframeFlags)(
						AEGP_StreamRefH			streamH,				/* >> */
						AEGP_KeyframeIndex		key_index,				/* >> */
						AEGP_KeyframeFlags		*flagsP);				/* << */

	SPAPI A_Err (*AEGP_SetKeyframeFlag)(								/* UNDOABLE */
						AEGP_StreamRefH			streamH,				/* >> */
						AEGP_KeyframeIndex		key_index,				/* >> */
						AEGP_KeyframeFlags		flag,					/* >> */	// set one at a time
						A_Boolean				true_falseB);			/* >> */

	SPAPI A_Err (*AEGP_GetKeyframeInterpolation)(
						AEGP_StreamRefH					streamH,		/* >> */
						AEGP_KeyframeIndex				key_index,		/* >> */
						AEGP_KeyframeInterpolationType	*in_interpP0,	/* << */
						AEGP_KeyframeInterpolationType	*out_interpP0);	/* << */

	SPAPI A_Err (*AEGP_SetKeyframeInterpolation)(						/* UNDOABLE */
						AEGP_StreamRefH					streamH,		/* >> */
						AEGP_KeyframeIndex				key_index,		/* >> */						
						AEGP_KeyframeInterpolationType	in_interp,		/* >> */
						AEGP_KeyframeInterpolationType	out_interp);	/* >> */

} AEGP_KeyframeSuite1;


/* -------------------------------------------------------------------- */


typedef A_long	AEGP_InstalledEffectKey;
#define AEGP_InstalledEffectKey_NONE	0

enum {
	AEGP_EffectFlags_NONE			=0,
	AEGP_EffectFlags_ACTIVE			=0x01L << 0,
	AEGP_EffectFlags_AUDIO_ONLY		=0x01L << 1,
	AEGP_EffectFlags_AUDIO_TOO		=0x01L << 2
};
typedef A_long	AEGP_EffectFlags;

typedef A_long AEGP_EffectIndex;


#define kAEGPEffectSuite				"AEGP Effect Suite"
#define kAEGPEffectSuiteVersion1		1

typedef struct AEGP_EffectSuite1 {

	SPAPI A_Err	(*AEGP_GetLayerNumEffects)(
						AEGP_LayerH			layerH,					/* >> */
						A_long				*num_effectsPL);		/* << */
	
	SPAPI A_Err	(*AEGP_GetLayerEffectByIndex)(
						AEGP_PluginID		aegp_plugin_id,			/* >> */
						AEGP_LayerH			layerH,					/* >> */
						AEGP_EffectIndex	layer_effect_indexL,			/* >> */
						AEGP_EffectRefH		*effectPH);				/* <<  MUST dispose with DisposeEffect*/ 

	SPAPI A_Err (*AEGP_GetInstalledKeyFromLayerEffect)(
						AEGP_EffectRefH		effect_refH,					/* >> */
						AEGP_InstalledEffectKey	*installed_effect_keyP);	/* << */

	SPAPI A_Err (*AEGP_GetEffectParamUnionByIndex)(
						AEGP_PluginID		aegp_plugin_id,			/* >> */
						AEGP_EffectRefH		effect_refH,			/* >> */
						PF_ParamIndex		param_index,			/* >> valid in range [1..AEGP_GetEffectNumParamStreams]*/
						PF_ParamType		*param_typeP,			/* << */
						PF_ParamDefUnion	*uP0);					/* << DO NOT USE THE VALUE FROM THIS PARAMDEF! */

	SPAPI A_Err (*AEGP_GetEffectFlags)( 
						AEGP_EffectRefH		effect_refH,			/* >> */
						AEGP_EffectFlags	*effect_flagsP);		/* << */

	SPAPI A_Err (*AEGP_SetEffectFlags)(
						AEGP_EffectRefH		effect_refH,			/* >> */
						AEGP_EffectFlags	effect_flags_set_mask,	/* >> */
						AEGP_EffectFlags	effect_flags);			/* >> */

	SPAPI A_Err	(*AEGP_ReorderEffect)(								/* UNDOABLE */
						AEGP_EffectRefH 	effect_refH,			/* >> */	
						A_long 				effect_indexL);			/* >> */

	SPAPI A_Err (*AEGP_EffectCallGeneric)(
						AEGP_PluginID		aegp_plugin_id,			/* >> */
						AEGP_EffectRefH		effect_refH,			/* >> */
						const A_Time		*timePT,				/* >> */
						void				*effect_extraPV);		/* <> */

	SPAPI A_Err (*AEGP_DisposeEffect)(
						AEGP_EffectRefH	effect_refH );				/* >> */
	
	SPAPI A_Err (*AEGP_ApplyEffect)(
						AEGP_PluginID		aegp_plugin_id,			/* >> */
						AEGP_LayerH			layerH,					/* >> */
						AEGP_InstalledEffectKey	installed_effect_key,/* >> */
						AEGP_EffectRefH		*effect_refPH);	/* <<  MUST BE DISPOSED with AEGP_DisposeEffect */

	SPAPI A_Err	(*AEGP_DeleteLayerEffect)(
						AEGP_EffectRefH effect_refH);					/* >>  undoable */

	SPAPI A_Err (*AEGP_GetNumInstalledEffects)(
									A_long *num_installed_effectsPL);	/* << */
	
	// pass in AEGP_InstalledEffectKey_NONE for installed_effect_key to get first effect key
	SPAPI A_Err (*AEGP_GetNextInstalledEffect)(
									AEGP_InstalledEffectKey	installed_effect_key,  /* >> */
									AEGP_InstalledEffectKey	*next_effectPH);	/* << */
													
	SPAPI A_Err	(*AEGP_GetEffectName)(
						AEGP_InstalledEffectKey		installed_effect_key,	/* >> */
						A_char						*nameZ);				/* << space for A_char[AEGP_MAX_EFFECT_NAME_SIZE] */

	SPAPI A_Err	(*AEGP_GetEffectMatchName)(
						AEGP_InstalledEffectKey		installed_effect_key,	/* >> */
						A_char						*match_nameZ);			/* << space for A_char[AEGP_MAX_EFFECT_MATCH_NAME_SIZE] */

	SPAPI A_Err	(*AEGP_GetEffectCategory)(
						AEGP_InstalledEffectKey		installed_effect_key,	/* >> */
						A_char						*categoryZ);			/* << space for A_char[AEGP_MAX_EFFECT_CATEGORY_NAME_SIZE] */


} AEGP_EffectSuite1;


/* -------------------------------------------------------------------- */

typedef A_long	AEGP_MaskIndex;

#define kAEGPMaskSuite					"AEGP Layer Mask Suite"
#define kAEGPMaskSuiteVersion1			2

typedef struct AEGP_MaskSuite1 {

	SPAPI A_Err	(*AEGP_GetLayerNumMasks)(
						AEGP_LayerH				aegp_layerH,		/* >> */
						A_long					*num_masksPL);		/* << */

	SPAPI A_Err	(*AEGP_GetLayerMaskByIndex)(
						AEGP_LayerH				aegp_layerH,		/* >> */
						AEGP_MaskIndex			mask_indexL,		/* >> */
						AEGP_MaskRefH			*maskPH);			/* << must be disposed by calling AEGP_DisposeMask() */

	SPAPI A_Err	(*AEGP_DisposeMask)(
						AEGP_MaskRefH			mask_refH);				/* >> */

	SPAPI A_Err	(*AEGP_GetMaskInvert)(
						AEGP_MaskRefH			mask_refH,				/* >> */
						A_Boolean				*invertPB);			/* << */

	SPAPI A_Err	(*AEGP_GetMaskMode)(
						AEGP_MaskRefH			mask_refH,				/* >> */
						PF_MaskMode				*modeP);			/* << */

	SPAPI A_Err	(*AEGP_GetMaskName)(
						AEGP_MaskRefH			mask_refH,				/* >> */
						A_char					*nameZ);			/* << space for A_char[AEGP_MAX_MASK_NAME_SIZE] */

	SPAPI A_Err	(*AEGP_GetMaskID)(
						AEGP_MaskRefH			mask_refH,				/* >> */
						AEGP_MaskIDVal			*id_valP);			/* << */

	SPAPI A_Err (*AEGP_CreateNewMask)(			//undoable
						AEGP_LayerH				layerH,			/* >> */
						AEGP_MaskRefH			*mask_refPH,	/* << */
						A_long					*mask_indexPL0);/* << */

	SPAPI A_Err (*AEGP_DeleteMaskFromLayer)(	//undoable
						AEGP_MaskRefH			mask_refH);		/* >> still need to Dispose MaskRefH */

} AEGP_MaskSuite1;


/* -------------------------------------------------------------------- */


typedef PF_PathVertex		AEGP_MaskVertex;

#define kAEGPMaskOutlineSuite				"AEGP Mask Outline Suite"
#define kAEGPMaskOutlineSuiteVersion1		2

typedef A_long	AEGP_VertexIndex;
#define AEGP_VertexIndex_END 10922


typedef struct AEGP_MaskOutlineSuite1 {

	SPAPI A_Err	(*AEGP_IsMaskOutlineOpen)(
						AEGP_MaskOutlineValH		mask_outlineH,		/* >> */
						A_Boolean					*openPB);			/* << */

	// N segments means there are segments [0..N-1]; segment J is defined by vertex J & J+1
	SPAPI A_Err	(*AEGP_GetMaskOutlineNumSegments)(
						AEGP_MaskOutlineValH		mask_outlineH,		/* >> */
						A_long						*num_segmentsPL);	/* << */
	
	// which_pointL range: [0..num_segments]; for closed masks vertex[0] == vertex[num_segments]
	SPAPI A_Err	(*AEGP_GetMaskOutlineVertexInfo)(
						AEGP_MaskOutlineValH		mask_outlineH,		/* >> */
						AEGP_VertexIndex			which_pointL,		/* >> */
						AEGP_MaskVertex				*vertexP);			/* << tangents are relative to position */

	// Setting vertex 0 is special. Its in tangent will actually set the out tangent
	// of the last vertex in the outline. 
	SPAPI A_Err	(*AEGP_SetMaskOutlineVertexInfo)(
						AEGP_MaskOutlineValH		mask_outlineH,		/* >> */
						AEGP_VertexIndex			which_pointL,		/* >> must already exists (use Create) */
						const AEGP_MaskVertex				*vertexP);	/* >> tangents are relative to position */

	SPAPI A_Err (*AEGP_CreateVertex)(
						AEGP_MaskOutlineValH	mask_outlineH,			/* >> */
						AEGP_VertexIndex		insert_position);		/* >> will insert at this index. moving other verticies index++*/

	SPAPI A_Err (*AEGP_DeleteVertex)(
						AEGP_MaskOutlineValH	mask_outlineH,			/* >> */
						AEGP_VertexIndex		index);					/* >> */
						
} AEGP_MaskOutlineSuite1;


/* -------------------------------------------------------------------- */


typedef	FIEL_Label AEGP_InterlaceLabel;

enum {
	AEGP_AlphaPremul 		= 0x1,		// otherwise straight
	AEGP_AlphaInverted		= 0x2,		//  255 = transparent
	AEGP_AlphaIgnore		= 0x4
};
typedef A_u_long	AEGP_AlphaFlags;

typedef struct {
	AEGP_AlphaFlags	flags;
	A_u_char		redCu;		// color that was matted (if premul)
	A_u_char		greenCu;
	A_u_char		blueCu;
} AEGP_AlphaLabel;

enum {
	AEGP_PulldownPhase_NO_PULLDOWN = 0,
	AEGP_PulldownPhase_WSSWW = 1,
	AEGP_PulldownPhase_SSWWW,
	AEGP_PulldownPhase_SWWWS,
	AEGP_PulldownPhase_WWWSS,
	AEGP_PulldownPhase_WWSSW
};
typedef A_long AEGP_PulldownPhase;

typedef struct {
	A_long			loops;
	A_long			reserved;		// set to 0; reserved for future use (palindrome, etc.)
} AEGP_LoopBehavior;

typedef struct {
	AEGP_InterlaceLabel		il;
	AEGP_AlphaLabel			al;
	AEGP_PulldownPhase		pd;
	AEGP_LoopBehavior		loop;
	A_Ratio					pix_aspect_ratio;
	A_FpLong				native_fpsF;
	A_FpLong				conform_fpsF;
	A_long					depthL;
	A_Boolean				motion_dB;
} AEGP_FootageInterp;



#define AEGP_FOOTAGE_MAIN_FILE_INDEX		0

#define kAEGPFootageSuite				"AEGP Footage Suite"
#define kAEGPFootageSuiteVersion1		3

typedef struct AEGP_FootageSuite1 {

	SPAPI A_Err	(*AEGP_GetMainFootageFromItem)(						// error if item isn't AEGP_ItemType_FOOTAGE!
						AEGP_ItemH 			itemH,					/* >> */
						AEGP_FootageH		*footagePH);			/* << */

	SPAPI A_Err	(*AEGP_GetProxyFootageFromItem)(					// error if has_proxy is false! (note, item could still be a comp)
						AEGP_ItemH 			itemH,					/* >> */
						AEGP_FootageH		*proxy_footagePH);		/* << */

	SPAPI A_Err	(*AEGP_GetFootageNumFiles)(
						AEGP_FootageH		footageH,				/* >> */
						A_long				*num_main_filesPL0,		/* << */
						A_long				*files_per_framePL0);	/* << includes main file. e.g. 1 for no aux data */

	SPAPI A_Err	(*AEGP_GetFootagePath)(
						AEGP_FootageH		footageH,				/* >> */
						A_long				frame_numL,				/* >> range is 0 to num_main_files */
						A_long			 	file_indexL,			/* >> AEGP_FOOTAGE_MAIN_FILE_INDEX is main file */
						A_char 				*file_pathZ);			/* << space for A_char[AEGP_MAX_PATH_SIZE] */
	
	SPAPI A_Err	(*AEGP_NewFootage)(
						const AEGP_PluginID			aegp_plugin_id,		/* >> */
						const A_char				*file_pathZ,		/* >> platform-specific delimiters */
						A_Boolean					make_sequenceB,		/* >> use all files of this type in same folder */
  						void*						reserved,			/* >> pass NULL */
						AEGP_FootageH				*footagePH);		/* << caller owns until disposed or added to project */
	
	SPAPI A_Err	(*AEGP_AddFootageToProject)(						/* UNDOABLE */
						AEGP_FootageH		footageH,				/* >> will be adopted by project, may not be added more than once */
						AEGP_ItemH 			folderH,				/* >> add to this folder */	
						AEGP_ItemH			*added_itemPH0);		/* << */

	SPAPI A_Err	(*AEGP_SetItemProxyFootage)(						/* UNDOABLE */
						AEGP_FootageH		footageH,				/* >> will be adopted by project, may not be set more than once */
						AEGP_ItemH 			itemH);					/* >> set for this item */	

	SPAPI A_Err	(*AEGP_ReplaceItemMainFootage)(						/* UNDOABLE */
						AEGP_FootageH		footageH,				/* >> will be adopted by project, may not be set more than once */
						AEGP_ItemH 			itemH);					/* >> replace main footage for this item */	

	SPAPI A_Err	(*AEGP_DisposeFootage)(
						AEGP_FootageH		footageH);				/* >> do not dipose footage that is owned or has been adopted by project */

	SPAPI A_Err (*AEGP_GetFootageInterpretation)(					
						const AEGP_FootageH		footageH,				/* >> */
						AEGP_FootageInterp		*interpP);				/* << */

	SPAPI A_Err (*AEGP_SetFootageInterpretation)(					/* UNDOABLE */
						const AEGP_FootageH			footageH,		/* >> */
						const AEGP_FootageInterp	*interpP);		/* >> */

	SPAPI A_Err (*AEGP_NewPlaceholderFootage)(						// Undoable
						AEGP_PluginID		plugin_id,				// >>
						const A_char		*nameZ,					// >>
						A_long				width,					// >> 
						A_long				height,					// >> 
						const A_Time		*durationPT,			// >>
						AEGP_FootageH		*footagePH);			// <<

	
	SPAPI A_Err (*AEGP_GetFootageSoundDataFormat)(
					AEGP_FootageH			footageH,				// >>
					AEGP_SoundDataFormat*	sound_formatP);			// << 


} AEGP_FootageSuite1;

/* -------------------------------------------------------------------- */


typedef A_long	AEGP_Command;

#define AEGP_Command_ALL		0

typedef enum {
	AEGP_WindType_NONE,
	AEGP_WindType_PROJECT,
	AEGP_WindType_COMP,
	AEGP_WindType_TIME_LAYOUT,
	AEGP_WindType_LAYER,
	AEGP_WindType_FOOTAGE,
	AEGP_WindType_RENDER_QUEUE,
	AEGP_WindType_QT,
	AEGP_WindType_DIALOG,
	AEGP_WindType_FLOWCHART,
	AEGP_WindType_EFFECT,
	AEGP_WindType_OTHER
} AEGP_WindowType;


enum {
	AEGP_HP_BeforeAE 		= 0x1,		// call hook before AE handles event (if AE handles)
	AEGP_HP_AfterAE			= 0x2		// call hook after AE handles event (if AE handles)
};
typedef A_u_long	AEGP_HookPriority;


typedef A_Err		(*AEGP_CommandHook)(
						AEGP_GlobalRefcon		plugin_refconP,			/* >> */
						AEGP_CommandRefcon	refconP,				/* >> */
						AEGP_Command		command,				/* >> */
						AEGP_HookPriority	hook_priority,			/* >> currently always AEGP_HP_BeforeAE */
						A_Boolean			already_handledB,		/* >> */
						A_Boolean			*handledPB);			/* << whether you handled */

typedef A_Err		(*AEGP_UpdateMenuHook)(
						AEGP_GlobalRefcon	plugin_refconP,			/* >> */
						AEGP_UpdateMenuRefcon	refconP,			/* >> */
						AEGP_WindowType		active_window);			/* >> */

typedef A_Err		(*AEGP_DeathHook)(
						AEGP_GlobalRefcon	plugin_refconP,			/* >> */
						AEGP_DeathRefcon	refconP);				/* >> */

typedef A_Err		(*AEGP_VersionHook)(							// As of 5.0, not called
						AEGP_GlobalRefcon	plugin_refconP,			/* >> */
						AEGP_VersionRefcon	refconP,				/* >> */
						A_u_long 			*pf_versionPLu);		/* << use PF_VERSION() macro to build and PF_Version_XXX() macros to access */

					// one line description. when displaying, AE will prepend name and version information.
					// this will be used to display a list of about info for all plugins

typedef A_Err		(*AEGP_AboutStringHook)(						// As of 5.0, not called
						AEGP_GlobalRefcon	plugin_refconP,			/* >> */
						AEGP_AboutStringRefcon	refconP,			/* >> */
						A_char				*aboutZ);				/* << space for A_char[AEGP_MAX_ABOUT_STRING_SIZE] */

					// bring up a dialog and tell us about yourself
typedef A_Err		(*AEGP_AboutHook)(								// not called
						AEGP_GlobalRefcon	plugin_refconP,			/* >> */
						AEGP_AboutRefcon	refconP);				/* >> */

typedef A_Err		(*AEGP_IdleHook)(							
						AEGP_GlobalRefcon	plugin_refconP,			/* >> */
						AEGP_IdleRefcon		refconP,				/* >> */
						long *max_sleepPL);							/* <> in 1/60 of a second*/

#define kAEGPRegisterSuite				"AEGP Register Suite"
#define kAEGPRegisterSuiteVersion1		1

typedef struct AEGP_RegisterSuite1 {

	SPAPI A_Err	(*AEGP_RegisterCommandHook)(
						AEGP_PluginID		aegp_plugin_id,			/* >> */
						AEGP_HookPriority	hook_priority,			/* >> */
						AEGP_Command		command,				/* >> use AEGP_Command_ALL to get all commands */
						AEGP_CommandHook	command_hook_func,		/* >> */
						AEGP_CommandRefcon	refconP);				/* >> */

	// this will be called anytime any menu is about to be drawn. it isn't specific to a menu so you
	// must enable all appropriate menu items when this hook is called.
	
	SPAPI A_Err	(*AEGP_RegisterUpdateMenuHook)(
						AEGP_PluginID		plugin_id,					/* >> */
						AEGP_UpdateMenuHook	update_menu_hook_func,		/* >> */
						AEGP_UpdateMenuRefcon				refconP);	/* >> */

	SPAPI A_Err	(*AEGP_RegisterDeathHook)(
						AEGP_PluginID		aegp_plugin_id,			/* >> */
						AEGP_DeathHook		death_hook_func,		/* >> */
						AEGP_DeathRefcon	refconP);				/* >> */

	SPAPI A_Err	(*AEGP_RegisterVersionHook)(
						AEGP_PluginID		aegp_plugin_id,			/* >> */
						AEGP_VersionHook	version_hook_func,		/* >> */
						AEGP_VersionRefcon	refconP);				/* >> */

	SPAPI A_Err	(*AEGP_RegisterAboutStringHook)(
						AEGP_PluginID			aegp_plugin_id,				/* >> */
						AEGP_AboutStringHook	about_string_hook_func,		/* >> */
						AEGP_AboutStringRefcon		refconP);				/* >> */

	SPAPI A_Err	(*AEGP_RegisterAboutHook)(
						AEGP_PluginID		aegp_plugin_id,					/* >> */
						AEGP_AboutHook		about_hook_func,				/* >> */
						AEGP_AboutRefcon	refconP);						/* >> */

	SPAPI A_Err	 (*AEGP_RegisterArtisan) (	
						A_Version					api_version,			/* >> */
						A_Version					artisan_version,		/* >> */
						AEGP_PluginID				aegp_plugin_id,			/* >> */
						void						*aegp_refconPV,
						const A_char				*match_nameZ,			/* >> */
						const A_char				*artisan_nameZ,			/* >> */
						PR_ArtisanEntryPoints		*entry_funcs);	

	SPAPI A_Err	 (*AEGP_RegisterIO) (
						AEGP_PluginID				aegp_plugin_id,			/* >> */
						AEGP_IORefcon				aegp_refconP,			/* >> */
						const AEIO_ModuleInfo		*io_infoP,				/* >> */
						const AEIO_FunctionBlock	*aeio_fcn_blockP);		/* >> */

	SPAPI A_Err	(*AEGP_RegisterIdleHook)(
						AEGP_PluginID		aegp_plugin_id,					/* >> */
						AEGP_IdleHook		idle_hook_func,				/* >> */
						AEGP_IdleRefcon		refconP);						/* >> */

} AEGP_RegisterSuite1;


/* -------------------------------------------------------------------- */


typedef enum {
	AEGP_Menu_NONE,
	
	AEGP_Menu_APPLE,
	AEGP_Menu_FILE,
	AEGP_Menu_EDIT,
	AEGP_Menu_COMPOSITION,
	AEGP_Menu_LAYER,
	AEGP_Menu_EFFECT,
	AEGP_Menu_WINDOW,
	
	AEGP_Menu_FLOATERS,
	AEGP_Menu_KF_ASSIST,
	AEGP_Menu_IMPORT,
	AEGP_Menu_SAVE_FRAME_AS,
	AEGP_Menu_PREFS,
	AEGP_Menu_EXPORT,
	AEGP_Menu_ANIMATION
} AEGP_MenuID;


#define AEGP_MENU_INSERT_SORTED			(-2)
#define AEGP_MENU_INSERT_AT_BOTTOM		(-1)
#define AEGP_MENU_INSERT_AT_TOP			0


#define kAEGPCommandSuite				"AEGP Command Suite"
#define kAEGPCommandSuiteVersion1		1

typedef struct AEGP_CommandSuite1 {

	SPAPI A_Err	(*AEGP_GetUniqueCommand)(
						AEGP_Command		*unique_commandP);		/* << */

	SPAPI A_Err	(*AEGP_InsertMenuCommand)(
						AEGP_Command		command,				/* >> */
						const A_char 		*nameZ,					/* >> */
						AEGP_MenuID 		menu_id,				/* >> */
						A_long 				after_itemL);			/* >> */
			
	SPAPI A_Err	(*AEGP_RemoveMenuCommand)(
						AEGP_Command		command);				/* >> */

	SPAPI A_Err	(*AEGP_SetMenuCommandName)(
						AEGP_Command		command,				/* >> */
						const A_char		*nameZ);				/* >> */

	SPAPI A_Err	(*AEGP_EnableCommand)(
						AEGP_Command		command);				/* >> */

	SPAPI A_Err	(*AEGP_DisableCommand)(
						AEGP_Command		command);				/* >> */

	SPAPI A_Err	(*AEGP_CheckMarkMenuCommand)(
						AEGP_Command		command,				/* >> */
						A_Boolean			checkB);				/* >> */

	SPAPI A_Err	(*AEGP_DoCommand)(
						AEGP_Command		command);				/* >> */

} AEGP_CommandSuite1;

/* -------------------------------------------------------------------- */


typedef struct {
	A_long				reservedAL[12];
} AEGP_ErrReportState;

#define kAEGPUtilitySuite				"AEGP Utility Suite"
#define kAEGPUtilitySuiteVersion1		3

typedef struct AEGP_UtilitySuite1 {

	SPAPI A_Err	(*AEGP_ReportInfo)(										// displays dialog with name of plugin followed by info string
						AEGP_PluginID			aegp_plugin_id,			/* >> */
						const A_char			*info_stringZ);			/* >> */


	SPAPI A_Err	(*AEGP_GetDriverPluginInitFuncVersion)(
						A_short					*major_versionPS,		/* << */
						A_short					*minor_versionPS);		/* << */

	SPAPI A_Err	(*AEGP_GetDriverImplementationVersion)(
						A_short					*major_versionPS,		/* << */
						A_short					*minor_versionPS);		/* << */

	SPAPI A_Err	(*AEGP_StartQuietErrors)(
						AEGP_ErrReportState		*err_stateP);			/* << */

	SPAPI A_Err	(*AEGP_EndQuietErrors)(
						A_Boolean				report_quieted_errorsB,	/* >> currently reports last quieted error */
						AEGP_ErrReportState		*err_stateP);			/* >> */

	SPAPI A_Err	(*AEGP_StartUndoGroup)(									// MUST be balanced with call to AEGP_EndUndoGroup()
						const A_char			*undo_nameZ);			/* >> */

	SPAPI A_Err	(*AEGP_EndUndoGroup)();

	SPAPI A_Err (*AEGP_RegisterWithAEGP)(
						AEGP_GlobalRefcon global_refcon,				/* >> global refcon passed in command handlers */
						const A_char			*plugin_nameZ,			/* >> name of this plugin. AEGP_MAX_PLUGIN_NAME_SIZE */
						AEGP_PluginID			*plugin_id);			/* << id for plugin to use in other AEGP calls */

	SPAPI A_Err (*AEGP_GetMainHWND)(
						void					*main_hwnd);

	} AEGP_UtilitySuite1;



/* -------------------------------------------------------------------- */
/*
	Render Queue Suite
	Used to add, remove, and modify items in the reder queue.

  */

#define kAEGPRenderQueueSuite			"AEGP Render Queue Suite"
#define kAEGPRenderQueueSuiteVersion1	1

enum {
	AEGP_RenderQueueState_STOPPED,
	AEGP_RenderQueueState_PAUSED,
	AEGP_RenderQueueState_RENDERING
};

typedef A_u_long AEGP_RenderQueueState;

typedef struct AEGP_RenderQueueSuite1 {
	SPAPI A_Err (*AEGP_AddCompToRenderQueue)(
					AEGP_CompH	comp,		/* >> */
					const A_char*	pathZ);

	// not legal to go from STOPPED to PAUSED.
	SPAPI A_Err (*AEGP_SetRenderQueueState)(
					AEGP_RenderQueueState	state);	/* >> */

	SPAPI A_Err (*AEGP_GetRenderQueueState)(
					AEGP_RenderQueueState	*stateP); /* << */

} AEGP_RenderQueueSuite1;

/* -------------------------------------------------------------------- */


/**
 ** Canvas Suite 
 ** Used by artisans to render layers
 **/

#define kAEGPCanvasSuite				"AEGP Canvas Suite"
#define kAEGPCanvasSuiteVersion1		4

enum {
	AEGP_RenderHints_NONE = 0,
	AEGP_RenderHints_IGNORE_EXTENTS = 0x1
};
typedef A_u_long AEGP_RenderHints;


typedef struct AEGP_CanvasSuite1 {

	SPAPI A_Err	(*AEGP_GetCompToRender)(						
						PR_RenderContextH 		render_contextH,   	/* >> */	
						AEGP_CompH				*compPH);			/* << */

	SPAPI A_Err	(*AEGP_GetNumLayersToRender)(
				const	PR_RenderContextH	render_contextH,
						A_long				*num_to_renderPL);


	SPAPI A_Err	(*AEGP_GetNthLayerContextToRender)(
				const	PR_RenderContextH			render_contextH,	/* >> */
						A_long						n,					/* >> */
						AEGP_RenderLayerContextH	*layer_contextPH);	/* << */

	SPAPI A_Err	(*AEGP_GetLayerFromLayerContext)(
				const	PR_RenderContextH			render_contextH,
						AEGP_RenderLayerContextH	layer_contextH,	
						AEGP_LayerH					*layerPH);

	SPAPI A_Err	(*AEGP_GetCompRenderTime)(
						PR_RenderContextH	render_contextH,	/* >> */
						A_Time				*time,				/* << */
						A_Time				*time_step);

	SPAPI A_Err	(*AEGP_GetCompDestinationBuffer)(
						PR_RenderContextH			render_contextH,	/* <> */
						AEGP_CompH					compH,				/* >> */	
						PF_World					*dst);				/* <> */

	SPAPI A_Err	(*AEGP_GetROI)(
						PR_RenderContextH			render_contextH,	/* <> */	
						A_Rect						*roiPR);			/* << */

	/// for rendering track mattes 
	SPAPI A_Err	(*AEGP_RenderLayer)(
						PR_RenderContextH			render_contextH,	/* <> */
						AEGP_LayerH					layerH,				/* >> */
						AEGP_RenderHints			render_hints,		/* >> */
						PF_World					*render_bufferP);	/* >> */

	// for rendering the texture map of a layer
	SPAPI A_Err	(*AEGP_RenderTexture)(
						PR_RenderContextH			render_contextH,	/* <> */
						AEGP_RenderLayerContextH	layer_contextH,		/* >> */
						AEGP_RenderHints			render_hints,		/* >> */
						A_FloatPoint				*suggested_scaleP0,	/* >> */
						A_FloatRect					*suggested_src_rectP0, /* >> */
						A_Matrix3					*src_matrixP0,		/* << */
						PF_World					*dst);				/* <> */


	SPAPI A_Err	(*AEGP_DisposeTexture)(
						PR_RenderContextH			render_contextH,	/* <> */
						AEGP_RenderLayerContextH	layer_contextH,		/* >> */
						PF_World					*dst0);				/* <> */

	SPAPI A_Err	(*AEGP_GetFieldRender)(						
						PR_RenderContextH 			render_contextH,   	/* >> */	
						PF_Field					*field);			/* << */

	// not thread safe on MacOS
	// only call when thread ID = 0
	SPAPI A_Err	(*AEGP_ReportArtisanProgress)(						
					PR_RenderContextH 				render_contextH,   	/* >> */	
					A_long							countL,				/* >> */
					A_long							totalL);			/* >> */

	SPAPI A_Err	(*AEGP_GetRenderDownsampleFactor)(
						PR_RenderContextH			render_contextH,	/* >> */
						AEGP_DownsampleFactor		*dsfP);				/* << */
	
	SPAPI A_Err	(*AEGP_IsBlankCanvas)(
						PR_RenderContextH			render_contextH,	/* >> */
						A_Boolean					*is_blankPB);		/* << */

	SPAPI A_Err (*AEGP_GetRenderLayerToWorldXform)(
					PR_RenderContextH				render_contextH,	/* >> */
					AEGP_RenderLayerContextH		layer_contextH,		/* >> */
					const A_Time					*comp_timeP,		/* >> */
					A_Matrix4						*transform);			/* << */

	SPAPI A_Err (*AEGP_GetRenderLayerBounds)(
					PR_RenderContextH				render_contextH,	/* >> */
					AEGP_RenderLayerContextH		layer_contextH,		/* >> */
					const A_Time					*comp_timeP,		/* >> */
					A_Rect							*boundsP);			/* << */

	SPAPI A_Err (*AEGP_GetRenderOpacity)(
					PR_RenderContextH				render_contextH,	/* >> */
					AEGP_RenderLayerContextH		layer_contextH,		/* >> */
					const A_Time					*comp_timePT,		/* >> */
					A_FpLong						*opacityPF);		/* << */

	SPAPI A_Err (*AEGP_IsRenderLayerActive)(
					PR_RenderContextH				render_contextH,	/* >> */
					AEGP_RenderLayerContextH		layer_contextH,		/* >> */
					const A_Time					*comp_timePT,		/* >> */
					A_Boolean						*activePB);			/* << */

	// set the layer index. If total > 0, set it too. 
	SPAPI A_Err	(*AEGP_SetArtisanLayerProgress)(						
					PR_RenderContextH 				render_contextH,   	/* >> */	
					A_long							countL,				/* >> */
					A_long							num_layersL);
	
} AEGP_CanvasSuite1;


/* -------------------------------------------------------------------- */


/**
 ** Artisan utility suite 
 ** 
 **/
#define kAEGPArtisanUtilSuite				"AEGP Artisan Util Suite"
#define kAEGPArtisanUtilSuiteVersion1		1


typedef struct AEGP_ArtisanUtilSuite1 {


	SPAPI A_Err	(*AEGP_GetGlobalContextFromInstanceContext)(
		const PR_InstanceContextH		instance_contextH,
			  PR_GlobalContextH			*global_contextPH);


	SPAPI A_Err	(*AEGP_GetInstanceContextFromRenderContext)(
		const PR_RenderContextH			render_contextH,
			  PR_InstanceContextH		*instance_contextPH);



	SPAPI A_Err	(*AEGP_GetInstanceContextFromQueryContext)(
		const PR_QueryContextH			query_contextH,
			  PR_InstanceContextH		*instance_contextPH);


	SPAPI A_Err	(*AEGP_GetGlobalData)(
		const PR_GlobalContextH			global_contextH,
			  PR_GlobalDataH			*global_dataPH);


	SPAPI A_Err	(*AEGP_GetInstanceData)(
		const PR_InstanceContextH		instance_contextH,
			  PR_InstanceDataH			*instance_dataPH);

	SPAPI A_Err	(*AEGP_GetRenderData)(
		const PR_RenderContextH			render_contextH,
			  PR_RenderDataH			*render_dataPH);

} AEGP_ArtisanUtilSuite1;



/* -------------------------------------------------------------------- */


/**
 ** Camera Suite 
 ** 
 **/
#define kAEGPCameraSuite				"AEGP Camera Suite"
#define kAEGPCameraSuiteVersion1		1



typedef struct AEGP_CameraSuite1 {

		SPAPI A_Err	(*AEGP_GetCamera)(						
						PR_RenderContextH 		render_contextH,   	/* >> */	
						const A_Time			*comp_timeP,		/* >> */
						AEGP_LayerH				*camera_layerPH);	/* << */

		SPAPI A_Err	(*AEGP_GetCameraType)(						
						AEGP_LayerH				camera_layerH,		/* >> */
						AEGP_CameraType			*camera_typeP);		/* << */


		SPAPI A_Err (*AEGP_GetDefaultCameraDistanceToImagePlane)(
					AEGP_CompH				compH,					/* >> */
					A_FpLong				*dist_to_planePF);		/* << */

} AEGP_CameraSuite1;



/* -------------------------------------------------------------------- */


#define kAEGPLightSuite				"AEGP Light Suite"
#define kAEGPLightSuiteVersion1		1



typedef struct AEGP_LightSuite1 {

	SPAPI A_Err	(*AEGP_GetLightType)(						
						AEGP_LayerH				light_layerH,		/* >> */
						AEGP_LightType			*light_typeP);		/* << */


} AEGP_LightSuite1;

/* -------------------------------------------------------------------- */


/**
 ** Query Xform suite 
 ** Called by artisans during a response to a Query For Transform call
 **/

#define kAEGPQueryXformSuite				"AEGP QueryXform Suite"
#define kAEGPQueryXformSuiteVersion1		1


/**
 ** the type of source or dst transformation wanted
 **/
enum {
	AEGP_Query_Xform_LAYER,
	AEGP_Query_Xform_WORLD,
	AEGP_Query_Xform_VIEW,
	AEGP_Query_Xform_SCREEN
};

typedef A_u_long AEGP_QueryXformType; 



typedef struct AEGP_QueryXformSuite1 {

	SPAPI A_Err	(*AEGP_QueryXformGetSrcType)(
					PR_QueryContextH		query_contextH,		/* <> */
					AEGP_QueryXformType		*src_type);			/* >> */	

	SPAPI A_Err	(*AEGP_QueryXformGetDstType)(
					PR_QueryContextH		query_contextH,		/* <> */
					AEGP_QueryXformType		*dst_type);			/* >> */
	
	SPAPI A_Err	(*AEGP_QueryXformGetLayer)(						
						PR_QueryContextH	query_contextH,		/* <> */	
						AEGP_LayerH			*layerPH);			/* << */

	SPAPI A_Err	(*AEGP_QueryXformGetComp)(						
						PR_QueryContextH	query_contextH,		/* <> */	
						AEGP_CompH			*compPH);			/* << */

	SPAPI A_Err	(*AEGP_QueryXformGetTransformTime)(						
						PR_QueryContextH	query_contextH,		/* <> */	
						A_Time				*time);				/* << */

	SPAPI A_Err	(*AEGP_QueryXformGetViewTime)(						
						PR_QueryContextH	query_contextH,		/* <> */	
						A_Time				*time);				/* << */

	SPAPI A_Err	(*AEGP_QueryXformGetCamera)(						
						PR_QueryContextH	query_contextH,		/* <> */	
						AEGP_LayerH			*camera_layerPH);	/* << */

	SPAPI A_Err	(*AEGP_QueryXformGetXform)(						
						PR_QueryContextH	query_contextH,		/* <> */	
						A_Matrix4			*xform);			/* << */

	SPAPI A_Err	(*AEGP_QueryXformSetXform)(						
						PR_QueryContextH	query_contextH,		/* <> */	
						A_Matrix4			*xform);			/* >> */

}AEGP_QueryXformSuite1;



/* -------------------------------------------------------------------- */



#define kAEGPCompositeSuite				"AEGP Composite Suite"
#define kAEGPCompositeSuiteVersion1		2


typedef A_Err (*AEGP_TransPixFunc)(
					PF_ProgPtr				effect_ref,		/* >> */ 
					long					num_pixels,		/* >> */
					const PF_CompositeMode	*c_mode,		/* >> */
					const PF_Pixel			*src,			/* >> */
					const PF_Pixel			*mask0,			/* >> */
					PF_Pixel				*dst);			/* <> */



typedef	A_Err (*AEGP_MatteFunc)(
					PF_ProgPtr			effect_ref,			/* >> */
					long				num_pix,			/* >> */
					const PF_Pixel		*src0,				/* >> */
					PF_Pixel			*dst);				/* <> */



typedef struct {
	AEGP_TransPixFunc	pix_func;
	A_Boolean			has_neutral;
	A_Boolean			requires_extension;
	A_Boolean			std_rgb_only;
	A_Boolean			combine_alpha;
	AEGP_MatteFunc		matte_func;
	AEGP_MatteFunc		dematte_func;	
} AEGP_TransferDesc;



typedef struct AEGP_CompositeSuite1 {

	SPAPI A_Err (*AEGP_ClearAlphaExceptRect)(
				A_Rect				*clipped_dest_rect, 
				PF_World			*dst);			

	SPAPI A_Err  (*AEGP_PrepTrackMatte)(
				long				num_pix,
				A_Boolean			deepB,
		const	PF_Pixel			*src_mask,
				PF_MaskFlags		mask_flags,
				PF_Pixel			*dst_mask);


	SPAPI A_Err (*AEGP_GetTransferDesc)(
		const	PF_CompositeMode		*c_mode,		/* >> */
				PF_Quality				quality,		/* >> */
				PF_ModeFlags			m_flags,		/* >> */
				AEGP_TransferDesc		*descP); 		/* << */

	
	SPAPI A_Err (*AEGP_DescriptionComposite)(
		const	AEGP_TransferDesc		*dsc,
				long					num_pix,
				A_u_char				*scratch_alpha,
		const	PF_CompositeMode		*cmode,
		const	PF_Pixel				*src_pix,
		const	PF_Pixel				*mask_pix,
				PF_Pixel				*dst_pix);

	SPAPI A_Err (*AEGP_TransferRect)(
				PF_Quality				quality,		/* >> */
				PF_ModeFlags			m_flags,		/* >> */
				PF_Field				field,			/* >> */
		const	A_Rect					*src_rec,		/* >> */
		const	PF_World				*src_world,		/* >> */
		const	PF_CompositeMode		*comp_mode,		/* >> */
		const	PF_MaskWorld			*mask_world0,	/* >> */
				long					dest_x,			/* >> */
				long					dest_y,			/* >> */
				PF_World				*dst_world);	/* <> */

	SPAPI A_Err (*AEGP_CopyBits_LQ) (
				PF_World				*src_worldP,
				A_Rect 					*src_r,		/* pass NULL for whole world */
				A_Rect					*dst_r,		/* pass NULL for whole world */
				PF_World				*dst_worldP);

	SPAPI A_Err (*AEGP_CopyBits_HQ_Straight) (
				PF_World				*src,
				A_Rect 					*src_r,		/* pass NULL for whole world */
				A_Rect					*dst_r,		/* pass NULL for whole world */
				PF_World				*dst);

	SPAPI A_Err (*AEGP_CopyBits_HQ_Premul) (
				PF_World				*src,
				A_Rect 					*src_r,		/* pass NULL for whole world */
				A_Rect					*dst_r,		/* pass NULL for whole world */
				PF_World				*dst);
				
} AEGP_CompositeSuite1;



/* -------------------------------------------------------------------- */



#define kAEGPIterateSuite				"AEGP Iterate Suite"
#define kAEGPIterateSuiteVersion1		1

typedef struct AEGP_IterateSuite1 {

	SPAPI A_Err (*AEGP_GetNumThreads)(
			A_long	*num_threadsPL);


	SPAPI A_Err (*AEGP_IterateGeneric)(
			A_long			iterationsL,						/* >> */		// can be PF_Iteratations_ONCE_PER_PROCESSOR
			void			*refconPV,							/* >> */
			A_Err			(*fn_func)(	void *refconPV,			/* >> */
									A_long thread_indexL,
									A_long i,
									A_long iterationsL));
				
} AEGP_IterateSuite1;


// Define the name of this function in your PiPL resource.
typedef A_Err (AEGP_PluginInitFuncPrototype)(
					struct SPBasicSuite		*pica_basicP,			/* >> */
					A_long				 	driver_major_versionL,	/* >> */		
					A_long					driver_minor_versionL,	/* >> */		
					const A_char		 	*file_pathZ,			/* >> platform-specific delimiters */
					const A_char		 	*res_pathZ,				/* >> platform-specific delimiters */
					AEGP_PluginID			aegp_plugin_id,			/* >> */
					AEGP_GlobalRefcon		*plugin_refconP);		/* << will be passed to all hooks! */

typedef AEGP_PluginInitFuncPrototype	*AEGP_PluginInitFunc;


/* -------------------------------------------------------------------- */

/** AEGPPFInterfaceSuite1

	These are basically wrappers for constructing various AEGP objects from
	the information available to an effect plug-in so that various other AEGP suites
	may be used.

	AEGP_GetEffectLayer				-- get AEGP_LayerH corresponding to layer that effect is applied to
	AEGP_GetNewEffectForEffect		-- get AEGP_EffectRefH corresponding to effect
	AEGP_ConvertEffectToCompTime	-- return comp time from time units passed to effect (layer time)
	AEGP_GetEffectCamera			-- get camera AEGP_LayerH which defines current 3D view
									-- NOTE : this may be null if no camera is defined

	AEGP_GetEffectCameraMatrix		-- use this to get the geometry for the camera.

	These may only be called during PF_Cmd_FRAME_SETUP, PF_Cmd_RENDER,
	and PF_Cmd_EVENT::PF_Event_DRAW
**/

#define kAEGPPFInterfaceSuite			"AEGP PF Interface Suite"
#define kAEGPPFInterfaceSuiteVersion1	1


typedef struct AEGP_PFInterfaceSuite1 {

	SPAPI A_Err	(*AEGP_GetEffectLayer)(
								PF_ProgPtr			effect_pp_ref,
								AEGP_LayerH			*layerPH);					/* << */

	SPAPI A_Err	(*AEGP_GetNewEffectForEffect)(									// must be disposed using AEGP_DisposeEffect!!!!!
								AEGP_PluginID		aegp_plugin_id,			/* >> */
								PF_ProgPtr			effect_pp_ref,
								AEGP_EffectRefH		*effect_refPH);					/* << */

	SPAPI A_Err	(*AEGP_ConvertEffectToCompTime)(
								PF_ProgPtr			effect_pp_ref,
								long				what_timeL,
								unsigned long		time_scaleLu,				// from in_data
								A_Time				*comp_timePT);				/* << */ 

	SPAPI A_Err	(*AEGP_GetEffectCamera)(
								PF_ProgPtr			effect_pp_ref,
								const A_Time		*comp_timePT,
								AEGP_LayerH			*camera_layerPH);			/* << */

	SPAPI A_Err	(*AEGP_GetEffectCameraMatrix)(
								PF_ProgPtr			effect_pp_ref,
								const A_Time		*comp_timePT,
								A_Matrix4			*camera_matrixP,
								A_FpLong			*dist_to_image_planePF,
								A_short				*image_plane_widthPL,
								A_short				*image_plane_heightPL);		/* << */
} AEGP_PFInterfaceSuite1;




#define kAEGPIOInSuite			"AEGP IO In Suite"
#define kAEGPIOInSuiteVersion1	1


typedef struct AEGP_IOInSuite1 {

	SPAPI A_Err	(*AEGP_GetInSpecOptionsHandle)(
						AEIO_InSpecH	pinH,
						void			**optionsPPV);

	SPAPI A_Err	(*AEGP_SetInSpecOptionsHandle)(
						AEIO_InSpecH	pinH,				/* >> */
						void			*optionsPV,			/* >> */
						void			**old_optionsPPV);	/* << */

	SPAPI A_Err	(*AEGP_GetInSpecFilePath)(
						AEIO_InSpecH	pinH,
						A_char			*file_nameZ);

	SPAPI A_Err	(*AEGP_GetInSpecNativeFPS)(
						AEIO_InSpecH	pinH,
						A_Fixed			*native_fpsP);

	SPAPI A_Err	(*AEGP_SetInSpecNativeFPS)(
						AEIO_InSpecH	pinH,
						A_Fixed			native_fps);

	SPAPI A_Err	(*AEGP_GetInSpecDepth)(
						AEIO_InSpecH	pinH,
						A_short			*depthPS);

	SPAPI A_Err	(*AEGP_SetInSpecDepth)(
						AEIO_InSpecH	pinH,
						A_short			depthS);

	SPAPI A_Err	(*AEGP_GetInSpecSize)(
						AEIO_InSpecH	pinH,
						A_long			*sizePL);

	SPAPI A_Err	(*AEGP_SetInSpecSize)(
						AEIO_InSpecH	pinH,
						A_long			sizeL);


	SPAPI A_Err	(*AEGP_GetInSpecInterlaceLabel)(
						AEIO_InSpecH		pinH,
						FIEL_Label	 		*interlaceP);

	SPAPI A_Err	(*AEGP_SetInSpecInterlaceLabel)(
						AEIO_InSpecH		pinH,
						const FIEL_Label	*interlaceP);

	SPAPI A_Err	(*AEGP_GetInSpecAlphaLabel)(
						AEIO_InSpecH			pinH,
						AEIO_AlphaLabel			*alphaP);

	SPAPI A_Err	(*AEGP_SetInSpecAlphaLabel)(
						AEIO_InSpecH			pinH,
						const AEIO_AlphaLabel	*alphaP);

	SPAPI A_Err	(*AEGP_GetInSpecDuration)(
						AEIO_InSpecH	pinH,
						A_Time			*durationP);

	SPAPI A_Err	(*AEGP_SetInSpecDuration)(
						AEIO_InSpecH	pinH,
						const A_Time	*durationP);

	SPAPI A_Err	(*AEGP_GetInSpecDimensions)(
						AEIO_InSpecH	pinH,
						A_long			*widthPL0,
						A_long			*heightPL0);


	SPAPI A_Err	(*AEGP_SetInSpecDimensions)(
						AEIO_InSpecH	pinH,
						A_long			widthL,
						A_long			heightL);

	SPAPI A_Err	(*AEGP_InSpecGetRationalDimensions)(
						AEIO_InSpecH				pinH,
						const AEIO_RationalScale	*rs0,
						A_long						*width0,
						A_long						*height0,
						A_Rect						*r0);


	SPAPI A_Err	(*AEGP_GetInSpecHSF)(
						AEIO_InSpecH	pinH,
						A_Ratio			*hsf);

	SPAPI A_Err	(*AEGP_SetInSpecHSF)(
						AEIO_InSpecH	pinH,
						const A_Ratio	*hsf);

	SPAPI A_Err	(*AEGP_GetInSpecSoundRate)(
						AEIO_InSpecH	pinH,
						A_FpLong		*ratePF);

	SPAPI A_Err	(*AEGP_SetInSpecSoundRate)(
						AEIO_InSpecH	pinH,
						A_FpLong		rateF);

	SPAPI A_Err	(*AEGP_GetInSpecSoundEncoding)(
						AEIO_InSpecH		pinH,
						AEIO_SndEncoding	*encodingP);

	SPAPI A_Err	(*AEGP_SetInSpecSoundEncoding)(
						AEIO_InSpecH		pinH,
						AEIO_SndEncoding	encoding);

	SPAPI A_Err	(*AEGP_GetInSpecSoundSampleSize)(
						AEIO_InSpecH		pinH,
						AEIO_SndSampleSize	*bytes_per_sampleP);

	SPAPI A_Err	(*AEGP_SetInSpecSoundSampleSize)(
						AEIO_InSpecH		pinH,
						AEIO_SndSampleSize	bytes_per_sample);

	SPAPI A_Err	(*AEGP_GetInSpecSoundChannels)(
						AEIO_InSpecH		pinH,
						AEIO_SndChannels	*num_channelsP);

	SPAPI A_Err	(*AEGP_SetInSpecSoundChannels)(
						AEIO_InSpecH		pinH,
						AEIO_SndChannels	num_channels);

	SPAPI A_Err	(*AEGP_AddAuxExtMap)(
						const A_char		*extension,
						A_long				file_type,
						A_long				creator);
} AEGP_IOInSuite1;



#define kAEGPIOOutSuite			"AEGP IO Out Suite"
#define kAEGPIOOutSuiteVersion1	1


typedef struct AEGP_IOOutSuite1 {
	SPAPI A_Err	(*AEGP_GetOutSpecOptionsHandle)(
						AEIO_OutSpecH	outH,
						void			**optionsPPV);

	SPAPI A_Err	(*AEGP_SetOutSpecOptionsHandle)(
						AEIO_OutSpecH	outH,
						void			*optionsPV,
						void			**old_optionsPPV);

	SPAPI A_Err	(*AEGP_GetOutSpecFilePath)(
						AEIO_OutSpecH	outH,
						A_char			*file_nameZ);

	SPAPI A_Err	(*AEGP_GetOutSpecFPS)(
						AEIO_OutSpecH	outH,
						A_Fixed			*native_fpsP);

	SPAPI A_Err	(*AEGP_SetOutSpecNativeFPS)(
						AEIO_OutSpecH	outH,
						A_Fixed			native_fpsP);

	SPAPI A_Err	(*AEGP_GetOutSpecDepth)(
						AEIO_OutSpecH	outH,
						A_short			*depthPS);

	SPAPI A_Err	(*AEGP_SetOutSpecDepth)(
						AEIO_OutSpecH	outH,
						A_short			depthPS);

	SPAPI A_Err	(*AEGP_GetOutSpecInterlaceLabel)(
						AEIO_OutSpecH		outH,
						FIEL_Label			*interlaceP);

	SPAPI A_Err	(*AEGP_SetOutSpecInterlaceLabel)(
						AEIO_OutSpecH			outH,
						const FIEL_Label		*interlaceP);

	SPAPI A_Err	(*AEGP_GetOutSpecAlphaLabel)(
						AEIO_OutSpecH	outH,
						AEIO_AlphaLabel	*alphaP);

	SPAPI A_Err	(*AEGP_SetOutSpecAlphaLabel)(
						AEIO_OutSpecH			outH,
						const AEIO_AlphaLabel	*alphaP);

	SPAPI A_Err	(*AEGP_GetOutSpecDuration)(
						AEIO_OutSpecH	outH,
						A_Time			*durationP);

	SPAPI A_Err	(*AEGP_SetOutSpecDuration)(
						AEIO_OutSpecH	outH,
						const A_Time	*durationP);

	SPAPI A_Err	(*AEGP_GetOutSpecDimensions)(
						AEIO_OutSpecH	outH,
						A_long			*widthPL0,
						A_long			*heightPL0);

	SPAPI A_Err	(*AEGP_GetOutSpecHSF)(
						AEIO_OutSpecH	outH,
						A_Ratio			*hsf);

	SPAPI A_Err	(*AEGP_SetOutSpecHSF)(
						AEIO_OutSpecH			outH,
						const A_Ratio			*hsf);

	SPAPI A_Err	(*AEGP_GetOutSpecSoundRate)(
						AEIO_OutSpecH	outH,
						A_FpLong		*ratePF);

	SPAPI A_Err	(*AEGP_SetOutSpecSoundRate)(
						AEIO_OutSpecH	outH,
						A_FpLong		rateF);

	SPAPI A_Err	(*AEGP_GetOutSpecSoundEncoding)(
						AEIO_OutSpecH		outH,
						AEIO_SndEncoding	*encodingP);

	SPAPI A_Err	(*AEGP_SetOutSpecSoundEncoding)(
						AEIO_OutSpecH		outH,
						AEIO_SndEncoding	encoding);

	SPAPI A_Err	(*AEGP_GetOutSpecSoundSampleSize)(
						AEIO_OutSpecH		outH,
						AEIO_SndSampleSize	*bytes_per_sampleP);

	SPAPI A_Err	(*AEGP_SetOutSpecSoundSampleSize)(
						AEIO_OutSpecH		outH,
						AEIO_SndSampleSize	bytes_per_sample);

	SPAPI A_Err	(*AEGP_GetOutSpecSoundChannels)(
						AEIO_OutSpecH		outH,
						AEIO_SndChannels	*num_channelsP);

	SPAPI A_Err	(*AEGP_SetOutSpecSoundChannels)(
						AEIO_OutSpecH		outH,
						AEIO_SndChannels	num_channels);

	SPAPI A_Err	(*AEGP_GetOutSpecIsStill)(
						AEIO_OutSpecH		outH,
						A_Boolean			*is_stillPB);

	SPAPI A_Err	(*AEGP_GetOutSpecPosterTime)(
						AEIO_OutSpecH		outH,
						A_Time				*poster_timeP);

	SPAPI A_Err	(*AEGP_GetOutSpecStartFrame)(
						AEIO_OutSpecH		outH,
						A_long				*start_frameP);

	SPAPI A_Err	(*AEGP_GetOutSpecPullDown)(
						AEIO_OutSpecH		outH,
						AEIO_Pulldown		*pulldownP);

	SPAPI A_Err	(*AEGP_GetOutSpecIsMissing)(
						AEIO_OutSpecH		outH,
						A_Boolean			*missingPB);
	
} AEGP_IOOutSuite1;


//	This suite allows you to take advantage of going through AE Import Dialog
//	and being treated as a native format type

//	Types and constants for AE File Import Manager

typedef	A_long	AE_FIM_ImportFlavorRef;

#define	AE_FIM_ImportFlavorRef_NONE	AEGP_Index_NONE

#define	AE_FIM_MAX_FLAVOR_NAME_LEN		63

enum {
	AE_FIM_ImportFlag_NONE 	= 	0x0,
	AE_FIM_ImportFlag_COMP	=	0x2
};
typedef	A_long	AE_FIM_ImportFlags;

enum {
	AE_FIM_SpecialAction_NONE				=	-1,
	AE_FIM_SpecialAction_DRAG_N_DROP_FILE	=	2
};
typedef	A_long	AE_FIM_SpecialAction;


typedef	struct	AE_FIM_RefconTag		*AE_FIM_Refcon; 

typedef	struct	AE_FIM_ImportOptionsTag	const *AE_FIM_ImportOptions;

// callbacks							
typedef A_Err (*AE_FIM_ImportFileCB)(
							const	char 				*file_fullpathZ,	// >>
							AE_FIM_ImportOptions		imp_options,		// >>	opaque structure; in the future  suite could be expanded with 
																			//		query functions
							AE_FIM_SpecialAction		action,				// >>	is it a special kind of import situation?
							AEGP_ItemH					itemH,				// >>	meaning varies depending on AE_FIM_SpecialAction
																			//		both for no special action and drag'n'drop it is
																			//		a folder where imported item should go
							AE_FIM_Refcon				refcon);			// >>	the client defines this and it is stored with import callbacks
							

									
typedef A_Err (*AE_FIM_VerifyImportableCB)(
							const char *		file_fullpathZ,		// >>
							AE_FIM_Refcon		refcon,				// >> the client defines this and it is stored with import callbacks
							A_Boolean			*importablePB);		// <<
								

typedef struct {
	AE_FIM_Refcon				refcon;		// the client defines this to be whatever the client wants it to be; it will be stored and passed back with the callbacks
	AE_FIM_ImportFileCB			import_cb;
	AE_FIM_VerifyImportableCB	verify_cb;
} AE_FIM_ImportCallbacks;


#define	kAEGPFIMSuite			"AEGP File Import Manager Suite"
#define	kAEGPFIMSuiteVersion1	1


typedef struct {
	SPAPI A_Err (*AEGP_RegisterImportFlavor)(
				const char 				*nameZ,		// format name you'd like to appear
													// in AE's Import Dialog Format pop-up
													// menu.
													// Limited to AE_FIM_MAX_IMPORT_FLAVOR_NAME_LEN.
													// Everything after that will be truncated.
				AE_FIM_ImportFlavorRef	*imp_refP);	// On return it is set to a valid opaque ref.
													// If error occured, it will be returned to
													// the caller and ref will be set to a special
													// value - AE_FIM_ImportFlavorRef_NONE.
													
	SPAPI A_Err (*AEGP_RegisterImportFlavorFileTypes)(
				AE_FIM_ImportFlavorRef	imp_ref,		// Received from AEGP_RegisterImportFlavor
				long					num_filekindsL,	// number of supported file types for this format
				const	AEIO_FileKind	*kindsAP,		// Array of supported file types for this format
				long					num_fileextsL,	// number of supported file exts for this format
				const	AEIO_FileKind	*extsAP);		// Array of supported file exts for this format
				
				
	SPAPI A_Err (*AEGP_RegisterImportFlavorImportCallbacks)(
				AE_FIM_ImportFlavorRef	imp_ref,			// Received from AEGP_RegisterImportFlavor
				AE_FIM_ImportFlags		single_flag,		// You can register callbacks only per single flag
															// this also registers the flag with the import flavor		
				const	AE_FIM_ImportCallbacks	*imp_cbsP);	// Callbacks your format installs per each flag
				
} AEGP_FIMSuite1;



/* --------------------------- Persistent Data Suite ------------------------------*/
/*
The persist data suite allows you to store persistant data with the application.

The data entries are accessed by SectionKey, ValueKey pairs. It is recommended
that plugins use their matchname as their SectionKey, or the prefix if using multiple
section names. THe available data types are void*, floating point numbers, and strings.

Void* unstructured data allows you to store any kind of data. You must pass in a size in
bytes along with the data.

String data supports the full 8 bit space, only 0x00 is reserved for string ending. 
This makes them ideal for storing UTF-8 encoded strings, ISO 8859-1, and plain ASCII. 
Both Section keys and Value keys are of this type.

FpLongs are stored with 6 decimal places of precision. There is no provision
for specifying a different precision.

Right now the only persistent data host is the application.

*/

#define	kAEGPPersistentDataSuite			"AEGP Persistent Data Suite"
#define	kAEGPPersistentDataSuiteVersion1	1

typedef struct {
	// get a handle of the application blob,
	// modifying this will modify the application
	SPAPI A_Err (*AEGP_GetApplicationBlob)(
						AEGP_PersistentBlobH	*blobPH);			/* >> */

	// section and value key management
	SPAPI A_Err (*AEGP_GetNumSections)(
						AEGP_PersistentBlobH	blobH,				/* >> */
						A_long					*num_sectionPL);	/* << */

	SPAPI A_Err (*AEGP_GetSectionKeyByIndex)(
						AEGP_PersistentBlobH	blobH,				/* >> */
						A_long					section_index,		/* >> */
						A_long					max_section_size,	/* >> */
						A_char					*section_keyZ);		/* << */

	SPAPI A_Err	(*AEGP_DoesKeyExist)(
						AEGP_PersistentBlobH		blobH,			/* >> */
						const A_char				*section_keyZ,	/* >> */
						const A_char				*value_keyZ,	/* >> */ 
						A_Boolean					*existsPB);		/* << */


	SPAPI A_Err (*AEGP_GetNumKeys)(
						AEGP_PersistentBlobH	blobH,				/* >> */ 
						const A_char			*section_keyZ,		/* >> */ 
						A_long					*num_keysPL);		/* << */

	SPAPI A_Err (*AEGP_GetValueKeyByIndex)(
						AEGP_PersistentBlobH	blobH,				/* >> */ 
						const A_char			*section_keyZ,		/* >> */ 
						A_long					key_index,			/* >> */  
						A_long					max_key_size,		/* >> */ 
						A_char					*value_keyZ);		/* << */

	// data access and manipulation

	// For the entry points below, if a given key is not found,
	//	the default value is both written to the blobH and
	//	returned as the value; if no default is provided, a blank value will be written
	//	and returned
	SPAPI A_Err (*AEGP_GetDataHandle)(
						AEGP_PluginID			plugin_id,
						AEGP_PersistentBlobH	blobH,			/* >> */
						const A_char			*section_keyZ,	/* >> */
						const A_char			*value_keyZ,	/* >> */
						AEGP_MemHandle			defaultH0,		/* >> never adopted, NULL means no default data */
						AEGP_MemHandle			*valuePH);		/* << newly allocated, owned by caller, NULL if would be zero sized handle */

	SPAPI A_Err (*AEGP_GetData)(
						AEGP_PersistentBlobH	blobH,			/* >> */
						const A_char			*section_keyZ,	/* >> */
						const A_char			*value_keyZ,	/* >> */
						A_u_long				data_sizeLu,	/* >> bufPV & default must be this big, if pref isn't then the default will be used */
						const void				*defaultPV0,	/* >> NULL means all zeros for default */
						void					*bufPV);		/* << */

	SPAPI A_Err (*AEGP_GetString)(
						AEGP_PersistentBlobH	blobH,			/* >> */
						const A_char			*section_keyZ,	/* >> */
						const A_char			*value_keyZ,	/* >> */
						const A_char			*defaultZ0,		/* >> NULL means '\0' is the default */
						A_u_long				buf_sizeLu,		/* >> size of buffer. Behavior dependent on actual_buf_sizeLu0 */
						char					*bufZ,			/* << will be "" if buf_size is too small */
						A_u_long				*actual_buf_sizeLu0);	/* << actual size needed to store the buffer (includes terminating NULL). Pass NULL for error reporting if size mismatch.*/

	SPAPI A_Err (*AEGP_GetLong)(
						AEGP_PersistentBlobH	blobH,			/* >> */
						const A_char			*section_keyZ,	/* >> */
						const A_char			*value_keyZ,	/* >> */
						A_long					defaultL,		/* >> */
						A_long					*valuePL);		/* << */

	SPAPI A_Err	(*AEGP_GetFpLong)(
						AEGP_PersistentBlobH	blobH,			/* >> */
						const A_char			*section_keyZ,	/* >> */
						const A_char			*value_keyZ,	/* >> */
						A_FpLong				defaultF,		/* >> */
						A_FpLong				*valuePF);		/* << */

// setters
	SPAPI A_Err	(*AEGP_SetDataHandle)(
						AEGP_PersistentBlobH	blobH,			/* >> */
						const A_char			*section_keyZ,	/* >> */
						const A_char			*value_keyZ,	/* >> */
						const AEGP_MemHandle	valueH);		/* >> not adopted */

	SPAPI A_Err	(*AEGP_SetData)(
						AEGP_PersistentBlobH	blobH,			/* >> */
						const A_char			*section_keyZ,	/* >> */
						const A_char			*value_keyZ,	/* >> */
						A_u_long				data_sizeLu,	/* >> */
						const void				*dataPV);		/* >> */

	SPAPI A_Err	(*AEGP_SetString)(
						AEGP_PersistentBlobH	blobH,			/* >> */
						const A_char			*section_keyZ,	/* >> */
						const A_char			*value_keyZ,	/* >> */
						const A_char			*strZ);			/* >> */

	SPAPI A_Err	(*AEGP_SetLong)(
						AEGP_PersistentBlobH	blobH,			/* >> */
						const A_char			*section_keyZ,	/* >> */
						const A_char			*value_keyZ,	/* >> */
						A_long					valueL);		/* >> */


	SPAPI A_Err	(*AEGP_SetFpLong)(
						AEGP_PersistentBlobH	blobH,			/* >> */
						const A_char			*section_keyZ,	/* >> */
						const A_char			*value_keyZ,	/* >> */
						A_FpLong				valueF);		/* >> */

	SPAPI A_Err	(*AEGP_DeleteEntry)(							// ok if entry not found
						AEGP_PersistentBlobH	blobH,			/* >> */
						const A_char			*section_keyZ,	/* >> */
						const A_char			*value_keyZ);	/* >> */


} AEGP_PersistentDataSuite1;


/* AEGP_CollectionSuite1

  */

#define	kAEGPCollectionSuite					"AEGP Collection Suite"
#define	kAEGPCollectionSuiteVersion1			1

typedef enum  {
	AEGP_CollectionItemType_NONE,

	AEGP_CollectionItemType_LAYER,
	AEGP_CollectionItemType_MASK,
	AEGP_CollectionItemType_EFFECT,
	AEGP_CollectionItemType_STREAM,
	AEGP_CollectionItemType_KEYFRAME,
	AEGP_CollectionItemType_MASK_VERTEX,

	AEGP_CollectionItemType_END,
	AEGP_CollectionItemType_BEGIN = AEGP_CollectionItemType_LAYER
} AEGP_CollectionItemType; 

typedef struct {
	AEGP_LayerH	layerH;		// comp derived from layerH
} AEGP_LayerCollectionItem;

typedef struct {
	AEGP_LayerH		layerH;		// containing layer
	AEGP_MaskIndex	index;		// index to layer.
}AEGP_MaskCollectionItem;

typedef struct {
	AEGP_LayerH			layerH;		// containing layer
	AEGP_EffectIndex	index;		// index to the effect
}AEGP_EffectCollectionItem;

typedef enum  {
	AEGP_StreamCollectionItemType_NONE,
	AEGP_StreamCollectionItemType_LAYER,
	AEGP_StreamCollectionItemType_MASK,
	AEGP_StreamCollectionItemType_EFFECT,

	AEGP_StreamCollectionItemType_END,
	AEGP_StreamCollectionItemType_BEGIN = AEGP_StreamCollectionItemType_LAYER
} AEGP_StreamCollectionItemType;

typedef struct {
	AEGP_MaskCollectionItem		mask;
	AEGP_MaskStream				mask_stream;
} AEGP_MaskStreamCollectionItem;

typedef struct {
	AEGP_EffectCollectionItem	effect;
	A_long						param_index;
} AEGP_EffectStreamCollectionItem;

typedef struct {
	AEGP_LayerH						layerH;
	AEGP_LayerStream				layer_stream;
} AEGP_LayerStreamCollectionItem;

typedef struct {
	AEGP_StreamCollectionItemType	type;
	union {
		AEGP_LayerStreamCollectionItem	layer_stream;			
		AEGP_MaskStreamCollectionItem	mask_stream;
		AEGP_EffectStreamCollectionItem	effect_stream;
	} u;
}AEGP_StreamCollectionItem;

typedef struct {
	AEGP_MaskCollectionItem	mask_sel;	/* the mask must be selected for a vertex to be selected */
	AEGP_VertexIndex	index;
}AEGP_MaskVertexCollectionItem;

typedef struct {
	AEGP_StreamCollectionItem	stream_coll;		
	AEGP_KeyframeIndex	index;
}AEGP_KeyframeCollectionItem;

typedef struct {
	AEGP_CollectionItemType	type;
	union {
		AEGP_LayerCollectionItem		layer;
		AEGP_MaskCollectionItem			mask;
		AEGP_EffectCollectionItem		effect;
		AEGP_StreamCollectionItem		stream;
		AEGP_MaskVertexCollectionItem	mask_vertex;
		AEGP_KeyframeCollectionItem		keyframe;
	} u;
} AEGP_CollectionItem;

typedef struct {
	SPAPI A_Err (*AEGP_NewCollection)(						/* dispose with dispose collection */
						AEGP_PluginID	plugin_id,			/* >> */
						AEGP_CollectionH *collectionPH);	/* << */

	SPAPI A_Err (*AEGP_DisposeCollection)(
						AEGP_CollectionH collectionH);		/* >> */

	SPAPI A_Err	(*AEGP_GetCollectionNumItems)(				/* constant time */
						AEGP_CollectionH	collectionH,			/* >> */
						A_u_long		*num_itemsPL);		/* << */

	SPAPI A_Err	(*AEGP_GetCollectionItemByIndex)(				/* constant time */
						AEGP_CollectionH	collectionH,	/* >> */
						A_u_long			indexL,				/* >> */
						AEGP_CollectionItem	*collection_itemP);	/* << */

	SPAPI A_Err (*AEGP_CollectionPushBack)(						/* constant time */
						AEGP_CollectionH			collectionH,		/* <> */
						const AEGP_CollectionItem	*collection_itemP);	/* >> */

	SPAPI A_Err (*AEGP_CollectionErase)(						/* O(n) */
						AEGP_CollectionH	collectionH,		/* <> */
						A_u_long			index_firstL,		/* >> */
						A_u_long			index_lastL);		/* >> */

} AEGP_CollectionSuite1;



#ifdef __cplusplus
}		// end extern "C"
#endif


#if PRAGMA_STRUCT_ALIGN
	#pragma options align=reset
#endif

#ifdef _WINDOWS
	#pragma pack( pop, AEalign4)
#endif



#endif
