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


/*	
	Easy_Cheese.cpp

	Demonstrates usage of Keyframer API, specifically the 
	KeyFrameSuite. 
	

	version		notes							engineer		date
	
	1.0			First implementation			bbb				10/15/00
	2.0			Added AEGP_SuiteHandler usage	bbb				11/6/00
	2.5			Added ADM Palette handling		bbb				1/4/01
	
*/

#include "Easy_Cheese.h"
#include "ColorPal.h"
#include "ADMDrawer.h"

static AEGP_Command			S_Easy_Cheese_cmd		=	0L;
static AEGP_PluginID		S_my_id					=	0L;
static long					S_idle_count			=	0L;
static unsigned long		S_mem_id				=	0L;
static Easy_CheeseOptions 	S_options;
static SPBasicSuite			*sP						=	NULL;
/* Used for the ADM palette */

static	ADMItemRef		S_name_item				=	0L;
static	ADMItemRef		S_edit_item				=	0L;
static	ADMItemRef		S_apply_item			=	0L;
static	A_Boolean		S_invalid_paramB		= 	FALSE;
static	A_Boolean		S_is_enabledB 			=	FALSE;
static	ADMDialogRef	S_cheese_dlgref			=	0L;

static	Dlg_layout_params	S_dlg_params;

extern long	ParseLayoutStr(const char *strZ);

ColorPal	*gColorPal = NULL;



/*	Use of AEGP_SuiteHandler requires the implementation of the following
	member function.
*/

void AEGP_SuiteHandler::MissingSuiteError() const
{
	throw PF_Err_INTERNAL_STRUCT_DAMAGED;	// quit out to the main app with an error code
};
	
static	void	ASAPI	EasyCheeseCalcLayout	(
ADMDialogRef	dlg_ref,
long			*widthPL,
long			*heightPL					)

{
	*widthPL = ADM_Floating_Palette_Width;
	*heightPL = ADM_Floating_Palette_Height;
}


