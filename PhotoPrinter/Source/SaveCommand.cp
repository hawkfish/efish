// SaveCommand.cp
// Copyright © 2000 Electric Fish, Inc

#include "SaveCommand.h"
#include "PhotoPrintDoc.h"

SaveCommand::SaveCommand(CommandT inCommand, PhotoPrintDoc* inDoc)
	:PhotoDocCommandAttachment (inCommand, inDoc)
{
}//end SaveCommand

void		
SaveCommand::FindCommandStatus		(SCommandStatus*	inStatus)
{
	*(inStatus->enabled) = mDoc->GetProperties().GetDirty();
}//end FindCommandStatus


void		
SaveCommand::ExecuteCommandNumber	(CommandT			/*inCommand*/,
									 void*				/*inCommandData*/)
{
	mDoc->DoSave();
}//end ExecuteCommandNumber									 
