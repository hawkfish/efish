/***********************************************************************/

/*                                                                     */

/* SPAdapts.h														   */

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



#ifndef __SPAdapters__

#define __SPAdapters__





/*******************************************************************************

 **

 **	Imports

 **

 **/



#include "SPTypes.h"

#include "SPCaches.h"

#include "SPMData.h"

#include "SPProps.h"



#ifdef __cplusplus

extern "C" {

#endif







/*******************************************************************************

 **

 ** Constants

 **

 **/



#define kSPAdaptersSuite					"SP Adapters Suite"

#define kSPAdaptersSuiteVersion				3



#define kSPAdaptersCaller					"SP Adapters"



#define kSPAdaptersStartupSelector			"Start up"

#define kSPAdaptersShutdownSelector			"Shut down"

#define kSPAdaptersDisposeInfoSelector		"Dispose info"

#define kSPAdaptersFlushSelector			"Flush"



// Second generation adapters (has property 'adpt'/2)

//---------------------------------------------------

// This message is called just after an adapter is started up

// asking it to scan Sweet Pea's list of files and look for plug-ins

// of its type and register those plug-ins.

#define kSPAdaptersRegisterPluginsSelector		"Register plugins"



// These messages tell the adapter to load/release the given plug-in, moving

// it in and out of memory.  The adapter does NOT need to issue the standard

// SP messages SPAccess/Reload or /Unload

#define kSPAdaptersLoadPluginSelector			"Load plugin"

#define kSPAdaptersReleasePluginSelector		"Release plugin"



// The main way for an app to talk to an adapted plug-in.  First gen. adapters

// provided a suite of functions for communicating with its plug-ins, putting 

// support effort inside the app.  These messages tell an Adapter to pass a 

// given caller/selector/message or an equivalent to one of its plug-ins, and it

// is the adapters responisibility to do any translation/interfacing.

#define kSPAdaptersSendMessageSelector			"Send message"



// First generation adapters (no 'adpt' property, or 'adpt'/1 )

//-------------------------------------------------------------

// These messages are only for the "SP2 Adapter" that is built into

// Sweet Pea and maybe legacy adapters (i.e., AI6Adapter for AI7).

// For newer adapters, all of these methods are now deprecated. 

// New adapters should do the vast majority of their processing via 

// the message kSPAdaptersSendMessageSelector.

#define kSPAdaptersFindPropertySelector			"Find property"

#define kSPAdaptersAboutSelector				"About"

#define kSPAdaptersAcquireSuiteHostSelector		"Acquire Suite"

#define kSPAdaptersReleaseSuiteHostSelector		"Release Suite"





#define kSPRuntimeAdapterList				((SPAdapterListRef)NULL)





/*******************************************************************************

 **

 ** Types

 **

 **/



typedef struct SPAdapter *SPAdapterRef;

typedef struct SPAdapterList *SPAdapterListRef;

typedef struct SPAdapterListIterator *SPAdapterListIteratorRef;



// Note for anyone looking at this as an example of how to implement a good

// api: this is what not to do.  This parameter block has information relevant

// to all adapter calls in it.  It would be better to have a number of specific

// message data structures.  This is marginally faster to type, but sloppy.

typedef struct SPAdaptersMessage {



	SPMessageData d;

	

	SPAdapterRef adapter;	

	

	struct SPPlugin *targetPlugin;

	SPErr targetResult;



	/* for Find property selector */

	PIType vendorID;

	PIType propertyKey;

	long propertyID;

	void *property;		/* returned here */



	/* for Flush selector */

	SPFlushCachesProc flushProc;

	long flushed;		/* returned here */



	/* for Suites selectors */

	struct SPSuiteList *suiteList;	/* use these if you need name, apiVersion, internalVersion */

	struct SPSuite *suite;											

	struct SPPlugin *host;			/* plug-in hosting the suite, to be aquired/released by adapter */

	const void *suiteProcs;			/* returned here if reallocated */

	long acquired;					/* returned here */



	/* For SendMessage selector. */

	const char *plugin_caller;		/* Caller to pass to adapted plug-in. */

	const char *plugin_selector;	/* Selector to pass to adapted plug-in */

	void *plugin_message;			/* Message data to pass to adapted plug-in */

} SPAdaptersMessage;





/*******************************************************************************

 **

 ** Suite

 **

 **/



typedef struct SPAdaptersSuite {



	SPAPI SPErr (*AllocateAdapterList)( struct SPStringPool *stringPool, SPAdapterListRef *adapterList );

	SPAPI SPErr (*FreeAdapterList)( SPAdapterListRef adapterList );



	SPAPI SPErr (*AddAdapter)( SPAdapterListRef adapterList, struct SPPlugin *host, const char *name,

				long version, SPAdapterRef *adapter );



	SPAPI SPErr (*SPFindAdapter)( SPAdapterListRef adapterList, const char *name, SPAdapterRef *adapter );



	SPAPI SPErr (*NewAdapterListIterator)( SPAdapterListRef adapterList, SPAdapterListIteratorRef *iter );

	SPAPI SPErr (*NextAdapter)( SPAdapterListIteratorRef iter, SPAdapterRef *adapter );

	SPAPI SPErr (*DeleteAdapterListIterator)( SPAdapterListIteratorRef iter );



	SPAPI SPErr (*GetAdapterHost)( SPAdapterRef adapter, struct SPPlugin **plugin );

	SPAPI SPErr (*GetAdapterName)( SPAdapterRef adapter, const char **name );

	SPAPI SPErr (*GetAdapterVersion)( SPAdapterRef adapter, long *version );



} SPAdaptersSuite;





SPAPI SPErr SPAllocateAdapterList( struct SPStringPool *stringPool, SPAdapterListRef *adapterList );

SPAPI SPErr SPFreeAdapterList( SPAdapterListRef adapterList );



SPAPI SPErr SPAddAdapter( SPAdapterListRef adapterList, struct SPPlugin *host, const char *name,

			long version, SPAdapterRef *adapter );



SPAPI SPErr SPFindAdapter( SPAdapterListRef adapterList, const char *name, SPAdapterRef *adapter );



SPAPI SPErr SPNewAdapterListIterator( SPAdapterListRef adapterList, SPAdapterListIteratorRef *iter );

SPAPI SPErr SPNextAdapter( SPAdapterListIteratorRef iter, SPAdapterRef *adapter );

SPAPI SPErr SPDeleteAdapterListIterator( SPAdapterListIteratorRef iter );



SPAPI SPErr SPGetAdapterHost( SPAdapterRef adapter, struct SPPlugin **plugin );

SPAPI SPErr SPGetAdapterName( SPAdapterRef adapter, const char **name );

SPAPI SPErr SPGetAdapterVersion( SPAdapterRef adapter, long *version );





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

