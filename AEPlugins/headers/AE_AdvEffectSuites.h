#ifndef _H_AE_AdvEffectSuites
#define _H_AE_AdvEffectSuites

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


#define kPFAdvAppSuite				"PF AE Adv App Suite"
#define kPFAdvAppSuiteVersion1		1


typedef struct PF_AdvAppSuite1 {

		SPAPI	PF_Err	(*PF_SetProjectDirty)(void);

		SPAPI	PF_Err	(*PF_SaveProject)(void);

		SPAPI	PF_Err	(*PF_SaveBackgroundState)(void);

		SPAPI	PF_Err	(*PF_ForceForeground)(void);

		SPAPI	PF_Err	(*PF_RestoreBackgroundState)(void);

		SPAPI	PF_Err	(*PF_RefreshAllWindows)(void);

		// 2 lines of text, same as calling PF_InfoDrawText3( line1Z0, line2Z0, NULL)
		SPAPI	PF_Err	(*PF_InfoDrawText)(
								const char	*line1Z0,
								const char	*line2Z0);

		SPAPI	PF_Err	(*PF_InfoDrawColor)(
								PF_Pixel	color);

		// 3 lines of text
		SPAPI	PF_Err	(*PF_InfoDrawText3)(
								const char	*line1Z0,
								const char	*line2Z0,
								const char	*line3Z0);

		// 3 lines, with two lines including formatting for right and left justification
		SPAPI	PF_Err	(*PF_InfoDrawText3Plus)(
								const char	*line1Z0,
								const char	*line2_jrZ0,
								const char	*line2_jlZ0,
								const char	*line3_jrZ0,
								const char	*line3_jlZ0);
								
} PF_AdvAppSuite1;



#define PF_MAX_TIME_LEN				31


typedef enum { PF_Step_FORWARD, PF_Step_BACKWARD } PF_Step;


enum {
	PF_TimeDisplayFormatTimecode,
	PF_TimeDisplayFormatFrames,
	PF_TimeDisplayFormatFeetFrames
};


typedef struct {
	char	time_display_format;
	char	framemax;
	char	nondrop30;
	char	frames_per_foot;
} PF_TimeDisplayPref;




#define kPFAdvTimeSuite				"PF AE Adv Time Suite"
#define kPFAdvTimeSuiteVersion1		1


typedef struct PF_AdvTimeSuite1 {

	SPAPI PF_Err (*PF_FormatTimeActiveItem)(	long				time_valueUL,		// time is value/scale in seconds
												unsigned long		time_scaleL,
												PF_Boolean				durationB,			// is the time value a duration or time?
												char				*time_buf);			// allocate as PF_MAX_TIME_LEN + 1
																	
	SPAPI PF_Err (*PF_FormatTime)(				PF_InData			*in_data,
												PF_World			*world,
												long				time_valueUL,
												unsigned long		time_scaleL,
												PF_Boolean				durationB,
												char				*time_buf);
																	
	SPAPI PF_Err (*PF_FormatTimePlus)(			PF_InData			*in_data,
												PF_World			*world,
												long				time_valueUL,
												unsigned long		time_scaleL,
												PF_Boolean				comp_timeB,
												PF_Boolean				durationB,
												char				*time_buf);
																	
												
	SPAPI PF_Err (*PF_GetTimeDisplayPref)(		PF_TimeDisplayPref	*tdp,
												long				*starting_frame_num);



} PF_AdvTimeSuite1;





#define kPFAdvItemSuite				"PF AE Adv Item Suite"
#define kPFAdvItemSuiteVersion1		1



typedef struct PF_AdvItemSuite1 {

	SPAPI PF_Err (*PF_MoveTimeStep)(			PF_InData		*in_data,
												PF_World		*world,
												PF_Step			time_dir,
												long			num_stepsL);

	SPAPI PF_Err (*PF_MoveTimeStepActiveItem) (	PF_Step			time_dir,
												long			num_stepsL);

								
	SPAPI PF_Err (*PF_TouchActiveItem)			(void);


	SPAPI PF_Err (*PF_ForceRerender)(			PF_InData		*in_data,
												PF_World		*world);
								

	SPAPI PF_Err (*PF_EffectIsActiveOrEnabled)(	PF_ContextH		contextH,
												PF_Boolean			*enabledPB);


} PF_AdvItemSuite1;



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
