/***********************************************************************/

/*                                                                     */

/* SPInterf.h                                                          */

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



#ifndef __SPInterface__

#define __SPInterface__





/*******************************************************************************

 **

 **	Imports

 **

 **/



#include "SPTypes.h"

#include "SPBasic.h"

#include "SPFiles.h"

#include "SPMData.h"

#include "SPPlugs.h"

#include "SPProps.h"



#ifdef __cplusplus

extern "C" {

#endif





/*******************************************************************************

 **

 ** Constants

 **

 **/



#define kSPInterfaceSuite				"SP Interface Suite"

#define kSPInterfaceSuiteVersion		2



#define kSPInterfaceCaller				"SP Interface"

#define kSPInterfaceStartupSelector		"Startup"

#define kSPInterfaceShutdownSelector	"Shutdown"

#define kSPInterfaceAboutSelector		"About"



#define kSPSweetPea2Adapter				"Sweet Pea 2 Adapter"

#define kSPSweetPea2AdapterVersion		1



/*******************************************************************************

 **

 ** Types

 **

 **/



typedef struct SPInterfaceMessage {



	SPMessageData d;



} SPInterfaceMessage;





/*******************************************************************************

 **

 ** Suite

 **

 **/



typedef struct SPInterfaceSuite {



	SPAPI SPErr (*SendMessage)( SPPluginRef plugin, const char *caller, const char *selector,

				void *message, SPErr *result );



	SPAPI SPErr (*SetupMessageData)( SPPluginRef plugin, SPMessageData *data );

	SPAPI SPErr (*EmptyMessageData)( SPPluginRef plugin, SPMessageData *data );



	SPAPI SPErr (*StartupExport)( SPPluginListRef pluginList, const char *name, long version,

				long *started );



} SPInterfaceSuite;





SPAPI SPErr SPSendMessage( SPPluginRef plugin, const char *caller, const char *selector,

			void *message, SPErr *result );



SPAPI SPErr SPSetupMessageData( SPPluginRef plugin, SPMessageData *data );

SPAPI SPErr SPEmptyMessageData( SPPluginRef plugin, SPMessageData *data );



SPAPI SPErr SPStartupExport( SPPluginListRef pluginList, const char *name, long version,

			long *started );





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