static	void	ASAPI	EasyCheeseApplyNotifyProc(ADMItemRef item, ADMNotifierRef notifier)
{
	A_Err		err = A_Err_NONE, err2 = A_Err_NONE;

	ADMItemSuite		*adm_itemP		=	NULL;
	ADMNotifierSuite	*adm_notifierP	=	NULL;
	
	err = sP->AcquireSuite(kADMItemSuite, kADMItemSuiteVersion, (const void **)&adm_itemP);

	if (!err && adm_itemP)
	{
		err = sP->AcquireSuite(kADMNotifierSuite, kADMNotifierSuiteVersion, (const void **)&adm_notifierP);
	}
	


	adm_itemP->DefaultNotify(item, notifier);
	
	if (adm_notifierP->IsNotifierType(notifier, kADMUserChangedNotifier))	
	{
		AEGP_SuiteHandler	suites(sP);
	
		AEGP_CompH			current_compH;
		AEGP_ItemH			current_selectionH = 0, 
							active_itemH = 0;
		AEGP_LayerH			current_layerH;
 		AEGP_StreamRefH		current_streamH, marker_streamH;
		AEGP_StreamFlags	stream_flags = AEGP_StreamFlag_NONE;
		AEGP_ItemType		type = AEGP_ItemType_NONE;

		A_long				key_iL		=	0, 
							key_lL		=	0, 
							key_rL		=	0, 
							num_layersL =	0,
							num_kfsL	=	0;

		AEGP_KeyframeInterpolationType	in_interp	=	AEGP_KeyInterp_NONE, 
										out_interp	=	AEGP_KeyInterp_NONE;
		int					i = 0;
		A_FpLong			min = 0, 
							max = 0;

		A_FpLong			*in_velP	= 0, 
							*out_velP	= 0, 
							*in_inflP	= 0, 
							*out_inflP	= 0;

		
		A_char				item_nameAC[AEGP_MAX_ITEM_NAME_SIZE] = {'\0'};

		
		err	=	suites.ItemSuite1()->AEGP_GetActiveItem(&active_itemH);

		if (!err && active_itemH)
		{
			err = suites.ItemSuite1()->AEGP_GetItemType(active_itemH, &type);

			if (!err && (AEGP_ItemType_COMP == type))
			{		
				err = suites.UtilitySuite1()->AEGP_StartUndoGroup("Easy Cheese!");					
			}
			if (!err)
			{
				err = suites.CompSuite1()->AEGP_GetCompFromItem(active_itemH, &current_compH);

				if (!err && current_compH)
				{
					err = suites.ItemSuite1()->AEGP_GetItemName(active_itemH, item_nameAC);

					if (!err && item_nameAC)
					{
						err = suites.LayerSuite1()->AEGP_GetCompNumLayers(current_compH, &num_layersL);

						if (!err && num_layersL)
						{
							if (!err)
							{
								//	Easy Cheese only works on the first layer in a comp. Sorry. 
								
								err = suites.LayerSuite1()->AEGP_GetCompLayerByIndex(current_compH, 0, &current_layerH);

								if (!err && current_layerH)
								{
									
									
									err = suites.StreamSuite1()->AEGP_GetNewLayerStream(S_my_id, 
																						current_layerH,
																						AEGP_LayerStream_MARKER,
																						&marker_streamH);


									if (!err && marker_streamH)
									{
									
										/*	Easy Cheese deals only with position. You can access 
											any AEGP_LayerStream this way.
										*/
								
										err = suites.StreamSuite1()->AEGP_GetNewLayerStream(S_my_id, 
																							current_layerH,
																							AEGP_LayerStream_POSITION, 
																							&current_streamH);


										if (!err && current_streamH)
										{
						
											err = suites.StreamSuite1()->AEGP_GetStreamProperties(	current_streamH,
																									&stream_flags,
																									&min,
																									&max);

											if (!err)
											{
												A_Boolean expressions_onB = FALSE;
												
												err = suites.StreamSuite1()->AEGP_GetExpressionState(	S_my_id,
																										current_streamH,
																										&expressions_onB);
																								
												if (!err && expressions_onB)
												{
													AEGP_MemHandle expressionHZ = NULL;
													err = suites.StreamSuite1()->AEGP_GetExpression(S_my_id,
																									current_streamH,
																									&expressionHZ);

													if (!err && *expressionHZ)
													{
														err = suites.UtilitySuite1()->AEGP_ReportInfo(S_my_id, "I found an expression.");
														
														err2 = suites.MemorySuite1()->AEGP_FreeMemHandle(expressionHZ);
													}


												}

												A_Boolean variesB = FALSE;
												
												err = suites.StreamSuite1()->AEGP_IsStreamTimevarying(current_streamH, &variesB);

												if (!err && variesB)
												{
													err = suites.KeyframeSuite1()->AEGP_GetStreamNumKFs(current_streamH, &num_kfsL);
												
													if (!err && num_kfsL)
													{
														AEGP_KeyframeInterpolationType in = AEGP_KeyInterp_NONE, out = AEGP_KeyInterp_NONE;
														short dims = 0;

														if (!err)
														{
															err = suites.KeyframeSuite1()->AEGP_GetStreamTemporalDimensionality(	current_streamH,
																																&dims);

															if (!err)
															{
																for (A_long j = 0; (!err && j < num_kfsL) ; j++)
																{
																	A_Time key_timeT;
																											
																	err = suites.KeyframeSuite1()->AEGP_GetKeyframeTime(current_streamH,
																														j,
																														AEGP_LTimeMode_LayerTime,
																														&key_timeT);

																	if (!err)
																	{
																		long new_indexL = 0;
																		
																		err = suites.KeyframeSuite1()->AEGP_InsertKeyframe(	marker_streamH,
																															AEGP_LTimeMode_LayerTime,
																															&key_timeT,
																															&new_indexL);

																		if (!err)
																		{
																			AEGP_StreamValue val;
																			
																			err = suites.KeyframeSuite1()->AEGP_GetNewKeyframeValue(S_my_id,
																																	marker_streamH,
																																	new_indexL,
																																	&val);

																			if (!err)
																			{
																				suites.AnsiSuite1()->strcpy((*(val.val.markerH))->nameAC, S_options.marker_text);
																				suites.AnsiSuite1()->strcpy((*(val.val.markerH))->urlAC, "http://www.adobe.com");
																				suites.AnsiSuite1()->sprintf((*(val.val.markerH))->chapterAC, "%d", j);

																				if (!err)
																				{
																					
																					err = suites.KeyframeSuite1()->AEGP_SetKeyframeValue(	marker_streamH,
																																			new_indexL,
																																			&val);
																				}
																			}
																		}
																	}

																	if (!err)
																	{
																		err = suites.KeyframeSuite1()->AEGP_GetKeyframeInterpolation(	current_streamH,
																																		j,
																																		&in,
																																		&out);
																		if (!err)
																		{
																			for (short k = 0; k < dims; k++)
																			{
																				AEGP_KeyframeEase ease_in, ease_out;

																				AEFX_CLR_STRUCT(ease_in);
																				AEFX_CLR_STRUCT(ease_out);

																				err = suites.KeyframeSuite1()->AEGP_GetKeyframeTemporalEase(current_streamH,
																																			j,
																																			k,
																																			&ease_in,
																																			&ease_out);
																				if (!err)
																				{
																					ease_in.speedF		=	ease_out.speedF		= 0.0;
																					ease_in.influenceF	=	ease_out.influenceF = .33333333;
																					
																					err = suites.KeyframeSuite1()->AEGP_SetKeyframeTemporalEase(current_streamH,
																																				j,
																																				k,
																																				&ease_in,
																																				&ease_out);
																					if (!err)
																					{
																						err = suites.KeyframeSuite1()->AEGP_SetKeyframeInterpolation(current_streamH,
																																					 j,
																																					 AEGP_KeyInterp_BEZIER,
																																					 AEGP_KeyInterp_BEZIER);

																					}
																				}
																			}
																		}
																	}
																}
															}
														}
													}
													if (!err)
													{
														A_char msg[AEGP_MAX_ABOUT_STRING_SIZE] = {'\0'};
														
														suites.AnsiSuite1()->sprintf(msg, "I found %d keyframes", num_kfsL);
														
														if (!err)
														{
															err = suites.UtilitySuite1()->AEGP_ReportInfo(S_my_id, msg);
														}
													}

													
													if (!err && num_kfsL)
													{
														err = suites.StreamSuite1()->AEGP_DisposeStream(current_streamH);
													}
												}
											}
										}
									}
								}
							} 
						}
					}
				}
				err2 = suites.UtilitySuite1()->AEGP_EndUndoGroup();


			}
		}
	}

	err = 	sP->ReleaseSuite(kADMItemSuite, kADMItemSuiteVersion);

	if (!err)
	{
		err = 	sP->ReleaseSuite(kADMNotifierSuite, kADMNotifierSuiteVersion);
	}

}
static A_Err
UpdateMenuHook(
	AEGP_GlobalRefcon		plugin_refconPV,	/* >> */
	AEGP_UpdateMenuRefcon	refconPV,			/* >> */
	AEGP_WindowType			active_window)		/* >> */
{
	A_Err 	err =		A_Err_NONE,		err2	=	A_Err_NONE;
	ADMDialogSuite		*adm_dialogP			=	NULL;
	AEGP_SuiteHandler	suites(sP);
	
	AEGP_ItemH		active_itemH				=	NULL;
		
	err = suites.ItemSuite1()->AEGP_GetActiveItem(&active_itemH);

	if (!err)
	{
		if (active_itemH)
		{
			err = suites.CommandSuite1()->AEGP_EnableCommand(S_Easy_Cheese_cmd);

			if (!err)
			{
				if (S_cheese_dlgref)
				{
					err = sP->AcquireSuite(kADMDialogSuite, kADMDialogSuiteVersion6, (const void**)&adm_dialogP);
				
					if (!err)
					{
						err = suites.CommandSuite1()->AEGP_CheckMarkMenuCommand(S_Easy_Cheese_cmd, adm_dialogP->IsVisible(S_cheese_dlgref));
					}
					
					err2 =	sP->ReleaseSuite(kADMDialogSuite, kADMDialogSuiteVersion6);
				}
			}
		}
		else 
		{
			err = suites.CommandSuite1()->AEGP_DisableCommand(S_Easy_Cheese_cmd);
		}					
	}

	if (err2 && !err)
	{
		err = err2;
	}

	return err;
}

