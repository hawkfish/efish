#include "ColorPal.h"

//	Include later to avoid Windoze stuipidity
#include "Easy_Cheese.h"

static AEGP_Command			S_Color_Pal_cmd			=	0L;
static AEGP_PluginID		S_my_id					=	0L;
static long					S_idle_count			=	0L;
static SPBasicSuite			*sP						=	NULL;
/* Used for the ADM palette */

ColorPal	*gColorPal = NULL;



/*	Use of AEGP_SuiteHandler requires the implementation of the following
	member function.
*/

void AEGP_SuiteHandler::MissingSuiteError() const
{
	throw PF_Err_INTERNAL_STRUCT_DAMAGED;	// quit out to the main app with an error code
};
	

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

#pragma mark -

static A_Err
UpdateMenuHook(
	AEGP_GlobalRefcon		plugin_refconPV,	/* >> */
	AEGP_UpdateMenuRefcon	refconPV,			/* >> */
	AEGP_WindowType			active_window)		/* >> */
{
	A_Err 	err =		A_Err_NONE,		err2	=	A_Err_NONE;
	AEGP_SuiteHandler	suites(sP);
	
	AEGP_ItemH		active_itemH				=	NULL;
		
	err = suites.ItemSuite1()->AEGP_GetActiveItem(&active_itemH);

	if (!err)
	{
		if (active_itemH)
		{
			err = suites.CommandSuite1()->AEGP_EnableCommand(S_Color_Pal_cmd);

			if (!err)
			{
				err = suites.CommandSuite1()->AEGP_CheckMarkMenuCommand(S_Color_Pal_cmd, gColorPal && gColorPal->UpdateMenu());
			}
		}
		else 
		{
			err = suites.CommandSuite1()->AEGP_DisableCommand(S_Color_Pal_cmd);
		}					
	}

	if (err2 && !err)
	{
		err = err2;
	}

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
	A_Err			err 		= A_Err_NONE;
	
	if (command == S_Color_Pal_cmd)
	{
		*handledPB = TRUE;
		if (gColorPal) gColorPal->Command ();
		
	}
	
	return err;
}

static	A_Err	
IdleHook(
	AEGP_GlobalRefcon	plugin_refconP,	
	AEGP_IdleRefcon		refconP,		
	long *max_sleepPL)
{
	A_Err					err			= A_Err_NONE;
	
	S_idle_count++;
	
	return err;
}

static A_Err		
DeathHook(
	AEGP_GlobalRefcon	plugin_refconP,			
	AEGP_DeathRefcon	refconP)
{
	A_Err					err = A_Err_NONE;

	if (gColorPal) {
		delete gColorPal;
		gColorPal = NULL;
		}//endif need to get rid of color palette handlers  . . . 
		
	return err;
}

#pragma mark -

DllExport A_Err
GPMain_Color_Pal(
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
	
	err = suites.CommandSuite1()->AEGP_GetUniqueCommand(&S_Color_Pal_cmd);
	
	
	if (!err && S_Color_Pal_cmd) 
	{
		err = suites.CommandSuite1()->AEGP_InsertMenuCommand(S_Color_Pal_cmd, "Color Pal", AEGP_Menu_FLOATERS, AEGP_MENU_INSERT_SORTED);
	}
	else
	{
		err = suites.UtilitySuite1()->AEGP_ReportInfo(S_my_id, "Color Pal: Could not add menu item.");
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
		err2 = suites.UtilitySuite1()->AEGP_ReportInfo(S_my_id, "Color Pal : Could not register command hook.");

	}
	else if (!gColorPal)
	{
		gColorPal = new ColorPal (pica_basicP, aegp_plugin_id);
	}
	
	return err;
}

#pragma mark -
#if 0

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

#endif