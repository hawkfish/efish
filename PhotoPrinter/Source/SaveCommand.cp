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
			*(inStatus->enabled) = ((mDoc->GetProperties().GetDirty() &&
									mDoc->IsFileSpecified()) ||
									(!mDoc->IsFileSpecified()));
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
	FSSpec outSpec;

	switch (inCommand) {
		case cmd_Save: {
			if (mDoc->IsFileSpecified())
				mDoc->DoSave();
			else
				mDoc->AskSaveAs(outSpec,false);
			}//case
			break;
		case cmd_SaveAs: {
			mDoc->AskSaveAs(outSpec, false);
			}//case
			break;
		}//end switch
}//end ExecuteCommandNumber									 
