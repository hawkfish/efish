/***********************************************************************/

/*                                                                     */

/* SPBlocks.h                                                          */

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



#ifndef __SPBlocks__

#define __SPBlocks__





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



#define kSPBlocksSuite				"SP Blocks Suite"

#define kSPBlocksSuiteVersion		2





/*******************************************************************************

 **

 **	Suite

 **

 **/



typedef struct SPBlocksSuite {



	SPAPI SPErr (*AllocateBlock)( long size, const char *debug, void **block );

	SPAPI SPErr (*FreeBlock)( void *block );

	SPAPI SPErr (*ReallocateBlock)( void *block, long newSize, const char *debug, void **rblock );



} SPBlocksSuite;





SPAPI SPErr SPAllocateBlock( long size, const char *debug, void **block );

SPAPI SPErr SPFreeBlock( void *block );

SPAPI SPErr SPReallocateBlock( void *block, long newSize, const char *debug, void **rblock );





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

