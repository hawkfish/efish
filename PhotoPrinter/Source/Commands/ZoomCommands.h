/*
	File:		ZoomCommands.h

	Contains:	Definition of the ZoomIn and ZoomOut command.

	Written by:	Dav Lion
	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		24 Aug 2000		dml		Created
*/

#pragma once

#include "PhotoDocCommandAttachment.h"

class ZoomInCommand : public PhotoDocCommandAttachment
{
public:
						ZoomInCommand		(const CommandT		inCommand,
											 PhotoPrintDoc*		inDoc);
	virtual 			~ZoomInCommand		();

protected:
	// ECommandAttachment
	virtual	void		ExecuteCommand			(void*				inCommandData);
	virtual	void		FindCommandStatus		(SCommandStatus*	inStatus);
};




class ZoomOutCommand : public PhotoDocCommandAttachment
{
public:
						ZoomOutCommand		(const CommandT		inCommand,
											 PhotoPrintDoc*		inDoc);
	virtual 			~ZoomOutCommand		();

protected:
	// ECommandAttachment
	virtual	void		ExecuteCommand			(void*				inCommandData);
	virtual	void		FindCommandStatus		(SCommandStatus*	inStatus);
};
