/*
	File:		RevealCommand.h

	Contains:	Definition of the Reveal in Finder command.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		14 Aug 2000		drd		Created
*/

#pragma once

#include "PhotoDocCommandAttachment.h"

class RevealCommand : public PhotoDocCommandAttachment
{
public:
						RevealCommand			(const CommandT		inCommand,
												 PhotoPrintDoc*		inDoc);
	virtual 			~RevealCommand			();

protected:
	// ECommandAttachment
	virtual	void		ExecuteCommand			(void*				inCommandData);
	virtual	void		FindCommandStatus		(SCommandStatus*	inStatus);
};
