/***********************************************************************/

/*                                                                     */

/* SPPlugs.h                                                           */

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



#ifndef __SPPlugins__

#define __SPPlugins__





/*******************************************************************************

 **

 **	Imports

 **

 **/



#include "SPTypes.h"

#include "SPFiles.h"

#include "SPAdapts.h"

#include "SPProps.h"

#include "SPStrngs.h"



#ifdef __cplusplus

extern "C" {

#endif





/*******************************************************************************

 **

 ** Constants

 **

 **/



#define kSPPluginsSuite				"SP Plug-ins Suite"

#define kSPPluginsSuiteVersion4		4

#define kSPPluginsSuiteVersion5		5

#define kSPPluginsSuiteVersion		kSPPluginsSuiteVersion4



#define kSPRuntimePluginList		((SPPluginListRef)NULL)





/*******************************************************************************

 **

 ** Types

 **

 **/



typedef struct SPPlugin *SPPluginRef;

typedef struct SPPluginList *SPPluginListRef;

typedef struct SPPluginListIterator *SPPluginListIteratorRef;



/*******************************************************************************

 **

 ** Suite

 **

 **/



typedef struct SPPluginsSuite {



	SPAPI SPErr (*AllocatePluginList)( SPStringPoolRef strings, SPPluginListRef *pluginList );

	SPAPI SPErr (*FreePluginList)( SPPluginListRef pluginList );



	SPAPI SPErr (*AddPlugin)( SPPluginListRef pluginList, const SPPlatformFileSpecification *fileSpec,

				PIPropertyList *PiPL, const char *adapterName, void *adapterInfo, SPPluginRef *plugin );



	SPAPI SPErr (*NewPluginListIterator)( SPPluginListRef pluginList, SPPluginListIteratorRef *iter );

	SPAPI SPErr (*NextPlugin)( SPPluginListIteratorRef iter, SPPluginRef *plugin );

	SPAPI SPErr (*DeletePluginListIterator)( SPPluginListIteratorRef iter );

	SPAPI SPErr (*GetPluginListNeededSuiteAvailable)( SPPluginListRef pluginList, SPBoolean *available );



	SPAPI SPErr (*GetPluginHostEntry)( SPPluginRef plugin, void **host );

	SPAPI SPErr (*GetPluginFileSpecification)( SPPluginRef plugin, SPPlatformFileSpecification *fileSpec );

	SPAPI SPErr (*GetPluginPropertyList)( SPPluginRef plugin, SPPropertyListRef *propertList );

	SPAPI SPErr (*GetPluginGlobals)( SPPluginRef plugin, void **globals );

	SPAPI SPErr (*SetPluginGlobals)( SPPluginRef plugin, void *globals );

	SPAPI SPErr (*GetPluginStarted)( SPPluginRef plugin, long *started );

	SPAPI SPErr (*SetPluginStarted)( SPPluginRef plugin, long started );

	SPAPI SPErr (*GetPluginSkipShutdown)( SPPluginRef plugin, long *skipShutdown );

	SPAPI SPErr (*SetPluginSkipShutdown)( SPPluginRef plugin, long skipShutdown );

	SPAPI SPErr (*GetPluginBroken)( SPPluginRef plugin, long *broken );

	SPAPI SPErr (*SetPluginBroken)( SPPluginRef plugin, long broken );

	SPAPI SPErr (*GetPluginAdapter)( SPPluginRef plugin, SPAdapterRef *adapter );

	SPAPI SPErr (*GetPluginAdapterInfo)( SPPluginRef plugin, void **adapterInfo );

	SPAPI SPErr (*SetPluginAdapterInfo)( SPPluginRef plugin, void *adapterInfo );



	SPAPI SPErr (*FindPluginProperty)( SPPluginRef plugin, PIType vendorID, PIType propertyKey, long propertyID, PIProperty **p );



	SPAPI SPErr (*GetPluginName)( SPPluginRef plugin, const char **name );

	SPAPI SPErr (*SetPluginName)( SPPluginRef plugin, const char *name );

	SPAPI SPErr (*GetNamedPlugin)( const char *name, SPPluginRef *plugin);



	SPAPI SPErr (*SetPluginPropertyList)( SPPluginRef plugin, SPFileRef file );



	// Plug-ins suite version 5

	/* This attribute frees the adapterInfo field for private data for adapters. */

	SPAPI SPErr (*GetPluginHostInfo)( SPPluginRef plugin, void **hostInfo );

	SPAPI SPErr (*SetPluginHostInfo)( SPPluginRef plugin, void *hostInfo );



} SPPluginsSuite;





SPAPI SPErr SPAllocatePluginList( SPStringPoolRef strings, SPPluginListRef *pluginList );

SPAPI SPErr SPFreePluginList( SPPluginListRef pluginList );

SPAPI SPErr SPGetPluginListNeededSuiteAvailable( SPPluginListRef pluginList, SPBoolean *available );



SPAPI SPErr SPAddPlugin( SPPluginListRef pluginList, const SPPlatformFileSpecification *fileSpec,

			PIPropertyList *PiPL, const char *adapterName, void *adapterInfo, SPPluginRef *plugin );



SPAPI SPErr SPNewPluginListIterator( SPPluginListRef pluginList, SPPluginListIteratorRef *iter );

SPAPI SPErr SPNextPlugin( SPPluginListIteratorRef iter, SPPluginRef *plugin );

SPAPI SPErr SPDeletePluginListIterator( SPPluginListIteratorRef iter );



SPAPI SPErr SPGetHostPluginEntry( SPPluginRef plugin, void **host );

SPAPI SPErr SPGetPluginFileSpecification( SPPluginRef plugin, SPPlatformFileSpecification *fileSpec );

SPAPI SPErr SPGetPluginPropertyList( SPPluginRef plugin, SPPropertyListRef *propertyList );

SPAPI SPErr SPGetPluginGlobals( SPPluginRef plugin, void **globals );

SPAPI SPErr SPSetPluginGlobals( SPPluginRef plugin, void *globals );

SPAPI SPErr SPGetPluginStarted( SPPluginRef plugin, long *started );

SPAPI SPErr SPSetPluginStarted( SPPluginRef plugin, long started );

SPAPI SPErr SPGetPluginSkipShutdown( SPPluginRef plugin, long *skipShutdown );

SPAPI SPErr SPSetPluginSkipShutdown( SPPluginRef plugin, long skipShutdown );

SPAPI SPErr SPGetPluginBroken( SPPluginRef plugin, long *broken );

SPAPI SPErr SPSetPluginBroken( SPPluginRef plugin, long broken );

SPAPI SPErr SPGetPluginAdapter( SPPluginRef plugin, SPAdapterRef *adapter );

SPAPI SPErr SPGetPluginAdapterInfo( SPPluginRef plugin, void **adapterInfo );

SPAPI SPErr SPSetPluginAdapterInfo( SPPluginRef plugin, void *adapterInfo );



SPAPI SPErr SPFindPluginProperty( SPPluginRef plugin, PIType vendorID, PIType propertyKey,

			long propertyID, PIProperty **p );



SPAPI SPErr SPGetPluginName( SPPluginRef plugin, const char **name );

SPAPI SPErr SPSetPluginName( SPPluginRef plugin, const char *name );

SPAPI SPErr SPGetNamedPlugin( const char *name, SPPluginRef *plugin);



SPAPI SPErr SPSetPluginPropertyList( SPPluginRef plugin, SPFileRef file );



SPErr SPAddHostPlugin( SPPluginListRef pluginList, void *entry, void *access, const char *adapterName,

			void *adapterInfo, SPPluginRef *plugin );

																  /* access is SPPlatformAccessRef */





// Plug-ins suite version 5

/* This attribute frees the adapterInfo field for private data for adapters. */

SPAPI SPErr SPGetPluginHostInfo( SPPluginRef plugin, void **hostInfo );

SPAPI SPErr SPSetPluginHostInfo( SPPluginRef plugin, void *hostInfo );





/*******************************************************************************

 **

 ** Errors

 **

 **/



#include "SPErrorCodes.h"



#ifdef __cplusplus

}

#endif



#endif

