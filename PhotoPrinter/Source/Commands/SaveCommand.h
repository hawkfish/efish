// SaveCommand.h
// Copyright © 2000 Electric Fish, Inc

#pragma once
#include "PhotoDocCommandAttachment.h"

class SaveCommand : public PhotoDocCommandAttachment {
	public:
				SaveCommand(CommandT inCommand, PhotoPrintDoc* inDoc);
		virtual ~SaveCommand(void) {};
		
		virtual	void		FindCommandStatus		(SCommandStatus*	inStatus);
		virtual	void		ExecuteCommandNumber	(CommandT			inCommand,
													 void*				inCommandData);
		
	};//end class PhotoPrintCommand