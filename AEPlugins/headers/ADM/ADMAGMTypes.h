/***********************************************************************/

/*                                                                     */

/* ADMAGMTypes.h                                                       */

/* Abstract references to AGM types for ADM interfaces                 */

/*                                                                     */

/* Copyright 1999 Adobe Systems Incorporated.                          */

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

/* Started by Eric Scouten, 06/24/1999                                 */

/*                                                                     */

/***********************************************************************/



#ifndef _ADMAGMTYPES_H_

#define _ADMAGMTYPES_H_



#include "ASTypes.h"



#ifdef __cplusplus

extern "C" {

#endif



typedef struct _t_ADMAGMColrSpace ADMAGMColorSpace;

typedef int ADMAGMColorSpaceFamily;

typedef struct _t_ADMAGMRasterPort ADMAGMRasterPort;

typedef struct _t_ADMAGMPort ADMAGMPort;

typedef ADMAGMPort* ADMAGMPortPtr;

typedef struct _t_ADMAGMColorTab ADMAGMColorTab;

typedef struct _t_ADMAGMImageRecord ADMAGMImageRecord;



typedef enum _ADMLineCap {

	kButtCap = 0,				/* Butt line caps */

	kRoundCap,					/* Round line caps */

	kProjectingCap				/* Projecting line caps */

} ADMLineCap;



struct _t_ADMAGMColorTab	{

	ASInt32	numColors;

	void*	theColors;

};



typedef struct _t_ADMAGMInt16Rect {

	ASInt16	xMin;

	ASInt16	yMin;

	ASInt16	xMax;

	ASInt16	yMax;

} ADMAGMInt16Rect;



struct _t_ADMAGMImageRecord	{

	ADMAGMInt16Rect	bounds;

	void*			baseAddr;

	ASInt32			byteWidth;

	ASInt16			colorSpace;

	ASInt16			bitsPerPixel;

	float*		decodeArray;

	ADMAGMColorTab	colorTab;

};



#ifdef __cplusplus

}

#endif



#endif // _ADMAGMTYPES_H_

