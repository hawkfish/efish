#ifndef _PR_Public_H
#define _PR_Public_H

/** PR_Public.h
 ** ©2000 by Adobe Systems Inc.
 **	Public header that defines the API for plug-in renderers
 **
 ** Plugin renderers are known as artisans. They are AEGP modules which register
 ** themselves with the plug-in render manager when they are loaded at 
 ** program startup. 
 ** Very little is passed to these functions, just an opaque context 
 ** and a handle to previously allocated data. The artisan must query 
 ** for other rendering parameters via AEGP suites. The appropriate 
 ** suites will take one of the opaque rendering contexts as one of their 
 ** parameters. 
 **
 ** The entry points are :
 ** PR_GlobalSetupFunc	
 **		Called at artisan load. If necessary allocate global data that will be shared
 **		among all instances of this type of artisan.
 **
 **	PR_GlobalSetdownFunc 
 **		Called at program termination. Delete the global data if any allocated.
 **
 **	PR_GlobalDoAboutFunc 
 **		Display an about box with revelant information about the artisan
 **
 **		
 **	PR_InstanceSetupFunc
 **		An instance of the artisan is associated with each comp. Each instance has its
 **		own data. It should be allocated within this function.
 **
 **	PR_InstanceSetdownFunc
 **		Delete allocated data if needed.
 **		
 **	PR_FlattenInstanceFunc
 **		Return a flattened platform independent version of the instance data.
 **		This is called when the artisan is being written to disk or a copy of the 
 **		artisan is being made. Do not disturb the src instance handle.
 **		
 **	PR_DoInstanceDialogFunc	
 **		Some artisans may have parameters. They may be set here.
 **	
 **	PR_FrameSetupFunc
 **		Called just berfore render. Allocate the render handle, if needed.
 **
 **	PR_RenderFunc
 **		This is the main rendering function. 
 **
 **	PR_FrameSetdownFunc	
 **		Called just after render. Deallocate the render handle if needed.
 **
 **	PR_Query	
 **		This can be called at any time after Instance Setup. It is used by AE
 **		to inquire about geomertic transforms used by the artisan. AE uses this information
 **		to draw layer handles, and manipulate the layers with a mouse better. 
 **
 **/


#include <A.h>
#include <SPBasic.h>

#if PRAGMA_STRUCT_ALIGN
	#pragma options align=power
#endif

#ifdef _WINDOWS
	#pragma pack( push, AEalign, 4 )
#endif