static	void	ASAPI	EasyCheeseToleranceNotifyProc(ADMItemRef item, ADMNotifierRef notifier)
{
	ADMItemSuite		*adm_itemP		=	NULL;
	ADMNotifierSuite	*adm_notifierP	=	NULL;
	A_Err				err				=	A_Err_NONE,
						err2			=	A_Err_NONE,
						err3			=	A_Err_NONE;
	double				temp			=	0.0;

	err = sP->AcquireSuite(kADMItemSuite, kADMItemSuiteVersion, (const void **)&adm_itemP);

	if (!err && adm_itemP)
	{
		err = sP->AcquireSuite(kADMNotifierSuite, kADMNotifierSuiteVersion, (const void **)&adm_notifierP);
	}
	
	if (!err && adm_notifierP)
	{
	
		adm_itemP->DefaultNotify(item, notifier);
		
		if (adm_notifierP->IsNotifierType(notifier, kADMUserChangedNotifier))	
		{
			adm_itemP->GetText(item, S_options.marker_text, AEGP_MAX_MARKER_NAME_SIZE);
		}
	}
	err2 = sP->ReleaseSuite(kADMItemSuite, kADMItemSuiteVersion);

	if (err2 && !err)
	{
		err = err2;
	}
	err3 = sP->ReleaseSuite(kADMNotifierSuite, kADMNotifierSuiteVersion);
	
	if (err3 && !err)
	{
		err = err3;
	}
}

void ASAPI DefaultDialogDraw( 
	ADMDialogRef dialog,
	ADMDrawerRef drawer)
{
	ADMDialogSuite *adm_dialogP;

	A_Err			err = A_Err_NONE;
	
	err = sP->AcquireSuite(kADMDialogSuite, kADMDialogSuiteVersion6, (const void**)&adm_dialogP);	
	
	if (!err && adm_dialogP)  
	{
		adm_dialogP->DefaultDraw(dialog, drawer);

//		DrawChip(dialog, drawer, 0);
	}//endif

	err = sP->ReleaseSuite(kADMDialogSuite, kADMDialogSuiteVersion6);
}

void ASAPI DefaultDialogNotify(
	ADMDialogRef dialog,
	ADMNotifierRef notifier)
{
	A_Err			err			=	A_Err_NONE,
					err2		=	A_Err_NONE;
	ADMDialogSuite *adm_dialogP	=	NULL;


	err = sP->AcquireSuite(kADMDialogSuite, kADMDialogSuiteVersion6, (const void**)&adm_dialogP);	

	if (!err && adm_dialogP)  
	{
		// start off with the default action!!
		adm_dialogP->DefaultNotify( dialog, notifier);

		ADMNotifierSuite	*adm_notifierP = NULL;
		err = sP->AcquireSuite(kADMNotifierSuite, kADMNotifierSuiteVersion, (const void**)&adm_notifierP);	
		if (!err && adm_notifierP) {
			if (adm_notifierP->IsNotifierType(notifier, kADMBoundsChangedNotifier)) {
				ADMItemRef theItem (adm_notifierP->GetItem(notifier));
				ADMDialogRef dialog (adm_notifierP->GetDialog(notifier));

				// if there is no item (and thus the dialog itself that's resizing), notify upstream
				if (theItem == NULL) {
					ASRect r;
					if (dialog != NULL) {
						adm_dialogP->GetLocalRect(dialog, &r);
						if (gColorPal)
							gColorPal->Layout(r);
						}//endif dialog exists!
					}//endif it's not the scroller so it must be the dialog

				}//endif bounds changed
			sP->ReleaseSuite(kADMNotifierSuite, kADMNotifierSuiteVersion);
			}//endif notifier suite found	

		err2 = sP->ReleaseSuite(kADMDialogSuite, kADMDialogSuiteVersion6);
	}//endif dialog suite obtained
	
	if (err2 && !err)
	{
		err = err2;
	}
}

