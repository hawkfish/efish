// Color_Pal.h

#pragma once

#ifdef MSWindows
	#include <windows.h>
#endif

#include <stdio.h>
#include <string.h>

#define __ADMDialogOld__ 1

#include "entry.h"
#include "AE_GeneralPlug.h"
#include "A.h"
#include "AE_EffectUI.h"
#include "ADM_PaletteUIConstants.h"
#include "ADMItem.h"
#include "ADMBasic.h"
#include "ADMList.h"
#include "ADMEntry.h"
#include "ADMDialog.h"
#include "ADMDialogGroup.h"
#include "ADMNotifier.h"
#include "SDK_ADM.h"
#include "DlgLayoutUtils.h"
#include "ADMItem.h"
#include "SPSuites.h"
#include "AE_AdvEffectSuites.h"
#include "AE_EffectCBSuites.h"
#include "AE_Macros.h"
#include "AEGP_SuiteHandler.h"
#include "stdlib.h"




#define ADMBasicSuite				ADMBasicSuite5
#define ADMDialogSuite				ADMDialogSuite6
#define ADMItemSuite				ADMItemSuite5
#define ADMListSuite				ADMListSuite2
#define ADMEntrySuite				ADMEntrySuite4
#define ADMNotifierSuite			ADMNotifierSuite2
#define ADMTrackerSuite				ADMTrackerSuite1
#define ADMDrawerSuite				ADMDrawerSuite3
#define ADMDialogGroupSuite			ADMDialogGroupSuite2
#define ADMIconSuite				ADMIconSuite2
#define ADMWinHostSuite				ADMWinHostSuite3
#define ADMMacHostSuite				ADMMacHostSuite2
#define ADMWindowSuite				ADMWindowSuite1


#define admOffsetRect(r, xOffset, yOffset) {(*r).left += xOffset; (*r).right += xOffset; (*r).top += yOffset; (*r).bottom += yOffset;}

#define AEGP_MAX_STREAM_DIM 4

#define MAX(A,B)			((A) > (B) ? (A) : (B))

#ifdef __cplusplus
extern "C" {
#endif

DllExport A_Err
GPMain_Color_Pal(
	struct SPBasicSuite		*pica_basicP,			/* >> */
	A_long				 	major_versionL,			/* >> */		
	A_long					minor_versionL,			/* >> */		
	const A_char		 	*file_pathZ,				/* >> platform-specific delimiters */
	const A_char		 	*res_pathZ,				/* >> platform-specific delimiters */
	AEGP_PluginID			aegp_plugin_id,			/* >> */
	void						*global_refconPV);		/* << */

#ifdef __cplusplus
}
#endif
