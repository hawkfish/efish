/*
	File:		PageSetupCommand.cp

	Contains:	Implementation of the PageSetup interface

	Written by:	Dav Lion

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		27 june 2000		dml		handle cmd_PageSetup
*/


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
		case cmd_PageSetup:
			{
			mDoc->DoPageSetup();
			}
			break;
		}//end switch
}//end ExecuteCommandNumber										 

