/*
	File:		PhotoPrintResources.h

	Contains:	Resource constants.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		16 aug 2000		dml		add curs_Rot
		15 Aug 2000		drd		Added si_RemoveCrop, si_Rotate, si_RemoveRotation
		14 Aug 2000		drd		Added si_Crop, si_CropZoom
		11 Aug 2000		drd		Moved curs_ here
		04 Aug 2000		drd		Created
*/

#pragma once

enum {
	curs_Hand = 1000,
	curs_Crop = 1001,
	curs_Rot = 1002,

	str_Redo = 259,							// 'STR#'
	str_Undo = 260,							// 'STR#'
		si_PasteImage = 1,
		si_PastePicture,
		si_PasteText,
		si_DeleteImage,
		si_CutImage,
		si_DropImage,
		si_DuplicateImage,
		si_DropPicture,
		si_DropText,
		si_Nudge,
		si_Move,
		si_Copy,
		si_Resize,
		si_MoveToBack,
		si_MoveToFront,
		si_MoveBackward,
		si_MoveForward,
		si_Crop,
		si_CropZoom,
		si_RemoveCrop,
		si_Rotate,
		si_RemoveRotation,

	rsrc_Fnord		// Allows us to put commas everywhere
};
