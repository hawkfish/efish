/*
	File:		ECommandAttachment.cp

	Contains:	Implementation of a PowerPlant command that's typically attached to a doc or app object

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.
				Copyright ©2001 by A Sharp, LLC. All rights reserved.

	Change History (most recent first):

		26 Jul 2001		rmgw	Move GetName logic to ExceptionHandler::GetCommandName.
		20 May 2001		drd		ExcecuteSelf clears executeHost if there's an exception; const
		21 sep 2000		dml		more exception handling to ExecuteSelf.  use GetName
		21 Sep 2000		drd		Added GetName
		20 sep 2000		dml		add (default) error handling.  see note in ExecuteCommandNumber
		14 Jun 2000		drd		Added ExecuteCommand (to avoid unused argument)
*/
#include "ECommandAttachment.h"
#include "PhotoExceptionHandler.h"

ECommandAttachment::ECommandAttachment		(const CommandT		inCommand)
	: mCommand (inCommand)										 
{
}//end ct

ECommandAttachment::~ECommandAttachment		(void)
{
}//end dt

/*
ExecuteCommand
	Subclasses should override if they handle just one command ID
*/
void		
ECommandAttachment::ExecuteCommand			(void*				/*inCommandData*/)
{
} // ExecuteCommand

/*
ExecuteCommandNumber
	Subclasses should override if they wish to handle more than one command ID
	Note that exception handling is done through the PhotoExceptionHandler class.
	Subclasses wishing more than default handling should install their own handler on the stack
	during execution.
*/
void		
ECommandAttachment::ExecuteCommandNumber	(const CommandT		/*inCommand*/,
											 void*				inCommandData)
{
	// The most common case is a command that handles one command ID so call that method
	this->ExecuteCommand(inCommandData);
}//end ExecuteCommandNumber

/*
ExecuteSelf {OVERRIDE}
*/
void		
ECommandAttachment::ExecuteSelf				(MessageT			inMessage,
						 void				*ioParam)
{
	// Setup for reporting on any exceptions that may occur
	MemoryExceptionHandler	commandHandler (this->GetCommand());

	try {
		Boolean				executeHost = true;
		switch (inMessage) {
			case msg_CommandStatus:
			{
				SCommandStatus* pStatus = (SCommandStatus*)ioParam;
				if (this->HandlesCommand(pStatus->command)) {
					this->FindCommandStatus(pStatus);
					executeHost = false;
				}//endif we handle this command
				break;
			} //end case

			default:
				if (this->HandlesCommand(inMessage)) {
					this->ExecuteCommandNumber(inMessage, ioParam);
					executeHost = false;
				}//endif
				break;
		}//end switch

		this->SetExecuteHost(executeHost);
	}//end try
	
	catch (LException e) {
		// Be sure nobody else handles the command -- important for cmd_New, cmd_Open
		this->SetExecuteHost(false);

		if (!ExceptionHandler::HandleKnownExceptions(e))
			throw;
	}//end catch
} // ExecuteSelf

/*
FindCommandStatus
	Sets its argument to TRUE if the command should be enabled.
	Subclasses must override to enable/disable commands.
*/
void		
ECommandAttachment::FindCommandStatus		(SCommandStatus*	/*ioStatus*/)
{
}//end FindCommandStatus

/*
GetCommand
*/
CommandT			
ECommandAttachment::GetCommand				(void) const
{
	return mCommand;
}//end GetCommand

/*
GetName
	Get a human-readable version of the command name (useful in error reporting)
*/
StringPtr
ECommandAttachment::GetName(Str255 outDescriptor) const
{
	return ExceptionHandler::GetCommandName (this->GetCommand(), outDescriptor);
	
} // GetName

/*
HandlesCommand
*/
bool		
ECommandAttachment::HandlesCommand(const CommandT			inCommand) const
{
	return (inCommand == this->GetCommand());
}//end HandlesCommand
