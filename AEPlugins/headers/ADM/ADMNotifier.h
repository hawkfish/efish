#ifndef __ADMNotifier__
#define __ADMNotifier__

/*
 *        Name:	ADMNotifier.h
 *   $Revision: 1.1 $
 *      Author:	Dave Lazarony 
 *        Date:	3/27/96
 *     Purpose:	ADM Notifier Suite.
 *
 * Copyright (c) 1986-2001 Adobe Systems Incorporated, All Rights Reserved.
 *
 */


/*
 * Includes
 */
 
#ifndef __ADMTypes__
#include "ADMTypes.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#pragma PRAGMA_ALIGN_BEGIN
#pragma PRAGMA_IMPORT_BEGIN

// Stupid Macintosh Headers #define GetItem.
#ifdef GetItem
#undef GetItem
#endif

/*
 * Constants
 */

#define kADMNotifierSuite				"ADM Notifier Suite"

#define kADMNotifierSuiteVersion1		1
#define kADMNotifierSuiteVersion		kADMNotifierSuiteVersion1
#define kADMNotifierSuiteVersion2		2


#define kADMUserChangedNotifier				"ADM User Changed Notifier"
#define kADMIntermediateChangedNotifier		"ADM Intermediate Changed Notifier"
#define kADMBoundsChangedNotifier			"ADM Bounds Changed Notifier"
#define kADMEntryTextChangedNotifier		"ADM Entry Text Changed Notifier"
#define kADMCloseHitNotifier				"ADM Close Hit Notifier"
#define kADMZoomHitNotifier					"ADM Zoom Hit Notifier"
#define kADMCycleNotifier					"ADM Cycle Notifier"
#define kADMCollapseNotifier				"ADM Collapse Notifier"
#define kADMExpandNotifier					"ADM Expand Notifier"
#define kADMContextMenuChangedNotifier		"ADM Context Menu Changed Notifier"
#define kADMWindowShowNotifier				"ADM Show Window Notifier"
#define kADMWindowHideNotifier				"ADM Hide Window Notifier"
#define kADMWindowActivateNotifier			"ADM Activate Window Notifier"
#define	kADMWindowDeactivateNotifier		"ADM Deactivate Window Notifier"
#define kADMNumberOutOfBoundsNotifier		"ADM Number Out Of Bounds Notifier"

// text item notifiers
#define	kADMPreClipboardCutNotifier			"ADM Pre Clipboard Cut Notifier"
#define	kADMPostClipboardCutNotifier		"ADM Post Clipboard Cut Notifier"
#define	kADMPreClipboardCopyNotifier		"ADM Pre Clipboard Copy Notifier"
#define	kADMPostClipboardCopyNotifier		"ADM Post Clipboard Copy Notifier"
#define	kADMPreClipboardPasteNotifier		"ADM Pre Clipboard Paste Notifier"
#define	kADMPostClipboardPasteNotifier		"ADM Post Clipboard Paste Notifier"
#define	kADMPreClipboardClearNotifier		"ADM Pre Clipboard Clear Notifier"
#define	kADMPostClipboardClearNotifier		"ADM Post Clipboard Clear Notifier"
#define kADMPreTextSelectionChangedNotifier	"ADM Pre Selection Change Notification"
#define kADMTextSelectionChangedNotifier	"ADM Text Selection Change Notification"


/*
 *  Notifier Suite
 */
typedef struct ADMNotifierSuite1
{
	ADMItemRef ASAPI (*GetItem)(ADMNotifierRef notifier);
	ADMDialogRef ASAPI (*GetDialog)(ADMNotifierRef notifier);
	
	ASBoolean ASAPI (*IsNotifierType)(ADMNotifierRef notifier, char *notifierType);
	void ASAPI (*GetNotifierType)(ADMNotifierRef notifier, char *notifierType, int maxLen);
		
} ADMNotifierSuite1;

typedef ADMNotifierSuite1 ADMNotifierSuite;		// original version

typedef struct ADMNotifierSuite2
{
	ADMItemRef ASAPI (*GetItem)(ADMNotifierRef notifier);
	ADMDialogRef ASAPI (*GetDialog)(ADMNotifierRef notifier);
	
	ASBoolean ASAPI (*IsNotifierType)(ADMNotifierRef notifier, char *notifierType);
	void ASAPI (*GetNotifierType)(ADMNotifierRef notifier, char *notifierType, int maxLen);
	void ASAPI (*SkipNextClipboardOperation)(ADMNotifierRef notifier, ASBoolean skip);
		
} ADMNotifierSuite2;

#pragma PRAGMA_IMPORT_END
#pragma PRAGMA_ALIGN_END

#ifdef __cplusplus
}
#endif


#endif








