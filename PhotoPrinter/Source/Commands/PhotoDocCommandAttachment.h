/*
	File:		PhotoDocCommandAttachment.h

	Contains:	Definition of a command relating to the document.

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		14 Jun 2000		drd		Removed unnecessary ExecuteCommandNumber
*/

#pragma once

#include "ECommandAttachment.h"

class PhotoPrintDoc;

class PhotoDocCommandAttachment : public ECommandAttachment {
	protected:
		PhotoPrintDoc*	mDoc;					// Document we operate on
		
	public:
							PhotoDocCommandAttachment(const CommandT inCommand, 
														PhotoPrintDoc* inDoc);
														
		virtual 			~PhotoDocCommandAttachment();
}; // end class PhotoDocCommandAttachment
