/***********************************************************************/

/*                                                                     */

/* SPProps.h                                                           */

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



#ifndef __SPProperties__

#define __SPProperties__





/*******************************************************************************

 **

 **	Imports

 **

 **/



#include "SPTypes.h"

#include "SPMdata.h"

#include "SPPiPL.h"



#ifdef __cplusplus

extern "C" {

#endif





/*******************************************************************************

 **

 ** Constants

 **

 **/



#define kSPPropertiesSuite				"SP Properties Suite"

#define kSPPropertiesSuiteVersion2		2

#define kSPPropertiesSuiteVersion		kSPPropertiesSuiteVersion2	// minimal is default

#define kSPPropertiesSuiteVersion3		3



#define kSPPropertiesCaller				"SP Properties"

#define kSPPropertiesAcquireSelector	"Acquire"

#define kSPPropertiesReleaseSelector	"Release"





/*******************************************************************************

 **

 ** Types

 **

 **/



typedef struct SPProperty *SPPropertyRef;

typedef struct SPPropertyList *SPPropertyListRef;

typedef struct SPPropertyListIterator *SPPropertyListIteratorRef;



typedef struct SPPropertiesMessage {



	SPMessageData d;



	PIType vendorID;

	PIType propertyKey;

	long propertyID;



	void *property;

	long refCon;

	long cacheable;



} SPPropertiesMessage;





/*******************************************************************************

 **

 ** Suite

 **

 **/



typedef struct SPPropertiesSuite {



	SPAPI SPErr (*AllocatePropertyList)( SPPropertyListRef *propertyList );

	SPAPI SPErr (*FreePropertyList)( SPPropertyListRef propertyList );	// note that this frees all property lists in the chain

	

	SPAPI SPErr (*AddProperties)( SPPropertyListRef propertyList, PIPropertyList *pList, long refCon, long cacheable );

	SPAPI SPErr (*AddProperty)( SPPropertyListRef propertyList, PIType vendorID, PIType propertyKey, long propertyID, PIProperty *p,

				long refCon, long cacheable, SPPropertyRef *property );



	SPAPI SPErr (*FindProperty)( SPPropertyListRef propertyList, PIType vendorID, PIType propertyKey, long propertyID, SPPropertyRef *property );



	SPAPI SPErr (*NewPropertyListIterator)( SPPropertyListRef propertyList, SPPropertyListIteratorRef *iter );

	SPAPI SPErr (*NextProperty)( SPPropertyListIteratorRef iter, SPPropertyRef *property );

	SPAPI SPErr (*DeletePropertyListIterator)( SPPropertyListIteratorRef iter );



	SPAPI SPErr (*GetPropertyPIProperty)( SPPropertyRef property, PIProperty **p );

	SPAPI SPErr (*GetPropertyRefCon)( SPPropertyRef property, long *refCon );

	SPAPI SPErr (*GetPropertyCacheable)( SPPropertyRef property, long *cacheable );

	SPAPI SPErr (*GetPropertyAllocatedByPlugin)( SPPropertyRef property, long *allocatedByPlugin );



	// kSPPropertiesSuiteVersion3

	SPAPI SPBoolean (*SPHasMultiplePropertyLists)(SPPropertyListRef propertyList);

	SPAPI SPErr (*GetNextPropertyList)(SPPropertyListRef propertyList, SPPropertyListRef *nextPropertyList);

	SPAPI SPErr (*FindPropertyLocal)( SPPropertyListRef propertyList, PIType vendorID, PIType propertyKey,

				long propertyID, SPPropertyRef *property );



} SPPropertiesSuite;





SPAPI SPErr SPAllocatePropertyList( SPPropertyListRef *propertyList );

SPAPI SPErr SPFreePropertyList( SPPropertyListRef propertyList );



SPAPI SPErr SPAddProperties( SPPropertyListRef propertyList, PIPropertyList *pList, long refCon,

			long cacheable );



SPAPI SPErr SPAddProperty( SPPropertyListRef propertyList, PIType vendorID, PIType propertyKey,

			long propertyID, PIProperty *p, long refCon, long cacheable, SPPropertyRef *property );



SPAPI SPErr SPFindProperty( SPPropertyListRef propertyList, PIType vendorID, PIType propertyKey,

			long propertyID, SPPropertyRef *property );



SPAPI SPErr SPNewPropertyListIterator( SPPropertyListRef propertyList, SPPropertyListIteratorRef *iter );

SPAPI SPErr SPNextProperty( SPPropertyListIteratorRef iter, SPPropertyRef *property );

SPAPI SPErr SPDeletePropertyListIterator( SPPropertyListIteratorRef iter );



SPAPI SPErr SPGetPropertyPIProperty( SPPropertyRef property, PIProperty **p );

SPAPI SPErr SPGetPropertyRefCon( SPPropertyRef property, long *refCon );

SPAPI SPErr SPGetPropertyCacheable( SPPropertyRef property, long *cacheable );

SPAPI SPErr SPGetPropertyAllocatedByPlugin( SPPropertyRef property, long *allocatedByPlugin );



SPAPI SPBoolean SPHasMultiplePropertyLists(SPPropertyListRef propertyList);

SPAPI SPErr SPGetNextPropertyList(SPPropertyListRef propertyList, SPPropertyListRef *nextPropertyList);



SPAPI SPErr SPFindPropertyLocal( SPPropertyListRef propertyList, PIType vendorID,

			PIType propertyKey, long propertyID, SPPropertyRef *property );



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

