/*
	File:		PaletteButton.h

	Contains:	Definition of a button in the PhotoPrinter floating palette.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		27 Sep 2000		rmgw	Change ItemIsAcceptable to DragIsAcceptable.
		27 Sep 2000		rmgw	Override DragIsAcceptable not item is acceptable
		21 Jun 2000		drd		Override EnterDropArea, LeaveDropArea, SuperDeactivate
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

						PaletteButton	(LStream*	inStream);
		virtual			~PaletteButton	(void);

		// LPane
		virtual	void	SuperDeactivate	(void);

		// LDropArea
		virtual	void	DoDragReceive	(DragReference 		inDragRef);
		virtual void	EnterDropArea	(DragReference		inDragRef,
										 Boolean			inDragHasLeftSender);
		virtual Boolean	DragIsAcceptable(DragReference 		inDragRef);
		virtual void	LeaveDropArea	(DragReference		inDragRef);

protected:
		Layout*						mLayout;
};