void ASAPI DefaultDialogTrack( 
	ADMDialogRef dialog,
	ADMTrackerRef tracker)
{
	A_Err				err 			= 	A_Err_NONE;
	ADMDialogSuite 		*adm_dialogP	=	NULL;

	err = sP->AcquireSuite(kADMDialogSuite, kADMDialogSuiteVersion6, (const void**)&adm_dialogP);	

	if (!err && adm_dialogP)  
	{		
		adm_dialogP->DefaultTrack( dialog, tracker);
		err = sP->ReleaseSuite(kADMDialogSuite, kADMDialogSuiteVersion6);


	ADMTrackerSuite	*adm_trackerP;
	err = sP->AcquireSuite(kADMTrackerSuite, kADMTrackerSuiteVersion, (const void**)&adm_trackerP);	
	if (!err && adm_trackerP) {
		ASPoint where;

		ADMBasicSuite *adm_basicP;
		
		ADMAction theAction (adm_trackerP->GetAction(tracker));
		if (theAction == kADMButtonUpAction) {
			}//endif mouse-up
	
		sP->ReleaseSuite(kADMTrackerSuite, kADMTrackerSuiteVersion);
		}//endif got tracker suite ok
	}//endif got dialog suite ok
}


static void DestroyTabbedPalette(
	ADMDialogRef dlg_ref)
{
	A_Err			err				=	A_Err_NONE;
	ADMDialogSuite	*adm_dialogP	=	NULL;
	
	err = sP->AcquireSuite(kADMDialogSuite, kADMDialogSuiteVersion6, (const void**)&adm_dialogP);	

	if (!err && adm_dialogP)  
	{		
		adm_dialogP->Destroy(dlg_ref);
		err = sP->ReleaseSuite(kADMDialogSuite, kADMDialogSuiteVersion6);
	}
}

ASErr ASAPI DefaultDialogInit(
	ADMDialogRef dialog)
{
	A_Err					err		= A_Err_NONE,
							err2	= A_Err_NONE;

	AE_ADMTabbedPaletteCalcProc	ae_init_proc;
	ADMDialogSuite			*adm_dialogP;

	err = sP->AcquireSuite(kADMDialogSuite, kADMDialogSuiteVersion6, (const void**)&adm_dialogP);

	if (!err)  
	{
		/* Each ADM dialog needs procedures (procs) for managing different functions. */

		adm_dialogP->SetDrawProc(dialog, 	(ADMDialogDrawProc)DefaultDialogDraw);
		adm_dialogP->SetNotifyProc(dialog,	(ADMDialogNotifyProc)DefaultDialogNotify);
		adm_dialogP->SetTrackProc(dialog, 	(ADMDialogTrackProc)DefaultDialogTrack);
		adm_dialogP->SetDestroyProc(dialog, (ADMDialogDestroyProc)DestroyTabbedPalette);

		ae_init_proc = (AE_ADMTabbedPaletteCalcProc)adm_dialogP->GetUserData(dialog);

		if (ae_init_proc)  
		{
			A_long 	cxL		= 0, 
					cyL		= 0;
			ASRect	bounds;

			// move the dialog offscreen
			
			adm_dialogP->GetBoundsRect(dialog, &bounds);
			
			// Why? Why not?
			
			admOffsetRect(&bounds, 20000, 20000);
			
			adm_dialogP->SetBoundsRect(dialog, &bounds);
			
			ae_init_proc(dialog, &cxL, &cyL);
			
			// need to do this in case restored minimized
			adm_dialogP->SetMaxWidth(dialog, cxL);
			adm_dialogP->SetMinWidth(dialog, cxL);
			adm_dialogP->SetMaxHeight(dialog, cyL);
			adm_dialogP->SetMinHeight(dialog, cyL);
		}//endif init proc
	else {
		gColorPal = new ColorPal(sP, S_my_id, dialog);
		}//else
	}
	if (err)  
	{
		// convert into ADM error
		err = kNotImplementedErr;
	} 
	else 
	{
		err = kNoErr;
	}

	err2 = sP->ReleaseSuite(kADMDialogSuite, kADMDialogSuiteVersion6);

	if (err2 && !err)
	{
		err = err2;
	}
	return err;
}

