/*
	File:		SelectAllCommand.h

	Contains:	Definition of the Select All command (in Edit menu).

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		11 Aug 2000		drd		Created
*/

#include "PhotoDocCommandAttachment.h"

class SelectAllCommand : public PhotoDocCommandAttachment
{
public:
						SelectAllCommand(const CommandT inCommand, 
										PhotoPrintDoc* inDoc);
						~SelectAllCommand();

	// ECommandAttachment
	virtual	void		ExecuteCommand			(void*				inCommandData);
	virtual	void		FindCommandStatus		(SCommandStatus*	inStatus);
};
