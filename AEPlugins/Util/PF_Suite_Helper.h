/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 1999 Adobe Systems Incorporated                       */
/* All Rights Reserved.                                            */
/*                                                                 */
/* NOTICE:  All information contained herein is, and remains the   */
/* property of Adobe Systems Incorporated and its suppliers, if    */
/* any.  The intellectual and technical concepts contained         */
/* herein are proprietary to Adobe Systems Incorporated and its    */
/* suppliers and may be covered by U.S. and Foreign Patents,       */
/* patents in process, and are protected by trade secret or        */
/* copyright law.  Dissemination of this information or            */
/* reproduction of this material is strictly forbidden unless      */
/* prior written permission is obtained from Adobe Systems         */
/* Incorporated.                                                   */
/*                                                                 */
/*******************************************************************/

#ifndef _H_PF_SUITE_HELPER
#define _H_PF_SUITE_HELPER

/* PF_Suite_Helper.h

	Contains helper routines for working with PICA suites.

	9/1/98	jja	Created.
*/

#include <AE_Effect.h>
#include <AE_EffectCB.h>
#include <AE_EffectSuites.h>
#include <SPBasic.h>
#include <SPSuites.h>

#ifdef __cplusplus
extern "C" {
#endif
PF_Err AEFX_AcquireSuite(	PF_InData		*in_data,			/* >> */
							PF_OutData		*out_data,			/* >> */
							const char		*name,				/* >> */
							long			version,			/* >> */
							const char		*error_stringPC0,	/* >> */
							void			**suite);			/* << */

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
PF_Err AEFX_ReleaseSuite(	PF_InData		*in_data,			/* >> */
							PF_OutData		*out_data,			/* >> */
							const char		*name,				/* >> */
							long			version,			/* >> */
							const char		*error_stringPC0);	/* >> */
#ifdef __cplusplus
}
#endif

#endif // _H_PF_SUITE_HELPER