A_Err ADM_CreateTabbedPalette(
	const char					*titleZ,			/* >> */
	ADMDialogStyle				style,				/* >> */
	ASInt32						options,			/* >> */
	ADMDialogInitProc			 init_proc,			/* >> */
	ADMDialogRef				*dialogP)			/* << */
{
	A_Err					err				= A_Err_NONE;
	ADMDialogRef			dlg_ref			= NULL;
	ADMDialogSuite			*adm_dialogP 	= NULL;
	ADMDialogGroupSuite		*adm_dlg_grpP 	= NULL;
	
	if (!titleZ)
	{
		err = A_Err_PARAMETER;
	}
	
	if (!err)
	{
		err = sP->AcquireSuite(kADMDialogSuite, kADMDialogSuiteVersion6, (const void**)&adm_dialogP);
	}
	
	if (!err && adm_dialogP)  
	{
		err = sP->AcquireSuite(kADMDialogGroupSuite, kADMDialogGroupSuiteVersion, (const void**)&adm_dlg_grpP);
	}

	dlg_ref	= (ADMDialogRef)adm_dialogP->Create(0,
												titleZ,
												0,
												kADMTabbedResizingFloatingDialogStyle, 
												init_proc, 
												0,
												options);

	if (!dlg_ref)  
	{
		err = A_Err_ALLOC;
	}
	
	//adm_dialogP->SetDialogName(dlg_ref, "Easy Cheese!");

	char name[256];

	char* ptr = name;
	
	ptr = (char*)adm_dialogP->GetDialogName(dlg_ref);

	if (!err && (adm_dialogP->GetDialogStyle(dlg_ref) == kADMTabbedFloatingDialogStyle) ||
				(adm_dialogP->GetDialogStyle(dlg_ref) == kADMTabbedResizingFloatingDialogStyle))  

	{
		adm_dlg_grpP->SetDialogGroupInfo(	dlg_ref, 
											"GP1", 
											327937 & kADM_DPDockVisibleMask & kADM_DPFrontTabMask);  // create hidden
	}

	
	if (!err)  
	{
		adm_dialogP->Show(dlg_ref, FALSE);
		*dialogP = dlg_ref;
	}

	err = sP->ReleaseSuite(kADMDialogSuite, kADMDialogSuiteVersion6);
	err = sP->ReleaseSuite(kADMDialogGroupSuite, kADMDialogGroupSuiteVersion);
	
	return err;
}

static A_Err
CommandHook(
	AEGP_GlobalRefcon	plugin_refconPV,		/* >> */
	AEGP_CommandRefcon	refconPV,				/* >> */
	AEGP_Command		command,				/* >> */
	AEGP_HookPriority	hook_priority,			/* >> */
	A_Boolean			already_handledB,		/* >> */
	A_Boolean			*handledPB)				/* << */
{
	A_Err			err 		= A_Err_NONE, err2 = A_Err_NONE;
	ADMDialogSuite	*adm_dlgP 	= NULL;
	ADMItemSuite	*adm_itemP	=	NULL;
	ADMDialogGroupSuite		*adm_dlg_grpP 	= NULL;
	ADMListSuite			*adm_listP = NULL;
	A_Boolean		first_timeB	= FALSE;
	AEGP_SuiteHandler	suites(sP);
	if (command == S_Easy_Cheese_cmd)
	{
		*handledPB = TRUE;

		err = sP->AcquireSuite(kADMDialogSuite, kADMDialogSuiteVersion6, (const void**)&adm_dlgP);

		if (!err)
		{
			err = sP->AcquireSuite(kADMItemSuite, kADMItemSuiteVersion, (const void **)&adm_itemP);
		}
		if (!err)
		{
			err = sP->AcquireSuite(kADMDialogGroupSuite, kADMDialogGroupSuiteVersion, (const void**)&adm_dlg_grpP);
		}
		if (!err)
		{
			err = sP->AcquireSuite(kADMListSuite, kADMListSuiteVersion, (const void**)&adm_listP);
		}
			
		// If I got all my suites, and have never created my dialog before...
		
		if (!err 			&&
			adm_dlgP 		&& 
			adm_itemP 		&& 
			adm_dlg_grpP 	&& 
			adm_listP		&&
			0L == S_cheese_dlgref)	
		{
			ASRect				rc = {0,0,0,0};
			AEGP_ItemH			active_itemH = NULL;
			first_timeB 		= TRUE;
			AEGP_SuiteHandler	suites(sP);			
			
			err = ADM_CreateTabbedPalette(	GetStringPtr(StrID_Name)/*"Easy Cheese"*/,
											(ADMDialogStyle)kADMTabbedFloatingDialogStyle,
											0,
											DefaultDialogInit,
											&S_cheese_dlgref);
						
			
		}
		
		// Copy initial state into static options structure.

		suites.AnsiSuite1()->strcpy(S_options.marker_text, "some silly text");
		
		if (S_cheese_dlgref)  
		{
			if (adm_dlgP->IsVisible(S_cheese_dlgref)) 
			{
				adm_dlgP->Activate(S_cheese_dlgref, FALSE);
				adm_dlgP->Show(S_cheese_dlgref, FALSE);
			} 
			else 
			{
				adm_dlgP->Show(S_cheese_dlgref, TRUE);
				adm_dlgP->Move(S_cheese_dlgref, 100, 100);
				adm_dlgP->Activate(S_cheese_dlgref, TRUE);
			}	
			if (first_timeB)
			{
				err = adm_dlg_grpP->SetTabGroup(S_cheese_dlgref,
												"GP1",
												TRUE);
			}
		}
		err =	sP->ReleaseSuite(kADMDialogSuite, 		kADMDialogSuiteVersion6);
		err = 	sP->ReleaseSuite(kADMItemSuite, 		kADMItemSuiteVersion);
		err = 	sP->ReleaseSuite(kADMDialogGroupSuite, 	kADMDialogGroupSuiteVersion);

	}
	return err;
}

