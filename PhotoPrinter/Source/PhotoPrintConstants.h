/*
	File:		PhotoPrintConstants.h

	Contains:	Miscellaneous constants.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		14 mar 2001		dml		add MENU_FontCopy
		29 Aug 2000		drd		kDragFlavor
		04 Aug 2000		drd		Created
*/

#pragma once

// PowerPlant
enum {
	// argument for LAction
	kNotAlreadyDone = false,
	kAlreadyDone = true
};

enum {
	// Flavor for dragging; also clipboard data type
	kDragFlavor = 'eIMG'
};

enum {
	MENU_FontCopy = 253
};