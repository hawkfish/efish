/*
	File:		cfrg.r

	Contains:	Description of the Projector CMM dode fragment.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1997-9 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

         <3>     3/30/99    rmgw    Crash recovery merging.
         <2>     3/18/99    rmgw    Fix description.
         <1>     3/17/99    rmgw    first checked in.
*/


#define UseExtendedCFRGTemplate 1

#include "CodeFragmentTypes.r"


resource 'cfrg' (0) {
	{	/* array memberArray: 4 elements */
		extendedEntry {
			kPowerPC,
			kFullLib,
			kNoVersionNum,
			kNoVersionNum,
			kDefaultStackSize,
			kNoAppSubFolder,
			kIsLib,
			kOnDiskFlat,
			kZeroOffset,
			kSegIDZero,
			"CMCVS",
			kFragSOMClassLibrary,
			"AbstractCMPlugin",
			"",
			"",
			"A Contextual Menu Plugin which interfaces to the CVS version control system"
		}
	}
};
