// PrintCommand.cp
// Copyright © 2000 Electric Fish, Inc

#include "PrintCommand.h"
#include "PhotoPrintDoc.h"

PrintCommand::PrintCommand(CommandT inCommand, PhotoPrintDoc* inDoc)
	: PhotoDocCommandAttachment (inCommand, inDoc)
{
}//end ct

PrintCommand::~PrintCommand(void)
{
}//end dt


void		
PrintCommand::FindCommandStatus		(SCommandStatus*	ioStatus)
{
	*(ioStatus->enabled) = !(mDoc->GetProperties().GetEmpty());
}//end FindCommandStatus



void		
PrintCommand::ExecuteCommandNumber	(CommandT			inCommand,
									 void*				/*inCommandData*/)
{
	switch (inCommand) {
		case cmd_Print:
			{
			mDoc->DoPrint();
			}
			break;
		}//end switch
}//end ExecuteCommandNumber										 

