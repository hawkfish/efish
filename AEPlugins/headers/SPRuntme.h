/***********************************************************************/

/*                                                                     */

/* SPRuntme.h                                                          */

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



#ifndef __SPRuntime__

#define __SPRuntime__





/*******************************************************************************

 **

 **	Imports

 **

 **/



#include "SPTypes.h"

#include "SPAdapts.h"

#include "SPFiles.h"

#include "SPPlugs.h"

#include "SPStrngs.h"

#include "SPSuites.h"

#include "SPStrngs.h"



#ifdef __cplusplus

extern "C" {

#endif





/*******************************************************************************

 **

 **	Constants

 **

 **/



#define kSPRuntimeSuite				"SP Runtime Suite"

#define kSPRuntimeSuiteVersion		5





/*******************************************************************************

 **

 **	Types

 **

 **/



/*

 *	Sweet Pea makes callbacks into the host through the host procs. The host

 *	procs are filled in by the host and passed to Sweet Pea at SPInit().

 *

 *	hostData - data that is given back to each host proc when Sweet Pea

 *		calls it. Sweet Pea does nothing with it itself.

 *

 *	extAllocate - implementation of the Block Suite's AllocateBlock() routine.

 *		It is identical to ANSI C malloc(). It returns a pointer to the

 *		beginning of the allocated block or NULL.

 *

 *	extFree - implementation of the Block Suite's FreeBlock() routine. It is

 *		identical to ANSI C free(). Note that you can pass it NULL.

 *

 *	extReallocate - implementation of the Block Suite's ReallocateBlock()

 *		routine. It is identical to ANSI C realloc(). It returns a pointer

 *		to the resized block or NULL. Note that you can pass it NULL or a

 *		newSize of 0.

 *

 *	intAllocate, intFree, intReallocate - routines used by Sweet Pea for

 *		its own memory needs. You may want to allocate blocks differently

 *		with plug-ins and Sweet Pea. Plug-ins are unbounded in their memory

 *		needs, while Sweet Pea's memory usage can be approximated.

 *

 *	startupNotify - called as each plug-in is started up. This is intended

 *		as a way to tell the user what's happening during start up.

 *		Note that plug-ins may start up at any time, not just during

 *		SPStartupPlugins().

 *

 *	shutdownNotify - called as each plug-in is shut down. Also intended as

 *		a way to let users know what's going on.

 *

 *	assertTrap - called when a fatal assert is triggered. Sweet Pea does

 *		not expect execution to continue after an assert.

 *

 *	throwTrap - called when an internal error is thrown. This can be used

 *		during debugging to catch errors as they happen. It should return

 *		to allow Sweet Pea to handle the error.

 *

 *

 *	To aid in getting Sweet Pea up and running quickly, you can set any of

 *	these to NULL and Sweet Pea will use a default implementation. However:

 *	you cannot mix your implementations of the memory routines with

 *	Sweet Pea's defaults.

 *

 *

 *	The string pool functions replace the default routines used internally 

 *	and exported by the Strings suite.  Because they are exported, the behaviors

 *	listed below should be followed.

 *	

 *	allocateStringPool - creates a new string pool instance. The host app and

 *		Sweet Pea have a string pool which can be used by a plug-in, or a plug-in

 *		can create its own.  See the notes in SPStrngs.h on how the pool is 

 *		implemented.  

 *		The function should return kSPNoError if the pool is allocated successfully 

 *		or kSPOutOfMemoryError if allocation fails.

 *		

 *	freeStringPool - disposes of the string pool and any associated memory.  The

 *		funtion should return kSPNoError

 *	

 *	makeWString - the string pool keeps a list of added strings. When a new string is 

 *		added with MakeWString(), the routine checks to see if it is already in the

 *		pool.  If so, the address of the string instance in the pool is returned.  If

 *		not, it will add it to the pool and return the address of the newly

 *		created string instance.  The behavior is:

 *		

 *			if ( string == NULL )

 *					*wString = NULL;

 *					returns kSPNoError;

 *			else if ( string in string pool )

 *					*wString = found string;

 *					returns kSPNoError;

 *			else add string 

 *				if successful

 *					*wString = new string;

 *					returns kSPNoError;

 *				else

 *					*wString = nil

 *					returns kSPOutOfMemoryError

 *			

 *	appStringPool - if the host application has already allocated a string pool to use, 

 *		it's reference should be passed here.  If this value is NULL, Sweet Pea will

 *		allocate the pool when initialized and dispose of it at termination.

 *	

 *	filterEvent - a function called for each event allowing the host to cancel it.  

 *		The event type is indicative of what the filter is to do.  A  file validation 

 *		is called before a directory entry is added to the file list (kAddFile).

 *		A plug-in validation before a file is checked for PiPL information (kAddPlugin);

 *		the host might examine the file name/type to determine whether it should be added.

 *		For these 'add' events the return value is TRUE if the item should be skipped 

 *		or FALSE if should be should be added. The default filter proc, used (if NULL) 

 *		is passed, will skip files/folders in ( ).

 *		The other event is kSuitesAvailable.  It is called when the last suite adding 

 *		plug-in	(as determined by available PiPL information) has been added.  This is 

 *		a point at which the host can cancel the startup process; for instance, if the host

 *		requires a suite from a plug-in, this is the time to check for it.  If the 

 *		host returns TRUE, the startup process continues.  If it returns FALSE, the

 *		plug-in startup is canceled and the host would likely terminate or startup in

 *		an alternate manner.

 *

 *	overrideAddPlugins - if supplied, SP will call the host to create the runtime

 *		plug-in list.  This occurs at SPStartupPlugins().  The function takes no parameters 

 *		as it is up to the host to determine how to do this.  For instance, the host can do 

 *		this from cached data or, as SP would, from the file list.  A returned error will 

 *		stop the plug-in startup process.

 *

 *	overrideStartup - a function called for each SP2 plug-in before it is sent the

 *		startup message.  If the host returns FALSE, SP will startup the plug-in normal.

 *		If the host returns true, it is assumed that the host has handled the startup

 *		for the plug-in, so SP will not do anything for the plug-in.  This is intended

 *		to be used with a plug-in caching scheme.  

 *		The host would be responsible, for instance, for defining the cacheable

 * 		information in the PiPL, adding it when the callback is made, and later issuing

 *		a startup message when the plug-in is actually needed (e.g. when a menu item 

 *		is selected.)  Two notes: don't forget to SetPluginStarted(), and make sure

 *		to use a string pooled char* to kSPInterfaceCaller and kSPInterfaceStartupSelector.

 *

 *	resolveLink - Windows only.  If the search for plug-ins is to recurse sub-folders,

 *		the host needs to suply this routine.  When a .lnk file is encountered, the

 *		resolveLink host callback function will be called and should return a resolved path.

 *		This is a host callback due to OLE issues such as initialization, which the SP

 *		libary does not currently handle.  If it returns an error code, the result will

 *		be ignored.

 *

 *	getPluginAccess - Allows the host to set the plug-in access information.  This would

 *		be used if, for instance, the host kept its own plug-in list (ala, Photoshop), but

 *		still needed these to be compatible with SPPlugins (e.g. whose accesses are used by ADM)

 *

 *	memoryIsCritical - Mac only.  Allows the host to indicate that memory is in a critical state 

 *		(really low, but can't be purged because you are, say, shutdown.)

 * 		If so and the plug-in load target heap is the app heap, when a plug-in fails to load

 *		SP will then try to load the plug-in into the system heap

 */



/* These are passed in startup and shutdown host notify procs and the filter file proc. */

typedef enum {

	kAddFile,				/* 	for startup notify, received after a file has been added as plugin, notifyData is the plug-in ref 

								for filter file, received before a file is added to a file list, notifyData is a pointer to the SPPlatformFileSpecification */

	kAddPlugin,				/* 	for startup notify, after a plug-in has been started, notifyData is SPPluginRef

								for filter file, received before a file is checked to see if it is a plugin, notifyData is the files SPFileRef */

	kSetMessage,				/*  used only by startup notify to specify a general message to be

								displayed in the splash screen (primarily for adapters), notifyData is char* */

	kSuitesAvailable,		/*  used only by event filter to allow host to check for suites it requires, notifyDatais NULL */

	kNoEvent = 0xffffffff

 } NotifyEvent;





/* To be compatible w/ other environment besides Metrowerks */

#if defined(MAC_ENV) && defined(__MWERKS__) && !defined(powerc)

	#pragma pointers_in_D0

#endif



typedef void *(*SPAllocateProc)( long size, void *hostData );

typedef void (*SPFreeProc)( void *block, void *hostData );

typedef void *(*SPReallocateProc)( void *block, long newSize, void *hostData );

typedef void (*SPStartupNotifyProc)( NotifyEvent event, void *notifyData, void *hostData );

typedef void (*SPShutdownNotifyProc)( NotifyEvent event, void *notifyData, void *hostData );

typedef void (*SPAssertTrapProc)( const char *failMessage, void *hostData );

typedef void (*SPThrowTrapProc)( SPErr error, void *hostData );

typedef void (*SPDebugTrapProc)( const char *debugMessage, void *hostData );



typedef SPAPI SPErr (*SPAllocateStringPoolProc)( SPStringPoolRef *pool ); 

typedef SPAPI SPErr (*SPFreeStringPoolProc)( SPStringPoolRef stringPool );

typedef SPAPI SPErr (*SPMakeWStringProc)( SPStringPoolRef stringPool, const char *string,

			const char **wString );



typedef SPAPI SPErr (*SPGetHostAccessInfoProc)( SPPlatformAccessInfo *spHostAccessInfo );



typedef SPAPI SPBoolean (*SPFilterEventProc)( NotifyEvent event, const void *eventData );

typedef SPAPI SPErr (*SPAddPluginsProc)( void );

typedef SPAPI SPBoolean (*SPOverrideStartupProc)( SPPluginRef currentPlugin );



#ifdef WIN_ENV

typedef SPAPI SPErr (*SPResolveLinkProc)(const char *shortcutFile, const char *resolvedPath);

#endif



typedef SPAPI SPErr (*GetNativePluginAccessProc)(SPPluginRef plugin, SPAccessRef *access);



typedef SPAPI SPBoolean (*MemoryIsCriticalProc)( void );







#if defined(MAC_ENV) && defined(__MWERKS__) && !defined(powerc)

	#pragma pointers_in_A0

#endif



typedef struct SPHostProcs {



	void *hostData;



	SPAllocateProc extAllocate;

	SPFreeProc extFree;

	SPReallocateProc extReallocate;



	SPAllocateProc intAllocate;

	SPFreeProc intFree;

	SPReallocateProc intReallocate;



	SPStartupNotifyProc startupNotify;

	SPShutdownNotifyProc shutdownNotify;



	SPAssertTrapProc assertTrap;

	SPThrowTrapProc throwTrap;

	SPDebugTrapProc debugTrap;



	SPAllocateStringPoolProc allocateStringPool;

	SPFreeStringPoolProc freeStringPool;

	SPMakeWStringProc makeWString;

	SPStringPoolRef appStringPool;

	

	SPFilterEventProc filterEvent;

	SPAddPluginsProc overrideAddPlugins;

	SPOverrideStartupProc overridePluginStartup;



#ifdef WIN_ENV

	SPResolveLinkProc resolveLink;

#endif



	GetNativePluginAccessProc getPluginAccess;

	

#ifdef MAC_ENV

	// enable second-chance plugin loading for success-critical situations

	MemoryIsCriticalProc memoryIsCritical;

#endif



} SPHostProcs;





/*******************************************************************************

 **

 **	Suite

 **

 **/



typedef struct SPRuntimeSuite {



	SPAPI SPErr (*GetRuntimeStringPool)( SPStringPoolRef *stringPool );

	SPAPI SPErr (*GetRuntimeSuiteList)( SPSuiteListRef *suiteList );

	SPAPI SPErr (*GetRuntimeFileList)( SPFileListRef *fileList );

	SPAPI SPErr (*GetRuntimePluginList)( SPPluginListRef *pluginList );

	SPAPI SPErr (*GetRuntimeAdapterList)( SPAdapterListRef *adapterList );

	SPAPI SPErr (*GetRuntimeHostProcs)( SPHostProcs **hostProcs );



	SPAPI SPErr (*GetRuntimePluginsFolder)( SPPlatformFileSpecification *pluginFolder );

	SPAPI SPErr (*GetRuntimeHostFileSpec)( SPPlatformFileSpecification *hostFileSpec );

} SPRuntimeSuite;





SPAPI SPErr SPGetRuntimeStringPool( SPStringPoolRef *stringPool );

SPAPI SPErr SPGetRuntimeSuiteList( SPSuiteListRef *suiteList );

SPAPI SPErr SPGetRuntimeFileList( SPFileListRef *fileList );

SPAPI SPErr SPGetRuntimePluginList( SPPluginListRef *pluginList );

SPAPI SPErr SPGetRuntimeAdapterList( SPAdapterListRef *adapterList );

SPAPI SPErr SPGetRuntimeHostProcs( SPHostProcs **hostProcs );

SPAPI SPErr SPGetRuntimePluginsFolder( SPPlatformFileSpecification *pluginFolder );

SPAPI SPErr SPSetRuntimePluginsFolder( SPPlatformFileSpecification *pluginFolder );

SPAPI SPErr SPGetRuntimeHostFileSpec( SPPlatformFileSpecification *hostFileSpec );





#ifdef __cplusplus

}

#endif



#endif

