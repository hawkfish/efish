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
	switch (GetCommand()) {
		case cmd_Save:
			*(inStatus->enabled) = (mDoc->GetProperties().GetDirty() &&
									mDoc->IsFileSpecified());
			break;
		case cmd_SaveAs:
			*(inStatus->enabled) = true;
			break;
		}//end switch			
}//end FindCommandStatus


void		
SaveCommand::ExecuteCommandNumber	(CommandT			inCommand,
									 void*				/*inCommandData*/)
{
	switch (inCommand) {
		case cmd_Save:
			mDoc->DoSave();
			break;
		case cmd_SaveAs: {
			FSSpec outSpec;
			mDoc->AskSaveAs(outSpec, false);
			}//case
			break;
		}//end switch
}//end ExecuteCommandNumber									 
