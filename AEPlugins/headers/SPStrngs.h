/***********************************************************************/

/*                                                                     */

/* SPStrngs.h                                                          */

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



#ifndef __SPStrings__

#define __SPStrings__





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

 ** Constants

 **

 **/



#define kSPStringsSuite				"SP Strings Suite"

#define kSPStringsSuiteVersion		2



#define kSPRuntimeStringPool		((SPStringPoolRef)NULL)





/*******************************************************************************

 **

 ** Types

 **

 **/



/*  If you override the default string pool handler by defining host proc routines,

 *	how the string pool memory allocation and searching is done is up to you.  As an example,

 *	the structure below is similar to what Sweet Pea uses for its default string pool 

 *	routines. The pool is a sorted list of strings of number count, kept in memory referenced 

 *	by the heap field.

 *	

 *			typedef struct SPStringPool {

 *		

 *				SPPoolHeapRef heap;

 *				long count;

 *	

 *			} SPStringPool;

 */

 

typedef struct SPStringPool *SPStringPoolRef;





/*******************************************************************************

 **

 ** Suite

 **

 **/



typedef struct SPStringsSuite {



	SPAPI SPErr (*AllocateStringPool)( SPStringPoolRef *stringPool );

	SPAPI SPErr (*FreeStringPool)( SPStringPoolRef stringPool );

	SPAPI SPErr (*MakeWString)( SPStringPoolRef stringPool, const char *string, const char **wString );



} SPStringsSuite;





SPAPI SPErr SPAllocateStringPool( SPStringPoolRef *stringPool );

SPAPI SPErr SPFreeStringPool( SPStringPoolRef stringPool );

SPAPI SPErr SPMakeWString( SPStringPoolRef stringPool, const char *string, const char **wString );





#ifdef __cplusplus

}

#endif



#endif

