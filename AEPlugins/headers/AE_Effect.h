/** AE_Effect.h

	Adobe After Effects effect plug-in header file

	Copyright (c) 1992-1998 Adobe Systems Incorporated
	All Rights Reserved

	CONTENTS
		Version Information
		Constants And Enumerations
		Output Flags
		Input Flags
		Command Selectors
		Simple Types
		Pixel Access Macros
		Effect Parameter Description Structures
		Interaction Callbacks
		Effect Parameter Blocks
		Effect Prototype

	NOTES
		It may be easiest to start reading this file at the bottom
		and work your way, section by section, to the top.  Of course,
		the best thing to do is start with the sample filter and the
		work your way from that back through the goodies in this file.

		All strings in this spec are NULL-terminated (C strings).

**/

#ifndef	_H_AE_Effect
#define	_H_AE_Effect


#if PRAGMA_STRUCT_ALIGN
#pragma options align=mac68k
#endif

#ifdef _WINDOWS
	#pragma pack( push, AEalign, 2 )
#endif



#ifdef __cplusplus
	extern "C" {
#endif

/** -------------------- Version Information ----------------------------------
 **
 ** Please use these macros for designating the version information
 ** of your plug-ins. Should After Effects encounter more than one version
 ** of a specific plug-in when it starts up, it will use this information
 ** to decide which plug-in to honor. The plug-in version information
 ** (field 'my_version' in the PF_OutData) should be set at GLOBAL_SETUP
 ** time.
 **
 ** This version information is meant for your version control, and should
 ** not be confused with the min_version and desired_version fields in the
 ** PF_OutData structure, which refer to the version of the PF specification.
 ** 
 **/

#define PF_Vers_BUILD_BITS		0x1ffL
#define PF_Vers_BUILD_SHIFT		0
#define PF_Vers_STAGE_BITS		0x3L
#define PF_Vers_STAGE_SHIFT		9
#define PF_Vers_BUGFIX_BITS		0xfL
#define PF_Vers_BUGFIX_SHIFT	11
#define PF_Vers_SUBVERS_BITS	0xfL
#define PF_Vers_SUBVERS_SHIFT	15
#define PF_Vers_VERS_BITS		0x7L
#define PF_Vers_VERS_SHIFT		19


#define PF_VERSION(vers, subvers, bugvers, stage, build)	\
	(unsigned long)(	\
		((((unsigned long)(vers)) & PF_Vers_VERS_BITS) << PF_Vers_VERS_SHIFT) |	\
		((((unsigned long)(subvers)) & PF_Vers_SUBVERS_BITS)<<PF_Vers_SUBVERS_SHIFT) |\
		((((unsigned long)(bugvers)) & PF_Vers_BUGFIX_BITS) << PF_Vers_BUGFIX_SHIFT) |\
		((((unsigned long)(stage)) & PF_Vers_STAGE_BITS) << PF_Vers_STAGE_SHIFT) |	\
		((((unsigned long)(build)) & PF_Vers_BUILD_BITS) << PF_Vers_BUILD_SHIFT)	\
	)

#define PF_Version_VERS(vers)		\
	((((unsigned long) vers) >> PF_Vers_VERS_SHIFT) & PF_Vers_VERS_BITS)

#define PF_Version_SUBVERS(vers)	\
	((((unsigned long) vers) >> PF_Vers_SUBVERS_SHIFT) & PF_Vers_SUBVERS_BITS)

#define PF_Version_BUGFIX(vers)	\
	((((unsigned long) vers) >> PF_Vers_BUGFIX_SHIFT) & PF_Vers_BUGFIX_BITS)

#define PF_Version_STAGE(vers)	\
	((((unsigned long) vers) >> PF_Vers_STAGE_SHIFT) & PF_Vers_STAGE_BITS)

#define PF_Version_BUILD(vers)	\
	((((unsigned long) vers) >> PF_Vers_BUILD_SHIFT) & PF_Vers_BUILD_BITS)


enum {
	PF_Stage_DEVELOP,
	PF_Stage_ALPHA,
	PF_Stage_BETA,
	PF_Stage_RELEASE
};
typedef long PF_Stage;






/** -------------------- Constants And Enumerations --------------------
**/

#define PF_PLUG_IN_VERSION					12
#define PF_PLUG_IN_SUBVERS					10

#define PF_AE41_PLUG_IN_VERSION				12
#define PF_AE41_PLUG_IN_SUBVERS				2

#define PF_AE40_PLUG_IN_VERSION				12
#define PF_AE40_PLUG_IN_SUBVERS				1

#define PF_AE31_PLUG_IN_VERSION				11
#define PF_AE31_PLUG_IN_SUBVERS				6
#define PF_AE31_PLUG_IN_SUBVERS_STRICTIFY	8

/* Note:	AE3.1 will drive any v11.x plugin
			AE4.0 will drive any v11.x or v12.x plugin
			AE4.1 will drive any v11.x or v12.x plugin, changed current version from 12.1 -> 12.2
			
			If you use PF_AE31_PLUG_IN_VERSION, PF_AE31_PLUG_IN_SUBVERS_STRICTIFY
			or later (e.g. 11.8 or 12.0) AE4.0 will enforce new stricter rules and
			report more plug-in errors.
*/


#define PF_MAX_EFFECT_NAME_LEN				31
#define PF_MAX_EFFECT_CATEGORY_NAME_LEN		31
#define PF_MAX_EFFECT_PARAM_NAME_LEN		31
#define PF_MAX_PARAM_DESCRIPTION_LEN		31
#define PF_MAX_PARAM_VALUE_LEN				31
#define PF_MAX_EFFECT_MSG_LEN				255

#define	PF_MAX_NUM_EFFECT_PARAMS			127

#define PF_MAX_WORLD_WIDTH					30000
#define PF_MAX_WORLD_HEIGHT					30000

#define	PF_FIRST_ERR						512


/* Standard effect categories for After Effects effects modules.
 * These need to be set in the PiPL.
 */
#define PF_Category_BLUR_AND_SHARPEN	"Blur & Sharpen"
#define PF_Category_CHANNEL				"Channel"
#define PF_Category_DISTORT				"Distort"
#define PF_Category_IMAGE_CONTROL		"Image Control"
#define PF_Category_KEYING				"Keying"
#define PF_Category_PERSPECTIVE			"Perspective"
#define PF_Category_STYLIZE				"Stylize"
#define PF_Category_TEXT				"Text"
#define PF_Category_VIDEO				"Video"
#define PF_Category_TRANSITION			"Transition"
#define PF_Category_AUDIO				"Audio"
#define PF_Category_OTHER				"Other"	

enum {
	PF_Quality_DRAWING_AUDIO = -1,		// only used to inform audio plugins that the output
										// will be used to draw the waveform; the plugin should
										// not perform any filtering operations that may weaken
										// the amplitude when rendering at low sampling rates

	PF_Quality_LO = 0,					// LO & HI are sent to both audio & visual effects
	PF_Quality_HI
};
typedef long PF_Quality;

enum {
	PF_MF_Alpha_PREMUL 		= 0,
	PF_MF_Alpha_STRAIGHT 	= (1L << 0)
};
typedef long PF_ModeFlags;

#define			PF_ALPHA_PREMUL(F) 		(((F) & 0x00000001) == 0)
#define			PF_ALPHA_STRAIGHT(F)	(((F) & 0x00000001))

enum {
	PF_Field_FRAME = 0L,
	PF_Field_UPPER = 1L,
	PF_Field_LOWER = 2L
};
typedef long PF_Field;



/* PF_ParamType
 */
enum {
	PF_Param_LAYER,
	PF_Param_SLIDER,
	PF_Param_FIX_SLIDER,
	PF_Param_ANGLE,
	PF_Param_CHECKBOX,
	PF_Param_COLOR,
	PF_Param_POINT,
	PF_Param_POPUP,
	PF_Param_CUSTOM,	/* obsolete */
	PF_Param_NO_DATA,	/* used for CustomUI in Effect window controls, with NO data stream */
	PF_Param_FLOAT_SLIDER,
	PF_Param_ARBITRARY_DATA,	// must combine with either PF_PUI_TOPIC/PF_PUI_CONTROL or PF_PUI_NO_ECW
	PF_Param_PATH,
	PF_Param_GROUP_START,
	PF_Param_GROUP_END
};
typedef long PF_ParamType;


/* PF_ParamFlags

	These flags are passed when adding a param (using PF_ADD_PARAM) to specify
	some details about how the param can be used.  The flags
	are:

	PF_ParamFlag_CANNOT_TIME_VARY
		If this is passed, the parameter will not be allowed to vary
		over time -- no keyframe controller will appear at the right.

	PF_ParamFlag_CANNOT_INTERP
		If this is passed, parameter values are not interpolated
		between.  You can still use no interp and discontinuous interp.
	
	PF_ParamFlag_COLLAPSE_TWIRLY / PF_ParamFlag_START_COLLAPSED
		Set this flag if you want the parameter's twirly arrow in the
		Effect Control Window to be twirled up by default when the
		effect is first applied.  New in AE 4.0: you can now set &
		clear this bit when handling PF_Cmd_UPDATE_PARAMS_UI and
		PF_Cmd_USER_CHANGED_PARAM messages, so as to twirl your
		parameters and groups up and down at will.

	PF_ParamFlag_SUPERVISE
		If this is passed, PF_Cmd_USER_CHANGED_PARAM will be sent when
		this parameter changes.

**/
enum {
	PF_ParamFlag_RESERVED1			= 1 << 0,
	PF_ParamFlag_CANNOT_TIME_VARY	= 1 << 1,		/* can't vary over time */
	PF_ParamFlag_CANNOT_INTERP		= 1 << 2,		/* can only vary discontinuously */
	PF_ParamFlag_RESERVED2			= 1 << 3,		/* was _old_ PF_ParamFlag_WANTS_UPDATE value, never used */
	PF_ParamFlag_RESERVED3			= 1 << 4,		/* was _old_ PF_ParamFlag_SEPARATE, now use PF_PUI_ECW_SEPARATOR */
	PF_ParamFlag_COLLAPSE_TWIRLY	= 1 << 5,		/* controls the twirl-state of the twirly-arrow in the ECW (dynamic) */
	PF_ParamFlag_SUPERVISE			= 1 << 6,		/* call me with PF_Cmd_USER_CHANGED_PARAM (new in AE 4.0) */
	
	PF_ParamFlag_START_COLLAPSED = PF_ParamFlag_COLLAPSE_TWIRLY	/* when first applied, param comes up collapsed */
};
typedef long PF_ParamFlags;


enum {
	PF_Err_NONE = 0,
	PF_Err_OUT_OF_MEMORY = 4,
	PF_Err_INTERNAL_STRUCT_DAMAGED = PF_FIRST_ERR,
	PF_Err_INVALID_INDEX,			/* out of range, or action not allowed on this index */
	PF_Err_UNRECOGNIZED_PARAM_TYPE,
	PF_Err_INVALID_CALLBACK,
	PF_Err_BAD_CALLBACK_PARAM,
	PF_Interrupt_CANCEL,				/* Returned when user interrupts rendering */
	PF_Err_CANNOT_PARSE_KEYFRAME_TEXT	/* Returned from PF_Arbitrary_SCAN_FUNC when effect
											cannot parse arbitrary data from text */
};
typedef	long PF_Err;


enum {
	PF_WorldFlag_DEEP 	   		= 1L << 0,
	PF_WorldFlag_WRITEABLE 		= 1L << 1,

	PF_WorldFlag_RESERVED		= 1L << 31
};

typedef long PF_WorldFlags;

#define PF_WORLD_IS_DEEP(W)		( ((W)->world_flags & PF_WorldFlag_DEEP) != 0 )

#define PF_MAX_CHAN8			255
#define PF_HALF_CHAN8			128
#define PF_MAX_CHAN16			32768
#define PF_HALF_CHAN16			16384



/** -------------------- Output Flags --------------------

	The out_flags field of the OutData can be set to an OR-ed
	combination of these flags to communicate various things to
	the driver program.  The flags are described here:

	PF_OutFlag_NONE
		This is the "empty" setting -- no outflags.

	PF_OutFlag_WIDE_TIME_INPUT
		Set this flag if the effect calls get_param to inquire a
		parameter at a time besides the current one (e.g. to get
		the previous video frame). This should be sent, if it is
		going to be sent, at PF_Cmd_GLOBAL_SETUP.

	PF_OutFlag_NON_PARAM_VARY
		Set this if the effect uses information other than the parameters
		in the param list to generate its output at the current time.
		For instance, if the effect uses the current time of the frame
		or some random value to decide the output, set this flag.  This
		flag should be sent at PF_Cmd_GLOBAL_SETUP.  If the effect
		produces changing frames when applied to a still image and
		all parameters are constant, that's a sure sign that this bit
		should be set (e.g. Wave Warp).	Can be over-ridden dynamically 
		during PF_Cmd_QUERY_DYNAMIC_FLAGS.

	PF_OutFlag_SEQUENCE_DATA_NEEDS_FLATTENING
		When you allocate a sequence data handle, the app 
		may write the handle out to disk and reuse it  
		later. Pass this flag if the handle is not "flat" 
		(i.e. has pointers or handles hanging off of it).  
		Basically, this gives you a chance to alter the 
		handle contents before it is written out to disk, 
		so  you won't get invalid handles or pointers. Once 
		you have flattened a handle, you will get an  
		opportunity to un-flatten it before the effect 
		needs to continue. For sequence data, you will be  
		invoked with a PF_Cmd_SEQUENCE_RESETUP call. You 
		should store a boolean at a common  offset in your 
		unflattened and flattened data that says whether 
		the data is flat or not. If you get a  
		PF_Cmd_SEQUENCE_RESETUP and the boolean indicated 
		the data is flattened, you should  unflatten the 
		data, free the flattened data handle, and set the 
		sequence_data handle in the  PF_OutData. If you 
		ever set the data to NULL when you flatten it, you 
		will NOT get the sequence  resetup call to 
		unflatten it. Instead, you may just get a RENDER 
		call with NULL data. Forewarned is  forearmed. This 
		flag, indicating if the data will need to be 
		flattened, should be set at  PF_Cmd_GLOBAL_SETUP time.  
	
	PF_OutFlag_I_DO_DIALOG
		Set this is the effect responds to a PF_Cmd_DO_DIALOG, i.e. Does this
		effect bring up an options dialog box.  PF_Cmd_DO_DIALOG is generated
		when the user presses the Options button on the Effect floater.
		This flag should be set at PF_Cmd_GLOBAL_SETUP time.

	PF_OutFlag_USE_OUTPUT_EXTENT
		The output layer is passed with an "extent rect" indicating
		the area of the layer that actually contains visible image data.  If
		the effect changes its behavior based on the extent rect (for instance,
		by not iterating over the entire image), set this flag, so the
		application will know whether having the extent	change should cause
		the frame to re-render.  Specify this flag at PF_Cmd_GLOBAL_SETUP.

	PF_OutFlag_SEND_DO_DIALOG
		Some filters need their options dialog box to be brought up at least
		once to be valid.  You can set this flag, and the driver app will
		automatically send a PF_Cmd_DO_DIALOG to the effect when it is applied.
		The DO_DIALOG will be sent after PF_Cmd_SEQUENCE_SETUP.  This flag
		should be set in PF_Cmd_SEQUENCE_SETUP if it is going to be set.

	PF_OutFlag_DISPLAY_ERROR_MESSAGE
		Whenever the return_msg field in the PF_OutData is set to a string,
		After Effects will bring up a simple dialog box containing that
		string.  If you set this flag, the dialog box will be made to look
		like an error message dialog box.  If you don't set this flag, it
		will be an undecorated dialog box.  Using this flag, an effects module
		can have and display its own error messages and not worry about the
		code for dialog boxes -- the program will do it for you.
		This flag can be sent after any command.
		
	PF_OutFlag_I_EXPAND_BUFFER
		Starting with After Effects 2.0, effects will be able to expand their buffers
		beyond the current layer's dimensions. This has always been part of the
		PF specification, but as an extra precaution (and hint to the AE rendering
		engine) set this flag at PF_Cmd_GLOBAL_SETUP if you plan to expand your
		buffer.
	
	PF_OutFlag_I_SHRINK_BUFFER
		Set this flag if you can shrink your buffer based on the extent-rects passed
		to you in order to be more memory efficient.

	PF_OutFlag_PIX_INDEPENDENT
		Set this flag if the output at a given pixel is not dependent on the values
		of the pixels around it. If this is set, the pixels After Effects does not
		care about (because of field rendering, for example) could be filled with garbage
		colors.  Please set this flag at PF_Cmd_GLOBAL_SETUP. 

	PF_OutFlag_I_WRITE_INPUT_BUFFER
		Set this flag if your effect would like to write into the input buffer. This
		can be useful if you need an scratch buffer, but it also invalidates some speedups
		in the AE rendering pipeline, so use it with some discretion. Please set this
		flag at PF_Cmd_GLOBAL_SETUP.

	PF_OutFlag_KEEP_RESOURCE_OPEN
		
		Set this flag if your effect expects its Macintosh resource fork to be open 
		at any time other than global setup.  Note that this does not mean that
		the resoure fork will be kept open at all times, just whenever the
		effect is being executed.

	PF_OutFlag_NOP_RENDER
	
		At PF_Cmd_FRAME_SETUP time, if the render would result in no changes to the source
		image ($$$ or audio?), set this flag to save some rendering time.
		
		Should mean that PF_Cmd_RENDER is never sent, but in fact this flag is currently ignored!

	PF_OutFlag_CUSTOM_UI

		This flag must be set if your effect has a custom UI in the Effect Controls
		Window, Layer Window or Comp Window.

	PF_OutFlag_REFRESH_UI

		Can be returned from PF_Cmd_EVENT, PF_Cmd_RENDER, and PF_Cmd_DO_DIALOG.
		Causes the effects control window, layer window, and comp window to be re-drawn.
	
	PF_OutFlag_I_USE_SHUTTER_ANGLE
		
		Must be set at PF_Cmd_GLOBAL_SETUP time if the effect uses
		the shutter_angle or the shutter_phase. Can be over-ridden dynamically during 
		PF_Cmd_QUERY_DYNAMIC_FLAGS.

	PF_OutFlag_I_USE_AUDIO
		
		Must be set at PF_Cmd_GLOBAL_SETUP time for a visual effect
		that calls the audio checkout calls.

	PF_OutFlag_I_AM_OBSOLETE

		Set at PF_Cmd_GLOBAL_SETUP time for effects that don't
		want to appear in the AE Effects menu (but will still be invoked
		if you load a project that has an old copy of the effect applied).

	PF_OutFlag_FORCE_RERENDER
	
		Set at PF_Cmd_EVENT if the effect modified sequence data,
		or did anything else that requires the effect needs to re-render.
		Note that setting PF_ChangeFlag_CHANGED_VALUE automatically
		causes a re-render, so don't worry about setting PF_OutFlag_FORCE_RERENDER
		in that case.

	PF_OutFlag_PiPL_OVERRIDES_OUTDATA_OUTFLAGS

		Valid only for setting in your PiPL.  When set out_flags will be
		ignored at PF_Cmd_GLOBAL_SETUP time (& thus don't need to match).

	PF_OutFlag_I_HAVE_EXTERNAL_DEPENDENCIES

		Set this flag at PF_Cmd_GLOBAL_SETUP time if the effect has dependencies
		that the user should know about before transporting their project to a
		different machine.  For example, dependencies on an installed font,
		or on an external file.  If set, the effect will receive a
		PF_Cmd_GET_EXTERNAL_DEPENDENCIES request, where the extra
		param will be a PF_ExtDependenciesExtra, and the effect should
		report its information based on the given sequence_data.

	PF_OutFlag_SEND_UPDATE_PARAMS_UI

		Set this flag at PF_Cmd_GLOBAL_SETUP time if you want to receive
		PF_Cmd_UPDATE_PARAMS_UI messages.

	PF_OutFlag_AUDIO_FLOAT_ONLY

		Set this flag if you only want to receive PF_SIGNED_FLOAT data
		when processing audio data.  Requires PF_OutFlag_AUDIO_EFFECT_TOO
		or PF_OutFlag_AUDIO_EFFECT_ONLY.

	PF_OutFlag_AUDIO_IIR

		Set this flag at PF_Cmd_GLOBAL_SETUP time if you are an
		Infinite-Impulse-Response audio filter (i.e. your output at a given
		time depends on your output from previous times).

	PF_OutFlag_I_SYNTHESIZE_AUDIO

		Set this flag at PF_Cmd_GLOBAL_SETUP time if you generate
		audio even when handed silence.  Requires PF_OutFlag_AUDIO_EFFECT_TOO
		or PF_OutFlag_AUDIO_EFFECT_ONLY.

	PF_OutFlag_AUDIO_EFFECT_TOO

		Must be set at PF_Cmd_GLOBAL_SETUP time for an effect that
		wants to filter the audio too (as opposed to just reading the audio).
		
	PF_OutFlag_AUDIO_EFFECT_ONLY

		Must be set at PF_Cmd_GLOBAL_SETUP time for an effect
		that only filters audio (no video).

	PF_OutFlag2_SUPPORTS_QUERY_DYNAMIC_FLAGS

		Set this during PF_Cmd_GLOBAL_SETUP if the effect handles PF_Cmd_QUERY_DYNAMIC_FLAGS. 
		Supporting this command can dramatically improve performance for certain 
		effects, because it provides dynamic information to the host about what 
		can be cached (as opposed to PIPL bits which cannot be changed at run-time)

	PF_OutFlag2_I_USE_3D_CAMERA
		
		This bit must be set if the effect ever uses the AEGP PF_Interface suite to
		access camera layers. Can be over-ridden dynamically during PF_Cmd_QUERY_DYNAMIC_FLAGS.

	PF_OutFlag2_I_USE_3D_LIGHTS
		
		This bit must be set if the effect ever uses the AEGP PF_Interface suite to
		access camera layers. Can be over-ridden dynamically during PF_Cmd_QUERY_DYNAMIC_FLAGS.


**/

enum {
	PF_OutFlag_NONE = 0L,

																// which PF_Cmds each flag is relevant for:
	PF_OutFlag_KEEP_RESOURCE_OPEN				= 1L << 0,		// PF_Cmd_GLOBAL_SETUP
	PF_OutFlag_WIDE_TIME_INPUT					= 1L << 1,		// PF_Cmd_GLOBAL_SETUP, PF_Cmd_QUERY_DYNAMIC_FLAGS
	PF_OutFlag_NON_PARAM_VARY					= 1L << 2,		// PF_Cmd_GLOBAL_SETUP, PF_Cmd_QUERY_DYNAMIC_FLAGS
	PF_OutFlag_RESERVED6						= 1L << 3,
	PF_OutFlag_SEQUENCE_DATA_NEEDS_FLATTENING	= 1L << 4,		// PF_Cmd_GLOBAL_SETUP
	PF_OutFlag_I_DO_DIALOG						= 1L << 5,		// PF_Cmd_GLOBAL_SETUP
	PF_OutFlag_USE_OUTPUT_EXTENT				= 1L << 6,		// PF_Cmd_GLOBAL_SETUP
	PF_OutFlag_SEND_DO_DIALOG					= 1L << 7,		// PF_Cmd_SEQUENCE_SETUP
	PF_OutFlag_DISPLAY_ERROR_MESSAGE			= 1L << 8,		// all PF_Cmds
	PF_OutFlag_I_EXPAND_BUFFER					= 1L << 9,		// PF_Cmd_GLOBAL_SETUP
	PF_OutFlag_PIX_INDEPENDENT					= 1L << 10,		// PF_Cmd_GLOBAL_SETUP
	PF_OutFlag_I_WRITE_INPUT_BUFFER				= 1L << 11,		// PF_Cmd_GLOBAL_SETUP
	PF_OutFlag_I_SHRINK_BUFFER					= 1L << 12,		// PF_Cmd_GLOBAL_SETUP
	PF_OutFlag_WORKS_IN_PLACE					= 1L << 13,		// PF_Cmd_GLOBAL_SETUP
	PF_OutFlag_RESERVED8						= 1L << 14,
	PF_OutFlag_CUSTOM_UI						= 1L << 15,		// PF_Cmd_GLOBAL_SETUP
	PF_OutFlag_RESERVED7						= 1L << 16,
	PF_OutFlag_REFRESH_UI						= 1L << 17,		// PF_Cmd_EVENT, PF_Cmd_RENDER, PF_Cmd_DO_DIALOG
	PF_OutFlag_NOP_RENDER						= 1L << 18,		// PF_Cmd_FRAME_SETUP
	PF_OutFlag_I_USE_SHUTTER_ANGLE				= 1L << 19,		// PF_Cmd_GLOBAL_SETUP, PF_Cmd_QUERY_DYNAMIC_FLAGS
	PF_OutFlag_I_USE_AUDIO						= 1L << 20,		// PF_Cmd_GLOBAL_SETUP
	PF_OutFlag_I_AM_OBSOLETE					= 1L << 21,		// PF_Cmd_GLOBAL_SETUP
	PF_OutFlag_FORCE_RERENDER					= 1L << 22,		// PF_Cmd_EVENT, PF_Cmd_USER_CHANGED_PARAM, PF_Cmd_UPDATE_PARAMS_UI
	PF_OutFlag_PiPL_OVERRIDES_OUTDATA_OUTFLAGS	= 1L << 23,		// PiPL-only-flag
	PF_OutFlag_I_HAVE_EXTERNAL_DEPENDENCIES		= 1L << 24,		// PF_Cmd_GLOBAL_SETUP
	PF_OutFlag_DEEP_COLOR_AWARE					= 1L << 25,		// PF_Cmd_GLOBAL_SETUP
	PF_OutFlag_SEND_UPDATE_PARAMS_UI			= 1L << 26,		// PF_Cmd_GLOBAL_SETUP

	// audio flags (PF_OutFlag_AUDIO_EFFECT_TOO or PF_OutFlag_AUDIO_EFFECT_ONLY required for audio effects)
	PF_OutFlag_AUDIO_FLOAT_ONLY					= 1L << 27,		// PF_Cmd_GLOBAL_SETUP
	PF_OutFlag_AUDIO_IIR						= 1L << 28,		// PF_Cmd_GLOBAL_SETUP
	PF_OutFlag_I_SYNTHESIZE_AUDIO				= 1L << 29,		// PF_Cmd_GLOBAL_SETUP
	PF_OutFlag_AUDIO_EFFECT_TOO					= 1L << 30,		// PF_Cmd_GLOBAL_SETUP
	PF_OutFlag_AUDIO_EFFECT_ONLY				= 1L << 31		// PF_Cmd_GLOBAL_SETUP
};
typedef long PF_OutFlags;

enum {
	PF_OutFlag2_NONE = 0L,
																// which PF_Cmds each flag is relevant for:
	PF_OutFlag2_SUPPORTS_QUERY_DYNAMIC_FLAGS	= 1L << 0,		// PF_Cmd_GLOBAL_SETUP
	PF_OutFlag2_I_USE_3D_CAMERA					= 1L << 1,		// PF_Cmd_GLOBAL_SETUP, PF_Cmd_QUERY_DYNAMIC_FLAGS
	PF_OutFlag2_I_USE_3D_LIGHTS					= 1L << 2		// PF_Cmd_GLOBAL_SETUP, PF_Cmd_QUERY_DYNAMIC_FLAGS
};
typedef long PF_OutFlags2;


enum {
	PF_InFlag_NONE = 0L
};
typedef long PF_InFlags;





/** -------------------- Command Selectors --------------------

	The first parameter to the effect routine is one of these command
	selectors.  The commands are described here.

	PF_Cmd_ABOUT
		This command should display an information dialog box about the
		effect module.  The easiest thing to do is PF_SPRINTF the info
		into the out_data->return_msg field.  After Effects will bring
		up a simple undecorated modal dialog with your text proudly displayed.
		This command can be sent at _any_ time, so don't count on having
		any global data or anything else set.  (Except, as always, the
		current resource file will be set to your effects module.)

	PF_Cmd_GLOBAL_SETUP
		When you get this command, you should check the version of the
		effect protocol with which you are being invoked, and set any of
		the necessary out flags (described above) or out data fields
		(described below).  If your global data was flattened, the flat
		data will be passed here and you should unflatten it, free the
		flat global data, and set the OutData global_data to the new un-flat
		data.  Alternatively, the global data may come in NULL and you can
		allocate new global data at this time.

	PF_Cmd_GLOBAL_SETDOWN
		You should free any global data you have allocated when you get
		this command.

	PF_Cmd_PARAMS_SETUP
		Here you should add any params your effect wants using the
		PF_ADD_PARAM callback described below.  This is called after global
		setup -- see the add_param callback described below.

	PF_Cmd_SEQUENCE_SETUP
		This is called when the effect is first applied to a layer.
		A sequence is a series of images that will usually be all be of the same
		size and in the same context.  You can allocate sequence data
		at this time -- many more input fields are defined at this time.
		See the PF_InData description below.

	PF_Cmd_SEQUENCE_RESETUP
		This call is made to unflatten flattened sequence data.
		There are at least three cases when this can happen:
		1) after the sequence data is written to disk, 2) after the
		sequence data is read in from disk, 3) after a duplicate
		is made (called on both original and the new sequence).

	PF_Cmd_SEQUENCE_FLATTEN
		This call is made to flatten unflat sequence data so it can be
		cached to disk.  After the data is flattened, free the un-flat data
		and set the out_data->sequence_data to the new flat data.
		If you don't want your sequence handle written to disk, you
		can set it to NULL (after disposing it) at this time.  Presumably
		you would then reallocate it at another time.

		This command will be sent when saving and when duplicating
		the sequence.

	PF_Cmd_SEQUENCE_SETDOWN
		You should free any sequence data you have allocated when you
		get this command.

	PF_Cmd_DO_DIALOG
		This command indicated that the Options button or command has
		been selected and the effect should bring up its options dialog.
		This command will only be sent it the effect has indicated that
		it has an options dialog with PF_OutFlag_I_DO_DIALOG.  This
		command will automatically be sent once upon applying the filter
		if PF_OutFlag_SEND_DO_DIALOG is set in SEQUENCE_SETUP.

	PF_Cmd_FRAME_SETUP
		This is called immediately before each frame is invoked.  You
		can allocate frame data at this time, if you wish, or you can
		just wait for the RENDER which will immediately follow.

	PF_Cmd_RENDER
		This is the call to render the frame.  All fields in the in_data
		will be valid at this time and you can inquire parameters or
		what-have-you.  This should set the output frame with the new
		image data.  This is the main action command.

	PF_Cmd_FRAME_SETDOWN
		If you allocated data in PF_Cmd_FRAME_SETDOWN, this is the time
		to free it and clean up after rendering the frame.

	PF_Cmd_USER_CHANGED_PARAM
		This command will be sent if you set the PF_ParamFlag_SUPERVISE
		flag for a param.  This allows you to modify the params array contents to
		control values or make one control affect others, including arbitrary
		data.  This command will be sent whenever the user interacts with a
		standard param controller that has PF_ParamFlag_SUPERVISE
		set.
		
		The "extra" field will be a pointer to a PF_UserChangedParamExtra structure
		which contains the param_index of the changed parameter.

		You can return PF_ChangeFlag_CHANGED_VALUE and/or call PF_UpdateParamUI()
		for any param.
	
	PF_Cmd_UPDATE_PARAMS_UI
		This command will be sent when the Effect Controls Window (ECW)
		needs to updated (e.g. after opening the ECW or moving the comp to
		a new time) if you have set PF_OutFlag_SEND_UPDATE_PARAMS_UI at
		global setup time.

		This gives you a chance to call PF_UpdateParamUI() to modify certain 
		iu fields for the params.  See the doc for PF_UpdateParamUI()
		to see which fields can be modified.

		WARNING: When handling PF_Cmd_UPDATE_PARAMS_UI, you can call
		PF_UpdateParamUI() for any param(s), but not PF_ChangeFlag_CHANGED_VALUE --
		only cosmetic changes can be made in response to this command.

	PF_Cmd_QUERY_DYNAMIC_FLAGS
		This command will be sent at arbitrary times if PF_OutFlag2_SUPPORTS_QUERY_DYNAMIC_FLAGS
		is set during global setup. During this call the effect may examine the values
		of its parameters at the current time (except layer parameters) by checking them out, 
		and decide whether or not the following flags apply:

			PF_OutFlag_WIDE_TIME_INPUT
			PF_OutFlag_NON_PARAM_VARY
			PF_OutFlag_I_USE_SHUTTER_ANGLE
			PF_OutFlag2_I_USE_3D_CAMERA
			PF_OutFlag2_I_USE_3D_LIGHTS

		The appropriate flags must be set in out_data->out_flags and out_data->out_flags2 before
		returning. The effect must decide what information is necessary to render a frame at 
		the current time, given only the values of parameters at that time. Clearing the 
		appropriate bits when possible can result in great performance improvements, but 
		incorrectly clearing bits will result in caching bugs, and you won't like that. Nope.

**/
enum {		/* The order of this enumeration must not be altered! */
	PF_Cmd_ABOUT = 0,			/* about can be called at any time */
	PF_Cmd_GLOBAL_SETUP,		/* check versions, cpu, serial #, etc. always */
	PF_Cmd_UNUSED_0,				
	PF_Cmd_GLOBAL_SETDOWN,
	PF_Cmd_PARAMS_SETUP,		/* add parameters here */
	PF_Cmd_SEQUENCE_SETUP,
	PF_Cmd_SEQUENCE_RESETUP,	/* if part of in_data changed */
	PF_Cmd_SEQUENCE_FLATTEN,	/* prepare handle to be written to disk */
	PF_Cmd_SEQUENCE_SETDOWN,
	PF_Cmd_DO_DIALOG,			/* called after SEQUENCE_SETUP only if effect requests */
	PF_Cmd_FRAME_SETUP,
	PF_Cmd_RENDER,
	PF_Cmd_FRAME_SETDOWN,
							
/* most of these command types make use of the 'extra' field */

	PF_Cmd_USER_CHANGED_PARAM, 	/* user just changed a param value, extra contains ptr to PF_UserChangedParamExtra */
	PF_Cmd_UPDATE_PARAMS_UI,	/* UI fields in paramdefs need to be refreshed according to new values, no extra info */
	PF_Cmd_EVENT,				/* handle some event. extra contains a ptr to a relevant structure */
	PF_Cmd_GET_EXTERNAL_DEPENDENCIES, /* new in AE4.1: return text description of things like fonts, etc. in PF_ExtDependenciesExtra */
	PF_Cmd_COMPLETELY_GENERAL,	/* new in AE4.1: Used for completely general effect calls via AEGP */

	PF_Cmd_QUERY_DYNAMIC_FLAGS, /* new in AE5.0: allows effect to control cetain flags based on parameter values */

	PF_Cmd_AUDIO_RENDER,		/* For Audio Effects */
	PF_Cmd_AUDIO_SETUP,
	PF_Cmd_AUDIO_SETDOWN,

	PF_Cmd_ARBITRARY_CALLBACK,	/* used for arbitrary data, passes PF_ArbParamsExtra * in extra */

	PF_Cmd_NUM_CMDS
};
typedef long PF_Cmd;


/** -------------------- Simple Types --------------------
**/

typedef struct _PF_LayerAudio *PF_LayerAudio;

typedef void *PF_SndSamplePtr;

/* The following is an opaque type you pass to callback routines */
struct PF_ProgressInfo;
typedef struct PF_ProgressInfo *PF_ProgPtr;

typedef long PF_ParamValue;

typedef struct {
	short		major;
	short		minor;
} PF_SpecVersion;

typedef long PF_ParamIndex;

typedef unsigned long	PF_UFixed;

#if defined(MSWindows) && !defined(A_INTERNAL)

	#ifndef PF_BASICTYPES_DEFINED
		typedef long		PF_Fixed;
		typedef char		PF_Boolean;
		typedef void		**PF_Handle;

		typedef struct {
			short v;
			short h;
		} PF_Point;

		typedef struct {
			short top, left, bottom, right;

		} PF_Rect;
	#endif

#else

	typedef Fixed		PF_Fixed;
	typedef Rect		PF_Rect;
	typedef Boolean		PF_Boolean;
	typedef Handle		PF_Handle;
	typedef Point		PF_Point;

#endif

typedef struct {
	PF_Fixed	x, y;
} PF_FixedPoint;

typedef struct {
	PF_Fixed	left, top, right, bottom;
} PF_FixedRect;

typedef struct {
	PF_Fixed				mat[3][3];
} PF_Matrix;

typedef float			PF_FpShort;
typedef double			PF_FpLong;

typedef struct {
	PF_FpLong				mat[3][3];
} PF_FloatMatrix;

typedef struct {
	long			num;	/* numerator */
	unsigned long	den;	/* denominator */
} PF_RationalScale;


typedef struct {
	unsigned char	alpha, red, green, blue;
} PF_Pixel;

typedef PF_Pixel		PF_Pixel8;


#ifdef PF_PIXEL16_RENAME_COMPONENTS
	// this style is useful for debugging
	
	typedef struct {
		unsigned short		alphaSu, redSu, greenSu, blueSu;
	} PF_Pixel16;

#else
	// this style (the dephault) is useful for c++ template implementation

	typedef struct {
		unsigned short		alpha, red, green, blue;
	} PF_Pixel16;
	
#endif



typedef unsigned long	PF_PixLong;

typedef struct _PF_PixelOpaque	*PF_PixelOpaquePtr;

#ifdef PF_DEEP_COLOR_AWARE
	typedef PF_PixelOpaquePtr		PF_PixelPtr;
#else
	typedef PF_Pixel				*PF_PixelPtr;
#endif



#define PF_HUE_UNDEFINED	0x80000000

typedef PF_Fixed	PF_RGB_Pixel[3];
typedef PF_Fixed	PF_YIQ_Pixel[3];
typedef PF_Fixed	PF_HLS_Pixel[3];




/** -------------------- Pixel Access Macros --------------------

	If you're using the PF_PixLong struct for pixel representation,
	you can use these macros to guarantee correct channel access
	for a given pixel.

**/

#define PF_PixLong_ALPHA(pl)			((unsigned char)(0xff & ((pl) >> 24)))
#define PF_PixLong_RED(pl)				((unsigned char)(0xff & ((pl) >> 16)))
#define PF_PixLong_GREEN(pl)			((unsigned char)(0xff & ((pl) >> 8)))
#define PF_PixLong_BLUE(pl)				((unsigned char)(0xff & (pl)))

#define PF_SET_PixLong_ALPHA(pl, v)		(pl)&=0x00ffffff, (pl)|=((long)(v)<<24)
#define PF_SET_PixLong_RED(pl, v)		(pl)&=0xff00ffff, (pl)|=((long)(v)<<16)
#define PF_SET_PixLong_GREEN(pl, v)		(pl)&=0xffff00ff, (pl)|=((long)(v)<<8)
#define PF_SET_PixLong_BLUE(pl, v)		(pl)&=0xffffff00, (pl)|=(0xff & (v))
#define PF_MAKE_PixLong(a, r, g, b)		\
		((PF_PixLong)(((long)(a)<<24) | ((long)(r)<<16) | ((long)(g)<<8) | (b)))


#ifndef MSWindows
/** -------------------- Mac QuickDraw Globals --------------------

	Effects modules are code resources and as such cannot access QuickDraw
	global variables.  To compensate, you are provided a parameter block
	with a read-only copy of the QuickDraw globals.

**/

typedef struct {

	long		randSeed;
	BitMap		screenBits;
	Cursor		arrow;
	Pattern		dkGray;
	Pattern		ltGray;
	Pattern		gray;
	Pattern		black;
	Pattern		white;
	GrafPtr		thePort;
	
} PF_QDGlobals;

#endif



/*********************** multi-channel data *********************/

/**
 ** the kinds of multichannels we understand
 **/
#define PF_ChannelType_DEPTH		 'DPTH'
#define PF_ChannelType_NORMALS		 'NRML'
#define PF_ChannelType_OBJECTID		 'OBID'
#define PF_ChannelType_MOTIONVECTOR	 'MTVR'
#define PF_ChannelType_BK_COLOR		 'BKCR'
#define PF_ChannelType_TEXTURE		 'TEXR'
#define PF_ChannelType_COVERAGE		 'COVR'
#define PF_ChannelType_NODE			 'NODE'
#define PF_ChannelType_MATERIAL		 'MATR'
#define PF_ChannelType_UNCLAMPED	 'UNCP'
#define PF_ChannelType_UNKNOWN		 'UNKN'

typedef long PF_ChannelType;


/**
 ** This is the elementry data types we understand.
 ** By convention we reserve the last characters of the type
 ** to designate the size in bytes of a plane of data.  This together
 ** with the dimension tells us the size of each  pixel.
 ** For example, data of PF_ChannelType_COLOR with PF_DataType_DOUBLE would 
 ** consist of 32 bytes per pixel.
 **/
#define PF_DataType_FLOAT			'FLT4'	/* 4 byte		*/
#define PF_DataType_DOUBLE			'DBL8'	/* 8 byte		*/
#define PF_DataType_LONG			'LON4'	/* 4 bytes		*/
#define PF_DataType_SHORT			'SHT2'	/* 2 bytes		*/
#define PF_DataType_FIXED_16_16		'FIX4'	/* 4 bytes		*/
#define PF_DataType_CHAR			'CHR1'	/* 1 byte		*/
#define PF_DataType_U_BYTE			'UBT1'	/* 1 byte		*/
#define PF_DataType_U_SHORT			'UST2'	/* 2 bytes		*/
#define PF_DataType_U_FIXED_16_16	'UFX4'	/* 4 bytes		*/
#define PF_DataType_RGB				'RBG '	/* 3 bytes		*/

typedef long PF_DataType;





/**
 ** for enumerating over all the channels
 **/
typedef long PF_ChannelIndex;

 



/** 
 ** a description of the channel
 ** use this when iterating through channels to determine
 ** the characteristics of the channel
 ** 
 **/
#define PF_CHANNEL_NAME_LEN	63

typedef struct  {
	PF_ChannelType	channel_type;
	char			name[PF_CHANNEL_NAME_LEN+1];
	PF_DataType		data_type;
	long			dimension;			// the number of data per pixel
} PF_ChannelDesc;						// eg 3 for normals
						




/**
 ** the opaque type representing the channel data
 **/
typedef struct {
	long opaque[8];
} PF_ChannelRef, *PF_ChannelRefPtr;






/**
 ** the channel data parallels the image data in size and shape.
 ** the width is the number of pixels, the height is the number of scanlines
 ** the height is image_height
 ** the dimension is the number of planes in a pixel
 ** the row_bytes is the length of a scanline in bytes 
 ** the data type is the type of data in a plane
 **	Note : a pixel consists of dimensionL * sizeof(data_type) bytes
 ** dataH is a handle to the data. 
 ** dataPV is a pointer to the dereferenced locked handle
 ** effects should always have dataPV non null.
 **/
typedef struct {
	PF_ChannelRef	channel_ref;
	long			widthL;
	long			heightL;
	long			dimensionL;
	long			row_bytesL;
	PF_DataType		data_type;
	PF_Handle			dataH;
	void			*dataPV;
} PF_ChannelChunk;



/** -------------------- Effect Parameter Description Structures --------------------

	In general each structure is divided into two parts:
	a) description of the parameter
	b) setting of parameter at the current invocation time

	A number of these structures end in char*, char[] unions.  These
	structures are "flattened" between the add_param callback (in which
	the char * should be used) and the read-only values accessible when
	the effect is later invoked (at which time the char [] is used and
	the string data is concatenated right at the end of the struct).

**/


enum {
	PF_LayerDefault_MYSELF = -1,	/* default to this layer */
	PF_LayerDefault_NONE = 0		/* default to no layer */
};

/** Layer -- PF_Param_LAYER

	Layer parameters represent movie or image layers in the composition.
	All effects automatically have 1 layer param, param[0], which is the
	layer to which they have been applied.  Some effects may have additional
	layer parameters to do compound effects or multi-channel effects.  If
	your effects module has a layer parameter other than the param[0] default,
	you will have to call the checkout_param callback to access the contents
	of that layer.  There is a special case for all layer params (except the
	param[0] automatic layer) which is that the user can set them to <none>,
	indicating that no layer is selected.  The effect can detect this, because
	the checkout_param callback will not return an error, but the "data" pointer
	in the returned LayerDef struct will be NULL.  For the NULL-layers, the
	effect should attempt some logical interpretation, like pretending there
	is an all alpha-zero layer or perhaps just using param[0].

**/
typedef struct PF_LayerDef {
	/* PARAMETER VALUE */
	
	PF_ParamValue		reserved0;
	PF_ParamValue		reserved1;
	
	PF_WorldFlags		world_flags;

	PF_PixelPtr			data;

	long				rowbytes;
	long				width;
	long				height;
	PF_Rect				extent_hint;
	/* For source, extent_hint is the smallest rect encompassing all opaque
	 * (non-zero alpha) areas of the layer.  For output, this encompasses
	 * the area that needs to be rendered (i.e. not covered by other layers,
	 * needs refreshing, etc.). if your plug-in varies based on extent (like
	 * a diffusion dither, for example) you should ignore this param and
	 * render the full frame each time.
	 */
	long				platform_ref;
	long				reserved[8];
	
	/* PARAMETER DESCRIPTION */
	long				dephault;		/* use a PF_LayerDefault constant defined above */

} PF_LayerDef;

typedef PF_LayerDef		PF_World;


enum {
	PF_UNSIGNED_PCM = 0,
	PF_SIGNED_PCM = 1,
	PF_SIGNED_FLOAT = 2
};
typedef short	PF_SoundFormat;


// Sound encoding is always SIGNED
enum {
	PF_SSS_1 = 1,
	PF_SSS_2 = 2,
	PF_SSS_4 = 4
};
typedef short	PF_SoundSampleSize; // in bytes

enum {
	PF_Channels_MONO = 1,
	PF_Channels_STEREO = 2
};
typedef short	PF_SoundChannels;


typedef struct {
	PF_FpLong				rateF;
	PF_SoundChannels		num_channels;
	PF_SoundFormat			format;
	PF_SoundSampleSize		sample_size;
} PF_SoundFormatInfo;

typedef struct {
	PF_SoundFormatInfo		fi;
	long					num_samples;
	void					*dataP;
} PF_SoundWorld;



enum {
	PF_ValueDisplayFlag_NONE 		= 0,
	PF_ValueDisplayFlag_PERCENT		= 1 << 0,		// append % to value display (assume max is 100%) $$$$ is this true? dmw, 24 Oct 00
	PF_ValueDisplayFlag_PIXEL		= 1 << 1		// assume 0..1 is a pixel value, either 0..255,  0..32768, or 0..1.0 in UI (value will always be 0..1)

};

#define PF_VALUEFLAG_IS_PERCENT(A)			(((A) & PF_ValueDisplayFlag_PERCENT) != 0)
#define PF_VALUEFLAG_IS_PIXEL(A)			(((A) & PF_ValueDisplayFlag_PIXEL) != 0)

typedef short PF_ValueDisplayFlags;


/** Slider -- PF_Param_SLIDER
**/
typedef struct {
	/* PARAMETER VALUE */
	PF_ParamValue	value;
	char			value_str[PF_MAX_PARAM_VALUE_LEN + 1]; /* string for value */
	char			value_desc[PF_MAX_PARAM_DESCRIPTION_LEN + 1]; /* qualitative descr */

	/* PARAMETER DESCRIPTION */
	PF_ParamValue	valid_min, valid_max;		/* acceptable input range */
	PF_ParamValue	slider_min, slider_max;		/* range represented by width of slider */
	PF_ParamValue	dephault;
} PF_SliderDef;


/** Fixed Point Slider -- PF_Param_FIX_SLIDER
**/
typedef struct {
	/* PARAMETER VALUE */
	PF_Fixed					value;
	char						value_str[PF_MAX_PARAM_VALUE_LEN + 1]; /* string for value */
	char						value_desc[PF_MAX_PARAM_DESCRIPTION_LEN + 1]; /* qualitative descr */

	/* PARAMETER DESCRIPTION */
	PF_Fixed					valid_min, valid_max;		/* acceptable input range */
	PF_Fixed					slider_min, slider_max;		/* range represented by width of slider */
	PF_Fixed					dephault;
	short						precision;					/* decimal places to display */
	PF_ValueDisplayFlags		display_flags;				/* set bit to 1 to enable special display:
												 *	--> bit 0 == append percent sign
												 */
} PF_FixedSliderDef;


#define		AEFX_AUDIO_DEFAULT_CURVE_TOLERANCE			0.05f


enum {
	PF_FSliderFlag_NONE				= 0,
	PF_FSliderFlag_WANT_PHASE		= 1L << 0	/* works for audio effects only */
};
typedef unsigned long	PF_FSliderFlags;

/** Floating Point Slider -- PF_Param_FLOAT_SLIDER
**/
typedef struct {
	/* PARAMETER VALUE */
	PF_FpLong				value;
	PF_FpLong				phase;					/* used for PF_FSliderFlag_WANT_PHASE */
	char					value_desc[PF_MAX_PARAM_DESCRIPTION_LEN + 1]; /* qualitative descr */

	/* PARAMETER DESCRIPTION */
	PF_FpShort				valid_min, valid_max;		/* acceptable input range */
	PF_FpShort				slider_min, slider_max;		/* range represented by width of slider */
	PF_FpShort				dephault;
	short					precision;					/* decimal places to display */
	PF_ValueDisplayFlags	display_flags;				/* set bit to 1 to enable special display:
														 *	--> bit 0 == append percent sign
														 */
	PF_FSliderFlags			fs_flags;
	PF_FpShort				curve_tolerance;		/* used for subdividing audio effects
														set to zero for default, or non-audio */
} PF_FloatSliderDef;


/** Angle -- PF_Param_ANGLE
**/
typedef struct {
	/* PARAMETER VALUE */
	PF_Fixed		value;		/* degrees with fixed point accuracy;
								 * this is NOT limited in range to 0 to 360.
								 */

	/* PARAMETER DESCRIPTION */
	PF_Fixed		dephault;
} PF_AngleDef;


/** CheckBox -- PF_Param_CHECKBOX
**/
typedef struct {
	/* PARAMETER VALUE */
	PF_ParamValue		value;

	/* PARAMETER DESCRIPTION */
	PF_Boolean		dephault;
	char		reserved;	/* padding	*/
	short		reserved1;
	union {
		const char	*nameptr;
		char		name[1];	/* even-padded C string */
	} u;
} PF_CheckBoxDef;


/** Color -- PF_Param_COLOR
**/
typedef struct {
	/* PARAMETER VALUE */
	PF_Pixel	value;

	/* PARAMETER DESCRIPTION */
	PF_Pixel	dephault;
} PF_ColorDef;


/** Float Color -- will eventually have a PF_Param_FLOAT_COLOR
 **/
typedef struct {
	PF_FpShort		alphaF;		// 0..1
	PF_FpShort		redF;	
	PF_FpShort		greenF;
	PF_FpShort		blueF;
} PF_PixelFloat;


/** Point -- PF_Param_POINT

	The values for the point use the source's coordinate system, with the
	origin at the top left. The values are expressed in fixed point, with
	16 bits of fractional precision (out of a total of 32 bits).
	
	The dephaults (sorry) are expressed as percentages (with 16 bits of
	fraction) with the origin at the top left. The percent can be negative,
	but should not be smaller than -600%. It should not be greater than 600%.
	
	If restrict_bounds is TRUE, the user will not be allowed to specify
	points outside the bounds of the layer to which they are applying the
	effect. If this is TRUE, the dephaults should be between 0.0 and 100.0.

**/
typedef struct {
	/* PARAMETER VALUE */
	PF_Fixed			x_value;
	PF_Fixed			y_value;

	/* PARAMETER DESCRIPTION */
	char				reserved[3];
	PF_Boolean			restrict_bounds;		/* restrict bounds to size of src */
	PF_Fixed			x_dephault;				/* percentage */
	PF_Fixed			y_dephault;				/* percentage */
} PF_PointDef;


/** Popup Menu -- PF_Param_POPUP
**/
typedef struct {
	/* PARAMETER VALUE */
	PF_ParamValue	value;

	/* PARAMETER DESCRIPTION */
	short		num_choices;
	short		dephault;
	union {
		const char	*namesptr;
		char		names[1];	/* even-padded, menu manager standard, '|' separator */
	} u;
} PF_PopupDef;




enum {
	PF_PathID_NONE = 0
};
typedef unsigned long	PF_PathID;


enum {
	PF_MaskMode_NONE = 0,			// mask shape does nothing
	PF_MaskMode_ADD,				// shape is added into mask (normal behavior (really screen!))
	PF_MaskMode_SUBTRACT,
	PF_MaskMode_INTERSECT,
	PF_MaskMode_LIGHTEN,
	PF_MaskMode_DARKEN,
	PF_MaskMode_DIFFERENCE,
	PF_MaskMode_ACCUM,				// real add, not screen (not exposed in UI!)

	PF_NUM_MASKMODES
};
#define PF_NUM_USER_MASKMODES (PF_NUM_MASKMODES - 1)
typedef long PF_MaskMode;



/** Path -- PF_Param_PATH

	Path parameters give access to the mask/path/shapes of the layer on which
	the effect is applied.  For more information
	on how to use these paths, see the PF_PathQuerySuite, and the PF_PathDataSuite
	in AE_EffectSuites.h

**/
typedef struct PF_PathDef {

	/* PARAMETER VALUE */
	PF_PathID			path_id;		/* to be used with PF_CheckoutPath()
											note that path_id != PF_PathID_NONE does not
											guarantee that PF_CheckoutPath will return a
											valid path (it may have been deleted) */

	/* PARAMETER DESCRIPTION */
	long				reserved0;		/*  not currently used, set to zero */

	long				dephault;		/*	0 means that the default is NONE,
											other numbers are the 1-based index of the
											path, if the path doesn't exist, the 
											path_idLu value will be PF_PathID_NONE.
										 */
} PF_PathDef;



/*  ---------------------  ARBITRARY DATA   -----------------------------------
 *	new arbitrary data type - you supply functions for various activities
 *	needed by After Effects to manipulate your arbitrary data.
 */

typedef PF_Handle PF_ArbitraryH;


typedef struct {
	short					id;			/* for effect use: lets effect distinguish between */
										/*	different arbitrary data param types in the same effect */
	short					pad;		/* padding, set to zero */
	PF_ArbitraryH			dephault;	/* becomes owned by host at ADD_PARAM time */
	PF_ArbitraryH			value;		/* pass NULL at ADD_PARAM time; owned by host at render time */
	void					*refconPV;	/* passed into all callbacks, for effect use */
} PF_ArbitraryDef;



enum {
	PF_Arbitrary_NEW_FUNC		= 0,
	PF_Arbitrary_DISPOSE_FUNC,
	PF_Arbitrary_COPY_FUNC,
	PF_Arbitrary_FLAT_SIZE_FUNC,
	PF_Arbitrary_FLATTEN_FUNC,
	PF_Arbitrary_UNFLATTEN_FUNC,
	PF_Arbitrary_INTERP_FUNC,
	PF_Arbitrary_COMPARE_FUNC,
	PF_Arbitrary_PRINT_SIZE_FUNC,
	PF_Arbitrary_PRINT_FUNC,
	PF_Arbitrary_SCAN_FUNC
};
typedef long PF_FunctionSelector;



enum {
	PF_ArbCompare_EQUAL = 0,
	PF_ArbCompare_LESS,
	PF_ArbCompare_MORE,
	PF_ArbCompare_NOT_EQUAL
};
typedef long PF_ArbCompareResult;


enum {
	PF_ArbPrint_NONE			= 0,	
	PF_ArbPrint_ABBREVIATED		= 1 << 0
};
typedef long PF_ArbPrintFlags;



/* this is what gets passed in the extra parameter with  
 * PF_Cmd_ARBITRARY_CALLBACKS 
 */
typedef struct {

	PF_FunctionSelector			which_function;

	short						id;
	short						padding;

	union {

		/*	PF_Arbitrary_NEW_FUNC
		 *
		 *	allocate and initialize the data
		 *  the passed in parameter,v, will already have its non data fields filled in
		 *  you just allocate and optionally fill out the data. 
		 */
		struct {
			void 				*refconPV;				/* >> */
			PF_ArbitraryH		*arbPH;					/* << */
		} new_func_params;



		/*	PF_Arbitrary_DISPOSE_FUNC
		 *
		 *	dispose of any allocated data 
		 */
		struct {
			void 				*refconPV;
			PF_ArbitraryH		arbH;
		} dispose_func_params;



		/*	PF_Arbitrary_COPY_FUNC
		 *
		 *	make a copy of src_dataP into dst_dataP. refconPV was assigned from parameter def.
		 */
		struct {
			void 				*refconPV;
			PF_ArbitraryH		src_arbH;
			PF_ArbitraryH 		*dst_arbPH;			/* << allocated by copy func, owned by caller */
		} copy_func_params;
		


		/*	PF_Arbitrary_FLAT_SIZE_FUNC
		 * 
		 *	return in flat_data_sizePLu the number of bytes that are needed to flatten the object.
		 */
		struct {
			void 				*refconPV;
			PF_ArbitraryH		arbH;
			unsigned long		*flat_data_sizePLu;		/* << */
		} flat_size_func_params;



		/*	PF_Arbitrary_FLATTEN_FUNC	
		 *
		 *	Flatten the arbH and place it into the supplied buffer, honoring its size.
		 */
		struct {
			void 				*refconPV;
			PF_ArbitraryH		arbH;
			unsigned long		buf_sizeLu;			// size of buffer	
			void				*flat_dataPV;		// buffer already allocated
		} flatten_func_params;



		/*	PF_Arbitrary_UNFLATTEN_FUNC
		 * 
		 *	opposite of FlattenFunc, unpack the buffer into a PF_ArbitraryH. 
		 */
		struct {
			void				*refconPV;
			unsigned long		buf_sizeLu;				/* size of buffer */
			const void			*flat_dataPV;			/* >> */
			PF_ArbitraryH		*arbPH;					/* << */	// owned by caller
		} unflatten_func_params;



		/*	PF_Arbitrary_INTERP_FUNC
		 * 
		 *	The interpolation function takes a left and a right keyframe and a value between
		 *	0 and 1, 0 being the left keyframe, 1 represents the right keyframe.
		 *  allocate PF_ArbitraryH and fill it with interpolated data.
		 *  The velocity curves have already been accounted for when the normalized time value was 
		 *  calculated. 
		 */
		struct {
			void				*refconPV;
			PF_ArbitraryH		left_arbH;			/* >> */ // left keyframe
			PF_ArbitraryH		right_arbH;			/* >> */ // right keyframe
			PF_FpLong			tF;					/* >> */ // range [0,1]
			PF_ArbitraryH		*interpPH;			/* << */ // becomes owned by caller
		} interp_func_params;


		/*	PF_Arbitrary_COMPARE_FUNC
		 * 
		 *	compare two arbitrary data types for equality
		 *  return 0 if equal, otherwise return 1 in comparePL
		 */
		struct	{
			void				*refconPV;
			PF_ArbitraryH		a_arbH;					/* >> */  // value 1
			PF_ArbitraryH		b_arbH;					/* >> */  // value 2
			PF_ArbCompareResult	*compareP;				/* << */  // result of compare use enumeration above
		} compare_func_params;



		/*	PF_Arbitrary_PRINT_SIZE_FUNC
		 * 
		 *	return in print_sizePLu the number of bytes that are needed to print the object.
		 */
		struct {
			void 				*refconPV;
			PF_ArbitraryH		arbH;					/* >> */	// the data
			unsigned long		*print_sizePLu;			/* << */	// size of buffer needed to print
		} print_size_func_params;


		/*	PF_Arbitrary_PRINT_FUNC
		 * 
		 *	return in buff a user-readable description of your arb data
		 */
		struct {
			void 				*refconPV;					/* >> */
			PF_ArbPrintFlags	print_flags;				/* >> */	// PF_ArbPrint_NONE means print the complete description
			PF_ArbitraryH		arbH;						/* >> */
			unsigned long		print_sizeLu;				/* >> */	// size of buffer	
			char				*print_bufferPC;			/* << */
		} print_func_params;



		/*	PF_Arbitrary_SCAN_FUNC
		 * 
		 *	return a PF_ArbitraryH from the text description of your arb data
		 */
		struct {
			void 				*refconPV;					/* >> */
			const char			*bufPC;						/* >> */
			unsigned long		bytes_to_scanLu;			/* >> */
			PF_ArbitraryH		*arbPH;						/* << */
		} scan_func_params;

	} u;

} PF_ArbParamsExtra;


typedef struct {
	PF_ParamIndex				param_index;
} PF_UserChangedParamExtra;		// passed as extra for PF_Cmd_USER_CHANGED_PARAM



enum {
	PF_DepCheckType_NONE = 0,
	PF_DepCheckType_ALL_DEPENDENCIES,
	PF_DepCheckType_MISSING_DEPENDENCIES
};
typedef long PF_DepCheckType;

typedef struct {
	PF_DepCheckType				check_type;					/* >> */ // effect should ignore any unknown type
	PF_Handle					dependencies_strH;			/* << */ // NULL terminated string.  Flat and disposed of by the host
} PF_ExtDependenciesExtra;


typedef union {
	PF_LayerDef					ld;
	PF_SliderDef				sd;
	PF_FixedSliderDef			fd;
	PF_AngleDef					ad;
	PF_CheckBoxDef				bd;
	PF_ColorDef					cd;
	PF_PointDef					td;
	PF_PopupDef					pd;
	PF_FloatSliderDef			fs_d;
	PF_ArbitraryDef				arb_d;
	PF_PathDef					path_d;
} PF_ParamDefUnion;


/** Param UI Flags

	PF_PUI_TOPIC
		
		Set this flag if you handle PF_Cmd_EVENTs for the "topic" of
		the parameter.  The "topic" is the portion of the param UI
		in the Effect Controls Window (ECW) that is still visible
		when the twirly-arrow is twirled up for that param.

		If you set this flag, you must also set PF_OutFlag_CUSTOM_UI
		at PF_Cmd_GLOBAL_SETUP time.

	PF_PUI_CONTROL

		Set this flag if you handle PF_Cmd_EVENTs for the control
		area in the ECW.  This is the area that becomes invisible
		when you twirl up a parameter's twirly arrow (and is the
		usual place to have your custom UI).

		If you set this flag, you must also set PF_OutFlag_CUSTOM_UI
		at PF_Cmd_GLOBAL_SETUP time.

	PF_PUI_STD_CONTROL_ONLY
		
		Set this flag if you want the standard control only -- no
		data stream will be associated with this parameter, and
		thus no keyframes (nothing appears in the Time Layout window
		for this type of param).

		You might want to do this to control something in your
		sequence data with a standard control.  Or in your arb
		data, or custom UI in the comp window, or to group-set multiple
		other controls.

		
		This flag can be used with these param types:
			PF_Param_SLIDER, PF_Param_FIX_SLIDER, PF_Param_ANGLE,
			PF_Param_CHECKBOX, PF_Param_COLOR, PF_Param_POINT,
			PF_Param_POPUP, PF_Param_FLOAT_SLIDER

		but NOT:

			PF_Param_CUSTOM, PF_Param_NO_DATA,
			PF_Param_LAYER, PF_Param_ARBITRARY_DATA, PF_Param_PATH

		If you set this flag, you must also set PF_ParamFlag_SUPERVISE
		(otherwise you would never find out about value changes, and
		the setting would never be used for anything).  This flag
		does not require the setting of PF_OutFlag_CUSTOM_UI.

		If you want a standard control for PF_Param_ARBITRARY_DATA,
		just add one (or more) using PF_PUI_STD_CONTROL_ONLY with
		the supported param types, and then when handling 
		PF_Cmd_USER_CHANGED_PARAM you can modify your arb data.


	PF_PUI_NO_ECW_UI

		Set this flag if you want no UI to appear in the Effect Controls
		Window.  Presumably, you are setting the value of the parameter
		through some other method (e.g. custom UI in the comp window,
		or while handling PF_Cmd_USER_CHANGED_PARAM for a different param with 
		PF_ParamFlag_SUPERVISE set).

	PF_PUI_ECW_SEPARATOR

		Set this flag if you'd like a thick line above this parameter
		in the effect control window. This is provided so that parameters
		can be grouped visually, if needed (without adding groups).
		This flag can be changed at runtime through the PF_UpdateParamUI()
		method.


**/

enum {
	PF_PUI_NONE 			= 0,
	PF_PUI_TOPIC			= 1L << 0,	// effect wants events for this params' title (portion visible when twirled up)
	PF_PUI_CONTROL			= 1L << 1,	// effect wants events for this params' control (portion invisible when twirled up)
										//		use the width and height below

	// following flags are new in AE 4.0

	PF_PUI_STD_CONTROL_ONLY	= 1L << 2,	// param will be used as UI only, no data (new in AE 4.0) */
	PF_PUI_NO_ECW_UI		= 1L << 3,	// stop param from appearing in Effect Controls Window
	PF_PUI_ECW_SEPARATOR	= 1L << 4,	// draw a thick separating line above this param
	PF_PUI_DISABLED			= 1L << 5,	// disable (gray-out) UI for this param

	// following flags are new in AE 4.1

	// AE will not erase the ECW topic, it's up to the FX to erase/draw every pixel. 
	// Handy if FX author implements an offscreen, prevents flashing.  
	PF_PUI_DONT_ERASE_TOPIC		= 1L << 6,
	PF_PUI_DONT_ERASE_CONTROL	= 1L << 7

};
typedef long PF_ParamUIFlags;



/** PF_ChangeFlags

	New in AE 4.0 (although PF_ChangeFlag_CHANGED_VALUE was implemented
	by setting low bit of old changed field).

	PF_ChangeFlag_CHANGED_VALUE

		Set this flag for each param whose value you change when handling
		a PF_Cmd_USER_CHANGED_PARAM or PF_Cmd_EVENT.  You can change
		as many as you want at once.  These changes are undoable and
		re-doable by the user.  Exception: do not set PF_PUI_STD_CONTROL_ONLY
		param values with this flag, use PF_UpdateParamUI() instead.

	PF_ChangeFlag_SET_TO_VARY

		Not yet implemented.  Same restrictions as PF_ChangeFlag_CHANGED_VALUE.

	PF_ChangeFlag_SET_TO_CONSTANT

		Not yet implemented.  Same restrictions as PF_ChangeFlag_CHANGED_VALUE.

**/

enum {
	PF_ChangeFlag_NONE = 0,
	PF_ChangeFlag_CHANGED_VALUE		= 1L << 0,
	PF_ChangeFlag_RESERVED			= 1L << 1,
	PF_ChangeFlag_SET_TO_VARY		= 1L << 2,
	PF_ChangeFlag_SET_TO_CONSTANT	= 1L << 3
};
typedef long PF_ChangeFlags;


/** ParamDef
**/
typedef struct {
	/* PARAMETER VALUE */
	union {
		long				id;					// used by PF_ADD_PARAM callback, see doc
		PF_ChangeFlags		change_flags;		// set when handling PF_Cmd_USER_CHANGED_PARAM & PF_Cmd_UPDATE_PARAMS_UI
	} uu;

	PF_ParamUIFlags	ui_flags;
	short			ui_width;		/* ignored if !PF_PUI_CONTROL */
	short			ui_height;		/* ignored if !PF_PUI_CONTROL */
	
	/* PARAMETER DESCRIPTION */
	PF_ParamType	param_type;
	char			name[PF_MAX_EFFECT_PARAM_NAME_LEN + 1];
	PF_ParamFlags	flags;
	
	long			reserved_tdb;
	PF_ParamDefUnion u;

} PF_ParamDef, *PF_ParamDefPtr, **PF_ParamDefH;

typedef PF_ParamDef** PF_ParamList;





/** -------------------- Interaction Callbacks --------------------

	Effects modules use callbacks to define their parameters.  When invoked,
	they will be given the parameters values at the particular invocation
	moment, but some effects may need to ask for the parameter values at
	other times (notably of layer parameters for, say, a visual echo).

	While running, effects modules are responsible for checking for user
	interrupts.  This checking can be done with either the abort callback,
	which will return a value indicating if the user has taken any action,
	or with the progress callback, which performs user interrupt checking
	just like the abort callback, and also displays a progress display.

	At the bottom of this section are macros for accessing these callback
	routines.  The first parameter to each macro is a pointer to a PF_InData
	structure, defined below.  This pointer will be passed to your effect.

	checkout_param
		The checkout_param callback allows you to inquire param values at times
		other than the current one, and allows you to access layer params other
		than the default input layer and the output layer.  See the notes on the
		"params" structure at the end of this file.  The PF_ParamDef you must
		specify cannot point into the "params" array; the memory must exist else-
		where, such as on the stack.
		If you checkout a layer parameter and the layer popup is currently set
		to <none>, the return value will be filled with zeros.  You can check
		the "data" pointer.  If it is NULL, then the layer param is set to <none>
		and you should do something like faking an all alpha zero layer or some
		such nonsense. 

	checkin_param
		When you have called checkout_param, you must call checkin_param when you
		are done, so After Effects can clean up after itself and you.  This is
		very important for smooth functioning and also to save memory where possible.
		Once checked in, the fields in the PF_ParamDef will no longer be valid.

	add_param
		When given the PARAMS_SETUP message, the effect will generally make a
		series of calls to the add_param routine to define the interface that
		the After Effects user will see.  See the PF_ParamDefs defined above.
		Currently you can only add params at the end, and only at PARAMS_SETUP
		time.

	abort
		Periodically, you should check if the user wants to interrupt the
		current processing.  The abort proc here will return non-zero if
		the effects module should suspend its current processing.  If you
		call this routine and it returns a value other than zero, you should
		return that value when your effect returns.  That will let us know
		if the effect completed rendering or not.

	progress
		Alternatively, you may wish to display a progress bar while you are
		processing the image.  This routine combines the abort proc user
		interrupt checking with code that will display a progress bar for
		you.  The current and total params represent a fraction (current/total)
		that describes how far you are along in your processing.  Current
		should equal total when done.  Additionally, this routine will return
		non-zero if you should suspend/abort your current processing.  You
		should probably try not to call this too frequently (e.g. at every pixel).
		It is better to call it, say, once per scanline, unless your filter is
		really really slow.

**/

typedef struct _PF_CustomUIInfo PF_CustomUIInfo;

typedef struct {
	PF_Err (*checkout_param)(
		PF_ProgPtr		effect_ref,	/* reference from in_data */
		PF_ParamIndex	index, 		/* 0 = input, 1..n = param */
		long			what_time,
		long			time_step,	/* duration between frames in time_scale units */
		unsigned long	time_scale,
		PF_ParamDef		*param);	/* the param you've been waiting for...	*/

	PF_Err (*checkin_param)(
		PF_ProgPtr		effect_ref,	/* reference from in_data */
		PF_ParamDef		*param);

	PF_Err (*add_param)(
		PF_ProgPtr		effect_ref,	/* reference from in_data */
		PF_ParamIndex	index,		/* -1 = add to end */
		PF_ParamDefPtr	def);

	PF_Err (*abort)(
		PF_ProgPtr		effect_ref);	/* reference from in_data */

	PF_Err (*progress)(
		PF_ProgPtr		effect_ref,	/* reference from in_data */
		long			current,
		long			total);

	PF_Err (*register_ui)(
		PF_ProgPtr			effect_ref,	/* reference from in_data	*/
		PF_CustomUIInfo		*cust_info);

	PF_Err (*checkout_layer_audio)(
		PF_ProgPtr		effect_ref,	/* reference from in_data */
		PF_ParamIndex	index, 		/* 0 = input, 1..n = param */
		long			start_time,	/* in time_scale units */
		long			duration,	/* in time_scale units */
		unsigned long	time_scale,	/* units/sec */
		PF_UFixed		rate,		/* unsigned! */
		long			bytes_per_sample,
		long			num_channels,
		long			fmt_signed,	/* non-zero for signed, zero for unsigned */		
		PF_LayerAudio	*audio);	/* the LayerAudio you've been waiting for...	*/

	PF_Err (*checkin_layer_audio)(
		PF_ProgPtr		effect_ref,	/* reference from in_data */
		PF_LayerAudio	audio);

	PF_Err (*get_audio_data)(
		PF_ProgPtr		effect_ref,			/* reference from in_data */
		PF_LayerAudio	audio,
		PF_SndSamplePtr	*data0,				/* optional - packed array of samples, if stereo, left/right */
		long			*num_samples0,		/* optional */
		PF_UFixed		*rate0,				/* optional - unsigned! */
		long			*bytes_per_sample0,	/* optional */
		long			*num_channels0,		/* optional */
		long			*fmt_signed0);		/* optional - non-zero for signed, zero for unsigned */		

	long				reserved_str[3];
	
	long				reserved[10];

} PF_InteractCallbacks;


#define PF_CHECKOUT_PARAM(IN_DATA, INDEX, TIME, STEP, SCALE, PARAM)	\
		(*(IN_DATA)->inter.checkout_param)((IN_DATA)->effect_ref,	\
			(INDEX), (TIME), (STEP), (SCALE), (PARAM))

#define PF_CHECKIN_PARAM(IN_DATA, PARAM)	\
		(*(IN_DATA)->inter.checkin_param)((IN_DATA)->effect_ref, (PARAM))
	
#define PF_ADD_PARAM(IN_DATA, INDEX, DEF)	\
		(*(IN_DATA)->inter.add_param)((IN_DATA)->effect_ref, (INDEX), (DEF))
	
#define PF_ABORT(IN_DATA)	\
		(*(IN_DATA)->inter.abort)((IN_DATA)->effect_ref)
	
#define PF_PROGRESS(IN_DATA, CURRENT, TOTAL)	\
		(*(IN_DATA)->inter.progress)((IN_DATA)->effect_ref, (CURRENT), (TOTAL))

#define PF_REGISTER_UI(IN_DATA, CUST_INFO)	\
		(*(IN_DATA)->inter.register_ui)((IN_DATA)->effect_ref, (CUST_INFO))

#define PF_CHECKOUT_LAYER_AUDIO(IN_DATA, INDEX, START_TIME, DURATION, SCALE, RATE,	\
								BYTES_PER_SAMPLE, NUM_CHANNELS, FMT_SIGNED, AUDIO)	\
		(*(IN_DATA)->inter.checkout_layer_audio)((IN_DATA)->effect_ref,	\
			(INDEX), (START_TIME), (DURATION), (SCALE), (RATE), (BYTES_PER_SAMPLE),	\
			(NUM_CHANNELS), (FMT_SIGNED), (AUDIO))

