/***********************************************************************/

/*                                                                     */

/* ADMEntry.h                                                          */

/* ADM Entry Suite                                                     */

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

/* Started by Dave Lazarony, 05/07/1996                                */

/*                                                                     */

/***********************************************************************/



#ifndef __ADMEntry__

#define __ADMEntry__



#ifndef __ADMTypes__

#include "ADMTypes.h"

#endif



#ifndef __ADMTracker__

#include "ADMTracker.h"

#endif



#ifndef __ASHelp__

#include "ASHelp.h"

#endif





// -----------------------------------------------------------------------------



#ifdef __cplusplus

extern "C" {

#endif



#pragma PRAGMA_ALIGN_BEGIN

#pragma PRAGMA_IMPORT_BEGIN





// -----------------------------------------------------------------------------

//	Stupid Macintosh headers #define GetItem.



#ifdef GetItem

#undef GetItem

#endif





// -----------------------------------------------------------------------------

//	User provided procedures.



typedef ASBoolean ASAPI (*ADMEntryTimerProc)(ADMEntryRef inEntry, ADMTimerRef inTimer);



typedef void ASAPI (*ADMEntryTimerAbortProc)(ADMEntryRef inEntry, ADMTimerRef inTimer,

			ADMAction inAbortAction);





// -----------------------------------------------------------------------------

//	Standard checkmark glyphs



typedef enum

{

	kADMCheckmarkGlyphID = 0,

	kADMBulletGlyphID = 1,

	kADMHyphenGlyphID = 2,

	kADMDummyGlyphID = 0xFFFFFFFF

}

ADMStandardCheckGlyphID;





// =============================================================================

//		* ADM Entry Suite

// =============================================================================



#define kADMEntrySuite "ADM Entry Suite"

#define kADMEntrySuiteVersion4 4



// -----------------------------------------------------------------------------



typedef struct ADMEntrySuite4

{



	// *** This suite is FROZEN.

	// *** It shipped with Photoshop 5.5 and ImageReady 2.0.



	// entry creation/destruction



	ADMEntryRef ASAPI (*Create)(ADMListRef inList);

	void ASAPI (*Destroy)(ADMEntryRef inEntry);



	// default behaviors

	

	void ASAPI (*DefaultDraw)(ADMEntryRef inEntry, ADMDrawerRef inDrawer);

	ASBoolean ASAPI (*DefaultTrack)(ADMEntryRef inEntry, ADMTrackerRef inTracker);

	void ASAPI (*DefaultNotify)(ADMEntryRef inEntry, ADMNotifierRef inNotifier);

	void ASAPI (*SendNotify)(ADMEntryRef inEntry, const char* inNotifierType);



	// container accessors

	

	ASInt32 ASAPI (*GetIndex)(ADMEntryRef inEntry);

	ADMListRef ASAPI (*GetList)(ADMEntryRef inEntry);



	// entry ID

		

	void ASAPI (*SetID)(ADMEntryRef inEntry, ASInt32 inEntryID);

	ASInt32 ASAPI (*GetID)(ADMEntryRef inEntry);

	

	// entry user data storage

	

	void ASAPI (*SetUserData)(ADMEntryRef inEntry, ADMUserData inUserData);

	ADMUserData ASAPI (*GetUserData)(ADMEntryRef inEntry);



	// entry selection status



	void ASAPI (*Select)(ADMEntryRef inEntry, ASBoolean inSelect);

	ASBoolean ASAPI (*IsSelected)(ADMEntryRef inEntry);



	// entry visibility

	

	void ASAPI (*MakeInBounds)(ADMEntryRef inEntry);

	ASBoolean ASAPI (*IsInBounds)(ADMEntryRef inEntry);



	// entry state accessors



	void ASAPI (*Enable)(ADMEntryRef inEntry, ASBoolean inEnable);

	ASBoolean ASAPI (*IsEnabled)(ADMEntryRef inEntry);

	

	void ASAPI (*Activate)(ADMEntryRef inEntry, ASBoolean inActivate);

	ASBoolean ASAPI (*IsActive)(ADMEntryRef inEntry);

	

	void ASAPI (*Check)(ADMEntryRef inEntry, ASBoolean inCheck);

	ASBoolean ASAPI (*IsChecked)(ADMEntryRef inEntry);

	

	void ASAPI (*MakeSeparator)(ADMEntryRef inEntry, ASBoolean inSeparator);

	ASBoolean ASAPI (*IsSeparator)(ADMEntryRef inEntry);



	// entry bounds accessors



	void ASAPI (*GetLocalRect)(ADMEntryRef inEntry, ASRect* outLocalRect);

	void ASAPI (*GetBoundsRect)(ADMEntryRef inEntry, ASRect* outBoundsRect);

	

	// coordinate transformations



	void ASAPI (*LocalToScreenPoint)(ADMEntryRef inEntry, ASPoint* ioPoint);

	void ASAPI (*ScreenToLocalPoint)(ADMEntryRef inEntry, ASPoint* ioPoint);

	

	void ASAPI (*LocalToScreenRect)(ADMEntryRef inEntry, ASRect* ioRect);

	void ASAPI (*ScreenToLocalRect)(ADMEntryRef inEntry, ASRect* ioRect);



	// redraw requests

	

	void ASAPI (*Invalidate)(ADMEntryRef inEntry);

	void ASAPI (*InvalidateRect)(ADMEntryRef inEntry, const ASRect* inInvalRect);

	void ASAPI (*Update)(ADMEntryRef inEntry);

	

	// entry picture accessors

	

	void ASAPI (*SetPictureID)(ADMEntryRef inEntry, ASInt32 inPictureResID,

				const char* inPictureResName);



	void ASAPI (*GetPictureID)(ADMEntryRef inEntry, ASInt32* outPictureResID,

				const char** outPictureResName);

	

	void ASAPI (*SetSelectedPictureID)(ADMEntryRef inEntry, ASInt32 inPictureResID,

				const char* inPictureResName);



	void ASAPI (*GetSelectedPictureID)(ADMEntryRef inEntry, ASInt32* outPictureResID,

				const char** outPictureResName);

	

	void ASAPI (*SetDisabledPictureID)(ADMEntryRef inEntry, ASInt32 inPictureResID,

				const char* inPictureResName);

				

	void ASAPI (*GetDisabledPictureID)(ADMEntryRef inEntry, ASInt32* outPictureResID,

				const char** outPictureResName);

	

	void ASAPI (*SetPicture)(ADMEntryRef inEntry, ADMIconRef inPicture);

	ADMIconRef ASAPI (*GetPicture)(ADMEntryRef inEntry);



	void ASAPI (*SetSelectedPicture)(ADMEntryRef inEntry, ADMIconRef inPicture);

	ADMIconRef ASAPI (*GetSelectedPicture)(ADMEntryRef inEntry);



	void ASAPI (*SetDisabledPicture)(ADMEntryRef inEntry, ADMIconRef inPicture);

	ADMIconRef ASAPI (*GetDisabledPicture)(ADMEntryRef inEntry);



	// entry text accessors



	void ASAPI (*SetText)(ADMEntryRef inEntry, const char* inText);

	void ASAPI (*GetText)(ADMEntryRef inEntry, char* outText, ASInt32 inMaxLength);

	ASInt32 ASAPI (*GetTextLength)(ADMEntryRef inEntry);

	

	// entry timer accessors

	

	ADMTimerRef ASAPI (*CreateTimer)(ADMEntryRef inEntry, ASUInt32 inMilliseconds,

				ADMActionMask inAbortMask, ADMEntryTimerProc inTimerProc,

				ADMEntryTimerAbortProc inTimerAbortProc, ASInt32 inOptions);



	void ASAPI (*AbortTimer)(ADMEntryRef inEntry, ADMTimerRef inTimer);



	// entry help ID



	ASHelpID ASAPI (*GetHelpID)(ADMEntryRef inEntry);

	void ASAPI (*SetHelpID)(ADMEntryRef inEntry, ASHelpID inHelpID);

	void ASAPI (*Help)(ADMEntryRef inEntry);



	// entry checkmark accessors



	void ASAPI (*SetCheckGlyph)(ADMEntryRef inEntry, ADMStandardCheckGlyphID inCheckGlyph);

	ADMStandardCheckGlyphID ASAPI (*GetCheckGlyph)(ADMEntryRef inEntry);

	

}

ADMEntrySuite4;	//	kADMEntrySuiteVersion4



// -----------------------------------------------------------------------------



#ifndef __ADMEntryOld__

#ifdef MAC_ENV

#include "ADMEntryOld.h"

#else

#include ".\Legacy\ADMEntryOld.h"

#endif

#endif



// -----------------------------------------------------------------------------



#pragma PRAGMA_IMPORT_END

#pragma PRAGMA_ALIGN_END



#ifdef __cplusplus

}

#endif



#endif

