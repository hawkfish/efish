/*
 * Name:
 *	ASPragma.h
 *
 * Copyright 1986-2001 Adobe Systems Incorporated.
 * All Rights Reserved.
 *
 * Purpose:
 *	Makes #defines Pragmas to eliminate compiler dependency.
 *
 * Distribution:
 *	PUBLIC
 *
 * Version history:
 *	1.0.0 1/26/1996	DL	First version.
 *		Created by Dave Lazarony.
 */

#ifndef __ASPragma__
#define __ASPragma__

#ifndef __ASConfig__
#include "ASConfig.h"
#endif


#ifdef MAC_ENV

#ifdef MACPPC_ENV
#define	PRAGMA_ALIGN_BEGIN			options align=mac68k
#define PRAGMA_ALIGN_END			options align=reset
#define PRAGMA_IMPORT_BEGIN			import on
#define PRAGMA_IMPORT_END			import off
#endif

#ifdef MAC68K_ENV
#define	PRAGMA_ALIGN_BEGIN			
#define PRAGMA_ALIGN_END			
#define PRAGMA_IMPORT_BEGIN			
#define PRAGMA_IMPORT_END			
#endif

#endif


// NOTE: Illustrator uses 2 byte align now, but may change to native align on MSWINDOWS later.
// Currently, the alignment for plugin on MSWINDOWS is set within the project file.
#ifdef WIN_ENV
#define	PRAGMA_ALIGN_BEGIN			pack(push, 4)			
#define PRAGMA_ALIGN_END			pack(pop)
#define PRAGMA_IMPORT_BEGIN			
#define PRAGMA_IMPORT_END		
#endif


#endif
