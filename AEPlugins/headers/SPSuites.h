/***********************************************************************/

/*                                                                     */

/* SPSuites.h                                                          */

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



#ifndef __SPSuites__

#define __SPSuites__





/*******************************************************************************

 **

 **	Imports

 **

 **/



#include "SPTypes.h"

#include "SPAccess.h"

#include "SPPlugs.h"

#include "SPStrngs.h"



#ifdef __cplusplus

extern "C" {

#endif





/*******************************************************************************

 **

 ** Constants

 **

 **/



#define kSPSuitesSuite				"SP Suites Suite"

#define kSPSuitesSuiteVersion		2



#define kSPLatestInternalVersion	0



#define kSPRuntimeSuiteList			((SPSuiteListRef)NULL)





/*******************************************************************************

 **

 ** Types

 **

 **/



typedef struct SPSuite *SPSuiteRef;

typedef struct SPSuiteList *SPSuiteListRef;

typedef struct SPSuiteListIterator *SPSuiteListIteratorRef;





/*******************************************************************************

 **

 ** Suite

 **

 **/



typedef struct SPSuitesSuite {



	SPAPI SPErr (*AllocateSuiteList)( SPStringPoolRef stringPool, SPPluginListRef plugins,

				SPSuiteListRef *suiteList );



	SPAPI SPErr (*FreeSuiteList)( SPSuiteListRef suiteList );



	SPAPI SPErr (*AddSuite)( SPSuiteListRef suiteList, SPPluginRef host, const char *name,

				long apiVersion, long internalVersion, const void *suiteProcs, SPSuiteRef *suite );



	SPAPI SPErr (*AcquireSuite)( SPSuiteListRef suiteList, const char *name, long apiVersion,

				long internalVersion, const void **suiteProcs );



	SPAPI SPErr (*ReleaseSuite)( SPSuiteListRef suiteList, const char *name, long apiVersion,

				long internalVersion );



	SPAPI SPErr (*FindSuite)( SPSuiteListRef suiteList, const char *name, long apiVersion,

				long internalVersion, SPSuiteRef *suite );



	SPAPI SPErr (*NewSuiteListIterator)( SPSuiteListRef suiteList, SPSuiteListIteratorRef *iter );

	SPAPI SPErr (*NextSuite)( SPSuiteListIteratorRef iter, SPSuiteRef *suite );

	SPAPI SPErr (*DeleteSuiteListIterator)( SPSuiteListIteratorRef iter );



	SPAPI SPErr (*GetSuiteHost)( SPSuiteRef suite, SPPluginRef *plugin );

	SPAPI SPErr (*GetSuiteName)( SPSuiteRef suite, const char **name );

	SPAPI SPErr (*GetSuiteAPIVersion)( SPSuiteRef suite, long *version );

	SPAPI SPErr (*GetSuiteInternalVersion)( SPSuiteRef suite, long *version );

	SPAPI SPErr (*GetSuiteProcs)( SPSuiteRef suite, const void **suiteProcs );

	SPAPI SPErr (*GetSuiteAcquireCount)( SPSuiteRef suite, long *count );



} SPSuitesSuite;





SPAPI SPErr SPAllocateSuiteList( SPStringPoolRef stringPool, SPPluginListRef plugins,

			SPSuiteListRef *suiteList );



SPAPI SPErr SPFreeSuiteList( SPSuiteListRef suiteList );



SPAPI SPErr SPAddSuite( SPSuiteListRef suiteList, SPPluginRef host, const char *name,

			long apiVersion, long internalVersion, const void *suiteProcs, SPSuiteRef *suite );



SPAPI SPErr SPAcquireSuite( SPSuiteListRef suiteList, const char *name, long apiVersion,

			long internalVersion, const void **suiteProcs );



SPAPI SPErr SPReleaseSuite( SPSuiteListRef suiteList, const char *name, long apiVersion,

			long internalVersion );



SPAPI SPErr SPFindSuite( SPSuiteListRef suiteList, const char *name, long apiVersion,

			long internalVersion, SPSuiteRef *suite );



SPAPI SPErr SPNewSuiteListIterator( SPSuiteListRef suiteList, SPSuiteListIteratorRef *iter );

SPAPI SPErr SPNextSuite( SPSuiteListIteratorRef iter, SPSuiteRef *suite );

SPAPI SPErr SPDeleteSuiteListIterator( SPSuiteListIteratorRef iter );



SPAPI SPErr SPGetSuiteHost( SPSuiteRef suite, SPPluginRef *plugin );

SPAPI SPErr SPGetSuiteName( SPSuiteRef suite, const char **name );

SPAPI SPErr SPGetSuiteAPIVersion( SPSuiteRef suite, long *version );

SPAPI SPErr SPGetSuiteInternalVersion( SPSuiteRef suite, long *version );

SPAPI SPErr SPGetSuiteProcs( SPSuiteRef suite, const void **suiteProcs );

SPAPI SPErr SPGetSuiteAcquireCount( SPSuiteRef suite, long *count );





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

