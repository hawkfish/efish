// PageSetupCommand.cp
// Copyright © 2000 Electric Fish, Inc

#include "PageSetupCommand.h"
#include "PhotoPrintDoc.h"

PageSetupCommand::PageSetupCommand(CommandT inCommand, PhotoPrintDoc* inDoc)
	: PhotoDocCommandAttachment (inCommand, inDoc)
{
}//end ct

PageSetupCommand::~PageSetupCommand(void)
{
}//end dt


void		
PageSetupCommand::FindCommandStatus		(SCommandStatus*	ioStatus)
{
	*(ioStatus->enabled) = !(mDoc->GetProperties().GetEmpty());
}//end FindCommandStatus



void		
PageSetupCommand::ExecuteCommandNumber	(CommandT			inCommand,
									 void*				/*inCommandData*/)
{
	switch (inCommand) {
		case cmd_Print:
			{
			mDoc->DoPageSetup();
			}
			break;
		}//end switch
}//end ExecuteCommandNumber										 

