/***********************************************************************/

/*                                                                     */

/* ADMNotifierOld.h                                                    */

/* Old versions of ADM Notifier Suite                                  */

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



#ifndef __ADMNotifierOld__

#define __ADMNotifierOld__



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

//		* ADM Notifier Suite, version 1

// =============================================================================



#define kADMNotifierSuiteVersion1 1

#define kADMNotifierSuiteVersion kADMNotifierSuiteVersion1



typedef struct ADMNotifierSuite1

{

	ADMItemRef ASAPI (*GetItem)(ADMNotifierRef inNotifier);

	ADMDialogRef ASAPI (*GetDialog)(ADMNotifierRef inNotifier);

	

	ASBoolean ASAPI (*IsNotifierType)(ADMNotifierRef inNotifier, const char* inNotifierType);

	void ASAPI (*GetNotifierType)(ADMNotifierRef inNotifier, char* outNotifierType, ASUInt32 inMaxLen);



}

ADMNotifierSuite1;



typedef ADMNotifierSuite1 ADMNotifierSuite;



// -----------------------------------------------------------------------------



#pragma PRAGMA_IMPORT_END

#pragma PRAGMA_ALIGN_END



#ifdef __cplusplus

}

#endif



#endif

