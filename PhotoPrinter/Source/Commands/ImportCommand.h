/*
	File:		ImportCommand.h

	Contains:	Interface of the Import menu command

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		23 Aug 2000		drd		Created
*/

#pragma once

#include "PhotoDocCommandAttachment.h"

class ImportCommand : public PhotoDocCommandAttachment {
public:
			ImportCommand(CommandT inCommand, PhotoPrintDoc* inDoc);
	virtual ~ImportCommand(void);
	
	virtual	void		FindCommandStatus		(SCommandStatus*	inStatus);
	virtual	void		ExecuteCommand			(void*				inCommandData);
	
}; //end class ImportCommand
