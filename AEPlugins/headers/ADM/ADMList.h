/***********************************************************************/

/*                                                                     */

/* ADMList.h                                                           */

/* ADM List Suite                                                      */

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



#ifndef __ADMList__

#define __ADMList__



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

//	User-provided function types



typedef ASErr ASAPI (*ADMEntryInitProc)(ADMEntryRef inEntry);

typedef void ASAPI (*ADMEntryDrawProc)(ADMEntryRef inEntry, ADMDrawerRef inDrawer);

typedef ASBoolean ASAPI (*ADMEntryTrackProc)(ADMEntryRef inEntry, ADMTrackerRef inTracker);

typedef void ASAPI (*ADMEntryNotifyProc)(ADMEntryRef inEntry, ADMNotifierRef inNotifier);

typedef void ASAPI (*ADMEntryDestroyProc)(ADMEntryRef inEntry);





// =============================================================================

//		* ADM List Suite

// =============================================================================



#define kADMListSuite "ADM List Suite"

#define kADMListSuiteVersion2 2

#define kADMListSuiteVersion3 3



// -----------------------------------------------------------------------------



typedef struct ADMListSuite2

{



	// *** This suite is FROZEN.

	// *** It shipped with Photoshop 5.5 and ImageReady 2.0.



	// menu IDs



	void ASAPI (*SetMenuID)(ADMListRef inList, SPPluginRef inMenuResPlugin,

				ASInt32 inMenuResID, const char* inMenuResName);



	ASInt32 ASAPI (*GetMenuID)(ADMListRef inList);



	// container accessor

	

	ADMItemRef ASAPI (*GetItem)(ADMListRef inList);



	// user data storage

	

	void ASAPI (*SetUserData)(ADMListRef inList, ADMUserData inUserData);

	ADMUserData ASAPI (*GetUserData)(ADMListRef inList);



	// override hooks



	void ASAPI (*SetInitProc)(ADMListRef inList, ADMEntryInitProc inInitProc);

	ADMEntryInitProc ASAPI (*GetInitProc)(ADMListRef inList);

	

	void ASAPI (*SetDrawProc)(ADMListRef inList, ADMEntryDrawProc inDrawProc);

	ADMEntryDrawProc ASAPI (*GetDrawProc)(ADMListRef inList);

	

	void ASAPI (*SetTrackProc)(ADMListRef inList, ADMEntryTrackProc inTrackProc);

	ADMEntryTrackProc ASAPI (*GetTrackProc)(ADMListRef inList);

	

	void ASAPI (*SetNotifyProc)(ADMListRef inList, ADMEntryNotifyProc inNotifyProc);

	ADMEntryNotifyProc ASAPI (*GetNotifyProc)(ADMListRef inList);

	

	void ASAPI (*SetDestroyProc)(ADMListRef inList, ADMEntryDestroyProc inDestroyProc);

	ADMEntryDestroyProc ASAPI (*GetDestroyProc)(ADMListRef inList);



	// entry bounds accessors

	

	void ASAPI (*SetEntryWidth)(ADMListRef inList, ASInt32 inWidth);

	ASInt32 ASAPI (*GetEntryWidth)(ADMListRef inList);

	

	void ASAPI (*SetEntryHeight)(ADMListRef inList, ASInt32 inHeight);

	ASInt32 ASAPI (*GetEntryHeight)(ADMListRef inList);

	

	void ASAPI (*SetEntryTextRect)(ADMListRef inList, const ASRect* inRect);

	void ASAPI (*GetEntryTextRect)(ADMListRef inList, ASRect* outRect);

	

	// entry array accessors

	

	ADMEntryRef ASAPI (*InsertEntry)(ADMListRef inList, ASInt32 inIndex);

	void ASAPI (*RemoveEntry)(ADMListRef inList, ASInt32 inIndex);

	

	ADMEntryRef ASAPI (*GetEntry)(ADMListRef inList, ASInt32 inEntryID);

	ADMEntryRef ASAPI (*IndexEntry)(ADMListRef inList, ASInt32 inIndex);

	ADMEntryRef ASAPI (*FindEntry)(ADMListRef inList, const char* inText);

	ADMEntryRef ASAPI (*PickEntry)(ADMListRef inList, const ASPoint* inPoint);

	

	ADMEntryRef ASAPI (*GetActiveEntry)(ADMListRef inList);

	ADMEntryRef ASAPI (*IndexSelectedEntry)(ADMListRef inList, ASInt32 inSelectionIndex);

	

	ASInt32 ASAPI (*NumberOfEntries)(ADMListRef inList);

	ASInt32 ASAPI (*NumberOfSelectedEntries)(ADMListRef inList);



	// item action mask

	

	void ASAPI (*SetMask)(ADMListRef inEntry, ADMActionMask inActionMask);

	ADMActionMask ASAPI (*GetMask)(ADMListRef inEntry);



	// user data storage for notifiers



	void ASAPI (*SetNotifierData)(ADMListRef inEntry, ADMUserData inUserData);

	ADMUserData ASAPI (*GetNotifierData)(ADMListRef inEntry);



}