#define PF_CHECKIN_LAYER_AUDIO(IN_DATA, AUDIO)	\
		(*(IN_DATA)->inter.checkin_layer_audio)((IN_DATA)->effect_ref, (AUDIO))

#define PF_GET_AUDIO_DATA(IN_DATA, AUDIO, DATA_0, NUM_SAMPLES_0, RATE_0,			\
								BYTES_PER_SAMPLE_0, NUM_CHANNELS_0, FMT_SIGNED_0)	\
		(*(IN_DATA)->inter.get_audio_data)((IN_DATA)->effect_ref,	\
			(AUDIO), (DATA_0), (NUM_SAMPLES_0), (RATE_0), (BYTES_PER_SAMPLE_0),		\
			(NUM_CHANNELS_0), (FMT_SIGNED_0))


/** -------------------- Effect Parameter Blocks --------------------

	The effects module itself is invoked with input and output blocks
	of parameters for various messages that it needs to handle.  The
	output block (values returned from the effect to the program) and
	the input block (values provided by the program for the edification
	of the effect) are defined here.  Discussion follows.

	The Output Block

	Most fields of the output block are only examined for changes after
	certain commands have been sent to the effect module.  Each field
	below describes when it will be checked.

		my_version
			This is the version number of your plug-in effect, not to be confused
			with the version of the plug-in specification. Please set this
			at PF_Cmd_GLOBAL_SETUP.

		name
			This lets you override the name of the effect in the Time Layout and
			in the Effect Controls windows when the effect is applied. The name from
			the PiPL resource is always used in the Effect menu. This field is
			checked after PF_Cmd_SEQUENCE_SETUP. You will almost always leave
			this field empty.

		global_data
			This is a Handle that you can allocate at PF_Cmd_GLOBAL_SETUP
			time.  It will be passed back to you verbatim in the input
			parameter block for use later on.  In PF_Cmd_GLOBAL_SETUP,
			the global_data field in the Input Block may be set to a
			flattened version of your global data, in which case you should
			unflatten it, free the flat version, and set this field to the
			unflattened new global data.

			It will be locked & unlocked for you automatically like sequence_data.

		num_params
			The calling application will sanity check the num_params field
			vs the times add_param is called.  The implicit main
			layer parameter MUST be included in the parameter count.  Use the
			num_params value in the in_data as a starting value (it will
			include the implicit layer parameter).
			Set this field when you get PF_Cmd_PARAMS_SETUP.

		sequence_data
			This is a Handle that you can allocate (using PF_NEW_HANDLE)
			at PF_Cmd_SEQUENCE_SETUP time.
			It will be passed back to you in the input parameter block
			for later use.
			
			WARNING: this handle is always locked for you before your plugin
			is called, and the lock-state is restored on the way out.  If you
			have arbitrary data, your plugin can be called re-entrantly, so
			if you have a lock/unlock inside your plugin, the handle will
			become unlocked prematurely -- instead simply depend on the host
			to lock & unlock your sequence data for you.  Of course during
			PF_Cmd_SEQUENCE_SETUP the handle you allocate will not be
			locked unless you explicitly lock it at that time.

			The contents of this handle will be written out to
			disk.  If other handles	hang off this block, you must specify the
			PF_OutFlag_SEQUENCE_DATA_NEEDS_FLATTENING out flag when you get
			the PF_Cmd_GLOBAL_SETUP command.  You will then receive the
			PF_Cmd_SEQUENCE_FLATTEN before your handle is written out.  At that
			time, you should create a flat version of the handle contents,
			free the old unflat handle, and set this field to the flattened
			version of the handle.  Or after disposing, simply set to NULL
			and nothing will be written to disk.
			
			You will receive a PF_Cmd_SEQUENCE_RESETUP
			call to unflatten this handle (as well as to adjust the sequence
			data to altered frame rates, etc).  If your
			sequence data can be flat or unflat, you should store its current
			state along with the other data, and check that value in Resetup.
			If the handle is flat, Resetup should unflatten it, free the flat
			handle, and set this field to the new unflat usable handle.

		flat_sdata_size
			OBSOLETE.  Turns out, it was never used.  Since the handle
			set in sequence_data must be allocated using PF_NEW_HANDLE,
			the host can find out the size of the handle without asking.

		frame_data
			This is a Handle that you can allocated at PF_Cmd_FRAME_SETUP
			time.  It will be passed to you in the input parameters, as
			with the global_data and the sequence_data.  This will not
			be written out to disk.  There is no particular use for this.
			Set this field in PF_Cmd_FRAME_SETUP, if you must.

			It will be locked & unlocked for you like sequence_data.
		
		width
		height
		origin
			You set these fields at PF_Cmd_FRAME_SETUP time to indicate
			that the output image will be larger than the input image.
			You	should set width and height to the size that you want the
			output buffer to be.  Set origin to the place that the
			point (0,0) in the input should map to in the new larger
			output.  Thus, if you created a 5 pixel drop shadow up and left,
			you would set origin to (5, 5).

		out_flags
		out_flags2
			This field can be set to an OR-ed combination of the PF_OutFlag
			and PF_OutFlag2 constants (don't mix them up!) to communicate 
			things to After Effects.  This will be checked after every 
			command, but only certain flags are relevant at given times.  
			Check the PF_OutFlag constants above.

		return_msg
			This is a message string (in C string format) that will be
			interpretted as either an error message or a useful display
			message (for instance, for handling PF_Cmd_ABOUT).  Fill
			this string with a message you want After Effects to report
			to the user for you.  It will come up in a simple dialog
			with an OK button.  Set the first byte of this string to '\0'
			to indicate no string -- it is set that way upon entry.  This
			field is examined after every PF_Cmd.

	The Input Block

	Many parts of the input block are actually structures defined elsewhere
	in this file, or in the companion file AE_EffectCB.h.  See the documentation
	where those structures are defined to understand their contents.
	With any given PF_Cmd, only certain fields in the Input Block will
	have valid values.  Each field described below tells when it is valid.

		in_flags
		in_flags2
			These are various flags indicating some boolean value to
			the effect module.  This is a combination of PF_InFlag values
			OR-ed together.  This is set for all commands, though most
			flags make sense only at certain times.  Usually this is
			just a copy of the PiPL global flags.  See PF_OutFlags and
			PF_OutFlags2 above.
		
		inter
			This is a structure defined above containing callbacks
			related to user interaction.  This has callbacks to add parameters,
			to check if the user has interrupted the effect, to display
			a progress bar, and to inquire parameter values outside of the
			current moment.  See the doc above.  When each callback can
			validly be executed is defined above.
		
		utils
			This is a pointer to a block of useful graphical and mathematical
			callbacks provided for the effects module.  The documentation
			for this block is in the AE_EffectCB.h file.  This is a void *,
			which can be confusing.  See AE_EffectCB.h for macros to use these
			functions.  This pointer will be defined at all times.
		
		effect_ref
			This is a opaque piece of data that needs to be passed to
			most of the various callback routines.  Don't worry about it.
		
		quality
			This is set to one of the PF_Quality constants above to
			describe the Quality currently chosen by the user.  Ideally,
			your effect should do a faster version with LO quality, and
			a better, "broadcast"-able version with HI quality.  Also,
			some of the utility callbacks perform differently between
			LO and HI quality.  This is defined for all PF_Cmds related
			to SEQUENCE and FRAME (obviously, including RENDER).
		
		version
			This is the version of the effects spec with which you are
			being invoked.  This will not be defined until after GLOBAL_SETUP.
		
		serial_num
			This is the serial number of the invoking application.
		
		appl_id
			This is the identifier of the invoking application.  It will
			be the creator long of the app.
		
		freq
			This is an estimate of the frequency with which you should
			call the abort check callback.  You can ignore this.
		
		num_params
			This is set to the number of input parameters you are receiving.
		
		what_cpu
			This is set to the return value from Gestalt asking what sort
			of CPU your machine has.  If your effect requires a certain
			type of CPU it should check this value and return an error
			indicating that it cannot run.  After Effects only runs on
			68020s and higher, so don't sweat it if you require that.
		
		what_fpu
			This is set to the return value from Gestalt asking what sort
			of FPU your machine has.  If you require a floating point unit,
			you should return the OutFlag indicating that in GLOBAL_SETUP,
			and then do not execute your floating point code if this value
			is set to 0 -- just do a PF_COPY of the input to the output when
			you get the PF_Cmd_RENDER.  See OutFlag description above.
		
		qd_globals
			This is a pointer to a read only copy of the QuickDraw globals.
			See the description with the structure above.
		
		current_time
			This is the time of the current frame.  It will be set in RENDER.
			The number of the current frame is current_time / time_step.
			All effects sequences start at time 0.
		
		time_step
			This is the time difference to the next or last frame.  This value
			and current_time and total_time are in units given by time_scale.
			The time between frames is time_step, not 1.  This value will be 0
			at SEQUENCE_SETUP if it is not constant for all frames.  It will
			be set correctly in the FRAME calls, even if it's not constant.
		
		total_time
			This is the amount of time from the start to the end of the
			image sequence on which this effect is being invoked.  The total
			number of frames is total_time / time_step.
		
		time_scale
			These are the units that current_time, time_step, and total_time
			are in.  See QuickTime for an explanation of how these time values work.
		
		width
		height
			These are the size of the input image.  They are the same as the
			width and height fields in param[0].

		field
			Will be set to PF_Field_UPPER or PF_Field_LOWER during field-rendering
			if certain conditions are met -- for example: effect must have
			PF_OutFlag_PIX_INDEPENDENT set, and the layer to which the effect is
			applied must not be rotated, scaled nor sub-pixelly positioned.
			You can safely ignore the setting of this field, but might be able to
			optimize things by only processing the specified field.

		extent_hint
			This is a rectangle that indicates the intersection of the visible
			portions of the input and output layers.  For an effect that does
			not do a geometric distortion of the image, copying just this rectangle
			from the source image to the destination image is sufficient to copy
			all the image data that the user will see.  This can speed up effects
			very much.  Just iterate over only this rectangle of pixels.

		output_origin_x
		output_origin_y
			These fields correspond to the origin returned in the out_data at
			PF_Cmd_FRAME_SETUP time. They indicate the position of the top left
			corner of the input buffer in the output buffer.

		downsample_x
		downsample_y
			For speed, the user may have asked for only every Nth vertical or
			horizontal pixel to be actually rendered by AfterEffects.  The width
			and height of all effect parameters (including layers) will be
			automatically adjusted to compensate, but the effect needs to know
			the downsampling factors to correctly interpret scalar parameters
			(ie. sliders) that represent pixel distances in the image.
			Downsample factors will be in the range 1 to 999+.  This is set in
			SEQUENCE_SETUP or RESETUP as the case may be. As of PF_PLUG_IN_VERSION 2,
			this factor is a rational quantity.
			
		global_data
		sequence_data
		frame_data
			These fields are copied from the out data on previous invocations
			and set here for you to access as you need them.  They will only
			be set if they have been allocated during previous commands.

		pre_effect_source_origin_x
		pre_effect_source_origin_y
			These fields are the origin of the source image in the input buffer. They are set
			only during frame calls (PF_Cmd_FRAME_SETUP, PF_Cmd_RENDER, PF_Cmd_FRAME_SETDOWN).
			They will be non-zero only if one or more effects that preceded this effect
			on the same layer resized the output buffer (i.e. specified that the output
			image will be larger or smaller than the input image).

**/


