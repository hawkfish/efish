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

/**	AE_Macros.h
	
	Part of the Adobe After Effects 5.0 SDK.
	
	REVISION HISTORY	
		06/12/96	bsa		Updated for After Effects 3.1
		04/06/97	bsa		Updated for After Effects 3.1 Windows version
		03/01/99	bbb		Added DH.

**/

#ifndef _H_AE_MACROS
#define _H_AE_MACROS

#define			AEFX_CLR_STRUCT(STRUCT)		\
	do {									\
		long _t = sizeof(STRUCT);			\
		char *_p = (char*)&(STRUCT);		\
		while (_t--) {						\
			*_p++ = 0;						\
		}									\
	} while (0);										


#ifndef AEFX_COPY_STRUCT

#define			AEFX_COPY_STRUCT(FROM, TO)	\
	do {									\
		long _t = sizeof(FROM);				\
		char *_p = (char*)&(FROM);			\
		char *_q = (char*)&(TO);			\
		while (_t--) {						\
			*_q++ = *_p++;					\
		}									\
	} while (0);	

#endif

#define DH(h)				(*(h))

#define FIX2INT(X)				((long)(X) >> 16)
#define INT2FIX(X)				((long)(X) << 16)
#define FIX2INT_ROUND(X)		(FIX2INT((X) + 32768))
#define	FIX2FLOAT(X)			((double)(X) / 65536.0)
#define	FLOAT2FIX(F)			((PF_Fixed)((F) * 65536 + (((F) < 0) ? -0.5 : 0.5)))
#define ABS(N)					((N) < 0 ? -(N) : (N))
#define MIN(A,B)			((A) < (B) ? (A) : (B))
#define ABS(N)				((N) < 0 ? -(N) : (N))

#define	PF_RECT_2_FIXEDRECT(R,FR)	do {	\
	(FR).left = INT2FIX((R).left);			\
	(FR).top = INT2FIX((R).top);			\
	(FR).right = INT2FIX((R).right);		\
	(FR).bottom = INT2FIX((R).bottom);		\
	} while (0)

#define	PF_FIXEDRECT_2_RECT(FR,R)	do {			\
	(R).left = (short)FIX2INT_ROUND((FR).left);		\
	(R).top  = (short)FIX2INT_ROUND((FR).top);		\
	(R).right = (short)FIX2INT_ROUND((FR).right);	\
	(R).bottom = (short)FIX2INT_ROUND((FR).bottom);	\
	} while (0)


#define SWAP_LONG(a)		((a >> 24) | ((a >> 8) & 0xff00) | ((a << 8) & 0xff0000) | (a << 24))


#define RGB8_MAX			255
#define RGB8_HALF			128
#define RGB16_MAX			32768
#define RGB16_HALF			16384

#define CONVERT8TO16(A)		( (((long)(A) * RGB16_MAX) + RGB8_HALF) / RGB8_MAX )
#define CONVERT16TO8(A)		( (((long)(A) * RGB8_MAX) + RGB16_HALF) / RGB16_MAX)

#endif		// _H_AX_MACROS