ADMListSuite2;



// -----------------------------------------------------------------------------



typedef struct ADMListSuite3

{



	// menu IDs



	void ASAPI (*SetMenuID)(ADMListRef inList, SPPluginRef inMenuResPlugin,

				ASInt32 inMenuResID, const char* inMenuResName);



	ASInt32 ASAPI (*GetMenuID)(ADMListRef inList);



	// container accessor

	

	ADMItemRef ASAPI (*GetItem)(ADMListRef inList);



	// user data storage

	

	void ASAPI (*SetUserData)(ADMListRef inList, ADMUserData inUserData);

	ADMUserData ASAPI (*GetUserData)(ADMListRef inList);



	// override hooks



	void ASAPI (*SetInitProc)(ADMListRef inList, ADMEntryInitProc inInitProc);

	ADMEntryInitProc ASAPI (*GetInitProc)(ADMListRef inList);

	

	void ASAPI (*SetDrawProc)(ADMListRef inList, ADMEntryDrawProc inDrawProc);

	ADMEntryDrawProc ASAPI (*GetDrawProc)(ADMListRef inList);

	

	void ASAPI (*SetTrackProc)(ADMListRef inList, ADMEntryTrackProc inTrackProc);

	ADMEntryTrackProc ASAPI (*GetTrackProc)(ADMListRef inList);

	

	void ASAPI (*SetNotifyProc)(ADMListRef inList, ADMEntryNotifyProc inNotifyProc);

	ADMEntryNotifyProc ASAPI (*GetNotifyProc)(ADMListRef inList);

	

	void ASAPI (*SetDestroyProc)(ADMListRef inList, ADMEntryDestroyProc inDestroyProc);

	ADMEntryDestroyProc ASAPI (*GetDestroyProc)(ADMListRef inList);



	// entry bounds accessors

	

	void ASAPI (*SetEntryWidth)(ADMListRef inList, ASInt32 inWidth);

	ASInt32 ASAPI (*GetEntryWidth)(ADMListRef inList);

	

	void ASAPI (*SetEntryHeight)(ADMListRef inList, ASInt32 inHeight);

	ASInt32 ASAPI (*GetEntryHeight)(ADMListRef inList);

	

	void ASAPI (*SetEntryTextRect)(ADMListRef inList, const ASRect* inRect);

	void ASAPI (*GetEntryTextRect)(ADMListRef inList, ASRect* outRect);

	

	// entry array accessors

	

	ADMEntryRef ASAPI (*InsertEntry)(ADMListRef inList, ASInt32 inIndex);

	void ASAPI (*RemoveEntry)(ADMListRef inList, ASInt32 inIndex);

	

	ADMEntryRef ASAPI (*GetEntry)(ADMListRef inList, ASInt32 inEntryID);

	ADMEntryRef ASAPI (*IndexEntry)(ADMListRef inList, ASInt32 inIndex);

	ADMEntryRef ASAPI (*FindEntry)(ADMListRef inList, const char* inText);

	ADMEntryRef ASAPI (*PickEntry)(ADMListRef inList, const ASPoint* inPoint);

	

	ADMEntryRef ASAPI (*GetActiveEntry)(ADMListRef inList);

	ADMEntryRef ASAPI (*IndexSelectedEntry)(ADMListRef inList, ASInt32 inSelectionIndex);

	

	ASInt32 ASAPI (*NumberOfEntries)(ADMListRef inList);

	ASInt32 ASAPI (*NumberOfSelectedEntries)(ADMListRef inList);



	// item action mask

	

	void ASAPI (*SetMask)(ADMListRef inEntry, ADMActionMask inActionMask);

	ADMActionMask ASAPI (*GetMask)(ADMListRef inEntry);



	// user data storage for notifiers



	void ASAPI (*SetNotifierData)(ADMListRef inEntry, ADMUserData inUserData);

	ADMUserData ASAPI (*GetNotifierData)(ADMListRef inEntry);



	// customizing appearance

	

	void ASAPI (*SetBackgroundColor)(ADMListRef inList, ADMColor inColor);

	

	// searching

	

	void ASAPI (*SelectByText)(ADMListRef inList, const char* inFindText);



} ADMListSuite3;



// -----------------------------------------------------------------------------



#ifndef __ADMListOld__

#ifdef MAC_ENV

#include "ADMListOld.h"

#else

#include ".\Legacy\ADMListOld.h"

#endif

#endif



// -----------------------------------------------------------------------------



#pragma PRAGMA_IMPORT_END

#pragma PRAGMA_ALIGN_END



#ifdef __cplusplus

}

#endif



#endif

