/*
 * Name:
 *	ASMac68KPlugin.h
 *
 * Copyright 1986-2001 Adobe Systems Incorporated.
 * All Rights Reserved.
 *
 * Purpose:
 *	Adobe Macintosh 68K Plugin support.
 *
 * Distribution:
 *	PUBLIC
 *
 * Version history:
 *	1.0.0 1/26/1995	DL	First version.
 *		Created by Dave Lazarony.
 */

#ifndef __ASMac68KPlugin__
#define __ASMac68KPlugin__

#ifndef __ASConfig__
#include "ASConfig.h"
#endif

#if !defined(__BUILD_PLUGIN__)
#if defined(ILLUSTRATOR_H)
#define __BUILD_PLUGIN__ 0
#else
#define __BUILD_PLUGIN__ 1
#endif
#endif

#if __BUILD_PLUGIN__ && defined(MAC68K_ENV)
	#include <A4Stuff.h>
	#define PUSH_GLOBALS	long __oldA4 = SetCurrentA4();
	#define POP_GLOBALS		SetA4(__oldA4);
	#define kPlatformUnloadedSuiteProc	((void *)0x50FF8001L)
#endif


#if !defined(PUSH_GLOBALS)
	#define PUSH_GLOBALS
	#define POP_GLOBALS
	#define kPlatformUnloadedSuiteProc	NULL
#endif

#endif
	