DllExport A_Err
GPMain_Easy_Cheese(
	struct SPBasicSuite		*pica_basicP,		/* >> */
	A_long				 	major_versionL,		/* >> */		
	A_long					minor_versionL,		/* >> */		
	const A_char		 	*file_pathZ,		/* >> platform-specific delimiters */
	const A_char		 	*res_pathZ,			/* >> platform-specific delimiters */
	AEGP_PluginID			aegp_plugin_id,		/* >> */
	void					*global_refconPV)	/* << */
{
	S_my_id										= aegp_plugin_id;
	SPSuiteRef				my_ref 				= 0;
	A_Err 					err 				= A_Err_NONE, err2 = A_Err_NONE;
	AEGP_RegisterSuite1		*register_suiteP	= 0;
	AEGP_CommandSuite1		*command_suiteP		= 0;
	SPSuitesSuite			*suites_suiteP		= 0;
	A_Boolean				suites_loadedB		= FALSE;
	
	sP = pica_basicP;
	
	AEGP_SuiteHandler suites(pica_basicP);
	
	S_options.layerH	= NULL;
	S_options.compH		= NULL;	

	InitDlgLayoutParams();
				
	err = suites.CommandSuite1()->AEGP_GetUniqueCommand(&S_Easy_Cheese_cmd);
	
	
	if (!err && S_Easy_Cheese_cmd) 
	{
		err = suites.CommandSuite1()->AEGP_InsertMenuCommand(S_Easy_Cheese_cmd, "Easy Cheese", AEGP_Menu_FLOATERS, AEGP_MENU_INSERT_SORTED);
	}
	else
	{
		err = suites.UtilitySuite1()->AEGP_ReportInfo(S_my_id, "Easy Cheese: Could not add menu item.");
	}
	
	if (!err) 
	{
		err = suites.RegisterSuite1()->AEGP_RegisterCommandHook(S_my_id, AEGP_HP_BeforeAE, AEGP_Command_ALL, CommandHook, 0);

		if (!err) 
		{
			err = suites.RegisterSuite1()->AEGP_RegisterUpdateMenuHook(S_my_id, UpdateMenuHook, 0);

			if (!err)
			{
				err = suites.RegisterSuite1()->AEGP_RegisterIdleHook(S_my_id, IdleHook, 0);
			}
		}
	} 
	if (!err)
	{
		err = suites.RegisterSuite1()->AEGP_RegisterDeathHook(S_my_id, DeathHook, 0);
	}
	if (err) // not !err, err!
	{
		err2 = suites.UtilitySuite1()->AEGP_ReportInfo(S_my_id, "Easy_Cheese : Could not register command hook.");

	}
	return err;
}

static	void	InitDlgLayoutParams(void)
{
	long	numL;
	
	numL = ParseLayoutStr(GetStringPtr(10));
	if (numL)	{
		S_dlg_params.left_offsetL = numL;
	}	else	{
		S_dlg_params.left_offsetL = ADM_PALETTE_LEFT_OFFSET;
	}
	numL = ParseLayoutStr(GetStringPtr(11));
	if (numL)	{
		S_dlg_params.top_offsetL = numL;
	}	else	{
		S_dlg_params.top_offsetL = ADM_PALETTE_TOP_OFFSET;
	}
	numL = ParseLayoutStr(GetStringPtr(12));
	if (numL)	{
		S_dlg_params.rbn_heightL = numL;
	}	else	{
		S_dlg_params.rbn_heightL = ADM_PALETTE_RBN_HEIGHT;
	}
	numL = ParseLayoutStr(GetStringPtr(13));
	if (numL)	{
		S_dlg_params.rbn_offsetL = numL;
	}	else	{
		S_dlg_params.rbn_offsetL = ADM_PALETTE_RBN_OFFSET;
	}
	numL = ParseLayoutStr(GetStringPtr(14));
	if (numL)	{
		S_dlg_params.popup_offsetL = numL;
	}	else	{
		S_dlg_params.popup_offsetL = ADM_PALETTE_POPUP_OFFSET;
	}
	numL = ParseLayoutStr(GetStringPtr(15));
	if (numL)	{
		S_dlg_params.btn_heightL = numL;
	}	else	{
		S_dlg_params.btn_heightL = ADM_PALETTE_BTN_HEIGHT;
	}
	numL = ParseLayoutStr(GetStringPtr(16));
	if (numL)	{
		S_dlg_params.popup_heightL = numL;
	}	else	{
		S_dlg_params.popup_heightL = ADM_PALETTE_POPUP_HEIGHT;
	}
	numL = ParseLayoutStr(GetStringPtr(17));
	if (numL)	{
		S_dlg_params.txt_heightL = numL;
	}	else	{
		S_dlg_params.txt_heightL = ADM_PALETTE_TXT_HEIGHT;
	}
	numL = ParseLayoutStr(GetStringPtr(18));
	if (numL)	{
		S_dlg_params.edit_heightL = numL;
	}	else	{
		S_dlg_params.edit_heightL = ADM_PALETTE_EDIT_HEIGHT;
	}
	numL = ParseLayoutStr(GetStringPtr(19));
	if (numL)	{
		S_dlg_params.cbx_offsetL = numL;
	}	else	{
		S_dlg_params.cbx_offsetL = ADM_PALETTE_CBX_OFFSET;
	}
	numL = ParseLayoutStr(GetStringPtr(20));
	if (numL)	{
		S_dlg_params.cbx_heightL = numL;
	}	else	{
		S_dlg_params.cbx_heightL = ADM_PALETTE_CBX_HEIGHT;
	}
	numL = ParseLayoutStr(GetStringPtr(21));
	if (numL)	{
		S_dlg_params.txt_offsetL = numL;
	}	else	{
		S_dlg_params.txt_offsetL = ADM_PALETTE_TXT_OFFSET;
	}
	numL = ParseLayoutStr(GetStringPtr(22));
	if (numL)	{
		S_dlg_params.palette_divider_heightL = numL;
	}	else	{
		S_dlg_params.palette_divider_heightL = ADM_PALETTE_DIVIDER_HEIGHT;
	}
}

