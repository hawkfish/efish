#ifndef __ADMResource__
#define __ADMResource__

/*
 *        Name:	ADMResource.h
 *   $Revision: 1.1 $
 *      Author:	Dave Lazarony 
 *        Date:	5/12/96
 *     Purpose:	ADM Resources
 *
 * Copyright (c) 1986-2001 Adobe Systems Incorporated, All Rights Reserved.
 *
 */


/*
 * ADM Picture IDs
 */
#define kADMAttentionPictureID					-1000
#define kADMErrorPictureID						-1001

#define kADMSliderPictureID						-16000
#define kADMSliderSelectedPictureID				-16001
#define kADMSliderDisabledPictureID				-16002
#define kADMNewEntryPictureID					-16010
#define kADMNewEntryDisabledPictureID			-16012
#define kADMDeleteEntryPictureID				-16020
#define kADMDeleteEntryDisabledPictureID		-16022
#define kADMUpSpinArrowPictureID				-16040
#define kADMUpSpinArrowDisabledPictureID		-16042
#define kADMDownSpinArrowPictureID				-16050
#define kADMDownSpinArrowDisabledPictureID		-16052
#define kADMLeftSpinArrowPictureID				-16060
#define kADMLeftSpinArrowDisabledPictureID		-16062
#define kADMRightSpinArrowPictureID				-16070
#define kADMRightSpinArrowDisabledPictureID		-16072
#define kADMRightPopupArrowPictureID			-16080
#define kADMRightPopupArrowDisabledPictureID	-16082
#define kADMDownPopupArrowPictureID				-16090
#define kADMDownPopupArrowDisabledPictureID		-16092
#define kADMHierarchyListExpandPictureID		-16094
#define kADMHierarchyListCollapsePictureID		-16095
		

/*
 * ADM Cursor IDs
 */
#define kADMIBeamCursorID						-1
#define kADMCrossCursorID						-2
#define kADMWaitCursorID						-3
#define kADMArrowCursorID						-4
#define kADMCancelCursorID						-5
#define kADMFingerCursorID						-6
#define kADMFistCursorID						-7
#define kADMFistPlusCursorID					-8
#define kADMHostControlsCursorID				-9

#define kADMSmallestCursorID					kADMHostControlsCursorID
#define kADMLargestCursorID						kADMIBeamCursorID


/*
 * Or this flag with the dialog style of a Windows dialog resource and the dialog's
 * units are read as Pixel Units instead of Dialog Units.
 */
#define kADMPixelUnitsDialogStyle				0x0020		

#endif
