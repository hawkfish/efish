/***********************************************************************/

/*                                                                     */

/* ADMDialogOld.h                                                      */

/* Old versions of ADM Dialog Suite                                    */

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



#ifndef __ADMDialogOld__

#define __ADMDialogOld__



#ifndef __ADMDialog__

#include "ADMDialog.h"

#endif





// -----------------------------------------------------------------------------



#ifdef __cplusplus

extern "C" {

#endif



#pragma PRAGMA_ALIGN_BEGIN

#pragma PRAGMA_IMPORT_BEGIN





// =============================================================================

//		* ADM Dialog Suite, version 2

// =============================================================================



#define kADMDialogSuiteVersion2 2



// -----------------------------------------------------------------------------



typedef struct ADMDialogSuite2

{



	// dialog creation



	ADMDialogRef ASAPI (*Create)(SPPluginRef inPluginRef, const char* inName,

				ASInt32 inDialogID, ADMDialogStyle inDialogStyle,

				ADMDialogInitProc inInitProc, ADMUserData inUserData);



	// modal dialogs



	ASInt32 ASAPI (*Modal)(SPPluginRef inPluginRef, ASInt32 inDialogID,

				ADMDialogStyle inDialogStyle, ADMDialogInitProc inInitProc,

				ADMUserData inUserData);



	// dialog destruction



	void ASAPI (*Destroy)(ADMDialogRef inDialog);



	// dialog draw proc

	

	void ASAPI (*SetDrawProc)(ADMDialogRef inDialog, ADMDialogDrawProc inDrawProc);

	ADMDialogDrawProc ASAPI (*GetDrawProc)(ADMDialogRef inDialog);

	void ASAPI (*DefaultDraw)(ADMDialogRef inDialog, ADMDrawerRef inDrawer);



	// dialog track proc

	

	void ASAPI (*SetTrackProc)(ADMDialogRef inDialog, ADMDialogTrackProc inTrackProc);

	ADMDialogTrackProc ASAPI (*GetTrackProc)(ADMDialogRef inDialog);

	ASBoolean ASAPI (*DefaultTrack)(ADMDialogRef inDialog, ADMTrackerRef inTracker);



	void ASAPI (*SetMask)(ADMDialogRef inDialog, ADMActionMask inMask);

	ADMActionMask ASAPI (*GetMask)(ADMDialogRef inDialog);



	// dialog notify proc



	void ASAPI (*SetNotifyProc)(ADMDialogRef inDialog, ADMDialogNotifyProc inNotifyProc);

	ADMDialogNotifyProc ASAPI (*GetNotifyProc)(ADMDialogRef inDialog);

	void ASAPI (*DefaultNotify)(ADMDialogRef inDialog, ADMNotifierRef inNotifier);

	void ASAPI (*SendNotify)(ADMDialogRef inDialog, const char* inNotifierType);



	void ASAPI (*SetNotifierData)(ADMDialogRef inDialog, ADMUserData l);

	ADMUserData ASAPI (*GetNotifierData)(ADMDialogRef inDialog);



	// dialog destroy proc



	void ASAPI (*SetDestroyProc)(ADMDialogRef inDialog, ADMDialogDestroyProc inDestroyProc);

	ADMDialogDestroyProc ASAPI (*GetDestroyProc)(ADMDialogRef inDialog);

	

	// dialog timer



	ADMTimerRef ASAPI (*CreateTimer)(ADMDialogRef inDialog, ASUInt32 inMilliseconds,

				ADMActionMask inAbortMask, ADMDialogTimerProc inTimerProc,

				ADMDialogTimerAbortProc inAbortProc);

	

	void ASAPI (*AbortTimer)(ADMDialogRef inDialog, ADMTimerRef inTimerID);



	// dialog user data



	void ASAPI (*SetUserData)(ADMDialogRef inDialog, ADMUserData inUserData);

	ADMUserData ASAPI (*GetUserData)(ADMDialogRef inDialog);



	// dialog information accessors



	ASAPI const char* (*GetDialogName)(ADMDialogRef inDialog);

	ASInt32 ASAPI (*GetID)(ADMDialogRef inDialog);

	SPPluginRef ASAPI (*GetPluginRef)(ADMDialogRef inDialog);

	ASWindowRef ASAPI (*GetWindowRef)(ADMDialogRef inDialog);

	

	void ASAPI (*SetDialogStyle)(ADMDialogRef inDialog, ADMDialogStyle inDialogStyle);

	ADMDialogStyle ASAPI (*GetDialogStyle)(ADMDialogRef inDialog);

	

	// dialog state accessors



	void ASAPI (*Show)(ADMDialogRef inDialog, ASBoolean inShow);

	ASBoolean ASAPI (*IsVisible)(ADMDialogRef inDialog);

	

	void ASAPI (*Enable)(ADMDialogRef inDialog, ASBoolean inEnable);

	ASBoolean ASAPI (*IsEnabled)(ADMDialogRef inDialog);

	

	void ASAPI (*Activate)(ADMDialogRef inDialog, ASBoolean inActivate);

	ASBoolean ASAPI (*IsActive)(ADMDialogRef inDialog);



	// dialog bounds accessors

	

	void ASAPI (*SetLocalRect)(ADMDialogRef inDialog, const ASRect* inLocalRect);

	void ASAPI (*GetLocalRect)(ADMDialogRef inDialog, ASRect* outLocalRect);

	

	void ASAPI (*SetBoundsRect)(ADMDialogRef inDialog, const ASRect* inBoundsRect);

	void ASAPI (*GetBoundsRect)(ADMDialogRef inDialog, ASRect* outBoundsRect);

	

	void ASAPI (*Move)(ADMDialogRef inDialog, ASInt32 inHorizPosition, ASInt32 inVertPosition);

	void ASAPI (*Size)(ADMDialogRef inDialog, ASInt32 inWidth, ASInt32 inHeight);



	// coordinate system transformations

	

	void ASAPI (*LocalToScreenPoint)(ADMDialogRef inDialog, ASPoint* ioPoint);

	void ASAPI (*ScreenToLocalPoint)(ADMDialogRef inDialog, ASPoint* ioPoint);

	

	void ASAPI (*LocalToScreenRect)(ADMDialogRef inDialog, ASRect* ioRect);

	void ASAPI (*ScreenToLocalRect)(ADMDialogRef inDialog, ASRect* ioRect);

	

	// dialog redraw requests

	

	void ASAPI (*Invalidate)(ADMDialogRef inDialog);

	void ASAPI (*Update)(ADMDialogRef inDialog);

	

	// cursor ID accessors



	ASBoolean ASAPI (*SetCursorID)(ADMDialogRef inDialog, SPPluginRef inPluginRef,

				ASInt32 inCursorID);



	void ASAPI (*GetCursorID)(ADMDialogRef inDialog, SPPluginRef* outPluginRef,

				ASInt32* outCursorID);



	// dialog text accessors



	void ASAPI (*SetFont)(ADMDialogRef inDialog, ADMFont inFont);

	ADMFont ASAPI (*GetFont)(ADMDialogRef inDialog);



	void ASAPI (*SetText)(ADMDialogRef inDialog, const char* inText);

	void ASAPI (*GetText)(ADMDialogRef inDialog, char* outText, ASInt32 inMaxLength);

	ASInt32 ASAPI (*GetTextLength)(ADMDialogRef inDialog);



	// dialog size constraints

	

	void ASAPI (*SetMinWidth)(ADMDialogRef inDialog, ASInt32 inWidth);

	ASInt32 ASAPI (*GetMinWidth)(ADMDialogRef inDialog);

	

	void ASAPI (*SetMinHeight)(ADMDialogRef inDialog, ASInt32 inHeight);

	ASInt32 ASAPI (*GetMinHeight)(ADMDialogRef inDialog);

	

	void ASAPI (*SetMaxWidth)(ADMDialogRef inDialog, ASInt32 inWidth);

	ASInt32 ASAPI (*GetMaxWidth)(ADMDialogRef inDialog);

	

	void ASAPI (*SetMaxHeight)(ADMDialogRef inDialog, ASInt32 inHeight);

	ASInt32 ASAPI (*GetMaxHeight)(ADMDialogRef inDialog);

	

	void ASAPI (*SetHorizontalIncrement)(ADMDialogRef inDialog, ASInt32 inIncrement);

	ASInt32 ASAPI (*GetHorizontalIncrement)(ADMDialogRef inDialog);



	void ASAPI (*SetVerticalIncrement)(ADMDialogRef inDialog, ASInt32 inIncrement);

	ASInt32 ASAPI (*GetVerticalIncrement)(ADMDialogRef inDialog);



	// item accessors



	ADMItemRef ASAPI (*GetItem)(ADMDialogRef inDialog, ASInt32 inItemID);

	

	ADMItemRef ASAPI (*CreateItem)(ADMDialogRef inDialog, ASInt32 inItemID,

				ADMItemType inItemType, const ASRect* inBoundsRect,

				ADMItemInitProc inInitProc, ADMUserData inUserData);



	void ASAPI (*DestroyItem)(ADMDialogRef inDialog, ADMItemRef inItem);

	

	ADMItemRef ASAPI (*GetNextItem)(ADMDialogRef inDialog, ADMItemRef inItem);

	ADMItemRef ASAPI (*GetPreviousItem)(ADMDialogRef inDialog, ADMItemRef inItem);



	// default/cancel items



	void ASAPI (*SetDefaultItemID)(ADMDialogRef inDialog, ASInt32 inItemID);

	ASInt32 ASAPI (*GetDefaultItemID)(ADMDialogRef inDialog);

	void ASAPI (*SetCancelItemID)(ADMDialogRef inDialog, ASInt32 inItemID);

	ASInt32 ASAPI (*GetCancelItemID)(ADMDialogRef inDialog);

	

	ASBoolean ASAPI (*EndModal)(ADMDialogRef inDialog, ASInt32 inModalResultID,

				ASBoolean inCancelling);

	

	ASErr ASAPI (*RegisterItemType)(SPPluginRef inPluginRef, ADMItemType inItemType);

	ASErr ASAPI (*UnregisterItemType)(SPPluginRef inPluginRef, ADMItemType inItemType);



	// tooltips



	void ASAPI (*LoadToolTips)(ADMDialogRef inDialog, ASInt32 inStringID);



	// context help



	void ASAPI (*SetHelpID)(ADMDialogRef inDialog, ASHelpID inHelpID);

	ASHelpID ASAPI (*GetHelpID)(ADMDialogRef inDialog);

	void ASAPI (*Help)(ADMDialogRef inDialog);



	// dialog state accessors



	ASBoolean ASAPI (*IsCollapsed)(ADMDialogRef inDialog);

	

}

ADMDialogSuite2;





// =============================================================================

//		* ADM Dialog Suite, version 3

// =============================================================================



#define kADMDialogSuiteVersion3 3

#define kADMDialogSuiteVersion kADMDialogSuiteVersion3



// -----------------------------------------------------------------------------



typedef struct ADMDialogSuite3

{



	// dialog creation



	ADMDialogRef ASAPI (*Create)(SPPluginRef inPluginRef, const char* inName,

				ASInt32 inDialogID, ADMDialogStyle inDialogStyle,

				ADMDialogInitProc inInitProc, ADMUserData inUserData);



	// modal dialogs



	ASInt32 ASAPI (*Modal)(SPPluginRef inPluginRef, const char* inName,

				ASInt32 inDialogID, ADMDialogStyle inDialogStyle,

				ADMDialogInitProc inInitProc, ADMUserData inUserData);



	ASInt32 ASAPI (*DisplayAsModal)(ADMDialogRef inDialog);



	// dialog destruction



	void ASAPI (*Destroy)(ADMDialogRef inDialog);



	// dialog draw proc

	

	void ASAPI (*SetDrawProc)(ADMDialogRef inDialog, ADMDialogDrawProc inDrawProc);

	ADMDialogDrawProc ASAPI (*GetDrawProc)(ADMDialogRef inDialog);

	void ASAPI (*DefaultDraw)(ADMDialogRef inDialog, ADMDrawerRef inDrawer);



	// dialog track proc

	

	void ASAPI (*SetTrackProc)(ADMDialogRef inDialog, ADMDialogTrackProc inTrackProc);

	ADMDialogTrackProc ASAPI (*GetTrackProc)(ADMDialogRef inDialog);

	ASBoolean ASAPI (*DefaultTrack)(ADMDialogRef inDialog, ADMTrackerRef inTracker);



	void ASAPI (*SetMask)(ADMDialogRef inDialog, ADMActionMask inMask);

	ADMActionMask ASAPI (*GetMask)(ADMDialogRef inDialog);



	// dialog notify proc



	void ASAPI (*SetNotifyProc)(ADMDialogRef inDialog, ADMDialogNotifyProc inNotifyProc);

	ADMDialogNotifyProc ASAPI (*GetNotifyProc)(ADMDialogRef inDialog);

	void ASAPI (*DefaultNotify)(ADMDialogRef inDialog, ADMNotifierRef inNotifier);

	void ASAPI (*SendNotify)(ADMDialogRef inDialog, const char* inNotifierType);



	void ASAPI (*SetNotifierData)(ADMDialogRef inDialog, ADMUserData inUserData);

	ADMUserData ASAPI (*GetNotifierData)(ADMDialogRef inDialog);



	// dialog destroy proc



	void ASAPI (*SetDestroyProc)(ADMDialogRef inDialog, ADMDialogDestroyProc inDestroyProc);

	ADMDialogDestroyProc ASAPI (*GetDestroyProc)(ADMDialogRef inDialog);



	// dialog timer

	

	ADMTimerRef ASAPI (*CreateTimer)(ADMDialogRef inDialog, ASUInt32 inMilliseconds,

				ADMActionMask inAbortMask, ADMDialogTimerProc inTimerProc,

				ADMDialogTimerAbortProc inAbortProc);

	

	void ASAPI (*AbortTimer)(ADMDialogRef inDialog, ADMTimerRef inTimerID);



	// dialog user data



	void ASAPI (*SetUserData)(ADMDialogRef inDialog, ADMUserData inUserData);

	ADMUserData ASAPI (*GetUserData)(ADMDialogRef inDialog);



	// dialog information accessors

	

	ASAPI const char* (*GetDialogName)(ADMDialogRef inDialog);

	ASInt32 ASAPI (*GetID)(ADMDialogRef inDialog);

	SPPluginRef ASAPI (*GetPluginRef)(ADMDialogRef inDialog);

	ASWindowRef ASAPI (*GetWindowRef)(ADMDialogRef inDialog);

	

	void ASAPI (*SetDialogStyle)(ADMDialogRef inDialog, ADMDialogStyle inDialogStyle);

	ADMDialogStyle ASAPI (*GetDialogStyle)(ADMDialogRef inDialog);

	

	// dialog state accessors

	

	void ASAPI (*Show)(ADMDialogRef inDialog, ASBoolean inShow);

	ASBoolean ASAPI (*IsVisible)(ADMDialogRef inDialog);

	

	void ASAPI (*Enable)(ADMDialogRef inDialog, ASBoolean inEnable);

	ASBoolean ASAPI (*IsEnabled)(ADMDialogRef inDialog);

	

	void ASAPI (*Activate)(ADMDialogRef inDialog, ASBoolean inActivate);

	ASBoolean ASAPI (*IsActive)(ADMDialogRef inDialog);

	

	// dialog bounds accessors

	

	void ASAPI (*SetLocalRect)(ADMDialogRef inDialog, const ASRect* inLocalRect);

	void ASAPI (*GetLocalRect)(ADMDialogRef inDialog, ASRect* outLocalRect);

	

	void ASAPI (*SetBoundsRect)(ADMDialogRef inDialog, const ASRect* inBoundsRect);

	void ASAPI (*GetBoundsRect)(ADMDialogRef inDialog, ASRect* outBoundsRect);

	

	void ASAPI (*Move)(ADMDialogRef inDialog, ASInt32 inHorizPosition, ASInt32 inVertPosition);

	void ASAPI (*Size)(ADMDialogRef inDialog, ASInt32 inWidth, ASInt32 inHeight);

	

	// coordinate system transformations

	

	void ASAPI (*LocalToScreenPoint)(ADMDialogRef inDialog, ASPoint* ioPoint);

	void ASAPI (*ScreenToLocalPoint)(ADMDialogRef inDialog, ASPoint* ioPoint);

	

	void ASAPI (*LocalToScreenRect)(ADMDialogRef inDialog, ASRect* ioRect);

	void ASAPI (*ScreenToLocalRect)(ADMDialogRef inDialog, ASRect* ioRect);



	// dialog redraw requests

	

	void ASAPI (*Invalidate)(ADMDialogRef inDialog);

	void ASAPI (*InvalidateRect)(ADMDialogRef inDialog, const ASRect* inInvalRect);

	void ASAPI (*Update)(ADMDialogRef inDialog);



	// cursor ID accessors

	

	ASBoolean ASAPI (*SetCursorID)(ADMDialogRef inDialog, SPPluginRef inPluginRef,

				ASInt32 inCursorID);



	void ASAPI (*GetCursorID)(ADMDialogRef inDialog, SPPluginRef* outPluginRef,

				ASInt32* outCursorID);



	// dialog text accessors



	void ASAPI (*SetFont)(ADMDialogRef inDialog, ADMFont inFont);

	ADMFont ASAPI (*GetFont)(ADMDialogRef inDialog);



	void ASAPI (*SetText)(ADMDialogRef inDialog, const char* inText);

	void ASAPI (*GetText)(ADMDialogRef inDialog, char* outText, ASInt32 inMaxLength);

	ASInt32 ASAPI (*GetTextLength)(ADMDialogRef inDialog);

	

	// dialog size constriants

	

	void ASAPI (*SetMinWidth)(ADMDialogRef inDialog, ASInt32 inWidth);

	ASInt32 ASAPI (*GetMinWidth)(ADMDialogRef inDialog);

	

	void ASAPI (*SetMinHeight)(ADMDialogRef inDialog, ASInt32 inHeight);

	ASInt32 ASAPI (*GetMinHeight)(ADMDialogRef inDialog);

	

	void ASAPI (*SetMaxWidth)(ADMDialogRef inDialog, ASInt32 inWidth);

	ASInt32 ASAPI (*GetMaxWidth)(ADMDialogRef inDialog);

	

	void ASAPI (*SetMaxHeight)(ADMDialogRef inDialog, ASInt32 inHeight);

	ASInt32 ASAPI (*GetMaxHeight)(ADMDialogRef inDialog);

	

	void ASAPI (*SetHorizontalIncrement)(ADMDialogRef inDialog, ASInt32 inIncrement);

	ASInt32 ASAPI (*GetHorizontalIncrement)(ADMDialogRef inDialog);



	void ASAPI (*SetVerticalIncrement)(ADMDialogRef inDialog, ASInt32 inIncrement);

	ASInt32 ASAPI (*GetVerticalIncrement)(ADMDialogRef inDialog);



	// item accessors



	ADMItemRef ASAPI (*GetItem)(ADMDialogRef inDialog, ASInt32 inItemID);

	

	ADMItemRef ASAPI (*CreateItem)(ADMDialogRef inDialog, ASInt32 inItemID,

				ADMItemType inItemType, const ASRect* inBoundsRect,

				ADMItemInitProc inInitProc, ADMUserData inUserData);



	void ASAPI (*DestroyItem)(ADMDialogRef inDialog, ADMItemRef inItem);

	

	ADMItemRef ASAPI (*GetNextItem)(ADMDialogRef inDialog, ADMItemRef inItem);

	ADMItemRef ASAPI (*GetPreviousItem)(ADMDialogRef inDialog, ADMItemRef inItem);



	// default/cancel items



	void ASAPI (*SetDefaultItemID)(ADMDialogRef inDialog, ASInt32 inItemID);

	ASInt32 ASAPI (*GetDefaultItemID)(ADMDialogRef inDialog);



	void ASAPI (*SetCancelItemID)(ADMDialogRef inDialog, ASInt32 inItemID);

	ASInt32 ASAPI (*GetCancelItemID)(ADMDialogRef inDialog);

	

	ASBoolean  ASAPI (*EndModal)(ADMDialogRef inDialog, ASInt32 inModalResultID,

				ASBoolean inCancelling);



	// custom item types

	

	ASErr ASAPI (*RegisterItemType)(SPPluginRef inPluginRef, ADMItemType inItemType);

	ASErr ASAPI (*UnregisterItemType)(SPPluginRef inPluginRef, ADMItemType inItemType);



	// tooltips



	void ASAPI (*LoadToolTips)(ADMDialogRef inDialog, ASInt32 inStringID);



	// context help



	void ASAPI (*SetHelpID)(ADMDialogRef inDialog, ASHelpID inHelpID);

	ASHelpID ASAPI (*GetHelpID)(ADMDialogRef inDialog);

	void ASAPI (*Help)(ADMDialogRef inDialog);



	// dialog state accessors



	ASBoolean ASAPI (*IsCollapsed)(ADMDialogRef inDialog);

	

}

ADMDialogSuite3;



typedef ADMDialogSuite3 ADMDialogSuite; 





// =============================================================================

//		* ADM Dialog Suite, version 4

// =============================================================================



#define kADMDialogSuiteVersion4 4



// -----------------------------------------------------------------------------



typedef struct ADMDialogSuite4

{



	// dialog creation



	ADMDialogRef ASAPI (*Create)(SPPluginRef inPluginRef, const char* inName,

				ASInt32 inDialogID, ADMDialogStyle inDialogStyle,

				ADMDialogInitProc inInitProc, ADMUserData inUserData, ASInt32 inOptions);



	// modal dialogs



	ASInt32 ASAPI (*Modal)(SPPluginRef inPluginRef, const char* inName,

				ASInt32 inDialogID, ADMDialogStyle inDialogStyle,

				ADMDialogInitProc inInitProc, ADMUserData inUserData, ASInt32 inOptions);



	ASInt32 ASAPI (*PopupModal)(SPPluginRef inPluginRef, const char* inName,

				ASInt32 inDialogID, ADMDialogInitProc inInitProc, ADMUserData inUserData,

				ASInt32 inOptions);



	ASInt32 ASAPI (*DisplayAsModal)(ADMDialogRef inDialog);



	// dialog destruction



	void ASAPI (*Destroy)(ADMDialogRef inDialog);



	// dialog draw proc

	

	void ASAPI (*SetDrawProc)(ADMDialogRef inDialog, ADMDialogDrawProc inDrawProc);

	ADMDialogDrawProc ASAPI (*GetDrawProc)(ADMDialogRef inDialog);

	void ASAPI (*DefaultDraw)(ADMDialogRef inDialog, ADMDrawerRef inDrawer);



	// dialog track proc

	

	void ASAPI (*SetTrackProc)(ADMDialogRef inDialog, ADMDialogTrackProc inTrackProc);

	ADMDialogTrackProc ASAPI (*GetTrackProc)(ADMDialogRef inDialog);

	ASBoolean ASAPI (*DefaultTrack)(ADMDialogRef inDialog, ADMTrackerRef inTracker);



	void ASAPI (*SetMask)(ADMDialogRef inDialog, ADMActionMask inMask);

	ADMActionMask ASAPI (*GetMask)(ADMDialogRef inDialog);



	// dialog notify proc



	void ASAPI (*SetNotifyProc)(ADMDialogRef inDialog, ADMDialogNotifyProc inNotifyProc);

	ADMDialogNotifyProc ASAPI (*GetNotifyProc)(ADMDialogRef inDialog);

	void ASAPI (*DefaultNotify)(ADMDialogRef inDialog, ADMNotifierRef inNotifier);

	void ASAPI (*SendNotify)(ADMDialogRef inDialog, const char* inNotifierType);



	void ASAPI (*SetNotifierData)(ADMDialogRef inDialog, ADMUserData inUserData);

	ADMUserData ASAPI (*GetNotifierData)(ADMDialogRef inDialog);



	// dialog destroy proc



	void ASAPI (*SetDestroyProc)(ADMDialogRef inDialog, ADMDialogDestroyProc inDestroyProc);

	ADMDialogDestroyProc ASAPI (*GetDestroyProc)(ADMDialogRef inDialog);



	// dialog timer

	

	ADMTimerRef ASAPI (*CreateTimer)(ADMDialogRef inDialog, ASUInt32 inMilliseconds,

				ADMActionMask inAbortMask, ADMDialogTimerProc inTimerProc,

				ADMDialogTimerAbortProc inAbortProc, ASInt32 inOptions);

	

	void ASAPI (*AbortTimer)(ADMDialogRef inDialog, ADMTimerRef inTimerID);



	// dialog user data



	void ASAPI (*SetUserData)(ADMDialogRef inDialog, ADMUserData inUserData);

	ADMUserData ASAPI (*GetUserData)(ADMDialogRef inDialog);



	// dialog information accessors



	ASAPI const char* (*GetDialogName)(ADMDialogRef inDialog);

	ASInt32 ASAPI (*GetID)(ADMDialogRef inDialog);

	SPPluginRef ASAPI (*GetPluginRef)(ADMDialogRef inDialog);

	ASWindowRef ASAPI (*GetWindowRef)(ADMDialogRef inDialog);

	

	void ASAPI (*SetDialogStyle)(ADMDialogRef inDialog, ADMDialogStyle inDialogStyle);

	ADMDialogStyle ASAPI (*GetDialogStyle)(ADMDialogRef inDialog);



	// dialog state accessors

	

	void ASAPI (*Show)(ADMDialogRef inDialog, ASBoolean inShow);

	ASBoolean ASAPI (*IsVisible)(ADMDialogRef inDialog);

	

	void ASAPI (*Enable)(ADMDialogRef inDialog, ASBoolean inEnable);

	ASBoolean ASAPI (*IsEnabled)(ADMDialogRef inDialog);

	

	void ASAPI (*Activate)(ADMDialogRef inDialog, ASBoolean inActivate);

	ASBoolean ASAPI (*IsActive)(ADMDialogRef inDialog);



	// dialog bounds accessors

	

	void ASAPI (*SetLocalRect)(ADMDialogRef inDialog, const ASRect* inLocalRect);

	void ASAPI (*GetLocalRect)(ADMDialogRef inDialog, ASRect* outLocalRect);

	

	void ASAPI (*SetBoundsRect)(ADMDialogRef inDialog, const ASRect* inBoundsRect);

	void ASAPI (*GetBoundsRect)(ADMDialogRef inDialog, ASRect* outBoundsRect);

	

	void ASAPI (*Move)(ADMDialogRef inDialog, ASInt32 inHorizPosition, ASInt32 inVertPosition);

	void ASAPI (*Size)(ADMDialogRef inDialog, ASInt32 inWidth, ASInt32 inHeight);



	// coordinate system transformations

	

	void ASAPI (*LocalToScreenPoint)(ADMDialogRef inDialog, ASPoint* ioPoint);

	void ASAPI (*ScreenToLocalPoint)(ADMDialogRef inDialog, ASPoint* ioPoint);

	

	void ASAPI (*LocalToScreenRect)(ADMDialogRef inDialog, ASRect* ioRect);

	void ASAPI (*ScreenToLocalRect)(ADMDialogRef inDialog, ASRect* ioRect);

	

	// dialog redraw requests

	

	void ASAPI (*Invalidate)(ADMDialogRef inDialog);

	void ASAPI (*InvalidateRect)(ADMDialogRef inDialog, const ASRect* inInvalRect);

	void ASAPI (*Update)(ADMDialogRef inDialog);



	// cursor ID accessors

	

	ASBoolean ASAPI (*SetCursorID)(ADMDialogRef inDialog, SPPluginRef inPluginRef,

				ASInt32 inCursorID);



	void ASAPI (*GetCursorID)(ADMDialogRef inDialog, SPPluginRef* outPluginRef,

				ASInt32* outCursorID);



	// dialog text accessors



	void ASAPI (*SetFont)(ADMDialogRef inDialog, ADMFont inFont);

	ADMFont ASAPI (*GetFont)(ADMDialogRef inDialog);



	void ASAPI (*SetText)(ADMDialogRef inDialog, const char* inText);

	void ASAPI (*GetText)(ADMDialogRef inDialog, char* outText, ASInt32 inMaxLength);

	ASInt32 ASAPI (*GetTextLength)(ADMDialogRef inDialog);

	

	// dialog size constraints

	

	void ASAPI (*SetMinWidth)(ADMDialogRef inDialog, ASInt32 inWidth);

	ASInt32 ASAPI (*GetMinWidth)(ADMDialogRef inDialog);

	

	void ASAPI (*SetMinHeight)(ADMDialogRef inDialog, ASInt32 inHeight);

	ASInt32 ASAPI (*GetMinHeight)(ADMDialogRef inDialog);

	

	void ASAPI (*SetMaxWidth)(ADMDialogRef inDialog, ASInt32 inWidth);

	ASInt32 ASAPI (*GetMaxWidth)(ADMDialogRef inDialog);

	

	void ASAPI (*SetMaxHeight)(ADMDialogRef inDialog, ASInt32 inHeight);

	ASInt32 ASAPI (*GetMaxHeight)(ADMDialogRef inDialog);

	

	void ASAPI (*SetHorizontalIncrement)(ADMDialogRef inDialog, ASInt32 inIncrement);

	ASInt32 ASAPI (*GetHorizontalIncrement)(ADMDialogRef inDialog);



	void ASAPI (*SetVerticalIncrement)(ADMDialogRef inDialog, ASInt32 inIncrement);

	ASInt32 ASAPI (*GetVerticalIncrement)(ADMDialogRef inDialog);

	

	// item accessors



	ADMItemRef ASAPI (*GetItem)(ADMDialogRef inDialog, ASInt32 inItemID);

	

	ADMItemRef ASAPI (*CreateItem)(ADMDialogRef inDialog, ASInt32 inItemID,

				ADMItemType inItemType, const ASRect* inBoundsRect,

				ADMItemInitProc inInitProc, ADMUserData inUserData, ASInt32 inOptions);



	void ASAPI (*DestroyItem)(ADMDialogRef inDialog, ADMItemRef inItem);

	

	ADMItemRef ASAPI (*GetNextItem)(ADMDialogRef inDialog, ADMItemRef inItem);

	ADMItemRef ASAPI (*GetPreviousItem)(ADMDialogRef inDialog, ADMItemRef inItem);



	// default/cancel items



	void ASAPI (*SetDefaultItemID)(ADMDialogRef inDialog, ASInt32 inItemID);

	ASInt32 ASAPI (*GetDefaultItemID)(ADMDialogRef inDialog);



	void ASAPI (*SetCancelItemID)(ADMDialogRef inDialog, ASInt32 inItemID);

	ASInt32 ASAPI (*GetCancelItemID)(ADMDialogRef inDialog);

	

	ASBoolean  ASAPI (*EndModal)(ADMDialogRef inDialog, ASInt32 inModalResultID,

				ASBoolean inCancelling);



	// custom item types

	

	ASErr ASAPI (*RegisterItemType)(SPPluginRef inPluginRef, ADMItemType inItemType);

	ASErr ASAPI (*UnregisterItemType)(SPPluginRef inPluginRef, ADMItemType inItemType);



	// tooltips



	void ASAPI (*LoadToolTips)(ADMDialogRef inDialog, ASInt32 inStringID);



	// context help



	void ASAPI (*SetHelpID)(ADMDialogRef inDialog, ASHelpID inHelpID);

	ASHelpID ASAPI (*GetHelpID)(ADMDialogRef inDialog);

	void ASAPI (*Help)(ADMDialogRef inDialog);



	// dialog state accessors



	ASBoolean ASAPI (*IsCollapsed)(ADMDialogRef inDialog);



	ADMItemRef ASAPI (*CreateGroupInDialog)(ADMDialogRef inDialog, SPPluginRef inPluginRef,

				const char* inName, ASInt32 inDialogID, ADMItemInitProc inInitProc,

				ADMUserData inUserData, ASInt32 inOptions);



	void ASAPI (*SetWindowRef)(ADMDialogRef inDialog, ASWindowRef inWindowRef);

	

	ASAPI void (*SetDialogName)(ADMDialogRef inDialog, const char* inName);



}

ADMDialogSuite4;





// -----------------------------------------------------------------------------



#pragma PRAGMA_IMPORT_END

#pragma PRAGMA_ALIGN_END



#ifdef __cplusplus

}

#endif



#endif  // __ADMDialogOld__

