/***********************************************************************/

/*                                                                     */

/* ADMItem.h                                                           */

/* ADM Item Suite                                                      */

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

/* Started by Dave Lazarony, 03/10/1996                                */

/*                                                                     */

/***********************************************************************/



#ifndef __ADMItem__

#define __ADMItem__



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

//	Item types



typedef const char* ADMItemType;



#define kADMDialType "ADM Dial Type"

#define kADMFrameType "ADM Frame Type"

#define kADMItemGroupType "ADM Item Group Type"

#define kADMTabbedMenuType "ADM Tabbed Menu Type"

#define kADMListBoxType "ADM List Box Type"

#define kADMHierarchyListBoxType "ADM Hierarchy List Box Type"			

#define kADMPictureCheckBoxType "ADM Picture Check Box Button Type"		

#define kADMPicturePushButtonType "ADM Picture Push Button Type"		

#define kADMPictureRadioButtonType "ADM Picture Radio Button Type"		

#define kADMPictureStaticType "ADM Picture Static Type"

#define kADMPopupControlType "ADM Popup Control Type"

#define kADMPopupControlButtonType "ADM Popup Control Button Type"

#define kADMPopupSpinEditControlType "ADM Popup Spin Edit Control Type"

#define kADMPopupListType "ADM Popup List Type"

#define kADMPopupMenuType "ADM Popup Menu Type"

#define kADMResizeType "ADM Resize Type"	

#define kADMScrollbarType "ADM Scrollbar Type"

#define kADMScrollingPopupListType "ADM Scrolling Popup List Type"

#define kADMSliderType "ADM Slider Type"

#define kADMSpinEditType "ADM Spin Edit Type"

#define kADMSpinEditPopupType "ADM Spin Edit Popup Type"

#define kADMSpinEditScrollingPopupType "ADM Spin Edit Scrolling Popup Type"

#define kADMTextCheckBoxType "ADM Text Check Box Type"

#define kADMTextEditType "ADM Text Edit Type"

#define kADMTextEditMultiLineType "ADM Text Edit Multi Line Type"

#define kADMTextEditPopupType "ADM Text Edit Popup Type"

#define kADMTextEditScrollingPopupType "ADM Text Edit Scrolling Popup Type"

#define kADMTextPushButtonType "ADM Text Push Button Type"

#define kADMTextRadioButtonType "ADM Text Radio Button Type"

#define kADMTextStaticType "ADM Text Static Type"

#define kADMTextStaticMultiLineType "ADM Text Static Multi Line Type"

#define kADMUserType "ADM User Type"

//RAD-As much as I would like to avoid this, Win9x doesn't respect ES_ after

//the control is created.  So this 'style' must be a type so that the style can

//be set in time.  We could make justification work in text by doing the same

//thing.  The alternative is to have the application set the style in their

//resource.  This deals with creating one without using the resource file.

#define kADMPasswordTextEditType "ADM Password Text Edit Type"  





// -----------------------------------------------------------------------------

//	Style codes



typedef ASInt32 ADMItemStyle;



typedef enum

{

	kADMBlackFrameStyle = 0,

	kADMGrayFrameStyle = 1,

	kADMSunkenFrameStyle = 2,

	kADMRaisedFrameStyle = 3,

	kADMEtchedFrameStyle = 4,

	kADMDummyFrameStyle = 0xFFFFFFFF

} 

ADMFrameStyle;



typedef enum

{

	kADMBlackAndSunkenRectPictureButtonStyle = 0,

	kADMBlackRectPictureButtonStyle = 1,

	kADMDummyPictureButtonStyle = 0xFFFFFFFF

}

ADMPictureButtonStyle;



	// Picture static styles

#define kADMDisableAutoActivatePictureStaticStyle (1L << 1)



	// Integer numeric style (used in the precision of a numeric)

#define kADMIntegerNumeric -1



	// List box styles

#define kADMMultiSelectListBoxStyle (1L << 0)

#define kADMDividedListBoxStyle (1L << 1)

#define kADMTileListBoxStyle (1L << 2)

#define kADMEntryAlwaysSelectedListBoxStyle (1L << 3)

#define kADMBlackRectListBoxStyle (1L << 4)

#define kADMUseImageListBoxStyle (1L << 5)

