/*
	File:		DrawMaxBoundsCommand.h

	Contains:	allow toggle of proxy usages

	Written by:	Dav Lion
	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		02 Mar 2001		dml		Created
*/

#pragma once

#include "PhotoAppCommandAttachment.h"

class DrawMaxBoundsCommand : public PhotoAppCommandAttachment {

public:
						DrawMaxBoundsCommand				(const CommandT		inCommand,
													 PhotoPrintApp*		inApp);
	virtual 			~DrawMaxBoundsCommand				() {};

protected:
	// ECommandAttachment
	virtual	void		ExecuteCommand				(void*				inCommandData);
	virtual	void		FindCommandStatus			(SCommandStatus*	inStatus);
};

