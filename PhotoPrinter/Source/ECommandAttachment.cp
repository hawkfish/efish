/*
	File:		ECommandAttachment.cp

	Contains:	Implementation of ???

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):
		
		21 Sep 2000		drd		Added GetName
		20 sep 2000		dml		add (default) error handling.  see note in ExecuteCommandNumber
		14 Jun 2000		drd		Added ExecuteCommand (to avoid unused argument)
*/
#include "ECommandAttachment.h"
#include "PhotoExceptionHandler.h"

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
	Note that exception handling is done through the PhotoExceptionHandler class.
	Subclasses wishing more than default handling should install their own handler on the stack
	during execution.
*/
void		
ECommandAttachment::ExecuteCommandNumber	(CommandT			/*inCommand*/,
											 void*				inCommandData)
{
	try {
		// The most common case is a command that handles one command ID so call that method
		this->ExecuteCommand(inCommandData);
		}//end try

	catch (LException e) {
		if (!PhotoExceptionHandler::HandleKnownExceptions(e))
			throw;
		}//end catch
	
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
GetName
	Get a human-readable version of the command name (useful in error reporting)
*/
StringPtr
ECommandAttachment::GetName(Str255 outDescriptor) const
{
	// First try to get it from the menu
	ResIDT		theID;
	MenuHandle	theMacMenu;
	SInt16		theItem;
	LMenuBar::GetCurrentMenuBar()->FindMenuItem(this->GetCommand(), theID, theMacMenu, theItem);
	if (theItem != 0) {
		LMenu	*theMenu = LMenuBar::GetCurrentMenuBar()->FetchMenu(theID);
		::GetMenuItemText(theMacMenu, theItem, outDescriptor);
		// If the menu ends with an ellipsis, get rid of it
		if (outDescriptor[outDescriptor[0]] == 'É')
			outDescriptor[0]--;
	} else {
		// Not found in a menu, so come up with something (cryptic though it may be). If the
		// command seems to be numeric, show it as a number, otherwise as its 4-letter code.
		LStr255		fakeName;
		if (this->GetCommand() < '    ') {
			fakeName = (SInt32)this->GetCommand();
		} else {
			fakeName = (FourCharCode)this->GetCommand();
			fakeName = (UInt8)'Ô' + fakeName;
			fakeName += (UInt8)'Õ';
		}
		::BlockMoveData(fakeName, outDescriptor, fakeName.Length() + 1);
	}
	return outDescriptor;
} // GetName

/*
HandlesCommand
*/
bool		
ECommandAttachment::HandlesCommand			(CommandT			inCommand) const
{
	return (inCommand == this->GetCommand());
}//end HandlesCommand


