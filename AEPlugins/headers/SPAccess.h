/***********************************************************************/

/*                                                                     */

/* SPAccess.h                                                          */

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



#ifndef __SPAccess__

#define __SPAccess__





/*******************************************************************************

 **

 **	Imports

 **

 **/



#include "SPTypes.h"

#include "SPPlugs.h"

#include "SPMData.h"



#ifdef __cplusplus

extern "C" {

#endif





/*******************************************************************************

 **

 ** Constants

 **

 **/



/*

 *	The name and version number of the Sweet Pea Plug-in Access suite.

 */



#define kSPAccessSuite				"SP Access Suite"

#define kSPAccessSuiteVersion		3





/*

 *	The access suite sends reload and unload messages to Sweet Pea plug-ins.

 *	They are used to inform the plug-in that it has just been loaded into

 *	RAM or is about to be unloaded. A plug-in is automatically loaded by

 *	Sweet Pea when the host app or another plug-in calls it or acquires a

 *	suite that it exports. A plug-in may be unloaded at any time if it

 *	is not being used.

 *

 *	"Reload" is the very first message that a Sweet Pea plug-in receives before

 *	it is started up (in this case the message really means "Load"). "Unload" is

 *	the very last message a plug-in receives after it is shut down. In between,

 *	a plug-in must be prepared to be unloaded and reloaded any time it is not

 *	being used. "Unload" is the plug-in's opportunity to save state information

 *	that it will re-instantiate when it is reloaded. How this is done depends

 *	on the platform and environment.

 *

 *	Here is the sequence of events:

 *

 *	1) Reload: plug-in does nothing, since it hasn't been started up yet.

 *	2) Startup: plug-in initializes globals, adds features to application.

 *	...

 *	n) Unload: plug-in saves globals (using the 'globals' field in the

 *	   SPMessageData structure).

 *	...

 *	o) Reload: plug-in restores globals.

 *	...

 *	p) Unload: plug-in saves globals.

 *	...

 *	z-1) Reload: plug-in restores globals.

 *	z) Shutdown: plug-in saves preferences, removes features from application.

 *

 *	In addition, a plug-in that exports a suite will need to unload and reload

 *	the suite's procedure pointers. Unloading the suite means stuffing the

 *	suite's procedure pointers with the address of the Undefined() function in

 *	the Basic suite (which is always available). This is a protective measure

 *	against other plug-ins that may mistakenly use the suite after they have

 *	released it. Reloading the suite means stuffing the suite's procedure

 *	pointers with the updated addresses of their functions, since the plug-in

 *	is likely to be in a new position in memory.

 *

 *	Example:

 *

 *	SPErr UnloadSuite( MySuite *mySuite, SPAccessMessage *message ) {

 *

 *		mySuite->functionA = (void *) message->d.basic->Undefined;

 *		mySuite->functionB = (void *) message->d.basic->Undefined;

 *

 *	}

 *

 *

 *	SPErr ReloadSuite( MySuite *mySuite, SPAccessMessage *message ) {

 *

 *		mySuite->functionA = functionA;

 *		mySuite->functionB = functionB;

 *

 *	}

 */



#define kSPAccessCaller				"SP Access"

#define kSPAccessReloadSelector		"Reload"

#define kSPAccessUnloadSelector		"Unload"





/*

 *	kSPFlushAccessType is of interest only to the host application or to the

 *	rare plug-in that uses the Caches suite. It is the name of the object

 *	that the Access suite caches and which can be purged when Sweet Pea's

 *	caches are flushed. During a cache flush the SPFlushCachesProc may be

 *	asked about purging one of these. The SPFlushCachesProc may make any calls

 *	to the Access suite to collect information about the SPAccessRef in

 *	question before deciding to purge it or not.

 *

 *	For example, an application may implement a strategy where filter plug-ins

 *	are purged before window plug-ins:

 *

 *	long myFlushCachesProc( const char *type, void *data, SPErr *error ) {

 *	

 *		long flush = TRUE;

 *	

 *		if ( strcmp( type, kSPFlushAccessType ) == 0 ) {

 *	

 *			SPAccessRef access = data;

 *			SPPluginRef plugin = sAccess->GetAccessPlugin( access, error );

 *			fail( error );

 *

 *			switch ( myPluginType( plugin ) ) {

 *	

 *				case kMyFilterPlugin:

 *					if ( gPurgeLevel < kFilterPurgeLevel )

 *						flush = FALSE;

 *					break;

 *	

 *				case kMyWindowPlugin:

 *					if ( gPurgeLevel < kWindowPurgeLevel )

 *						flush = FALSE;

 *					break;

 *

 *			}

 *	

 *		}

 *	

 *		return flush;

 *			

 *	}

 */



#define kSPFlushAccessType			"SP Flush Access"





/*******************************************************************************

 **

 ** Types

 **

 **/



/*

 *	SPEntry is the prototype for a Sweet Pea plug-in's entry point. The parms

 *	'caller' and 'selector' tell why the plug-in is being called, and the

 *	'message' is any data associated with it. For example, when the Access

 *	suite calls a plug-in to tell it that it is being unloaded, the caller and

 *	selector are kSPAccessCaller and kSPAccessUnloadSelector (above), and the

 *	message is an SPAccessMessage (below).

 *

 *	The prototype is modified by SPAPI, which is used on some platforms to

 *	standardize the parameter-passing conventions. See SPTypes.h for details.

 */



typedef SPAPI SPErr (*SPEntry)( const char *caller, const char *selector, void *message );





/*

 *	An SPAccessRef is an access path to an open Sweet Pea plug-in. It is

 *	returned when you acquire a plug-in (which may cause the plug-in to be

 *	reloaded) and given up when the plug-in is released. The SPAccessRef is

 *	used primarily for making calls to a plug-in.

 *

 *	Example:

 *

 *	SPErr SendMessage( SPPluginRef plugin, const char *caller, const char *selector, void *message, SPErr *error ) {

 *	

 *		SPErr result;

 *		SPAccessRef access;

 *	

 *		access = sAccess->AcquirePlugin( plugin, error );

 *		fail( error );

 *	

 *		result = sAccess->CallPlugin( access, caller, selector, message, error );

 *		fail( error );

 *	

 *		sAccess->ReleasePlugin( access, error );

 *		fail( error );

 *	

 *		return result;

 *	

 *	}

 *

 *	Access paths are reference-counted, which means it's very important to

 *	balance all of your calls to AcquirePlugin() and ReleasePlugin(). It also

 *	means that you don't need to concern yourself with whether an access path

 *	to a given plug-in already exists. If the plug-in is not loaded, it is

 *	reloaded and a new access path is created. If it is already loaded, the

 *	existing access path is referenced and returned. A plug-in will be

 *	unloaded only if the reference count of its access path is zero. 

 */



typedef struct SPAccess *SPAccessRef;





/*

 *	An SPPlatformAccessInfo is platform-specific information about the access

 *	path to an open plug-in. On the Mac it contains resource chain information

 *	and a time stamp of when it was last called. On Windows it contains the

 *	handle to the plug-in's library. These are used by plug-ins that export

 *	suites since they must manually establish a resource context within any of

 *	the suite procedures before they can access resources. This is not

 *	necessary when a plug-in is called through its entry point, since

 *	CallPlugin() sets up the resource context for you.

 */





#ifdef MAC_ENV



#if !Platform_Carbon

typedef struct {

	void *TopMapHndl;

	short CurMap;

} SPMacResChain;

#else	

typedef struct {

	short fileRefNum;

} SPMacResChain;

#endif	// !Platform_Carbon



typedef SPMacResChain *SPPlatformAccessRef;



#endif



#ifdef WIN_ENV



typedef void *SPPlatformAccessRef;



#endif





typedef struct {						/* resources on Mac, library on Windows */

	SPPlatformAccessRef defaultAccess;	/* file access for entry and resources unless resourceAccess is used */

	SPPlatformAccessRef resourceAccess; /* Used if SetPluginResourceAccess() has been used, otherwise NULL */

	unsigned long lastAccessTicks;

} SPPlatformAccessInfo;







/*

 *	This is the message sent to a plug-in whenever it is called by the Access

 *	suite. It consists solely of an SPMessageData, which is the standard

 *	header for all messages sent by Sweet Pea, and an indicator of when the

 * 	access is occuring.  Note that not all access points will necessarily be

 *  received. For instance, if a plug-in was never unloaded from memory, it's

 *  sole time of reload access would be kStartup and its only unload 

 *  access point would be kShutdown. A startup or shutdown access point received at 

 *  reload actually means pre-startup and a shutdown access point at unload means

 *	post-shutdown.  kTerminal will only be received with an unload message by a plug-in

 *	that for one reason or another has an access count when when the application 

 *	terminates.  Plug-ins with an access count at termination will be unloaded and 

 *	this forced unload will have kTerminal in its when field.  A plug-in that 

 *	has purposely acquired itself to remain in memory should not free any resources

 *	and not acquire others when this terminal unload is received.

 */

 

typedef enum {

	kStartup = 0,	

	kRuntime,

	kShutdown,

	kTerminal

} SPAccessPoint;



typedef struct SPAccessMessage {



	SPMessageData d;

	SPAccessPoint when;



} SPAccessMessage;





/*******************************************************************************

 **

 ** Suite

 **

 **/



typedef struct SPAccessSuite {



/*

 *	SPAccessRef AcquirePlugin( SPPluginRef plugin, SPErr *error );

 *

 *	Given a reference to a Sweet Pea plug-in, AcquirePlugin() opens the plug-in

 *	and prepares it to be called. It returns an access reference, which you must

 *	use when calling the plug-in and which you must give to ReleasePlugin()

 *	when you are through.

 *

 *	Access paths are reference-counted, which means it's very important to

 *	balance all of your calls to AcquirePlugin() and ReleasePlugin(). It also

 *	means that you don't need to concern yourself with whether an access path

 *	to a given plug-in already exists. If the plug-in is not loaded, it is

 *	reloaded and a new access path is created. If it is already loaded, the

 *	existing access path is referenced and returned. A plug-in will be

 *	unloaded only if the reference count of its access path is zero.

 *

 *	Here's a trick: You can acquire yourself if you want to stay loaded in

 *	memory even when you're not being reference by anyone.

 *

 *	Sweet Pea plug-ins rarely call one another directly. Usually program

 *	control flows from plug-in to plug-in through suites, which are layered on

 *	top of the Access suite. Plug-in adapters (plug-ins that provide protocol

 *	conversion to non-Sweet Pea plug-ins) will add their own access suites to

 *	provide access to their foreign plug-ins.

 */



	SPAPI SPErr (*AcquirePlugin)( SPPluginRef plugin, SPAccessRef *access );





/*

 *	void ReleasePlugin( SPAccessRef access, SPErr *error );

 *

 *	You must call ReleasePlugin() when you are through with a plug-in that

 *	you had previously acquired. ReleasePlugin() adjusts the reference count

 *	of the plug-in, possibly making it eligible for unloading. After

 *	releasing a plug-in the access reference is no longer valid.

 */



	SPAPI SPErr (*ReleasePlugin)( SPAccessRef access );





/*

 *	SPAccessRef GetPluginAccess( SPPluginRef plugin, SPErr *error );

 *

 *	Given a plug-in, GetPluginAccess() returns an access path to the plug-in

 *	ONLY if the plug-in is already open. It returns NULL if the plug-in is

 *	unloaded.

 *

 *	You can use this inside of your plug-in to get the access path to

 *	yourself. This is necessary when you export a suite, since you need the

 *	resource chain or library information stored in the access path.

 */



	SPAPI SPErr (*GetPluginAccess)( SPPluginRef plugin, SPAccessRef *access );

	SPAPI SPErr (*GetAccessPlugin)( SPAccessRef access, SPPluginRef *plugin );

	SPAPI SPErr (*GetAccessEntry)( SPAccessRef access, SPEntry *entry );

	SPAPI SPErr (*GetAccessCount)( SPAccessRef access, long *count );

	SPAPI SPErr (*GetAccessInfo)( SPAccessRef access, SPPlatformAccessInfo *info );



	SPAPI SPErr (*GetPluginResourceAccess)( SPPluginRef plugin, SPPlatformAccessRef *resourceAccess );

	SPAPI SPErr (*SetPluginResourceAccess)( SPPluginRef plugin, SPPlatformAccessRef resourceAccess );



	SPAPI SPErr (*CallPlugin)( SPAccessRef access, const char *caller, const char *selector,

					void *message, SPErr *result );



	SPAPI SPErr (*GetCurrentPlugin)( SPPluginRef *plugin );

	SPAPI SPErr (*SetCurrentPlugin)( SPPluginRef plugin );



} SPAccessSuite;





SPAPI SPErr SPAcquirePlugin( SPPluginRef plugin, SPAccessRef *access );

SPAPI SPErr SPReleasePlugin( SPAccessRef access );



SPAPI SPErr SPGetPluginAccess( SPPluginRef plugin, SPAccessRef *access );

SPAPI SPErr SPGetAccessPlugin( SPAccessRef access, SPPluginRef *plugin );

SPAPI SPErr SPGetAccessEntry( SPAccessRef access, SPEntry *entry );

SPAPI SPErr SPGetAccessCount( SPAccessRef access, long *count );

SPAPI SPErr SPGetAccessInfo( SPAccessRef access, SPPlatformAccessInfo *info );



SPAPI SPErr SPGetPluginResourceAccess( SPPluginRef plugin, SPPlatformAccessRef *resourceAccess );

SPAPI SPErr SPSetPluginResourceAccess( SPPluginRef plugin, SPPlatformAccessRef resourceAccess );



SPAPI SPErr SPCallPlugin( SPAccessRef access, const char *caller, const char *selector,

			void *message, SPErr *result );



SPAPI SPErr SPGetCurrentPlugin( SPPluginRef *plugin );

SPAPI SPErr SPSetCurrentPlugin( SPPluginRef plugin );





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

