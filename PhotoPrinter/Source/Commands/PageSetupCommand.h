// PageSetupCommand.h
// Copyright © 2000 Electric Fish, Inc

#include "PhotoDocCommandAttachment.h"

class PageSetupCommand : public PhotoDocCommandAttachment {
	public:
				PageSetupCommand(CommandT inCommand, PhotoPrintDoc* inDoc);
		virtual ~PageSetupCommand(void);
		
		virtual	void		FindCommandStatus		(SCommandStatus*	inStatus);
		virtual	void		ExecuteCommandNumber	(CommandT			inCommand,
													 void*				inCommandData);
		
	};//end class PrintCommand