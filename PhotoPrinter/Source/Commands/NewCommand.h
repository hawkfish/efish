/*
	File:		NewCommand.h

	Contains:	Definition of the New command (used by the buttons).

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		23 Jun 2000		drd		Created
*/

#include "PhotoAppCommandAttachment.h"

class NewCommand : public PhotoAppCommandAttachment
{
public:
						NewCommand(const CommandT inCommand, 
									PhotoPrintApp* inApp);
						~NewCommand();

	// ECommandAttachment
	virtual	void		ExecuteCommandNumber	(CommandT			inCommand,
												 void*				inCommandData);
	virtual	void		FindCommandStatus		(SCommandStatus*	inStatus);
};
