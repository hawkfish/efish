/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 1999 Adobe Systems Incorporated                       */
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
/*
	File: AE_FPF.h

	Part of the Adobe After Effects 4.0 SDK.
	Copyright 1998 Adobe Systems Incorporated.
	All Rights Reserved.

	This file describes the Foreign Project Format functionality.
*/


#ifndef	_H_AE_FPF
	#define	_H_AE_FPF
	
	#define	WINDOWS_MAX_PATH_LEN	260
	
	#if MSWindows
		#define	MAX_PATH_NAME_LEN	(WINDOWS_MAX_PATH_LEN - 1)
	#else
		#define	MAX_PATH_NAME_LEN	255
	#endif
	
	#define	MAX_ITEM_NAME_LEN	31

	#define	FPF_USE_DEFAULT	-1L
	
	#define	FPF_NO_RSRC_FORK	-32766L
	
	enum	{
		FPF_Level_INFO	= 0,
		FPF_Level_WARNING,
		FPF_Level_PROBLEM
	};
	
	typedef	long	FPF_Severity_LEVEL;

	enum	{
		FPF_FootageType_UNKNOWN = -1,	//no match for the following types
		FPF_FootageType_FOLDER = 0,
		FPF_FootageType_STILL = 1,	//still image
		FPF_FootageType_SEQUENCE,	//sequence of still images
		FPF_FootageType_VIDEO,		
		FPF_FootageType_AUDIO,
		FPF_FootageType_A_V,		//has both video and audio tracks
		FPF_FootageType_SOLID,
		FPF_FootageType_NONE,		//use this to indicate that no processing is required	
		FPF_NUM_FOOTAGE_TYPES
	};

	typedef	long	FPF_FootageType;
	
	enum	{
		FPF_Timebase_NO_TIME_BASE = FPF_USE_DEFAULT,
		FPF_Timebase_FILM,
		FPF_Timebase_PAL,
		FPF_Timebase_NTSC_DROP,
		FPF_Timebase_NTSC_NON_DROP,
		FPF_NUM_TIME_BASES
	};
	
	typedef	long	FPF_Timebase;
	
	enum	{
		FPF_projectSelectorReadPrepare = 1,
		FPF_projectSelectorReadProjectInfo,
		FPF_projectSelectorReadFootageInfo,
		FPF_projectSelectorReadFinish
	};
	
	typedef	short	FPF_Selector;
	
	enum	{
		FPF_Flag_SILENT_ERROR = 0x0001
	};
	
	typedef	long	FPF_Flags;

#ifndef	Handle
		typedef	char**	Handle;
#endif
#ifndef	Ptr
		typedef	char*	Ptr;
