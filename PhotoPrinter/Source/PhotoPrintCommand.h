// PhotoPrintCommand.h
// Copyright © 2000 Electric Fish, Inc

#include "PhotoDocCommandAttachment.h"

class PhotoPrintCommand : public PhotoDocCommandAttachment {
	public:
				PhotoPrintCommand(CommandT inCommand, PhotoPrintDoc* inDoc);
		virtual ~PhotoPrintCommand(void);
		
		virtual	void		FindCommandStatus		(SCommandStatus*	inStatus);
		virtual	void		ExecuteCommandNumber	(CommandT			inCommand,
													 void*				inCommandData);
		
	};//end class PhotoPrintCommand