#define kADMEntryTextEditableListBoxStyle (1L << 6)



typedef enum

{

	kADMRightPopupMenuStyle = 0,

	kADMBottomPopupMenuStyle = 1,

	kADMDummyPopupMenuStyle = 0xFFFFFFFF

} 

ADMPopupMenuStyle;



typedef enum

{

	kADMVerticalSpinEditStyle = 0,

	kADMHorizontalSpinEditStyle = 1,

	kADMDummySpinEditStyle = 0xFFFFFFFF

}

ADMSpinEditStyle;



typedef enum

{

	kADMVerticalSpinEditPopupStyle = 0,

	kADMHorizontalSpinEditPopupStyle = 4,

	kADMDummySpinEditPopupStyle = 0xFFFFFFFF

} 

ADMSpinEditPopupStyle;



typedef enum

{

	kADMSingleLineTextEditStyle = 0,

	kADMNumericTextEditStyle = 2,		// 'Numeric' means float. Default.

	kADMPasswordTextEditStyle = 32,		// Win32 value for ES_PASSWORD

	kADMDummyTextEditStyle = 0xFFFFFFFF

} 

ADMTextEditStyle;



	// Text static styles

#define kADMClippedTextStaticStyle (1L << 0)

#define kADMDisableAutoActivateTextStaticStyle (1L << 1)

#define kADMTruncateEndTextStaticStyle (1L << 2)	// clipped style has priority

#define kADMTruncateMiddleTextStaticStyle (1L << 3)	// truncate end has priority



typedef enum

{

	kADMSingleLineEditPopupStyle = 0,

	kADMNumericEditPopupStyle = 2,

	kADMExclusiveEditPopupStyle = 5,

	kADMDummyTextEditPopupStyle = 0xFFFFFFFF

}

ADMTextEditPopupStyle;



	// Must set the same style on all buttons in the radio group

typedef enum

{

	kADMRadioButtonOneAlwaysSetStyle = 0,

	kADMRadioButtonAllowNoneSetStyle = 2,

	kADMDummyRadioButtonStyle = 0xFFFFFFFF

}

ADMRadioButtonStyle;



typedef enum

{

	kADMNoArrowDialStyle = 0,

	kADMArrowAtEndDialStyle = 1,

	kADMArrowAtCenterDialStyle = 2,

	kADMDummyDialStyle = 0xFFFFFFFF

} 

ADMDialStyle;



typedef enum

{

	kADMNoSliderStyle = 0,

	kADMNonLinearSliderStyle = 1,

	kADMShowFractionSliderStyle = 2,

	kADMDummySliderStyle = 0xFFFFFFFF

} 

ADMSliderStyle;





// -----------------------------------------------------------------------------

//	Child item IDs



typedef enum

{

	kADMListBoxScrollbarChildID = 1,

	kADMListBoxListChildID = 2,

	kADMListBoxTextEditBoxChildID = 3,



	kADMListBoxDummyChildID = 0xFFFFFFFF

} 

ADMListBoxChildID;



typedef enum

{

	kADMSpinEditUpButtonChildID = 1,

	kADMSpinEditDownButtonChildID = 2,

	kADMSpinEditTextEditChildID = 3,

	kADMSpinEditDummyChildID = 0xFFFFFFFF

}

ADMSpinEditChildID;



typedef enum

{

	kADMSpinEditPopupUpButtonChildID = 1,

	kADMSpinEditPopupDownButtonChildID = 2,

	kADMSpinEditPopupTextEditChildID = 3,

	kADMSpinEditPopupPopupChildID = 4,

	kADMSpinEditPopupDummyChildID = 0xFFFFFFFF

}

ADMSpinEditPopupChildID;



typedef enum

{

	kADMTextEditPopupTextEditChildID = 3,

	kADMTextEditPopupPopupChildID = 4,

	kADMTextEditPopupDummyChildID = 0xFFFFFFFF

}

ADMTextEditPopupChildID;





// -----------------------------------------------------------------------------

//	User-provided function types



typedef ASErr ASAPI (*ADMItemInitProc)(ADMItemRef inItem);

typedef void ASAPI (*ADMItemDrawProc)(ADMItemRef inItem, ADMDrawerRef inDrawer);

