// PhotoPrintCommand.cp
// Copyright © 2000 Electric Fish, Inc

#include "PhotoPrintCommand.h"
#include "PhotoPrintDoc.h"

PhotoPrintCommand::PhotoPrintCommand(CommandT inCommand, PhotoPrintDoc* inDoc)
	: PhotoDocCommandAttachment (inCommand, inDoc)
{
}//end ct

PhotoPrintCommand::~PhotoPrintCommand(void)
{
}//end dt


void		
PhotoPrintCommand::FindCommandStatus		(SCommandStatus*	ioStatus)
{
	*(ioStatus->enabled) = !(mDoc->GetProperties().GetEmpty());
}//end FindCommandStatus



void		
PhotoPrintCommand::ExecuteCommandNumber	(CommandT			inCommand,
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

