/***********************************************************************/

/*                                                                     */

/* SPCaches.h                                                          */

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



#ifndef __SPCaches__

#define __SPCaches__





/*******************************************************************************

 **

 **	Imports

 **

 **/



#include "SPTypes.h"

#include "SPMData.h"



#ifdef __cplusplus

extern "C" {

#endif





/*******************************************************************************

 **

 **	Constants

 **

 **/



#define kSPCachesSuite				"SP Caches Suite"

#define kSPCachesSuiteVersion		2



/* To receive these, an PIConditionalMessages property must be present 

 * with the PIAcceptsPurgeCachesMessage flag set. */

#define kSPCacheCaller					"SP Cache"

#define kSPPluginPurgeCachesSelector	"Plug-in Purge"



/* The plug-in should return one of these two responses when it receives the kSPPluginPurgeCachesSelector */

//#define kSPPluginCachesFlushResponse	'pFls'

//#define kSPPluginCouldntFlushResponse	kSPNoError;

#include "SPErrorCodes.h"



/*******************************************************************************

 **

 **	Types

 **

 **/



typedef SPErr (*SPFlushCachesProc)( const char *type, void *data, long *flushed );



typedef struct SPPurgeCachesMessage {



	SPMessageData d;



} SPPurgeCachesMessage;





/*******************************************************************************

 **

 **	Suite

 **

 **/



typedef struct SPCachesSuite {



	SPAPI SPErr (*SPFlushCaches)( SPFlushCachesProc flushProc, long *flushed );



} SPCachesSuite;





SPAPI SPErr SPFlushCaches( SPFlushCachesProc flushProc, long *flushed );





#ifdef __cplusplus

}

#endif



#endif