typedef ASBoolean ASAPI (*ADMItemTrackProc)(ADMItemRef inItem, ADMTrackerRef inTracker);

typedef void ASAPI (*ADMItemNotifyProc)(ADMItemRef inItem, ADMNotifierRef inNotifier);

typedef void ASAPI (*ADMItemDestroyProc)(ADMItemRef inItem);



typedef ASBoolean ASAPI (*ADMItemTimerProc)(ADMItemRef inItem, ADMTimerRef inTimer);



typedef void ASAPI (*ADMItemTimerAbortProc)(ADMItemRef inItem, ADMTimerRef inTimer,

			ADMAction inAbortAction);



typedef ASBoolean ASAPI (*ADMItemFloatToTextProc)(ADMItemRef inItem, float inValue,

			char* outText, ASInt32 inMaxLength);



typedef ASBoolean ASAPI (*ADMItemTextToFloatProc)(ADMItemRef inItem, const char* inText,

			float* outValue);





// =============================================================================

//		* ADM Item Suite

// =============================================================================



#define kADMItemSuite "ADM Item Suite"

#define kADMItemSuiteVersion5 5



// -----------------------------------------------------------------------------



typedef struct ADMItemSuite5

{



	// *** This suite is FROZEN.

	// *** It shipped with Photoshop 5.5 and ImageReady 2.0.



	// item creation/destruction



	ADMItemRef ASAPI (*Create)(ADMDialogRef inDialog, ASInt32 inItemID,

				ADMItemType inItemType, const ASRect* inBoundsRect,

				ADMItemInitProc inInitProc, ADMUserData inUserData,

				ASInt32 inOptions);



	void ASAPI (*Destroy)(ADMItemRef inItem);

	

	// item draw proc

	

	void ASAPI (*SetDrawProc)(ADMItemRef inItem, ADMItemDrawProc inDrawProc);

	ADMItemDrawProc ASAPI (*GetDrawProc)(ADMItemRef inItem);

	void ASAPI (*DefaultDraw)(ADMItemRef inItem, ADMDrawerRef inDrawer);



	// item track proc

	

	void ASAPI (*SetTrackProc)(ADMItemRef inItem, ADMItemTrackProc inTrackProc);

	ADMItemTrackProc ASAPI (*GetTrackProc)(ADMItemRef inItem);

	ASBoolean ASAPI (*DefaultTrack)(ADMItemRef inItem, ADMTrackerRef inTracker);



	void ASAPI (*SetMask)(ADMItemRef inItem, ADMActionMask inActionMask);

	ADMActionMask ASAPI (*GetMask)(ADMItemRef inItem);



	// item notify proc



	void ASAPI (*SetNotifyProc)(ADMItemRef inItem, ADMItemNotifyProc inNotifyProc);

	ADMItemNotifyProc ASAPI (*GetNotifyProc)(ADMItemRef inItem);

	void ASAPI (*DefaultNotify)(ADMItemRef inItem, ADMNotifierRef inNotifier);

	void ASAPI (*SendNotify)(ADMItemRef inItem, const char* inNotifierType);



	void ASAPI (*SetNotifierData)(ADMItemRef inItem, ADMUserData inNotifierData);

	ADMUserData ASAPI (*GetNotifierData)(ADMItemRef inItem);



	// item destroy proc



	void ASAPI (*SetDestroyProc)(ADMItemRef inItem, ADMItemDestroyProc inDestroyProc);

	ADMItemDestroyProc ASAPI (*GetDestroyProc)(ADMItemRef inItem);

	

	// item timer

	

	ADMTimerRef ASAPI (*CreateTimer)(ADMItemRef inItem, ASUInt32 inMilliseconds,

				ADMActionMask inAbortMask, ADMItemTimerProc inTimerProc,

				ADMItemTimerAbortProc inTimerAbortProc, ASInt32 inOptions);



	void ASAPI (*AbortTimer)(ADMItemRef inItem, ADMTimerRef inTimer);



	// item user data



	void ASAPI (*SetUserData)(ADMItemRef inItem, ADMUserData inUserData);

	ADMUserData ASAPI (*GetUserData)(ADMItemRef inItem);



	// item information accessors



	ASInt32 ASAPI (*GetID)(ADMItemRef inItem);

	ADMDialogRef ASAPI (*GetDialog)(ADMItemRef inItem);

	ASWindowRef ASAPI (*GetWindowRef)(ADMItemRef inItem);

	ADMListRef ASAPI (*GetList)(ADMItemRef inItem);

	ADMItemRef ASAPI (*GetChildItem)(ADMItemRef inItem, ASInt32 inChildID);	



	void ASAPI (*SetPluginRef)(ADMItemRef inItem, SPPluginRef inPluginRef);

	SPPluginRef ASAPI (*GetPluginRef)(ADMItemRef inItem);



	void ASAPI (*SetItemType)(ADMItemRef inItem, ADMItemType inItemType);

	ADMItemType ASAPI (*GetItemType)(ADMItemRef inItem);

	

	void ASAPI (*SetItemStyle)(ADMItemRef inItem, ADMItemStyle inItemStyle);

	ADMItemStyle ASAPI (*GetItemStyle)(ADMItemRef inItem);

	

	// item state accessors

	

	void ASAPI (*Show)(ADMItemRef inItem, ASBoolean inShow);

	ASBoolean ASAPI (*IsVisible)(ADMItemRef inItem);

	

	void ASAPI (*Enable)(ADMItemRef inItem, ASBoolean inEnable);

	ASBoolean ASAPI (*IsEnabled)(ADMItemRef inItem);

	

	void ASAPI (*Activate)(ADMItemRef inItem, ASBoolean inActivate);

	ASBoolean ASAPI (*IsActive)(ADMItemRef inItem);

	

	void ASAPI (*Known)(ADMItemRef inItem, ASBoolean inKnown);

	ASBoolean ASAPI (*IsKnown)(ADMItemRef inItem);

	

	// item bounds accessors



	void ASAPI (*SetLocalRect)(ADMItemRef inItem, const ASRect* inLocalRect);

	void ASAPI (*GetLocalRect)(ADMItemRef inItem, ASRect* outLocalRect);

	

	void ASAPI (*SetBoundsRect)(ADMItemRef inItem, const ASRect* inBoundsRect);

	void ASAPI (*GetBoundsRect)(ADMItemRef inItem, ASRect* outBoundsRect);

	

	void ASAPI (*GetBestSize)(ADMItemRef inItem, ASPoint* outBestSize);



	void ASAPI (*Move)(ADMItemRef inItem, ASInt32 inHorizPosition, ASInt32 inVertPosition);

	void ASAPI (*Size)(ADMItemRef inItem, ASInt32 inWidth, ASInt32 inHeight);



	// coordinate system transformations	



	void ASAPI (*LocalToScreenPoint)(ADMItemRef inItem, ASPoint* ioPoint);

	void ASAPI (*ScreenToLocalPoint)(ADMItemRef inItem, ASPoint* ioPoint);

	

	void ASAPI (*LocalToScreenRect)(ADMItemRef inItem, ASRect* ioRect);

	void ASAPI (*ScreenToLocalRect)(ADMItemRef inItem, ASRect* ioRect);

	

	// item display

	

	void ASAPI (*Invalidate)(ADMItemRef inItem);

	void ASAPI (*InvalidateRect)(ADMItemRef inDialog, const ASRect* inInvalRect);

	void ASAPI (*Update)(ADMItemRef inItem);

	

	// cursor ID accessors

	

	ASBoolean ASAPI (*SetCursorID)(ADMItemRef inItem, SPPluginRef inPluginRef, ASInt32 inCursorID,

				const char* inCursorName);



	void ASAPI (*GetCursorID)(ADMItemRef inItem, SPPluginRef* outPluginRef, ASInt32* outCursorID,

				const char** outCursorName);



	// picture ID accessors



	void ASAPI (*SetPictureID)(ADMItemRef inItem, ASInt32 inPictureResID,

				const char* inPictureResName);



	void ASAPI (*GetPictureID)(ADMItemRef inItem, ASInt32* outPictureResID,

				const char** outPictureResName);

	

	void ASAPI (*SetSelectedPictureID)(ADMItemRef inItem, ASInt32 inPictureResID,

				const char* inPictureResName);



	void ASAPI (*GetSelectedPictureID)(ADMItemRef inItem, ASInt32* outPictureResID,

				const char** outPictureResName);

	

	void ASAPI (*SetDisabledPictureID)(ADMItemRef inItem, ASInt32 inPictureResID,

				const char* inPictureResName);



	void ASAPI (*GetDisabledPictureID)(ADMItemRef inItem, ASInt32* outPictureResID,

				const char** outPictureResName);



	// tooltips



	void ASAPI (*SetTipString)(ADMItemRef inItem, const char* inTipString);

	void ASAPI (*GetTipString)(ADMItemRef inItem, char* outTipString, ASInt32 inMaxLength);

	

	ASInt32 ASAPI (*GetTipStringLength)(ADMItemRef inItem);

	

	void ASAPI (*EnableTip)(ADMItemRef inItem, ASBoolean inEnable);

	ASBoolean ASAPI (*IsTipEnabled)(ADMItemRef inItem);

	

	void ASAPI (*ShowToolTip)(ADMItemRef inItem, const ASPoint* inWhere);

	void ASAPI (*HideToolTip)(ADMItemRef inItem);

	

	// item text accessors



	void ASAPI (*SetFont)(ADMItemRef inItem, ADMFont inFontID);

	ADMFont ASAPI (*GetFont)(ADMItemRef inItem);



	void ASAPI (*SetText)(ADMItemRef inItem, const char* inText);

	void ASAPI (*GetText)(ADMItemRef inItem, char* outText, ASInt32 inMaxLength);

	ASInt32 ASAPI (*GetTextLength)(ADMItemRef inItem);

	

	void ASAPI (*SetMaxTextLength)(ADMItemRef inItem, ASInt32 inLength);

	ASInt32 ASAPI (*GetMaxTextLength)(ADMItemRef inItem);

	

	void ASAPI (*SelectAll)(ADMItemRef inItem);

	void ASAPI (*SetSelectionRange)(ADMItemRef inItem, ASInt32 inSelStart, ASInt32 inSelEnd);

	void ASAPI (*GetSelectionRange)(ADMItemRef inItem, ASInt32* outSelStart, ASInt32* outSelEnd);



	void ASAPI (*SetJustify)(ADMItemRef inItem, ADMJustify inJustify);

	ADMJustify ASAPI (*GetJustify)(ADMItemRef inItem);

	

	void ASAPI (*SetUnits)(ADMItemRef inItem, ADMUnits inUnits);

	ADMUnits ASAPI (*GetUnits)(ADMItemRef inItem);

	

	// item value accessors



	void ASAPI (*SetPrecision)(ADMItemRef inItem, ASInt32 inNumberOfDecimalPlaces);

	ASInt32 ASAPI (*GetPrecision)(ADMItemRef inItem);

	

	void ASAPI (*SetBooleanValue)(ADMItemRef inItem, ASBoolean inValue);

	ASBoolean ASAPI (*GetBooleanValue)(ADMItemRef inItem);

	

	void ASAPI (*SetIntValue)(ADMItemRef inItem, ASInt32 inValue);

	ASInt32 ASAPI (*GetIntValue)(ADMItemRef inItem);

		

	void ASAPI (*SetFixedValue)(ADMItemRef inItem, ASFixed inValue);	

	ASFixed ASAPI (*GetFixedValue)(ADMItemRef inItem);

	

	void ASAPI (*SetFloatValue)(ADMItemRef inItem, float inValue);	

	float ASAPI (*GetFloatValue)(ADMItemRef inItem);

	

	void ASAPI (*SetMinIntValue)(ADMItemRef inItem, ASInt32 inValue);

	ASInt32 ASAPI (*GetMinIntValue)(ADMItemRef inItem);

		

	void ASAPI (*SetMinFixedValue)(ADMItemRef inItem, ASFixed inValue);	

	ASFixed ASAPI (*GetMinFixedValue)(ADMItemRef inItem);

	

	void ASAPI (*SetMaxIntValue)(ADMItemRef inItem, ASInt32 inValue);

	ASInt32 ASAPI (*GetMaxIntValue)(ADMItemRef inItem);

	

	void ASAPI (*SetMinFloatValue)(ADMItemRef inItem, float inValue);	

	float ASAPI (*GetMinFloatValue)(ADMItemRef inItem);

	

	void ASAPI (*SetMaxFixedValue)(ADMItemRef inItem, ASFixed inValue);	

	ASFixed ASAPI (*GetMaxFixedValue)(ADMItemRef inItem);

	

	void ASAPI (*SetMaxFloatValue)(ADMItemRef inItem, float inValue);	

	float ASAPI (*GetMaxFloatValue)(ADMItemRef inItem);

	

	void ASAPI (*SetSmallIncrement)(ADMItemRef inItem, float inIncrement);	

	float ASAPI (*GetSmallIncrement)(ADMItemRef inItem);

	

	void ASAPI (*SetLargeIncrement)(ADMItemRef inItem, float inIncrement);

	float ASAPI (*GetLargeIncrement)(ADMItemRef inItem);

	

	void ASAPI (*SetAllowUnits)(ADMItemRef inItem, ASBoolean inAllowUnits);

	ASBoolean ASAPI (*GetAllowUnits)(ADMItemRef inItem);

	void ASAPI (*SetAllowMath)(ADMItemRef inItem, ASBoolean inAllowMath);

	ASBoolean ASAPI (*GetAllowMath)(ADMItemRef inItem);

	

	void ASAPI (*SetFloatToTextProc)(ADMItemRef inItem, ADMItemFloatToTextProc inProc);

	ADMItemFloatToTextProc ASAPI (*GetFloatToTextProc)(ADMItemRef inItem);

	

	ASBoolean ASAPI (*DefaultFloatToText)(ADMItemRef inItem, float inValue, char* outText,

		ASInt32 inMaxLength);



	void ASAPI (*SetTextToFloatProc)(ADMItemRef inItem, ADMItemTextToFloatProc inProc);

	ADMItemTextToFloatProc ASAPI (*GetTextToFloatProc)(ADMItemRef inItem);

	ASBoolean ASAPI (*DefaultTextToFloat)(ADMItemRef inItem, const char* inText, float* outValue);



	void ASAPI (*ShowUnits)(ADMItemRef inItem, ASBoolean inShowUnits);

	ASBoolean ASAPI (*GetShowUnits)(ADMItemRef inItem);



	ASBoolean ASAPI (*WasPercentageChange)(ADMItemRef inItem);



	// item help ID

	

	void ASAPI (*SetHelpID)(ADMItemRef inItem, ASHelpID inHelpID);

	ASHelpID ASAPI (*GetHelpID)(ADMItemRef inItem);

	void ASAPI (*Help)(ADMItemRef inDialog);



	// hierarchy list accessor

	

	ADMHierarchyListRef ASAPI (*GetHierarchyList)(ADMItemRef inItem);

	

	// item group accessors

	

 	void ASAPI (*AddItem)(ADMItemRef inGroup, ADMItemRef inItem);

 	void ASAPI (*RemoveItem)(ADMItemRef inGroup, ADMItemRef inItem);



	// item pictures (from icons)

	

	void ASAPI (*SetPicture)(ADMItemRef inItem, ADMIconRef inPicture);

	ADMIconRef ASAPI (*GetPicture)(ADMItemRef inItem);



	void ASAPI (*SetSelectedPicture)(ADMItemRef inItem, ADMIconRef inPicture);

	ADMIconRef ASAPI (*GetSelectedPicture)(ADMItemRef inItem);



	void ASAPI (*SetDisabledPicture)(ADMItemRef inItem, ADMIconRef inPicture);

	ADMIconRef ASAPI (*GetDisabledPicture)(ADMItemRef inItem);

	

	// popup control accessors

	

	void ASAPI (*SetPopupDialog)(ADMItemRef inItem, ASInt32 inPopupItemID, ADMDialogRef inDialog);

	ADMDialogRef ASAPI (*GetPopupDialog)(ADMItemRef inItem);



}

ADMItemSuite5;



// -----------------------------------------------------------------------------



#ifndef __ADMItemOld__

#ifdef MAC_ENV

#include "ADMItemOld.h"

#else

#include ".\Legacy\ADMItemOld.h"

#endif

#endif



// -----------------------------------------------------------------------------



#pragma PRAGMA_IMPORT_END

#pragma PRAGMA_ALIGN_END



#ifdef __cplusplus

}

#endif



#endif

