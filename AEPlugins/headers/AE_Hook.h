#ifndef _H_HOOK_AE
#define _H_HOOK_AE


#include <AE_Effect.h>			// for PF_Version, other types


#ifdef __cplusplus
	extern "C" {
#endif


#define AE_HOOK_PLUGIN_TYPE		'AEgp'

#define AE_HOOK_MAJOR_VERSION		3
#define AE_HOOK_MINOR_VERSION		0

enum {
	AE_PixFormat_NONE = -1,		// sentinel -- meaningless
	AE_PixFormat_ARGB = 0,
	AE_PixFormat_BGRA
};

typedef long AE_PixFormat;

enum {
	AE_BlitOutFlag_NONE = 0,
	AE_BlitOutFlag_ASYNCHRONOUS = 1L << 0
};

enum {
	AE_BlitInFlag_NONE = 0,
	AE_BlitInFlag_RENDERING = 1L << 0
};

typedef long AE_BlitInFlags;
typedef long AE_BlitOutFlags;

// AE now supports 8bpp and 16 bpp
// depthL, chan_bytesL, and plane_bytesL
// can now change.
typedef struct {
	long			widthL;
	long			heightL;
	long			depthL;			// 32 or 64
	AE_PixFormat	pix_format;		// always AE_PixFormat_ARGB on Mac, BGRA on Windows (for now)

	long			row_bytesL;
	long			chan_bytesL;	// 4 or 8
	long			plane_bytesL;	// 1 or 2

	void			*pixelsPV;

} AE_PixBuffer;



typedef struct {
	long		frame_widthL;			// original size of image regardless of region of interest
	long		frame_heightL;

	long		origin_xL;				// where the pix buffer is placed in frame coords
	long		origin_yL;		

	long		view_rect_leftL;		// what the user is actually seeing in the window
	long		view_rect_topL;
	long		view_rect_rightL;
	long		view_rect_bottomL;


} AE_ViewCoordinates;



typedef struct _AE_BlitReceipt			*AE_BlitReceipt;		// opaque

typedef struct _AE_CursorInfo			*AE_CursorInfo;			// opaque until I can decide what this means



typedef void		(*AE_DeathHook)(	void *hook_refconPV);						/* >> */	

typedef void		(*AE_VersionHook)(	void *hook_refconPV,						/* >> */	
										unsigned long *versionPV);					/* << */


typedef struct		FILE_Spec			**AE_FileSpecH;


typedef void		(*AE_BlitCompleteFunc)(	AE_BlitReceipt			receipt,
											PF_Err					err);		// non zero if error during asynch operation


typedef PF_Err		(*AE_BlitHook)(		void						*hook_refconPV,	/* >> */
										const AE_PixBuffer			*pix_bufP0,		/* >> */
										const AE_ViewCoordinates	*viewP,			/* >> */
										AE_BlitReceipt				receipt,		/* >> */
										AE_BlitCompleteFunc			complete_func0,	/* >> */
										AE_BlitInFlags				in_flags,		/* >> */
										AE_BlitOutFlags				*out_flags);	/* << */



typedef void		(*AE_CursorHook)(	void						*hook_refconPV,	/* >> */	
										const AE_CursorInfo			*cursorP);		/* >> */

typedef struct {
	// must match NIM_Hooks
	
	void				*hook_refconPV;
	void				*reservedAPV[8];
	AE_DeathHook		death_hook_func;
	AE_VersionHook		version_hook_func;
	struct SPBasicSuite	*pica_basicP;
	AE_BlitHook			blit_hook_func;
	AE_CursorHook		cursor_hook_func;

} AE_Hooks;



typedef PF_Err (*AE_HookPluginEntryFunc)(	long major_version,			/* >> */		
											long minor_version,			/* >> */		
											AE_FileSpecH file_specH,	/* >> */		
											AE_FileSpecH res_specH,		/* >> */		
											AE_Hooks *hooksP);			/* << */

#ifdef __cplusplus
	}
#endif


#endif