typedef struct {
	unsigned long		my_version;			/* version # for plug-in code */
	char				name[PF_MAX_EFFECT_NAME_LEN + 1];	/* only used at seq setup to change */
	PF_Handle			global_data;
	long				num_params;
	PF_Handle			sequence_data;
	long				flat_sdata_size;/* obsolete */
	PF_Handle			frame_data;
	long				width;			/* change if you want resized output */
	long				height;
	PF_Point			origin;
	PF_OutFlags			out_flags;		/* ORed combo of PF_OutFlag values */
	char				return_msg[PF_MAX_EFFECT_MSG_LEN + 1];
	long				start_sampL;	/* used only for audio commands */
	long				dur_sampL;		/* used only for audio commands */
	PF_SoundWorld		dest_snd;		/* used only for audio commands */
	PF_OutFlags2		out_flags2;		/* ORed combo of PF_OutFlag2 values */
} PF_OutData;


typedef struct {
	PF_InteractCallbacks		inter;	/* effect interaction related callbacks */
	struct _PF_UtilCallbacks	*utils;	/* utility callbacks -- see AE_EffectCB.h */
	PF_ProgPtr			effect_ref;		/* opaque value for callbacks */
	PF_Quality			quality;		/* quality user has selected */
	PF_SpecVersion		version;
	long				serial_num;
	long				appl_id;
	long				num_params;
	long				reserved;
	long				what_cpu;		/* return value from Gestalt asking CPU */
	long				what_fpu;		/* as above for FPU */
#ifdef MSWindows
	void				*qd_globals;	/* reserved -- undefined for Windows */
#else
	PF_QDGlobals		*qd_globals;	/* read-only copy of the QuickDraw globals */	
#endif
	long				current_time;
	long				time_step;		/* time diff between calls or to next call	*/
	long				total_time;
	long				local_time_step;/* time step in local comp */
	unsigned long		time_scale; 	/* units per second of time system */
	PF_Field			field;			/* if field is specified, other field may be garbage */
	PF_Fixed			shutter_angle;	/* motion blur shutter angle (range is 0 to 1) */
	long				width;			/* full resolution width of source layer */
	long				height;			/* full resolution height of source layer */
	PF_Rect				extent_hint;	/* intersection of input and output extents */
	long				output_origin_x; /* origin of input buffer in output buffer */
	long				output_origin_y; /* non-zero only when effect changes buffer size */
	PF_RationalScale	downsample_x;
	PF_RationalScale	downsample_y;
	PF_RationalScale	pixel_aspect_ratio;	/* h/v of pixel aspect ratio (NTSC D-1 -> 0.9) */
	PF_InFlags			in_flags;
	PF_Handle			global_data;	/* the data created by global_setup */
	PF_Handle			sequence_data;	/* data from sequence_setup */
	PF_Handle			frame_data;		/* data from frame_data -- state for this render */
	long				start_sampL;	/* used only for audio commands */
	long				dur_sampL;		/* used only for audio commands */
	long				total_sampL;	/* used only for audio commands */
	PF_SoundWorld		src_snd;		/* used only for audio commands */
	struct SPBasicSuite	*pica_basicP;	/* pointer to PICA basic suite. Enjoy! */
	long				pre_effect_source_origin_x;		/* origin of original source in input buffer */
	long				pre_effect_source_origin_y;		/* non-zero only during frame calls (setup, render, setdown) 
															when effect follows an effect that resizes its output buffer */
	PF_Fixed			shutter_phase;	/* offset from frame time to shutter open time as a percentage of a frame */
} PF_InData;


