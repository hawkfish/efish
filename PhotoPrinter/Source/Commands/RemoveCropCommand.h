/*
	File:		RemoveCropCommand.h

	Contains:	Definition of the Remove Cropping command.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		15 Aug 2000		drd		Created
*/

#pragma once

#include "PhotoDocCommandAttachment.h"

class RemoveCropCommand : public PhotoDocCommandAttachment
{
public:
						RemoveCropCommand		(const CommandT		inCommand,
												 PhotoPrintDoc*		inDoc);
	virtual 			~RemoveCropCommand		();

protected:
	// ECommandAttachment
	virtual	void		ExecuteCommand			(void*				inCommandData);
	virtual	void		FindCommandStatus		(SCommandStatus*	inStatus);
};
