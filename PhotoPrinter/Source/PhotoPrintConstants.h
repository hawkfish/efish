/*
	File:		PhotoPrintConstants.h

	Contains:	Miscellaneous constants.

	Written by:	David Dunham

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		03 Jul 2001		drd		kMinFontSize, kMaxFontSize
		21 May 2001		drd		Moved MENU_FontCopy to PhotoPrintResources.h
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
	kDragFlavor = 'eIMG',

	// Fonts must be at least 3 points, and no more than 127 points
	kMinFontSize = 3,
	kMaxFontSize = 127
};
