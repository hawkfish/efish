/*
 *  EnvPanel.h - 'VCS Environment' Drop-In Preferences
 *
 *  Copyright © 1998 Electric Fish, Inc.  All rights reserved.
 *
 */

#pragma once

#ifndef __TYPES__
#	include <Types.h>
#endif

#include "EnvList.h"

/*
 *	AppleScript dictionary info.  As a rule of thumb, dropin panels should use the 
 *	same terminology and numeric code in their 'aete' that the IDE uses if there 
 *	is already a similar item in the IDE's 'aete'.  That is the case here, so we 
 *	merely duplicate applicable 68K Project and 68K Linker user terms below.
 */

enum {
/*	Symbolic Name				   Code		AETE Terminology		*/
	class_EnvList				= 'cVEL',

	prefsPR_EnvList				= 'PREL'	/* Environment List			*/
};
