//-- PhotoAppCommandAttachment.cp
//-- Copyright © 2000 Electric Fish, Inc.

#include "PhotoAppCommandAttachment.h"
#include "PhotoPrintApp.h"
PhotoAppCommandAttachment::PhotoAppCommandAttachment(CommandT inCommand, 
													PhotoPrintApp* inApp)
		: ECommandAttachment(inCommand)
		, mApp (inApp)
{
	mApp->AddAttachment(this);
	}//end ct
														
		

														