#ifdef __cplusplus
	extern "C" {
#endif


		
#define PR_FileType_ARTISAN			'ARt'
#define PR_ARTISAN_EXTENSION		".aex"

#define PR_ARTISAN_API_VERSION_MAJOR		1
#define PR_ARTISAN_API_VERSION_MINOR		0


#define PR_PUBLIC_MATCH_NAME_LEN			31
#define PR_PUBLIC_ARTISAN_NAME_LEN			31


/** $$$ move to aegp.h ***/
typedef struct _Up_OpaqueMem			**PR_Handle;
typedef PR_Handle						PR_FlatHandle;
									
typedef PR_Handle						PR_GlobalDataH;				// holds data private to the plug-in
typedef PR_Handle						PR_InstanceDataH;
typedef PR_Handle						PR_RenderDataH;

typedef struct PR_GlobalContext			**PR_GlobalContextH;	// opaque until PR.h
typedef struct PR_InstanceContext		**PR_InstanceContextH;	// opaque until PR.h
typedef struct PR_RenderContext			**PR_RenderContextH;	// opaque until PR.h
typedef struct PR_QueryContext			**PR_QueryContextH;		// opaque until PR.h


/**
 ** in data is passed to every pr entry function
 **/
typedef void (*PR_MessageFunc) (A_Err err_number, const A_char *msgA);


typedef struct PR_InData {
	PR_MessageFunc				msg_func;
	const struct SPBasicSuite	*pica_basicP;
	long						aegp_plug_id;
	void						*aegp_refconPV;
} PR_InData;





/**
 ** response from dialog box function
 **/
enum {
	PR_DialogResult_NO_CHANGE,
	PR_DialogResult_CHANGE_MADE
};
typedef long PR_DialogResult;



/**
 ** The types of queries that will be made.
 **
 **/
enum { 
	PR_QueryType_NONE			= 0,
	PR_QueryType_TRANSFORM
};

typedef A_u_long	PR_QueryType;




/**
 ** PR_InstanceSetupFunc flags
 **/
enum {
	PR_InstanceFlags_NONE = 0x0,
	PR_InstanceFlags_DUPLICATE
};

typedef A_u_long PR_InstanceFlags;


/***********************   plugin entry points  *****************************
 ** the main routine of each plugin fills in these function pointers
 ** AE will call them as appropriate
 ****************************************************************************/



/**
 ** called after main. This happens just once, after the plugin is loaded.
 ** The global data is common across all instances of the plugin
 **/
typedef	A_Err	(*PR_GlobalSetupFunc)(	const PR_InData				*in_dataP,				/* >> */
										PR_GlobalContextH			global_contextH,		/* >> */
										PR_GlobalDataH				*global_dataPH);		/* << */

/**
 ** dispose of the global data
 **/
typedef	A_Err	(*PR_GlobalSetdownFunc)(	const PR_InData			*in_dataP,			/* >> */
											PR_GlobalContextH		global_contextH,	/* >> */
											PR_GlobalDataH			global_dataH);		/* <> */ // must be disposed by plugin


/**
 ** display an about box
 **/
typedef A_Err	(*PR_GlobalDoAboutFunc)(	const PR_InData			*in_dataP,			/* >> */
											PR_GlobalContextH		global_contextH,	/* >> */
											PR_GlobalDataH			global_dataH);		/* <> */



/**
 ** Analogous to an Effect's Sequence setup call. This sets up the renderer's
 ** instance data. 
 **/
typedef	A_Err	(*PR_InstanceSetupFunc)(
										const PR_InData			*in_dataP,					/* >> */
										PR_GlobalContextH		global_contextH,			/* >> */
										PR_InstanceContextH		instance_contextH,			/* >> */
										PR_GlobalDataH			global_dataH,				/* >> */
										PR_InstanceFlags		flags,
										PR_FlatHandle			flat_dataH0,				/* >> */
										PR_InstanceDataH		*instance_dataPH);			/* << */


/**
 ** dispose of the instance data
 **/
typedef	A_Err	(*PR_InstanceSetdownFunc)(
										const PR_InData				*in_dataP,			/* >> */
										const PR_GlobalContextH		global_contextH,		/* >> */
										const PR_InstanceContextH	instance_contextH,		/* >> */
										PR_GlobalDataH				global_dataH,			/* >> */
										PR_InstanceDataH			instance_dataH);		/* >> */ // must be disposed by plugin



/**
 ** flatten your data in preparation to being written to disk.
 ** Make sure its OS independent
 **/
typedef	A_Err	(*PR_FlattenInstanceFunc)(
										const PR_InData			*in_dataP,			/* >> */
										PR_GlobalContextH		global_contextH,		/* >> */
										PR_InstanceContextH		instance_contextH,		/* >> */
										PR_GlobalDataH			global_dataH,			/* <> */
										PR_InstanceDataH		instance_dataH,			/* <> */
										PR_FlatHandle			*flatH);				/* << */





/**
 ** if the renderer has parameters, this is where they get set or changed.
 **/


typedef	A_Err	(*PR_DoInstanceDialogFunc)(		const PR_InData		*in_dataP,			/* >> */
												PR_GlobalContextH	global_contextH,		/* >> */
												PR_InstanceContextH	instance_contextH,		/* >> */
												PR_GlobalDataH		global_dataH,			/* <> */
												PR_InstanceDataH	instance_dataH,			/* <> */
												PR_DialogResult		*resultP);				/* << */
			


/** 
 ** allocate render data if needed
 **/
typedef A_Err	(*PR_FrameSetupFunc)(
										const PR_InData			*in_dataP,				/* >> */
										PR_GlobalContextH		global_contextH,			/* >> */
										PR_InstanceContextH		instance_contextH,			/* >> */
										PR_RenderContextH		render_contextH,			/* >> */
										PR_GlobalDataH			global_dataH,				/* <> */
										PR_InstanceDataH		instance_dataH,				/* <> */
										PR_RenderDataH			*render_dataPH);			/* << */


/** 
 ** deallocate render data
 **/
typedef A_Err	(*PR_FrameSetdownFunc)(
										const PR_InData			*in_dataP,				/* >> */
										PR_GlobalContextH		global_contextH,			/* >> */
										PR_InstanceContextH		instance_contextH,			/* >> */
										PR_RenderContextH		render_contextH,			/* >> */
										PR_GlobalDataH			global_dataH,				/* <> */
										PR_InstanceDataH		instance_dataH,				/* <> */
										PR_RenderDataH			render_dataH);	


/** 
 ** the main drawing routine 
 **/
typedef A_Err	(*PR_RenderFunc)(
										const PR_InData			*in_dataP,					/* >> */
										PR_GlobalContextH		global_contextH,			/* >> */
										PR_InstanceContextH		instance_contextH,			/* >> */
										PR_RenderContextH		render_contextH,			/* >> */
										PR_GlobalDataH			global_dataH,				/* <> */
										PR_InstanceDataH		instance_dataH,				/* <> */
										PR_RenderDataH			render_dataH);




/**
 ** AE will need to have the artisan process data on its behalf such as 
 ** projecting points to the screen, transforming axis, etc. This routine will handle
 ** it all
 **/
typedef A_Err	(*PR_QueryFunc)(	const PR_InData			*in_dataP,			/* >> */
									PR_GlobalContextH		global_contextH,	/* >> */
									PR_InstanceContextH		instance_contextH,  /* >> */
									PR_QueryContextH		query_contextH,		/* <> */
									PR_QueryType			query_type,			/* >> */
									PR_GlobalDataH			global_data,		/* >> */
									PR_InstanceDataH		instance_dataH);	/* >> */
									



/**
 ** main fills this in, just once at plugin load time
 ** These are the entry points that AE calls to use an artisan.
 **/

typedef struct {

	PR_GlobalSetupFunc					global_setup_func0;
	PR_GlobalSetdownFunc				global_setdown_func0;
	PR_GlobalDoAboutFunc				global_do_about_func0;

	PR_InstanceSetupFunc				setup_instance_func0;
	PR_InstanceSetdownFunc				setdown_instance_func0;
	PR_FlattenInstanceFunc				flatten_instance_func0;
	PR_DoInstanceDialogFunc				do_instance_dialog_func0;
	
	PR_FrameSetupFunc					frame_setup_func0;
	PR_RenderFunc						render_func;			// must have at least this one function
	PR_FrameSetdownFunc					frame_setdown_func0;

	PR_QueryFunc						query_func0;
	
} PR_ArtisanEntryPoints;







#if PRAGMA_STRUCT_ALIGN
	#pragma options align=reset
#endif

#ifdef _WINDOWS
	#pragma pack( pop, AEalign)
#endif



#ifdef __cplusplus
	}
#endif

							
#endif
