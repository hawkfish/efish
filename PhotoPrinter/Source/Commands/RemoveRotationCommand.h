/*
	File:		RemoveRotationCommand.h

	Contains:	Definition of the Remove Cropping command.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		21 Aug 2000		dml		Created
*/

#pragma once

#include "PhotoDocCommandAttachment.h"

class RemoveRotationCommand : public PhotoDocCommandAttachment
{
public:
						RemoveRotationCommand		(const CommandT		inCommand,
												 PhotoPrintDoc*		inDoc);
	virtual 			~RemoveRotationCommand		();

protected:
	// ECommandAttachment
	virtual	void		ExecuteCommand			(void*				inCommandData);
	virtual	void		FindCommandStatus		(SCommandStatus*	inStatus);
};
