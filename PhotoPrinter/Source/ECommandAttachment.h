/*
	File:		ECommandAttachment.h

	Contains:	Definition of ???

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		14 Jun 2000		drd		Added ExecuteCommand (to avoid unused argument); HandlesCommand bool
*/

#pragma once
#include <LAttachment.h>

class ECommandAttachment: public LAttachment {		
		CommandT			mCommand;
	
	protected:
		
		virtual	void		ExecuteCommand			(void*				inCommandData);
		virtual	void		ExecuteCommandNumber	(CommandT			inCommand,
													 void*				inCommandData);
		virtual	void		FindCommandStatus		(SCommandStatus*	inStatus);

		// LAttachment		
		virtual void		ExecuteSelf				(MessageT			inMessage,
													 void				*ioParam);
												 
	public:
	
							ECommandAttachment		(CommandT			inCommand);
		virtual				~ECommandAttachment		(void);
		
		CommandT			GetCommand				(void) const;

		virtual	bool		HandlesCommand			(CommandT			inCommand) const;
	}; //end class ECommandAttachment
	