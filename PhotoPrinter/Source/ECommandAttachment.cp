// ECommandAttachment.cp
// Copyright © 2000 Electric Fish, Inc

#include "ECommandAttachment.h"

ECommandAttachment::ECommandAttachment		(CommandT			inCommand)
	: mCommand (inCommand)										 
{
}//end ct

ECommandAttachment::~ECommandAttachment		(void)
{
}//end dt


CommandT			
ECommandAttachment::GetCommand				(void) const
{
	return mCommand;
}//end GetCommand


Boolean		
ECommandAttachment::HandlesCommand			(CommandT			inCommand) const
{
	return (inCommand == GetCommand());
}//end HandlesCommand


void		
ECommandAttachment::FindCommandStatus		(SCommandStatus*	/*inStatus*/)
{

}//end FindCommandStatus


void		
ECommandAttachment::ExecuteCommandNumber	(CommandT			/*inCommand*/,
											 void*				/*inCommandData*/)
{
}//end ExecuteCommandNumber


void		
ECommandAttachment::ExecuteSelf				(MessageT			inMessage,
						 void				*ioParam)
{
	Boolean executeHost (true);

	switch (inMessage) {
		case msg_CommandStatus: {
			SCommandStatus* pStatus = (SCommandStatus*)ioParam;
			if (HandlesCommand(pStatus->command)) {
				FindCommandStatus(pStatus);
				executeHost = false;
				}//endif we handle this command
			}//end case
			break;
		default:
			if (HandlesCommand(inMessage)) {
				ExecuteCommandNumber(inMessage, ioParam);
				executeHost = false;
				}//endif
			break;
		}//end switch
			
		SetExecuteHost(executeHost);
}//end ExecuteSelf

