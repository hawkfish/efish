/***********************************************************************/

/*                                                                     */

/* ADMIconOld.h                                                        */

/* Old versions of ADM Icon Suite                                      */

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



#ifndef __ADMIconOld__

#define __ADMIconOld__



#ifndef __ADMTypes__

#include "ADMTypes.h"

#endif



#ifndef __ADMIcon__

#include "ADMIcon.h"

#endif





// -----------------------------------------------------------------------------



#ifdef __cplusplus

extern "C" {

#endif



#pragma PRAGMA_ALIGN_BEGIN

#pragma PRAGMA_IMPORT_BEGIN





// =============================================================================

//		* ADM Icon Suite, version 1

// =============================================================================



#define kADMIconSuiteVersion1 1

#define kADMIconSuiteVersion kADMIconSuiteVersion1



// -----------------------------------------------------------------------------



typedef struct ADMIconSuite1

{



	// icon creation/destruction



	ADMIconRef ASAPI (*GetFromResource)(SPPluginRef inPluginRef, ASInt32 inIconID,

				ASInt32 inIconIndex);



	ADMIconRef ASAPI (*Create)(ADMIconType inIconType, ASInt32 inWidth,

				ASInt32 inHeight, void* inData);



	void ASAPI (*Destroy)(ADMIconRef inIcon);



	// icon data accessors



	ADMIconType ASAPI (*GetType)(ADMIconRef inIcon);

	ASInt32 ASAPI (*GetWidth)(ADMIconRef inIcon);

	ASInt32 ASAPI (*GetHeight)(ADMIconRef inIcon);

	ASAPI void* (*GetData)(ADMIconRef inIcon);



	ASBoolean ASAPI (*IsFromResource)(ADMIconRef inIcon);



}

ADMIconSuite1;



typedef ADMIconSuite1 ADMIconSuite;



// -----------------------------------------------------------------------------



#pragma PRAGMA_IMPORT_END

#pragma PRAGMA_ALIGN_END



#ifdef __cplusplus

}

#endif



#endif  // __ADMIconOld__

