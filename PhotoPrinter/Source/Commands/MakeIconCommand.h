/*
	File:		MakeIconCommand.h

	Contains:	Definition of the Make Icon command.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		18 Aug 2000		drd		Created
*/

#pragma once

#include "PhotoDocCommandAttachment.h"
#include "MResFile.h"
#include "PhotoPrintItem.h"

class MakeIconCommand : public PhotoDocCommandAttachment
{
public:
						MakeIconCommand			(const CommandT		inCommand,
												 PhotoPrintDoc*		inDoc);
	virtual 			~MakeIconCommand		();

protected:
	// ECommandAttachment
	virtual void		CheckAddPreview			(PhotoItemRef image, MResFile& theFile);
	virtual	void		ExecuteCommand			(void*				inCommandData);
	virtual	void		FindCommandStatus		(SCommandStatus*	inStatus);
};
