//-- PhotoAppCommandAttachment.h
//-- Copyright © 2000 Electric Fish, Inc.

#pragma once
#include "ECommandAttachment.h"

class PhotoPrintApp;

class PhotoAppCommandAttachment : public ECommandAttachment {
	protected:
		PhotoPrintApp*	mApp;
		
	public:
							PhotoAppCommandAttachment(CommandT inCommand, 
														PhotoPrintApp* inApp);
														
		virtual 			~PhotoAppCommandAttachment() {};
		
		virtual	void		ExecuteCommandNumber	(CommandT			inCommand,
													 void*				inCommandData) = 0;
													 
													 
		}; // end class PhotoDocCommandAttachment
	