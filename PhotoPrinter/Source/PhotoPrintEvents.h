/*
	File:		PhotoPrintEvents.h

	Contains:	Apple Event constants.

	Written by:	David Dunham

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		12 Jul 2001		rmgw	Convert the import event to make new import.
		23 Apr 2001		drd		Renumbered ae_Preferences to be same as PowerPlant 2.1.1
		17 Apr 2001		drd		Avoid duplicate ae_Preferences
		08 Nov 2000		drd		ae_Preferences
		23 Aug 2000		drd		Created
*/

#pragma once

enum {
	// ??? this is now defined in UAppleEventsMgr.h, and we should eventually transition
#if ( __PowerPlant__ < 0x02114004 )
	ae_Preferences = 1006		// For Aqua (same as UAppleEventsMgr.h)
#else
	ae_PPE_END					// Totally bogus
#endif
};
