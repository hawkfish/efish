/*
	File:		PasteCommand.h

	Contains:	Definition of the Paste command (in Edit menu).

	Written by:	David Dunham

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		22 May 2001		drd		69 Created
*/

#include "PhotoDocCommandAttachment.h"

class PasteCommand : public PhotoDocCommandAttachment
{
public:
						PasteCommand(const CommandT inCommand, 
									PhotoPrintDoc* inDoc);
						~PasteCommand();

	// ECommandAttachment
	virtual	void		ExecuteCommand			(void*				inCommandData);
	virtual	void		FindCommandStatus		(SCommandStatus*	inStatus);
};
