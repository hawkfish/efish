/*
 * Name:
 *	ASConfig.h
 *
 * Copyright 1986-2001 Adobe Systems Incorporated.
 * All Rights Reserved.
 *
 * Purpose:
 *	Adobe Standard Environment Configuration
 *
 * Distribution:
 *	PUBLIC
 *
 * Version history:
 *	1.0.0 1/26/1996	DL	First version.
 *		Created by Dave Lazarony.
 */

#ifndef __ASConfig__
#define __ASConfig__

/*
 * Defines for MPW and THINK C.
 */
#if defined(macintosh) || defined(THINK_C)
#ifndef MAC_ENV
#define MAC_ENV
#endif
#endif

/*
 * Defines for Metrowerks on Macintosh.
 */
#ifdef __MWERKS__

#if defined(__MC68K__) || defined(__POWERPC__)
#ifndef MAC_ENV
#define MAC_ENV
#endif
#endif

#ifdef __MC68K__
#ifndef MAC68K_ENV
#define MAC68K_ENV
#endif
#endif

#ifdef __POWERPC__
#ifndef MACPPC_ENV
#define MACPPC_ENV
#endif
#endif

#endif


/*
 * Defines for Metrowerks on Windows.
 */
#ifdef __MWERKS__
#ifdef __INTEL__
#ifndef WIN_ENV
#define WIN_ENV
#endif
#endif
#endif

/*
 * Defines for Visual C++ on Windows.
 */
#ifdef _WINDOWS
#ifndef WIN_ENV
#define WIN_ENV
#endif
#endif

#endif
