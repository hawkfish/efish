/*
	File:		PasteCommand.h

	Contains:	Definition of the Paste command (in Edit menu).

	Written by:	David Dunham

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		07 Aug 2001		rmgw	Replace PasteAction with ModelAction.  Bug #293.
		22 May 2001		drd		69 Created
*/

#pragma once

#include "PhotoDocCommandAttachment.h"

#include "PhotoPrintItem.h"

class PasteCommand : public PhotoDocCommandAttachment
{
public:
						PasteCommand(const CommandT inCommand, 
									PhotoPrintDoc* inDoc);
						~PasteCommand();

	// ECommandAttachment
	virtual	void		ExecuteCommand			(void*				inCommandData);
	virtual	void		FindCommandStatus		(SCommandStatus*	inStatus);

	// XMLItemParser
	virtual	void		OnItemRead				(PhotoItemRef 		inItem);
};