static	A_Err	IdleHook(
	AEGP_GlobalRefcon	plugin_refconP,	
	AEGP_IdleRefcon		refconP,		
	long *max_sleepPL)
{
	ADMItemSuite			*adm_itemP	=	NULL;
	A_Err					err			= A_Err_NONE, err2	= A_Err_NONE;
	A_Boolean				enableB		= FALSE, is_differentB = FALSE;
	
	AEGP_SuiteHandler		suites(sP);
	
	S_idle_count++;
	
	if (S_cheese_dlgref) // If the dialog exists
	{
		err = sP->AcquireSuite(kADMItemSuite, kADMItemSuiteVersion, (const void **)&adm_itemP);
		
		if (!err && adm_itemP)
		{
			AEGP_ItemH	itemH = NULL;
			
			err = suites.ItemSuite1()->AEGP_GetActiveItem(&itemH);

			//	enable if the current item isn't null; you may want to be a good
			//	deal smarter about enabling and disabling your plug-in than this.
			
			adm_itemP->Enable(S_apply_item, (NULL != itemH));
		}

		err2 = 	sP->ReleaseSuite(kADMItemSuite, kADMItemSuiteVersion);
	}

	if (err)	// not !err, err!
	{
		AEGP_SuiteHandler suites(sP);
		err = suites.UtilitySuite1()->AEGP_ReportInfo(S_my_id, GetStringPtr(StrID_SuiteError));
	}
	

	if (err2 && !err)
	{
		err = err2;
	}
	return err;
}

static A_Err
GP_EasyCheeseIsApplicable(
	AEGP_LayerH			layerH,				/* >> the current active layer */
	AEGP_StreamType		*sel_stream_type0,	/* << */
	A_Boolean			*valid)				/* << */
{
	*valid = TRUE;
	return A_Err_NONE;
}

static A_Err		
DeathHook(
	AEGP_GlobalRefcon	plugin_refconP,			
	AEGP_DeathRefcon	refconP)
{
	A_Err					err = A_Err_NONE;
/*	ADMDialogSuite			*adm_dialogP = NULL;
	
	err = sP->AcquireSuite(kADMDialogSuite, kADMDialogSuiteVersion6, (const void**)&adm_dialogP);	
	if (!err && adm_dialogP)  
	{
		adm_dialogP->Destroy(S_cheese_dlgref);
	}

	err =	sP->ReleaseSuite(kADMDialogSuite, kADMDialogSuiteVersion6);
*/	
	if (gColorPal) {
		delete gColorPal;
		gColorPal = NULL;
		}//endif need to get rid of color palette handlers  . . . 
		
	return err;
}

long	ParseLayoutStr(const char *strZ)

{
	long	numL = 0, str_lenL = 0;
	char	num_strAC[256];
	
	if (strZ)	
	{
		while (*strZ != LAYOUT_TAG && *strZ)	
		{
			strZ++;
		}
		if (LAYOUT_TAG == *strZ)	
		{
			strZ++;
			while (*strZ)	
			{
				num_strAC[str_lenL++] = *strZ++;
			}
		}
		num_strAC[str_lenL] = 0;
		if (str_lenL)	
		{
			numL = atol(num_strAC);
		}
	}
	
	return numL;
}


