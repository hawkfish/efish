/** AE_EffectUI.h

	After Effectsª Custom UI-Related Structure Header File

	Copyright (c) 1992-95 Adobe Systems Inc, Seattle WA
	All Rights Reserved

 
**/

#ifndef _H_AE_EffectUI
#define _H_AE_EffectUI


#include <AE_Effect.h>


#if PRAGMA_STRUCT_ALIGN
	#pragma options align=mac68k
#endif


#ifdef _WINDOWS
	#pragma pack( push, AEalign, 2 )
#endif




#ifdef __cplusplus
	extern "C" {
#endif


/** PF_CustomFlags
 **
 ** kinds of events and actions the custom parameter type might require
 **
 **/

enum {
	PF_CustomEFlag_NONE 			= 0,

	PF_CustomEFlag_COMP 			= 1L << 0,
	PF_CustomEFlag_LAYER			= 1L << 1,
	PF_CustomEFlag_EFFECT			= 1L << 2,
	PF_CustomEFlag_PREVIEW			= 1L << 3
};
typedef long PF_CustomEventFlags;


enum {
	PF_Window_NONE = -1,
	PF_Window_COMP,
	PF_Window_LAYER,
	PF_Window_EFFECT,
	PF_Window_PREVIEW
};
typedef long PF_WindowType;


/* $$$ document!

	new context -> evt extra has context and type, but everything else should be ignored
		fill in plugin_state[4]
	close context -> 
	all other evts -> params set up, but no layers 

*/
enum {
	PF_Event_NONE = -1,
	PF_Event_NEW_CONTEXT,
	PF_Event_ACTIVATE,
	PF_Event_DO_CLICK,
	PF_Event_DRAG,
	PF_Event_DRAW,
	PF_Event_DEACTIVATE,
	PF_Event_CLOSE_CONTEXT,
	PF_Event_IDLE,
	PF_Event_KEYDOWN,
	PF_Event_ADJUST_CURSOR,		// new for AE 4.0, sent when mouse moves over custom UI
	
	PF_Event_NUM_EVENTS
};
typedef long PF_EventType;


enum {
	PF_Cursor_NONE = 0,			// see comment in PF_AdjustCursorEventInfo
	PF_Cursor_CUSTOM,			// means effect set cursor itself with platform-specific calls
	PF_Cursor_ARROW,
	PF_Cursor_HOLLOW_ARROW,
	PF_Cursor_WATCH_N_WAIT,		// watch on the Mac, wait (hourglass) on Windows
	PF_Cursor_MAGNIFY,
	PF_Cursor_MAGNIFY_PLUS,
	PF_Cursor_MAGNIFY_MINUS,
	PF_Cursor_CROSSHAIRS,
	PF_Cursor_CROSS_RECT,
	PF_Cursor_CROSS_OVAL,
	PF_Cursor_CROSS_ROTATE,
	PF_Cursor_PAN,
	PF_Cursor_EYEDROPPER,
	PF_Cursor_HAND,
	PF_Cursor_PEN,
	PF_Cursor_PEN_ADD,
	PF_Cursor_PEN_DELETE,
	PF_Cursor_PEN_CLOSE,
	PF_Cursor_PEN_DRAG,
	PF_Cursor_PEN_CORNER,
	PF_Cursor_RESIZE_VERTICAL,
	PF_Cursor_RESIZE_HORIZONTAL,
	PF_Cursor_FINGER_POINTER,
	PF_Cursor_SCALE_HORIZ, 
	PF_Cursor_SCALE_DIAG_LR, 
	PF_Cursor_SCALE_VERT, 
	PF_Cursor_SCALE_DIAG_UR, 
	PF_Cursor_ROT_TOP, 
	PF_Cursor_ROT_TOP_RIGHT, 
	PF_Cursor_ROT_RIGHT, 
	PF_Cursor_ROT_BOT_RIGHT, 
	PF_Cursor_ROT_BOTTOM, 
	PF_Cursor_ROT_BOT_LEFT, 
	PF_Cursor_ROT_LEFT, 
	PF_Cursor_ROT_TOP_LEFT, 
	PF_Cursor_DRAG_CENTER,
	PF_Cursor_COPY, 
	PF_Cursor_ALIAS, 
	PF_Cursor_CONTEXT, 
	PF_Cursor_SLIP_EDIT, 
	PF_Cursor_ORBIT, 
	PF_Cursor_TRACK_XY, 
	PF_Cursor_TRACK_Z, 
	PF_Cursor_ROTATE_X, 
	PF_Cursor_ROTATE_Y, 
	PF_Cursor_ROTATE_Z, 
	PF_Cursor_ARROW_X, 
	PF_Cursor_ARROW_Y, 
	PF_Cursor_ARROW_Z, 
	PF_Cursor_SCISSORS, 
	PF_Cursor_FAT_EYEDROPPER, 
	PF_Cursor_FINGER_POINTER_SCRUB, 

	PF_MAX_CURSOR_PLUS_ONE
};
typedef long PF_CursorType;


enum {
	PF_Mod_NONE					= 0x0000,
	PF_Mod_CMD_CTRL_KEY			= 0x0100,		// cmd on Mac, ctrl on Windows
	PF_Mod_SHIFT_KEY			= 0x0200,
	PF_Mod_CAPS_LOCK_KEY		= 0x0400,
	PF_Mod_OPT_ALT_KEY			= 0x0800,		// option on Mac, alt on Windows
	PF_Mod_MAC_CONTROL_KEY		= 0x1000		// Mac control key only
};
typedef long PF_Modifiers;


typedef struct {
	PF_Point			screen_point;		/* >> where the mouse is right now */
	PF_Modifiers		modifiers;			/* >> modifiers right now */
	PF_CursorType		set_cursor;			/* << set this to your desired cursor, or PF_Cursor_CUSTOM if you
													set the cursor yourself, or PF_Cursor_NONE if you don't
													want to override the cursor (i.e. the app will set the
													cursor however it wants) */
} PF_AdjustCursorEventInfo;

typedef struct {
	unsigned long		when;
	PF_Point			screen_point;
	long				num_clicks;
	PF_Modifiers		modifiers;
	long				continue_refcon[4];	/* <> if send_drag is TRUE, set this */
	PF_Boolean			send_drag;			/* << set this from a do_click to get a drag */
	PF_Boolean			last_time;			/* >> set the last time you get a drag */
} PF_DoClickEventInfo;


typedef struct {
	PF_Rect		update_rect;		// in window's coordinate system
	long		depth;
	void		*gdeviceH;
} PF_DrawEventInfo;


typedef struct {					// effect should only eat keys when modal!
	unsigned long		when;
	PF_Point			screen_point;
	long				char_code;
	long				key_code;
	PF_Modifiers		modifiers;
} PF_KeyDownEvent;


typedef union {
	PF_DoClickEventInfo			do_click;		// also drag
	PF_DrawEventInfo			draw;
	PF_KeyDownEvent				key_down;
	PF_AdjustCursorEventInfo	adjust_cursor;

	// add other event types here
		
} PF_EventUnion;


#define			PF_CONTEXT_MAGIC		0x05ea771e
typedef struct {
	unsigned long		magic;
	PF_WindowType		w_type;
	void				*cgrafptr;
	long				reserved_flt;
	long				plugin_state[4];	// plug-in specific data
	
} PF_Context, *PF_ContextPtr, **PF_ContextH;


typedef struct {
	void		*refcon;			// front-end's refcon
	
	PF_Err		(*layer_to_comp)(void *refcon, 			/* >> */
								PF_ContextH context,	/* >> */
								long curr_time,			/* >> */
								long time_scale,		/* >> */
								PF_FixedPoint *pt);		/* << */
								
	PF_Err		(*comp_to_layer)(void *refcon, 			/* >> */
								PF_ContextH context, 	/* >> */
								long curr_time, 		/* >> */
								long time_scale,	 	/* >> */
								PF_FixedPoint *pt);		/* << */

	PF_Err		(*get_comp2layer_xform)(void *refcon, 	/* >> */
								PF_ContextH context,	/* >> */
								long curr_time, 		/* >> */
								long time_scale, 		/* >> */
								long *exists,			/* << non-zero if exists */
								PF_FloatMatrix *c2l);	/* << */
								
	PF_Err		(*get_layer2comp_xform)(void *refcon,	/* >> */
								PF_ContextH context, 	/* >> */
								long curr_time, 		/* >> */
								long time_scale, 		/* >> */
								PF_FloatMatrix *l2c);	/* << */
	
	PF_Err		(*source_to_frame)(void *refcon, PF_ContextH context, PF_FixedPoint *pt);
	PF_Err		(*frame_to_source)(void *refcon, PF_ContextH context, PF_FixedPoint *pt);

	PF_Err		(*info_draw_color)(void *refcon, PF_Pixel color);

	// 2 lines of text, same as calling PF_InfoDrawText3( line1Z0, line2Z0, NULL)
	PF_Err		(*info_draw_text)(void *refcon, const char *text1Z0, const char *text2Z0);	// Cstrings
	PF_Err		(*info_get_port)(void *refcon, void **cgrafptr_addr);

	// Due to this structure's containment within PF_EventExtra,
	// we are unable to add new functions to this structure in order
	// to remain backwards compatible.  In other words, do not add any
	// new functions here, add them to the PF_AdvAppSuite1 suite
	// within AE_AdvEffectSuites.h. -jja 10-24-2000
	
} PF_EventCallbacks, *PF_EventCallbacksPtr;

enum {
	PF_EA_NONE = 0,
	PF_EA_PARAM_TITLE,
	PF_EA_CONTROL
};
typedef	long	PF_EffectArea;

typedef struct {
	PF_ParamIndex			index;
	PF_EffectArea			area;
	PF_Rect					current_frame;		// full frame of the current area
	PF_Rect					param_title_frame;	// full frame of the param title area
 	long					horiz_offset;		// h offset to draw into title
} PF_EffectWindowInfo;

enum {
	PF_EO_NONE				= 0,
	PF_EO_HANDLED_EVENT		= (1L << 0)
};
typedef	long	PF_EventOutFlags;

enum {
	PF_EI_NONE = 0,
	PF_EI_DONT_DRAW			= 1L << 0		// don't draw controls in comp or layer window
};
typedef	long	PF_EventInFlags;

//	this info is passed for ALL event types, except those in the Effect Control Window
//	in ECW, you get the PF_EffectWindowInfo, during all events
typedef struct {
	PF_Rect					port_rect;
} PF_ItemWindowInfo;

/*
	to know what union to take, look in 
	(**contextH).w_type.  if == effect win, you get effect win, else you get item win
*/
typedef union {
	PF_EffectWindowInfo		effect_win;			/* >> only for Effect window do_click, draw, and adjust-cursor */
	PF_ItemWindowInfo		item_win;			/* >> comp or layer		*/
} PF_WindowUnion;

//	uncomment if you want to get the port rect during most events
//	#define	PF_USE_NEW_WINDOW_UNION
typedef struct {
	PF_ContextH				contextH;			/* >> */
	PF_EventType			e_type;				/* >> */
	PF_EventUnion			u;					/* >> based on e_type */
	
	#ifdef PF_USE_NEW_WINDOW_UNION
		PF_WindowUnion			window_union;		/* >> union of window-specific handy information	*/
	#else
		PF_EffectWindowInfo		effect_win;			/* >> only for Effect window do_click, draw, and adjust-cursor */
	#endif

	PF_EventCallbacks		cbs;				/* >> not for new_context or close_context */
	PF_EventInFlags			evt_in_flags;		/* >> */
	PF_EventOutFlags		evt_out_flags;		/* << */
} PF_EventExtra;

enum {
	PF_UIAlignment_NONE = 0,
	PF_UIAlignment_TOP = 1L << 0,
	PF_UIAlignment_LEFT = 1L << 1,
	PF_UIAlignment_BOTTOM = 1L << 2,
	PF_UIAlignment_RIGHT = 1L << 3
};

typedef long PF_UIAlignment;

struct _PF_CustomUIInfo {
	
	long					reserved;
	PF_CustomEventFlags		events;

	long				comp_ui_width;
	long				comp_ui_height;
	PF_UIAlignment		comp_ui_alignment;
	
	long				layer_ui_width;
	long				layer_ui_height;
	PF_UIAlignment		layer_ui_alignment;

	long				preview_ui_width;			// unused
	long				preview_ui_height;			// unused
	PF_UIAlignment		preview_ui_alignment;		// unused

};


#ifdef __cplusplus
}
#endif


#if PRAGMA_STRUCT_ALIGN
	#pragma options align=reset
#endif

#ifdef _WINDOWS
	#pragma pack( pop, AEalign)
#endif


#endif /* _H_AE_EffectUI */