/*
	File:		PaletteButton.h

	Contains:	Definition of a button in the PhotoPrinter floating palette.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		21 Jun 2000		drd		Created
*/

#pragma once

#include "CDragAndDrop.h"
#include <LCmdBevelButton.h>

class	Layout;

class PaletteButton : public LCmdBevelButton, CDragAndDrop
{
public:
	enum { class_ID = FOUR_CHAR_CODE('Pbtn') };

						PaletteButton(LStream*	inStream);
						~PaletteButton();

		// LDropArea
		virtual Boolean	ItemIsAcceptable(DragReference inDragRef, ItemReference inItemRef);

		// CDragAndDrop
		virtual	void	ReceiveDragItem(DragReference inDragRef, ItemReference inItemRef,
										Size inDataSize, Boolean inCopyData, 
										Boolean inFromFinder, Rect& inItemBounds);

protected:
		Layout*						mLayout;
};
