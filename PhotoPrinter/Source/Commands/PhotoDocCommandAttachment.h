//-- PhotoDocCommandAttachment.h
//-- Copyright © 2000 Electric Fish, Inc.

#pragma once
#include "ECommandAttachment.h"

class PhotoPrintDoc;

class PhotoDocCommandAttachment : public ECommandAttachment {
	protected:
		PhotoPrintDoc*	mDoc;
		
	public:
							PhotoDocCommandAttachment(CommandT inCommand, 
														PhotoPrintDoc* inDoc);
														
		virtual 			~PhotoDocCommandAttachment();
		
		virtual	void		ExecuteCommandNumber	(CommandT			inCommand,
													 void*				inCommandData);
													 
													 
		}; // end class PhotoDocCommandAttachment
	