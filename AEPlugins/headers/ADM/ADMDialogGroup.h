/***********************************************************************/

/*                                                                     */

/* ADMDialogGroup.h                                                    */

/* ADM Dialog Group Suite                                              */

/*                                                                     */

/* Copyright 1996-1999 Adobe Systems Incorporated.                     */

/* All Rights Reserved.                                                */

/*                                                                     */

/* Patents Pending                                                     */

/*                                                                     */

/* NOTICE: All information contained herein is the property of Adobe   */

/* Systems Incorporated. Many of the intellectual and technical        */

/* concepts contained herein are proprietary to Adobe, are protected   */

/* as trade secrets, and are made available only to Adobe licensees    */

/* for their internal use. Any reproduction or dissemination of this   */

/* software is strictly forbidden unless prior written permission is   */

/* obtained from Adobe.                                                */

/*                                                                     */

/* Started by Matt Foster, 09/28/1996                                  */

/*                                                                     */

/***********************************************************************/



#ifndef __ADMDialogGroup__

#define __ADMDialogGroup__



#ifndef __ADMTypes__

#include "ADMTypes.h"

#endif





// -----------------------------------------------------------------------------



#ifdef __cplusplus

extern "C" {

#endif



#pragma PRAGMA_ALIGN_BEGIN

#pragma PRAGMA_IMPORT_BEGIN





// -----------------------------------------------------------------------------

//	Error codes.



#define kDockHostConflictError	'DOCK'

#define kTabGroupNotFoundError	'T!FD'

#define kAlreadyDockedError		'DCKD'





// -----------------------------------------------------------------------------

//	For the xxGroupInfo() functions, positionCode is a code to restore a

//	dialog's position within a docked/tabbed group. The group is

//	referred to by name, this being the name of the ADM Dialog that is

//	the first tab in the top dcok of the group.

//

//	You don't need to know what positionCode means, but if you are curious:

//

//	byte		  		meaning

//	----		 		------------------------------

//	1 (0x000000ff)		dock position. 0 is no dock, 1 is first docked (i.e. top dock), 2 etc.

//	2 (0x0000ff00)		tab position, 0 is no tab group, 1 is the 1st tab, 2 etc.

//	3 (0x00010000)		bit 16, boolean, 1 for front tab.

//	3 (0x00020000)		bit 17, boolean, 0 is zoom up, 1 is zoom down.

//	3 (0x00040000)		bit 18, boolean, 0 is in hidden dock, 1 is in visible dock.

//	4 (0x00000000)		reserved. currently unused

//

//	So for stand alone palette, the tab, dock position code is (0, ?, 0, 0)





	// default position code for stand alone palette

#define kADM_DPDefaultCode 0x00030000



	// dialog position byte

#define kADM_DPDockBit 0

#define kADM_DPTabBit 8

#define kADM_DPFrontTabBit 16

#define kADM_DPZoomBit 17

#define kADM_DPDockVisibleBit 18



	// dialog position code masks

#define kADM_DPDockMask 0x000000ff

#define kADM_DPTabMask 0x0000ff00

#define kADM_DPFrontTabMask 0x00010000

#define kADM_DPZoomMask 0x00020000

#define kADM_DPDockVisibleMask 0x00040000



	// strings for saving palette state into preference file

#define kADM_DPLocationStr "Location"

#define kADM_DPSizeStr "Size"			// save only for resizable standalone palettes

#define kADM_DPVisibleStr "Visible"

#define kADM_DPDockCodeStr "DockCode"

#define kADM_DPDockGroupStr "DockGroup"



#define kADM_DPDockGroupStrMaxLen 65

			// group name max size = 64 chars. 





// =============================================================================

//		* ADM Dialog Group Suite

// =============================================================================



#define kADMDialogGroupSuite "ADM Dialog Group Suite"

#define kADMDialogGroupSuiteVersion2 2



// -----------------------------------------------------------------------------



typedef struct ADMDialogGroupSuite2

{



	// *** This suite is FROZEN.

	// *** It shipped with Photoshop 5.5 and ImageReady 2.0.



	// dialog info



	ASErr ASAPI (*GetDialogName)(ADMDialogRef inDialog, const char** outName);



	ASErr ASAPI (*GetDialogGroupInfo)(ADMDialogRef inDialog, const char** outGroupName,

				ASInt32* outPositionCode);



	ASErr ASAPI (*SetDialogGroupInfo)(ADMDialogRef inDialog, const char* inGroupName,

				ASInt32 inPositionCode);



	// docking group info



		// The position codes used below are obtained from above functions or prefs.



		// You should always create a palette that is not stand alone

	ASBoolean ASAPI (*IsStandAlonePalette)(ASInt32 inPositionCode);



		// Your "show palette" menu would be "Show..." unless palette dock code

		// returns true for IsDockVisible() and IsFrontTab().

	ASBoolean ASAPI (*IsDockVisible)(ASInt32 inPositionCode);

	ASBoolean ASAPI (*IsFrontTab)(ASInt32 inPositionCode);



		// You probably won't ever use this, but it here for completeness.

	ASBoolean ASAPI (*IsCollapsed)(ASInt32 inPositionCode);



	// place a dialog in a tab group



	ASErr ASAPI (*SetTabGroup)(ADMDialogRef inDialog, const char* inTabGroupName,

				ASBoolean inBringToFront);

 

 	// dialog list accessors

 

	ASErr ASAPI (*CountDialogs)(ASInt32* outCount);

	ASErr ASAPI (*GetNthDialog)(ASInt32 inIndex, ADMDialogRef* outDialog);

	ASErr ASAPI (*GetNamedDialog)(const char* inName, ADMDialogRef* outDialog);



	// show/hide floating palettes



	void ASAPI (*ToggleAllFloatingDialogs)();

	void ASAPI (*ShowAllFloatingDialogs)(ASBoolean inShow);



}

ADMDialogGroupSuite2;



// -----------------------------------------------------------------------------



#ifndef __ADMDialogGroupOld__

#ifdef MAC_ENV

#include "ADMDialogGroupOld.h"

#else

#include ".\Legacy\ADMDialogGroupOld.h"

#endif

#endif



// -----------------------------------------------------------------------------



#pragma PRAGMA_IMPORT_END

#pragma PRAGMA_ALIGN_END



#ifdef __cplusplus

}

#endif



#endif

