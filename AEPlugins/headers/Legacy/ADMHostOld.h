/***********************************************************************/

/*                                                                     */

/* ADMHostOld.h                                                        */

/* Old versions of ADM Host Suite                                      */

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



#ifndef __ADMHostOld__

#define __ADMHostOld__



#ifndef __ADMTypes__

#include "ADMTypes.h"

#endif



#ifndef __SPMessageData__

#include "SPMData.h"

#endif



#ifndef __SPAccess__

#include "SPAccess.h"

#endif



#ifndef __ADMHost__

#include "ADMHost.h"

#endif





// -----------------------------------------------------------------------------



#ifdef __cplusplus

extern "C" {

#endif



#pragma PRAGMA_ALIGN_BEGIN

#pragma PRAGMA_IMPORT_BEGIN





// =============================================================================

//		* ADM Host Suite, version 1

// =============================================================================



#define kADMHostSuiteVersion1 1



// -----------------------------------------------------------------------------



typedef struct ADMHostSuite1

{



	// AGM calibration



	void ASAPI (*SetAGMCalibrationProc)(ADMAGMCalibrationProc inCalibrationProc);

	ADMAGMCalibrationProc ASAPI (*GetAGMCalibrationProc)();



	// mouse information accessors



	ASBoolean ASAPI (*PointerInUse)();



	// palette layout bounds modifiers



	void ASAPI (*SetModifyPaletteLayoutBoundsProc)(ModifyPaletteLayoutBoundsProc inModifyBoundsProc);

	ModifyPaletteLayoutBoundsProc ASAPI (*GetModifyPaletteLayoutBoundsProc)();



}

ADMHostSuite1;





// =============================================================================

//		* ADM Host Suite, version 2

// =============================================================================



#define kADMHostSuiteVersion2 2

#define kADMHostSuiteVersion kADMHostSuiteVersion2



// -----------------------------------------------------------------------------



typedef struct ADMHostSuite2

{



	// AGM calibration



	void ASAPI (*SetAGMCalibrationProc)(ADMAGMCalibrationProc inCalibrationProc);

	ADMAGMCalibrationProc ASAPI (*GetAGMCalibrationProc)();

	

	// mouse information accessors

	

	ASBoolean ASAPI (*PointerInUse)();

	

	// palette layout bounds modifiers

	

	void ASAPI (*SetModifyPaletteLayoutBoundsProc)(ModifyPaletteLayoutBoundsProc inModifyBoundsProc);

	ModifyPaletteLayoutBoundsProc ASAPI (*GetModifyPaletteLayoutBoundsProc)();



	// host information accessors



	void ASAPI (*SetADMHostAppContextProcs)(const ADMHostAppContextProcs* inAppContextProcs);

	void ASAPI (*GetADMHostAppContextProcs)(ADMHostAppContextProcs* outAppContextProcs);

	

	void ASAPI (*SetADMHostAppName)(const char* inHostName);

	void ASAPI (*GetADMHostAppName)(char* outHostName, ASInt16 inMaxLength);



	void ASAPI (*SetADMResourceAccessProcs)(const ADMResourceAccessProcs* inResProcs);

	void ASAPI (*GetADMResourceAccessProcs)(ADMResourceAccessProcs* outResProcs);



	// ADM build version



	ASUInt32 ASAPI (*GetADMVersion)();

	

	// text behaviors

	

	void ASAPI (*SetADMDefaultTextInfo)(const ADMDefaultTextInfo* inDefaultTextInfo);

	void ASAPI (*GetADMDefaultTextInfo)(ADMDefaultTextInfo* outDefaultTextInfo);



}

ADMHostSuite2;	



typedef ADMHostSuite2 ADMHostSuite;





// =============================================================================

//		* ADM Mac Host Suite, version 1

// =============================================================================



#ifdef MAC_ENV



#define kADMMacHostSuiteVersion1 1



// -----------------------------------------------------------------------------



typedef struct ADMMacHostSuite1 

{



	// event hand-off to ADM



	ASBoolean ASAPI (*HandleADMEvent)(struct EventRecord* inEvent);

	

	// modal dialog event callback



	void ASAPI (*SetModalEventProc)(ADMModalEventProc inModalEventProc);

	ADMModalEventProc ASAPI (*GetModalEventProc)();

	

	// window activation



	void ASAPI (*ActivateWindows)(ASBoolean inActivate);



}

ADMMacHostSuite1;



#endif





// =============================================================================

//		* ADM Win Host Suite, version 1

// =============================================================================



#ifdef WIN_ENV



#define kADMWinHostSuiteVersion1 1



// -----------------------------------------------------------------------------



typedef struct ADMWinHostSuite1 

{



	ASBoolean ASAPI (*HandleADMMessage)(ASWindowRef inFrameWnd, ASUInt16 inMessage,

				ASUInt16 inWParam, ASInt32 inLParam);



	ASWindowRef ASAPI (*GetCurrentOwnerWindow)();



	ASBoolean ASAPI (*IsADMMessage)(const struct tagMSG* inMessage);



}

ADMWinHostSuite1;



#endif





// =============================================================================

//		* ADM Win Host Suite, version 2

// =============================================================================



#ifdef WIN_ENV



#define kADMWinHostSuiteVersion2 2

#define kADMWinHostSuiteVersion kADMWinHostSuiteVersion2



// -----------------------------------------------------------------------------



typedef struct ADMWinHostSuite2

{



	ASBoolean ASAPI (*HandleADMMessage)(ASWindowRef inFrameWnd, ASUInt16 inMessage,

				ASUInt16 inWParam, ASInt32 inLParam);



	ASWindowRef ASAPI (*GetCurrentOwnerWindow)();



	ASBoolean ASAPI (*IsADMMessage)(const struct tagMSG* inMessage);



	ASWindowRef ASAPI (*GetPlatformAppWindow)();

	void ASAPI (*SetPlatformAppWindow) (ASWindowRef inAppWindow);



	ADMModalMessageProc ASAPI (*GetModalMessageProc)();							

	void ASAPI (*SetModalMessageProc)(ADMModalMessageProc inModalMessageProc);	



}

ADMWinHostSuite2;	



typedef ADMWinHostSuite2 ADMWinHostSuite;



#endif



// -----------------------------------------------------------------------------



#pragma PRAGMA_IMPORT_END

#pragma PRAGMA_ALIGN_END



#ifdef __cplusplus

}

#endif



#endif

