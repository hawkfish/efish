#ifndef __ADMIcon__
#define __ADMIcon__

/*
 *        Name:	ADMIcon.h
 *   $Revision: 1.1 $
 *      Author:	Paul Asente 
 *        Date:	8/1/96
 *     Purpose:	ADM Icon Suite.
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

#define kADMIconSuite				"ADM Icon Suite"

#define kADMIconSuiteVersion1		1
#define kADMIconSuiteVersion		kADMIconSuiteVersion1


typedef enum {
	// Mac types
	kCICN, kPICT, kIconSuite,
	// Windows types
	kWinIcon, kBMP,
	// Either type
	kUnknown
} ADMIconType;


/*
 *  Icon Suite
 */
typedef struct ADMIconSuite1
{
	ADMIconRef ASAPI (*GetFromResource)(SPPluginRef pluginRef, int iconID, int iconIndex);
	ADMIconRef ASAPI (*Create)(ADMIconType type, int width, int height, void *data);
	void ASAPI (*Destroy)(ADMIconRef icon);

	ADMIconType ASAPI (*GetType)(ADMIconRef icon);
	int ASAPI (*GetWidth)(ADMIconRef icon);
	int ASAPI (*GetHeight)(ADMIconRef icon);
	ASAPI void * (*GetData)(ADMIconRef icon);
	ASBoolean ASAPI (*IsFromResource)(ADMIconRef icon);

} ADMIconSuite1;

typedef ADMIconSuite1 ADMIconSuite;

#pragma PRAGMA_IMPORT_END
#pragma PRAGMA_ALIGN_END

#ifdef __cplusplus
}
#endif


#endif








