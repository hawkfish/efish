/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2001 Adobe Systems Incorporated                       */
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

/* PF_Suite_Helper.c

	Contains helper routines for working with the suites.
	
	1.0	developed 9/98 by jja
*/

#include "PF_Suite_Helper.h"


PF_Err AEFX_AcquireSuite(	PF_InData		*in_data,		
							PF_OutData		*out_data,		
							const char		*name,			
							long			version,		
							const char		*error_stringPC0,
							void			**suite)			
{
	PF_Err			err = PF_Err_NONE;
	SPBasicSuite	*bsuite = in_data->pica_basicP;;

	if (bsuite) {
		(*bsuite->AcquireSuite)((char*)name, version, (const void**)suite);
		if (!*suite) {
			err = PF_Err_BAD_CALLBACK_PARAM;
		}
	} 
	else {
		err = PF_Err_BAD_CALLBACK_PARAM;
	}

	if (err) {
		const char	*error_stringPC	= error_stringPC0 ? error_stringPC0 : "Not able to acquire AEFX Suite.";
		out_data->out_flags |= PF_OutFlag_DISPLAY_ERROR_MESSAGE;
		PF_SPRINTF(out_data->return_msg, error_stringPC);
	}
	return err;
}

PF_Err AEFX_ReleaseSuite(	PF_InData		*in_data,	
							PF_OutData		*out_data,	
							const char		*name,		
							long			version,	
							const char		*error_stringPC0)
{
	PF_Err			err = PF_Err_NONE;
	SPBasicSuite	*bsuite = in_data->pica_basicP;

	if (bsuite) {
		(*bsuite->ReleaseSuite)((char*)name, version);
	} 
	else {
		err = PF_Err_BAD_CALLBACK_PARAM;
	}
	if (err) {
		const char	*error_stringPC	= error_stringPC0 ? error_stringPC0 : "Not able to release AEFX Suite.";
		out_data->out_flags |= PF_OutFlag_DISPLAY_ERROR_MESSAGE;
		PF_SPRINTF(out_data->return_msg, error_stringPC);
	}
	return err;
}