/** -------------------- Effect Prototype --------------------

	The effects module provides a single entry point corresponding to the
	below prototype through which all messages are dispatched to the
	appropriate code.

	The cmd parameter is one of the PF_Cmd enumeration defined above.
	All commands are discussed at that enumeration.  There are a lot.

	The in_data and out_data parameters are respectively, PF_InData and
	PF_OutData blocks defined and described above.  The in_data contains
	read only information that the effect can use.  The out_data contains
	write only information through which the effect communicates back to
	the calling program.  The descriptions above give a lot more detail.

	The params list is an array of pointers to variably sized PF_ParamDef
	structs.  This is in typical Unix-like argv format, where the last item
	points to NULL.  The entries in this array describe the current settings
	of the parameters to the effect.  Params[0] is the layer to which the
	effect has been applied and is a PF_LayerDef param.  Other parameters
	are defined by the effect.

	The output param is also a PF_LayerDef param, like params[0], and is
	an output buffer into which your effect should write the new contents
	of the layer.

	A brief explanation about parameter passing to effects:

	When you are invoked to Render, all "params" array entries will be filled
	with their value at the current time, except for layer params other than
	the default input layer parameter (ie. param[0]).  To get other layer param
	values, you must call the checkout_param callback.  (See callbacks above.)

	In other words, on PF_Cmd_RENDER, params[0] will be a valid ready-to-use
	PF_World and output will be a valid ready-to-use PF_World.  Other params that
	are not layer params (i.e. sliders, popups, etc.) will be ready-to-use, filled
	with their value at the current time; you do not need to call checkout_param for
	them.  However, other params (besides [0]) that are layer params will NOT be
	filled in correctly -- you must make a checkout_param callback to get them.

	ANY param which you call checkout_param on, you must also call checkin_param
	before you exit.  If not, After Effects will automatically erase your effect
	module code resource from the users hard disk, along with any files that contain
	your name or any nicknames you have.  Sorry.  Calling checkin_param is important!

**/

typedef PF_Err (*PF_FilterProc)(
	PF_Cmd			 cmd,
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamList	params,
	PF_LayerDef		*output,
	void			*extra);


#ifdef __cplusplus
}		// end extern "C"
#endif


#if PRAGMA_STRUCT_ALIGN
	#pragma options align=reset
#endif

#ifdef _WINDOWS
	#pragma pack( pop, AEalign)
#endif


#endif /* _H_AE_Effect */
