//SDK_ADM.h

#pragma once

#define ADM_Floating_Palette_Width	206
#define ADM_Floating_Palette_Height	68

#define kAE_ADMDialogSuite				1
#define kAE_ADMDialogGroupSuite			2
#define kAE_ADMDrawerSuite				3
#define	kAE_ADMIconSuite				4
#define kAE_ADMTrackerSuite				5
#define kAE_ADMItemSuite				6
#define	kAE_ADMEntrySuite				7
#define	kAE_ADMListSuite				8
#define	kAE_ADMNotifierSuite			9
#define	kAE_ADMHostSuite				10
#define	kAE_ADMBasicSuite				11
#define	kAE_ADMWindowSuite				12

#ifdef MSWindows
struct RGBColor {
	unsigned short red;                 /*magnitude of red component*/
	unsigned short green;               /*magnitude of green component*/
	unsigned short blue;                /*magnitude of blue component*/
};
#endif
	
	typedef void ASAPI (*ADM_ItemNotifyProc)(
						ADMItemRef		item,
						ADMNotifierRef	ref);

typedef struct	FLOAT_PALtemplate {			/* Format of a FLOAT_PAL resource	*/
	long		positionL;
	A_Rect		rc;							/* can't use ASRect because it's
												differently defined on platforms */
	char		group_nameA[256];
} FLOAT_PALtemplate, *FLOAT_PALtemplateP, **FLOAT_PALtemplateH;

typedef enum ADM_PalReplacement {
	ADM_ReplaceWithStandardColor,
	ADM_ReplaceWithRGBColor,
	ADM_ReplaceTerminator
} ADM_PalReplacementType;

typedef struct	ADM_PictPalReplacer  {	// PICT Color table replacement
	#ifdef MSWindows
		u_long				orig_entryLu;				
	#else
		RGBColor			orig_entry_rgb;
	#endif
	RGBColor				rgb_replacement;
	ADMColor				adm_color_replacement;
	short					color_table_indexS;
	ADM_PalReplacementType	replacement_type;
	char					paddingC;
} ADM_PictPalReplacer, *ADM_PictPalReplacerP;

typedef void ASAPI (*AE_ADMTabbedPaletteCalcProc)(
						ADMDialogRef	dialog,
						long			*widthPS,
						long			*heightPS);


A_Err ADM_Birth		(void);
A_Err ADM_Death		(void);

A_Err ADM_BeginDrawing(void);

A_Err ADM_EndDrawing	(void);

A_Err	ADM_GetSuite	(
					short				indexS,			/* >> */
					void				**suitePPV);	/* << */

A_Err	ADM_CreateTabbedPalette(
					const char			*titleZ,		/* >> */
					ADMDialogStyle		style,			/* >> */
					ASInt32				options,		/* >> */
					ADMDialogInitProc	data,			/* >> */
					ADMDialogRef		*dialogP);		/* << */

A_Err	ADM_DestroyTabbedPalette(
					ADMDialogRef		dlg_ref);		/* >> */

A_Err ADM_SetAppResFileRefNum(
					short				res_refnum);	/* >> */

A_Err ADM_DrawText(
					A_Boolean				eraseB,			/* >> */
					ADMDrawerRef		drawer,			/* >> */
					short				max_widthS,		/* >> */
					char				*bufPC,			/* >> */
					ASPoint				*pen_locP);		/* >> */

A_Err ADM_DrawTextRight(
					A_Boolean				eraseB,			/* >> */
					ADMDrawerRef		drawer,			/* >> */
					short				max_widthS,		/* >> */
					char				*bufPC,			/* >> */
					ASPoint				*pen_locP);		/* >> */

A_Err ADM_DrawTextRectCenter(
					ADMDrawerRef		drawer,			/* >> */
					char				*bufPC,			/* >> */
					ASRect				*dst_rectP);	/* >> */

A_Err ADM_SetTabPalWingMenu(
					short				res_refnumS,	/* >> */
					ADMDialogRef		dialog,			/* >> */
					short				menu_idS,		/* >> */
					ADM_ItemNotifyProc	notify_proc);	/* >> */

A_Err ADM_EnableTabPalWingMenu(
					ADMDialogRef		dialog,			/* >> */
					ASBoolean			enableB);		/* >> */

A_Err	ADM_AdherePaletteToPrefSize(
					A_Boolean				sizeB,			/* >> */
					A_Boolean				positionB,		/* >> */
					ADMDialogRef		dlg_ref,		/* >> */
					FLOAT_PALtemplateP 	float_dataP);	/* >> */

A_Err ADM_PrefShowPalette(
					ADMDialogRef		dlg_ref,		/* >> */
					FLOAT_PALtemplateP 	float_dataP);	/* >> */

A_Err ADM_ShowAllFloatingDialogs(
					A_Boolean				showB);			/* >> */

A_Err ADM_ShowDialog(
					ADMDialogRef		dlg_ref,		/* >> */	
					A_Boolean				showB);			/* >> */

A_Err ADM_CountDialogs(
					long				*countPL);		/* << */	

A_Err ADM_GetNthDialog(
					long				indexL,			/* >> */	
					ADMDialogRef		*dlg_refP);		/* << */	

A_Err ADM_IsDialogShowing(
					ADMDialogRef		dlg_ref,		/* >> */	
					A_Boolean				*showingPB);	/* << */

A_Err ADM_GetDialogBounds(
					ADMDialogRef		dlg_ref,		/* >> */	
					A_Rect				*boundsPR);		/* << */

A_Err ADM_ToolTipsEnabled(
					A_Boolean				*enabledPB);	/* << */	

A_Err ADM_EnableToolTips(
					A_Boolean				enableB);		/* >> */	

A_Err ADM_GetDialogBounds(
					ADMDialogRef		dlg_ref,		/* >> */	
					A_Rect				*boundsPR);		/* << */

A_Err ADM_EnableDialog(
					ADMDialogRef		dlg_ref,		/* >> */	
					A_Boolean				enableB);		/* >> */

A_Err	ADM_EnableAllFloaters(
					A_Boolean				enableB);		/* >> */

A_Err ADM_EnableTabPalWingMenu(
	ADMDialogRef	dialog, 
	ASBoolean		enableB);

A_Err ADM_SetTabPalWingMenu(
	short				res_refnumS,		/* >> */
	ADMDialogRef		dialog, 			/* >> */
	short				menu_idS,			/* >> */
	ADM_ItemNotifyProc	notify_proc); 		/* >> */ 