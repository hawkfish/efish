/*
	File:		PhotoAppCommandAttachment.h

	Contains:	Definition of a command attachment which relates to the application.

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		23 Jun 2000		drd		Removed ExecuteCommandNumber since it's already defined by superclass
*/

#pragma once

#include "ECommandAttachment.h"

class PhotoPrintApp;

class PhotoAppCommandAttachment : public ECommandAttachment
{
	protected:
		PhotoPrintApp*	mApp;
		
	public:
							PhotoAppCommandAttachment(CommandT inCommand, 
														PhotoPrintApp* inApp);
														
		virtual 			~PhotoAppCommandAttachment()		{}
		
}; // end class PhotoDocCommandAttachment