#endif
	
	//Handle procs
	typedef Handle (*NewFPFHandleProc) (long sizeL);
	typedef void (*DisposeFPFHandleProc) (Handle h);
	typedef long (*GetFPFHandleSizeProc) (Handle h);
	typedef short (*SetFPFHandleSizeProc) (Handle h, long newSizeL);
	typedef Ptr (*LockFPFHandleProc) (Handle h, Boolean moveHighB);
	typedef void (*UnlockFPFHandleProc) (Handle h);
	
	//auxillary procs
	typedef	void (*ReportStringFPFProc) (const char *strZ, const FPF_Severity_LEVEL level);

	#define FPF_CurrentHandleProcsVersion 1	

	typedef struct
	{
		short handleProcsVersionS;
		
		short numHandleProcsS;
		
		NewFPFHandleProc newProc;
		
		DisposeFPFHandleProc disposeProc;
		
		GetFPFHandleSizeProc getSizeProc;
		
		SetFPFHandleSizeProc setSizeProc;
		
		LockFPFHandleProc lockProc;
		
		UnlockFPFHandleProc unlockProc;
	} FPF_HandleProcs;


	#define FPF_CurrentHandleProcsCount \
		((sizeof(FPF_HandleProcs) - offsetof(FPF_HandleProcs, newProc)) / sizeof(void *))
		
	#if MSWindows
		#define	NEW_HANDLE(irP, size)	(*(irP)->handle_procs->newProc)((size))
	
		#define	DISPOSE_HANDLE(irP, h)	if((h)) (*(irP)->handle_procs->disposeProc)((Handle)(h))
	
		#define	LOCK_HANDLE(irP, h) (*(irP)->handle_procs->lockProc)((Handle)(h), TRUE)
	
		#define	UNLOCK_HANDLE(irP, h) (*(irP)->handle_procs->unlockProc)((Handle)(h))
	
		#define	SET_HANDLE_SIZE(irP, h, size)	(*(irP)->handle_procs->setSizeProc)((Handle)(h), (size))
	
		#define	GET_HANDLE_SIZE(irP, h)	(*(irP)->handle_procs->getSizeProc)((Handle)(h))
	
		#define	GET_HANDLE_STATE(h)	0
	#else
		#define	NEW_HANDLE(irP, size) (*(irP)->handle_procs->newProc ?	\
								(*(irP)->handle_procs->newProc)((size)) : \
								NewHandle((size)))
	
		#define	DISPOSE_HANDLE(irP, h)	if ((h)) (*(irP)->handle_procs->disposeProc ? \
								(*(irP)->handle_procs->disposeProc)((Handle)(h)) : \
								DisposeHandle((Handle)(h)))
	
		#define	LOCK_HANDLE(irP, h)	(*(irP)->handle_procs->lockProc ? \
							(*(irP)->handle_procs->lockProc)((Handle)(h), TRUE) : \
							(HLockHi((Handle)(h)), (*(h))))
	
		#define	UNLOCK_HANDLE(irP, h) (*(irP)->handle_procs->unlockProc ? \
								(*(irP)->handle_procs->unlockProc)((Handle)(h)) : \
								HUnlock((Handle)(h)))
								
		#define	SET_HANDLE_SIZE(irP, h, size)	(*(irP)->handle_procs->setSizeProc ? \
											(*(irP)->handle_procs->setSizeProc)((Handle)(h), (size)) : \
											(SetHandleSize((Handle)(h), (size)), LMGetMemErr()))
								
								
		#define	GET_HANDLE_SIZE(irP, h)	(*(irP)->handle_procs->getSizeProc ? \
								(*(irP)->handle_procs->getSizeProc)((Handle)(h)) : \
								GetHandleSize((Handle)(h)))
	
		#define	GET_HANDLE_STATE(h)	HGetState((Handle)(h))
	#endif
	
	enum	{
		FPF_TimeDescType_FRAMES = 0,
		FPF_TimeDescType_TIME_REC
	};
	
	typedef	long	FPF_TimeDescType;	
	
	typedef	struct	{
		long				valueL;
		unsigned	long	scaleLu;
	} FPF_TimeRec;
	
	typedef	struct	{
		FPF_TimeDescType	type;
		union	{
			long		framesL;
			FPF_TimeRec	time_rec;
		} u;
	}	FPF_TimeDesc;
	
	
	#define	FPF_NO_NUMBER_MARKER	-1L
	
	typedef	struct	{
		long			marker_numL;	//or unnumbered marker
		FPF_TimeDesc	marker;	//marker location in the clip/layer
	}	FPF_MarkerInfo, **FPF_MarkerInfoH;

	typedef	struct	{
		char			nameAC[MAX_PATH_NAME_LEN + 1];
		float			framerateF;	//if it's FPF_USE_DEFAULT, duration will not be used
		FPF_TimeDesc	duration;
		FPF_MarkerInfoH	markersH;	//NULL if no markers
		short			widthS;
		short			heightS;
	} FPF_ProjInfo;
		
	typedef	struct	{
		long			timecodeL;
		FPF_Timebase	time_base;
		char			nameAC[16];
	} FPF_ReelRec;
	
	enum	{
		FPF_InterlaceType_FRAME_RENDERED = 0,
		FPF_InterlaceType_INTERLACED,
		FPF_InterlaceType_HALF_HEIGHT,
		FPF_InterlaceType_FIELD_DOUBLED
	};
	
	typedef	unsigned long FPF_InterlaceType;
	
	enum	{
		FPF_InterlaceOrder_UPPER_FIRST = 0,
		FPF_InterlaceOrder_LOWER_FIRST
	};
	
	typedef unsigned long FPF_InterlaceOrder;
	
	typedef	struct	{
		FPF_InterlaceType	type;
		FPF_InterlaceOrder	order;
	} FPF_InterlaceDesc;
	
	enum	{
		FPF_AlphaType_STRAIGHT,
		FPF_AlphaType_PREMUL,
		FPF_AlphaType_IGNORE,
		FPF_AlphaType_NONE
	};
	
	typedef	unsigned char FPF_AlphaType;
	
	enum	{
		FPF_AlphaFlags_PREMUL = 0x1,	//straight, otherwise
		FPF_AlphaFlags_INVERTED = 0x2
	};
	
	typedef	unsigned long FPF_AlphaFlags;
	
	
	typedef	struct	{
		FPF_AlphaFlags	flags;
		unsigned char	redCu;
		unsigned char	greenCu;
		unsigned char	blueCu;
		FPF_AlphaType	type;
	} FPF_AlphaDesc;
	
	
	enum	{
		FPF_PulldownPhase_NO_PULLDOWN = 0,
		FPF_PulldownPhase_WSSWW,
		FPF_PulldownPhase_SSWWW,
		FPF_PulldownPhase_SWWWS,
		FPF_PulldownPhase_WWWSS,
		FPF_PulldownPhase_WWSSW
	};
	
	typedef	long	FPF_PulldownPhase;
	
	
	typedef	struct	{
		long			num;
		unsigned long	den;
	} FPF_Ratio;
	
	
	typedef struct	{
		FPF_InterlaceDesc	interlace;
		FPF_AlphaDesc		alpha;
		FPF_PulldownPhase	pulldown;
		long				loopsL;
		FPF_Ratio			hsf;				//horizontal stretch factor
		float				native_framerateF;
		float				conform_framerateF;	//can be zero
		long				depthL;
		Boolean				movingB;
		Boolean				time_basedB;
		Boolean				motion_deinterlaceB;
		char				reservedC;
	} FPF_InterpInfo, **FPF_InterpInfoH;
	
	
	typedef	struct	{
		unsigned char	alpha;
		unsigned char	red;
		unsigned char	green;
		unsigned char	blue;
	}	FPF_Color;
		
	#define	FPF_MERGED_LAYERS	-1L
	#define	FPF_PHOTOSHOP_TYPE	'8BPS'
	
	#define FPF_SWAP_LONG(A) ((((unsigned long)(A)) >> 24) | (((A) >> 8) & 0x0000ff00L) | (((A) << 8) & 0xff0000) | ((A) << 24))
	
		
	typedef	struct	{
		char			nameAC[MAX_PATH_NAME_LEN + 1];
		char			file_pathAC[MAX_PATH_NAME_LEN + 1];	//full file path for this item
		FPF_TimeDesc	dst_in_point;		//comp relative, types for all FPF_TimeDesc should be the same
		FPF_TimeDesc	dst_out_point;
		FPF_TimeDesc	src_in_point;		//footage relative
		FPF_TimeDesc	src_out_point;
		FPF_TimeDesc	duration;
		FPF_Ratio		stretchRt;			//time stretch; negative means backwards in time
		FPF_FootageType	type;
		long			num_in_folderL;		//used only for FPF_FootageType_FOLDER
		FPF_ReelRec		reel;
		FPF_InterpInfoH	interpretationH;	//NULL if footage is not interpreted; allocated by plug-in, disposed by app
		FPF_MarkerInfoH	markersH;			//NULL if no markers; allocated by plug-in, disposed by app
		FPF_Color		color;				//used only for FPF_FootageType_SOLID
		short			widthS;				//used only for FPF_FootageType_SOLID
		short			heightS;			//used only for FPF_FootageType_SOLID
		long			which_layerL;		//used only if it is a Photoshop file - indicates which layer to import
		Boolean			is_photoshopB;		//indicate if it is a Photoshop file
		Boolean			add_layerB;			//indicates whether to create a layer for this footage
		Boolean			goes_into_folderB;	//indicates if it goes into a folder that was set up last
		Boolean			stretch_to_compB;	//indicates if the layer should be stretched to comp size
	} FPF_FootageInfo;
		
		
	typedef	struct	{
		long					serial_numberL;
		long					data_forkL;	//refNum on the Mac, HANDLE for file on Windows
		long					res_forkL;	//valid only on Mac
		long					host_sigL;	//host app's creator
		long					platform_dataL;	//not used on Mac, HWND to frontmost window on Windows
		FPF_HandleProcs			*handle_procs;
		void					*dataPV;	//depends on selector with which plug-in's entry point is called
		Boolean					doneB;
		char					reservedAC[3];
		FPF_Flags				flags;
		struct	SPBasicSuite	*sp_basicP;
		ReportStringFPFProc	string_func;
	} FPF_ImportRecord;
	
	typedef	void (* FPF_MainEntryFunc) (FPF_Selector selector, FPF_ImportRecord *irP, short *resultPS);
		
#endif