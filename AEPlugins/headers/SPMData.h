/***********************************************************************/

/*                                                                     */

/* SPMData.h                                                           */

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



#ifndef __SPMessageData__

#define __SPMessageData__





/*******************************************************************************

 **

 **	Imports

 **

 **/



#include "SPTypes.h"



#ifdef __cplusplus

extern "C" {

#endif





/*******************************************************************************

 **

 ** Types

 **

 **/



/* If the message data associated with a call to a plug-in has come from *

 * SPSendMessage, or is prepared using SetupMessageData(), this constant *

 * will be in the first four bytes of the message, in SPCheck.			 */

#define kSPValidSPMessageData 'SPCk'



typedef struct SPMessageData {



	long SPCheck;			/* kSPValidSPMessageData if a valid SPMessage */

	struct SPPlugin *self;  /* SPPluginRef */

	void *globals;

	struct SPBasicSuite *basic;



} SPMessageData;





#ifdef __cplusplus

}

#endif



#endif