A_Err ADM_SetTabPalWingMenu(
	short				res_refnumS,		/* >> */
	ADMDialogRef		dialog, 			/* >> */
	short				menu_idS,			/* >> */
	ADM_ItemNotifyProc	notify_proc) 		/* >> */
{
	A_Err				err				=	A_Err_NONE;
	ADMItemSuite		*adm_itemP		=	NULL;
	ADMDialogSuite		*adm_dialogP	=	NULL;
	ADMListSuite		*adm_listP		=	NULL;
	ADMItemRef			item;
	ADMListRef			list;

	err = sP->AcquireSuite(kADMListSuite, kADMListSuiteVersion2, (const void **)&adm_listP);
	
	if (!err && adm_listP)
	{
		err = sP->AcquireSuite(kADMItemSuite, kADMItemSuiteVersion, (const void **)&adm_itemP);
	}

	if (!err && adm_itemP)
	{
		err = sP->AcquireSuite(kADMDialogSuite, kADMDialogSuiteVersion6, (const void**)&adm_dialogP);
	}


	if (0 == dialog)
	{
		err = A_Err_PARAMETER;
	}

	// add wing-tip menu
	if (!err)  
	{
		if (!(item = adm_dialogP->GetItem(dialog, kADMMenuItemID)))  
		{
			err = A_Err_GENERIC;
		}
	}
	if (!err)  
	{
		if (!(list = adm_itemP->GetList(item)))	  
		{
			err = A_Err_GENERIC;
		}
	}
	
	if (!err)  
	{
		// assign menu
		adm_listP->SetMenuID(list, 0, menu_idS, "");

		// set notifier for menu
		adm_itemP->SetNotifyProc(item, notify_proc);
	}

	return( err );
}

A_Err ADM_EnableTabPalWingMenu(
	ADMDialogRef	dialog, 
	ASBoolean		enableB) 
{
	A_Err				err				=	A_Err_NONE;
	long				i				=	0,  
						j				=	0;
	ADMItemSuite		*adm_itemP		=	NULL;
	ADMDialogSuite		*adm_dialogP	=	NULL;
	ADMListSuite		*adm_listP		=	NULL;
	ADMEntrySuite		*adm_entryP		=	NULL;
	ADMItemRef			item;
	ADMListRef			list;
	ADMEntryRef			entry;
	
	
	if (0 == dialog)
	{
		err = A_Err_PARAMETER;
	}

	if (!err)
	{
		err = sP->AcquireSuite(kADMListSuite, kADMListSuiteVersion2, (const void **)&adm_listP);
	}
	
	if (!err && adm_listP)
	{
		err = sP->AcquireSuite(kADMItemSuite, kADMItemSuiteVersion, (const void **)&adm_itemP);
	}

	if (!err && adm_itemP)
	{
		err = sP->AcquireSuite(kADMDialogSuite, kADMDialogSuiteVersion6, (const void**)&adm_dialogP);
	}

	if (!err && adm_dialogP)
	{
		err = sP->AcquireSuite(kADMEntrySuite, kADMEntrySuiteVersion4, (const void**)&adm_entryP);
	}

	if (!err)  
	{
		if (!(item = adm_dialogP->GetItem(dialog, kADMMenuItemID)))  
		{
			err = A_Err_GENERIC;
		}
	}

	if (!err)  
	{
		if (!(list = adm_itemP->GetList(item)))  
		{
			err = A_Err_GENERIC;
		}
	}

	if (!err)  
	{
		j = adm_listP->NumberOfEntries(list);
	}

	// enable/disable everything
	for (i = 0; i < j; i++) 
	{
		entry = adm_listP->GetEntry(list, i);
		if (entry)  
		{
			adm_entryP->Enable(entry, enableB);
		}
	}

	return err ;
}



#ifdef OLD
			long				btn_lenL;
			// Descriptive text
			
			rc.left		= 5;
			rc.top		= 28;
			rc.right	= 55;
			rc.bottom	= 44;
			
			generic_item = adm_itemP->Create(S_cheese_dlgref, 0, kADMTextStaticType, &rc, NULL, NULL, 0);
			adm_itemP->SetText(generic_item, "Number:");
			// Numeric text entry
			
			rc.left 	= rc.right;
			btn_lenL 	= 100;
			rc.right 	= (short)(rc.left + btn_lenL + S_dlg_params.txt_offsetL * 3);
			
			S_edit_item = adm_itemP->Create(S_cheese_dlgref, 1, kADMTextEditType, &rc, NULL, NULL, 0);

			adm_itemP->SetNotifyProc(S_edit_item, EasyCheeseToleranceNotifyProc);
			adm_itemP->SetText(S_edit_item, "some silly text.");
			adm_itemP->SelectAll(S_edit_item);
			
			// Apply button
			
			rc.left	=	165;
			rc.top	=	47;
			rc.right=	201;
			rc.bottom = 63;
			
			S_apply_item = adm_itemP->Create(	S_cheese_dlgref, 
												3, 
												kADMTextPushButtonType, 
												&rc, 
												NULL, 
												NULL, 
												NULL);

			adm_itemP->SetNotifyProc(S_apply_item, EasyCheeseApplyNotifyProc);
			adm_itemP->SetText(S_apply_item, "Apply");
			
			err = suites.LayerSuite1()->AEGP_GetActiveLayer(&S_options.layerH);

			if (NULL == S_options.layerH)	
			{
				adm_itemP->Enable(S_apply_item, FALSE);
			} 
			else 
			{
				adm_itemP->Enable(S_apply_item, TRUE);
			}
#endif //OLD	
