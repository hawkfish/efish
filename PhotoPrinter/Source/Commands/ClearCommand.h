/*
	File:		ClearCommand.h

	Contains:	Definition of the Clear command (in Edit menu).

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		04 Aug 2000		drd		Created
*/

#include "PhotoDocCommandAttachment.h"

class ClearCommand : public PhotoDocCommandAttachment
{
public:
						ClearCommand(const CommandT inCommand, 
									PhotoPrintDoc* inDoc);
						~ClearCommand();

	// ECommandAttachment
	virtual	void		ExecuteCommand			(void*				inCommandData);
	virtual	void		FindCommandStatus		(SCommandStatus*	inStatus);
};
