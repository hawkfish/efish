// OpenCommand.h
// Copyright © 2000 Electric Fish, Inc

#pragma once
#include "PhotoAppCommandAttachment.h"

class OpenCommand : public PhotoAppCommandAttachment {
	public:
				OpenCommand(CommandT inCommand, PhotoPrintApp* inApp);
		virtual ~OpenCommand(void) {};
		
		virtual	void		FindCommandStatus		(SCommandStatus*	inStatus);
		virtual	void		ExecuteCommandNumber	(CommandT			inCommand,
													 void*				inCommandData);
		
	};//end class OpenCommand