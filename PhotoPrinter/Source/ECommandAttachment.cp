/*
	File:		ECommandAttachment.cp

	Contains:	Implementation of ???

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		14 Jun 2000		drd		Added ExecuteCommand (to avoid unused argument)
*/
#include "ECommandAttachment.h"

ECommandAttachment::ECommandAttachment		(CommandT			inCommand)
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
*/
void		
ECommandAttachment::ExecuteCommandNumber	(CommandT			/*inCommand*/,
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
	Boolean executeHost (true);

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
}//end ExecuteSelf

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
HandlesCommand
*/
bool		
ECommandAttachment::HandlesCommand			(CommandT			inCommand) const
{
	return (inCommand == this->GetCommand());
}//end HandlesCommand
