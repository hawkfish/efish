/***********************************************************************/

/*                                                                     */

/* SPBckDbg.h                                                          */

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



#ifndef __SPBlockDebug__

#define __SPBlockDebug__





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

 **	Constants

 **

 **/



#define kSPBlockDebugSuite			"SP Block Debug Suite"

#define kSPBlockDebugSuiteVersion	2





/*******************************************************************************

 **

 **	Suite

 **

 **/



typedef struct SPBlockDebugSuite {



	SPAPI SPErr (*GetBlockDebugEnabled)( long *enabled );

	SPAPI SPErr (*SetBlockDebugEnabled)( long debug );

	SPAPI SPErr (*GetFirstBlock)( void **block );

	SPAPI SPErr (*GetNextBlock)( void *block, void **nextblock );

	SPAPI SPErr (*GetBlockDebug)( void *block, const char **debug );



} SPBlockDebugSuite;





SPAPI SPErr SPGetBlockDebugEnabled( long *enabled );

SPAPI SPErr SPSetBlockDebugEnabled( long debug );

SPAPI SPErr SPGetFirstBlock( void **block );

SPAPI SPErr SPGetNextBlock( void *block, void **nextblock );

SPAPI SPErr SPGetBlockDebug( void *block, const char **debug );





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

