/***********************************************************************/

/*                                                                     */

/* ADMDialogGroupOld.h                                                 */

/* Old versions of ADM Dialog Group Suite                              */

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

/***********************************************************************/



#ifndef __ADMDialogGroupOld__

#define __ADMDialogGroupOld__



#ifndef __ADMTypes__

#include "ADMTypes.h"

#endif





// -----------------------------------------------------------------------------



#ifdef __cplusplus

extern "C" {

#endif



#pragma PRAGMA_ALIGN_BEGIN

#pragma PRAGMA_IMPORT_BEGIN





// =============================================================================

//		* ADM Dialog Group Suite, version 1

// =============================================================================



#define kADMDialogGroupSuiteVersion1 1

#define kADMDialogGroupSuiteVersion 1

#define kADMDialogGroupVersion kADMDialogGroupSuiteVersion1



// -----------------------------------------------------------------------------



typedef struct ADMDialogGroupSuite1

{



	ASErr ASAPI (*GetDialogName)(ADMDialogRef inDialog, const char** outName);



	ASErr ASAPI (*GetDialogGroupInfo)(ADMDialogRef inDialog, const char** outGroupName,

				ASInt32* outPositionCode);



	ASErr ASAPI (*SetDialogGroupInfo)(ADMDialogRef inDialog, const char* inGroupName,

				ASInt32 inPositionCode);



	// The queries below take a docking position code obtained from above functions or prefs



		// You should always create a palette that is not stand alone

	ASBoolean ASAPI (*IsStandAlonePalette)(ASInt32 inPositionCode);



		// Your "show palette" menu would be "Show..." unless palette dock code returns true for

		// IsDockVisible() and IsFrontTab(). 

	ASBoolean ASAPI (*IsDockVisible)(ASInt32 inPositionCode);

	ASBoolean ASAPI (*IsFrontTab)(ASInt32 inPositionCode);



		// You probably won't ever use this, but it here for completeness.

	ASBoolean ASAPI (*IsCollapsed)(ASInt32 inPositionCode);



	ASErr ASAPI (*SetTabGroup)(ADMDialogRef inDialog, const char* inTabGroupName,

				ASBoolean inBringToFront);

 

	ASErr ASAPI (*CountDialogs)(ASInt32* outCount);

	ASErr ASAPI (*GetNthDialog)(ASInt32 inIndex, ADMDialogRef* outDialog);

	ASErr ASAPI (*GetNamedDialog)(const char* inName, ADMDialogRef* outDialog);



	void ASAPI (*ToggleAllFloatingDialogs)();



} ADMDialogGroupSuite1;



typedef ADMDialogGroupSuite1 ADMDialogGroupSuite;	// for historical reasons



// -----------------------------------------------------------------------------



#pragma PRAGMA_IMPORT_END

#pragma PRAGMA_ALIGN_END



#ifdef __cplusplus

}

#endif



#endif

