// PrintCommand.h
// Copyright © 2000 Electric Fish, Inc

#include "PhotoDocCommandAttachment.h"

class PrintCommand : public PhotoDocCommandAttachment {
	public:
				PrintCommand(CommandT inCommand, PhotoPrintDoc* inDoc);
		virtual ~PrintCommand(void);
		
		virtual	void		FindCommandStatus		(SCommandStatus*	inStatus);
		virtual	void		ExecuteCommandNumber	(CommandT			inCommand,
													 void*				inCommandData);
		
	};//end class PrintCommand