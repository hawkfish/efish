// ECommandAttachment.h
// Copyright © 2000 Electric Fish, Inc

#pragma once
#include <LAttachment.h>

class ECommandAttachment: public LAttachment {		
		CommandT			mCommand;
	
	protected:
		
		virtual	void		FindCommandStatus		(SCommandStatus*	inStatus);
		virtual	void		ExecuteCommandNumber	(CommandT			inCommand,
													 void*				inCommandData);
		
		virtual void		ExecuteSelf				(MessageT			inMessage,
													 void				*ioParam);
												 
	public:
	
							ECommandAttachment		(CommandT			inCommand);
		virtual				~ECommandAttachment		(void);
		
		CommandT			GetCommand				(void) const;

		virtual	Boolean		HandlesCommand			(CommandT			inCommand) const;
	};//end class ECommandAttachment
	