/***********************************************************************/

/*                                                                     */

/* ADMDrawerOld.h                                                      */

/* Old versions of ADM Drawer Suite                                    */

/*                                                                     */

/* Copyright 1996-1999 Adobe Systems Incorporated.                     */

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



#ifndef __ADMDrawerOld__

#define __ADMDrawerOld__



#ifndef __ADMTypes__

#include "ADMTypes.h"

#endif



#ifndef _ADMAGMTYPES_H_

#include "ADMAGMTypes.h"

#endif





// -----------------------------------------------------------------------------



#ifdef __cplusplus

extern "C" {

#endif



#pragma PRAGMA_ALIGN_BEGIN

#pragma PRAGMA_IMPORT_BEGIN





// =============================================================================

//		* ADM Basic Suite, version 2

// =============================================================================



#define kADMDrawerSuiteVersion2 2



// -----------------------------------------------------------------------------



typedef struct ADMDrawerSuite2

{



	// port accessor



	ASPortRef ASAPI (*GetPortRef)(ADMDrawerRef inDrawer);



	// clear entire area of drawer

	

	void ASAPI (*Clear)(ADMDrawerRef inDrawer);



	// bounds accessor

	

	void ASAPI (*GetBoundsRect)(ADMDrawerRef inDrawer, ASRect* outBoundsRect);



	// clipping

	

	void ASAPI (*GetClipRect)(ADMDrawerRef inDrawer, ASRect* outClipRect);

	void ASAPI (*SetClipRect)(ADMDrawerRef inDrawer, const ASRect* inClipRect);

	void ASAPI (*IntersectClipRect)(ADMDrawerRef inDrawer, const ASRect* inClipRect);

	void ASAPI (*UnionClipRect)(ADMDrawerRef inDrawer, const ASRect* inClipRect);

	void ASAPI (*SubtractClipRect)(ADMDrawerRef inDrawer, const ASRect* inClipRect);



	void ASAPI (*SetClipPolygon)(ADMDrawerRef inDrawer, const ASPoint* inPoints,

				ASInt32 inNumPoints);



	void ASAPI (*IntersectClipPolygon)(ADMDrawerRef inDrawer, const ASPoint* inPoints,

				ASInt32 inNumPoints);



	void ASAPI (*UnionClipPolygon)(ADMDrawerRef inDrawer, const ASPoint* inPoints,

				ASInt32 inNumPoints);



	void ASAPI (*SubtractClipPolygon)(ADMDrawerRef inDrawer, const ASPoint* inPoints,

				ASInt32 inNumPoints);

	

	// port origin

	

	void ASAPI (*GetOrigin)(ADMDrawerRef inDrawer, ASPoint* outOrigin);

	void ASAPI (*SetOrigin)(ADMDrawerRef inDrawer, const ASPoint* inOrigin);

	

	// drawing state accessors

	

	void ASAPI (*GetRGBColor)(ADMDrawerRef inDrawer, ASRGBColor* outColor);

	void ASAPI (*SetRGBColor)(ADMDrawerRef inDrawer, const ASRGBColor* inColor);

	

	ADMColor ASAPI (*GetADMColor)(ADMDrawerRef inDrawer);

	void ASAPI (*SetADMColor)(ADMDrawerRef inDrawer, ADMColor inColor);

	

	ADMDrawMode ASAPI (*GetDrawMode)(ADMDrawerRef inDrawer);

	void ASAPI (*SetDrawMode)(ADMDrawerRef inDrawer, ADMDrawMode inDrawMode);

	

	ADMFont ASAPI (*GetFont)(ADMDrawerRef inDrawer);

	void ASAPI (*SetFont)(ADMDrawerRef inDrawer, ADMFont inFont);



	// simple shape drawers

	

	void ASAPI (*DrawLine)(ADMDrawerRef inDrawer, const ASPoint* inStartPoint,

				const ASPoint* inEndPoint);

	

	void ASAPI (*DrawPolygon)(ADMDrawerRef inDrawer, const ASPoint* inPoints, ASInt32 inNumPoints);

	void ASAPI (*FillPolygon)(ADMDrawerRef inDrawer, const ASPoint* inPoints, ASInt32 inNumPoints);

	

	void ASAPI (*DrawRect)(ADMDrawerRef inDrawer, const ASRect* inRect);

	void ASAPI (*FillRect)(ADMDrawerRef inDrawer, const ASRect* inRect);

	void ASAPI (*ClearRect)(ADMDrawerRef inDrawer, const ASRect* inRect);

	void ASAPI (*DrawSunkenRect)(ADMDrawerRef inDrawer, const ASRect* inRect);

	void ASAPI (*DrawRaisedRect)(ADMDrawerRef inDrawer, const ASRect* inRect);

	void ASAPI (*InvertRect)(ADMDrawerRef inDrawer, const ASRect* inRect);

	

	void ASAPI (*DrawOval)(ADMDrawerRef inDrawer, const ASRect* inRect);

	void ASAPI (*FillOval)(ADMDrawerRef inDrawer, const ASRect* inRect);



	// AGM access

	

	ADMAGMPortPtr ASAPI (*GetAGMPort)(ADMDrawerRef inDrawer);



	void ASAPI (*DrawAGMImage)(ADMDrawerRef inDrawer,

				const struct _t_ADMAGMImageRecord* inImage,

				const ASFixedMatrix* inMatrix, ASInt32 inFlags);



	// resource drawing

	

	void ASAPI (*DrawResPicture)(ADMDrawerRef inDrawer, SPPluginRef inPluginRef,

				ASInt32 inRsrcID, const ASPoint* inTopLeftPoint);



	void ASAPI (*DrawResPictureCentered)(ADMDrawerRef inDrawer, SPPluginRef inPluginRef,

				ASInt32 inRsrcID, const ASRect* inRect);

	

	void ASAPI (*DrawRecoloredResPicture)(ADMDrawerRef inDrawer, SPPluginRef inPluginRef,

				ASInt32 inRsrcID, const ASPoint* inTopLeftPoint, ADMRecolorStyle inStyle);



	void ASAPI (*DrawRecoloredResPictureCentered)(ADMDrawerRef inDrawer,

				SPPluginRef inPluginRef, ASInt32 inRsrcID, const ASRect* inRect,

				ADMRecolorStyle inStyle);



	// icon drawing



	void ASAPI (*DrawIcon)(ADMDrawerRef inDrawer, ADMIconRef inIcon, const ASPoint* inTopLeftPoint);

	void ASAPI (*DrawIconCentered)(ADMDrawerRef inDrawer, ADMIconRef inIcon, const ASRect* inRect);



	void ASAPI (*DrawRecoloredIcon)(ADMDrawerRef inDrawer, ADMIconRef inIcon,

				const ASPoint* inTopLeftPoint, ADMRecolorStyle inStyle);



	void ASAPI (*DrawRecoloredIconCentered)(ADMDrawerRef inDrawer, ADMIconRef inIcon,

				const ASRect* inRect, ADMRecolorStyle inStyle);



	void ASAPI (*GetResPictureBounds)(ADMDrawerRef inDrawer, SPPluginRef inPluginRef,

				ASInt32 inRsrcID, ASRect* outBoundsRect);



	// text drawing



	ASInt32 ASAPI (*GetTextWidth)(ADMDrawerRef inDrawer, const char* inText);

	void ASAPI (*GetFontInfo)(ADMDrawerRef inDrawer, ADMFontInfo* outFontInfo);

	

	void ASAPI (*DrawText)(ADMDrawerRef inDrawer, const char* inText, const ASPoint* inPoint);

	void ASAPI (*DrawTextLeft)(ADMDrawerRef inDrawer, const char* inText, const ASRect* inRect);

	void ASAPI (*DrawTextCentered)(ADMDrawerRef inDrawer, const char* inText, const ASRect* inRect);

	void ASAPI (*DrawTextRight)(ADMDrawerRef inDrawer, const char* inText, const ASRect* inRect);



	// standard arrows

	

	void ASAPI (*DrawUpArrow)(ADMDrawerRef inDrawer, const ASRect* inRect);

	void ASAPI (*DrawDownArrow)(ADMDrawerRef inDrawer, const ASRect* inRect);

	void ASAPI (*DrawLeftArrow)(ADMDrawerRef inDrawer, const ASRect* inRect);

	void ASAPI (*DrawRightArrow)(ADMDrawerRef inDrawer, const ASRect* inRect);



	// drawer creation/destruction



	ADMDrawerRef ASAPI (*Create)(ASPortRef inPortRef, const ASRect* inBoundsRect, ADMFont inFont);	

	void ASAPI (*Destroy)(ADMDrawerRef inDrawer);



	// image drawing



	void ASAPI (*DrawADMImage)(ADMDrawerRef inDrawer, ADMImageRef inImage,

				const ASPoint* inTopLeftPoint);



	void ASAPI (*DrawADMImageCentered)(ADMDrawerRef inDrawer, ADMImageRef inImage,

				const ASRect* inRect);

	

}

ADMDrawerSuite2;





// -----------------------------------------------------------------------------



#pragma PRAGMA_IMPORT_END

#pragma PRAGMA_ALIGN_END



#ifdef __cplusplus

}

#endif



#endif

