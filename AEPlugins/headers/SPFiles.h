/***********************************************************************/

/*                                                                     */

/* SPFiles.h                                                           */

/*                                                                     */

/* Copyright 1995-1999 Adobe Systems Incorporated.                     */

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



#ifndef __SPFiles__

#define __SPFiles__





/*******************************************************************************

 **

 **	Imports

 **

 **/



#include "SPTypes.h"

#include "SPProps.h"



#ifdef __cplusplus

extern "C" {

#endif





/*******************************************************************************

 **

 ** Constants

 **

 **/



#define kSPFilesSuite				"SP Files Suite"

#define kSPFilesSuiteVersion		3



#define kSPRuntimeFileList			((SPFileListRef)NULL)



#define kFileDoesNotHavePiPL		(SPPropertyListRef)0xffffffff

#define kFileHasMulitplePiPLs		NULL



/*******************************************************************************

 **

 ** Types

 **

 **/

#define kMaxPathLength 300



typedef struct SPFile *SPFileRef;

typedef struct SPFileList *SPFileListRef;

typedef struct SPFileListIterator *SPFileListIteratorRef;



#ifdef MAC_ENV

#ifdef __MWERKS__

#pragma options align=mac68k

#endif



typedef struct {  /* identical to FSSpec */

	short vRefNum;

	long parID;

	unsigned char name[64];

} SPPlatformFileSpecification;



#ifdef __MWERKS__

#pragma options align=reset

#endif



typedef struct {

	unsigned long attributes;

	unsigned long creationDate;

	unsigned long modificationDate;

	unsigned long finderType;

	unsigned long finderCreator;

	unsigned short finderFlags;

} SPPlatformFileInfo;

#endif





#ifdef WIN_ENV

typedef struct {

	char path[kMaxPathLength];

} SPPlatformFileSpecification;



typedef struct {

	unsigned long attributes;

	unsigned long lowCreationTime;

	unsigned long highCreationTime;

	unsigned long lowModificationTime;

	unsigned long highModificationTime;

	const char *extension;

} SPPlatformFileInfo;

#endif



typedef SPBoolean (*SPAddPiPLFilterProc)( SPPlatformFileInfo *info );



/*******************************************************************************

 **

 ** Suite

 **

 **/



typedef struct SPFilesSuite {



	SPAPI SPErr (*AllocateFileList)( SPFileListRef *fileList );

	SPAPI SPErr (*FreeFileList)( SPFileListRef fileList );



	SPAPI SPErr (*AddFiles)( SPFileListRef fileList, const SPPlatformFileSpecification *file );



	SPAPI SPErr (*NewFileListIterator)( SPFileListRef fileList, SPFileListIteratorRef *iter );

	SPAPI SPErr (*NextFile)( SPFileListIteratorRef iter, SPFileRef *file );

	SPAPI SPErr (*DeleteFileListIterator)( SPFileListIteratorRef iter );



	SPAPI SPErr (*GetFileSpecification)( SPFileRef file, SPPlatformFileSpecification *fileSpec );

	SPAPI SPErr (*GetFileInfo)( SPFileRef file, SPPlatformFileInfo *info );



	SPAPI SPErr (*GetIsAPlugin)( SPFileRef file, SPBoolean *isAPlugin );

	SPAPI SPErr (*SetIsAPlugin)( SPFileRef file, SPBoolean isAPlugin );



	SPAPI SPErr (*GetFilePropertyList)( SPFileRef file, SPPropertyListRef *propertList );

	SPAPI SPErr (*SetFilePropertyList)( SPFileRef file, SPPropertyListRef propertList );

	

} SPFilesSuite;





SPAPI SPErr SPAllocateFileList( SPFileListRef *fileList );

SPAPI SPErr SPFreeFileList( SPFileListRef fileList );

SPAPI SPErr SPAddFiles( SPFileListRef fileList, const SPPlatformFileSpecification *file );



SPAPI SPErr SPNewFileListIterator( SPFileListRef fileList, SPFileListIteratorRef *iter );

SPAPI SPErr SPNextFile( SPFileListIteratorRef iter, SPFileRef *file );

SPAPI SPErr SPDeleteFileListIterator( SPFileListIteratorRef iter );



SPAPI SPErr SPGetFileSpecification( SPFileRef file, SPPlatformFileSpecification *fileSpec );

SPAPI SPErr SPGetFileInfo( SPFileRef file, SPPlatformFileInfo *info );



SPAPI SPErr SPGetIsAPlugin( SPFileRef file, SPBoolean *isAPlugin );

SPAPI SPErr SPSetIsAPlugin( SPFileRef file, SPBoolean isAPlugin );



SPAPI SPErr SPGetFilePropertyList( SPFileRef file, SPPropertyListRef *propertList );

SPAPI SPErr SPSetFilePropertyList( SPFileRef file, SPPropertyListRef propertList );



SPAPI SPErr SPAddFilePiPLs( SPFileListRef fileList, SPAddPiPLFilterProc filter );



/*******************************************************************************

 **

 **	Errors

 **

 **/



#include "SPErrorCodes.h"



#ifdef __cplusplus

}

#endif



#endif

