/*
	File:		UseProxyCommand.h

	Contains:	allow toggle of proxy usages

	Written by:	Dav Lion
	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		24 Aug 2000		dml		Created
*/

#pragma once

#include "PhotoAppCommandAttachment.h"

class UseProxyCommand : public PhotoAppCommandAttachment {

public:
						UseProxyCommand				(const CommandT		inCommand,
													 PhotoPrintApp*		inApp);
	virtual 			~UseProxyCommand				() {};

protected:
	// ECommandAttachment
	virtual	void		ExecuteCommand				(void*				inCommandData);
	virtual	void		FindCommandStatus			(SCommandStatus*	inStatus);
};

