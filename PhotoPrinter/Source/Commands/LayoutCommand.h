/*
	File:		LayoutCommand.h

	Contains:	Interface of the Layout menu command

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		23 Aug 2000		drd		Created
*/

#pragma once

#include "PhotoDocCommandAttachment.h"

class LayoutCommand : public PhotoDocCommandAttachment {
public:
			LayoutCommand(CommandT inCommand, PhotoPrintDoc* inDoc);
	virtual ~LayoutCommand(void);
	
	virtual	void		FindCommandStatus		(SCommandStatus*	inStatus);
	virtual	void		ExecuteCommand			(void*				inCommandData);
	
};
