/*
	File:		PhotoPrintEvents.h

	Contains:	Apple Event constants.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		17 Apr 2001		drd		Avoid duplicate ae_Preferences
		08 Nov 2000		drd		ae_Preferences
		23 Aug 2000		drd		Created
*/

#pragma once

enum {
	kAEPhotoPrintSuite = 'eFSH',
		kAEImport = 'impo',

	ae_Import = 4001,
	
	// ??? this is now defined in UAppleEventsMgr.h, and we should eventually transition
#if ( __PowerPlant__ < 0x02114004 )
	ae_Preferences = 4002		// For Aqua
#else
	ae_PPE_END					// Totally bogus
#endif
};
