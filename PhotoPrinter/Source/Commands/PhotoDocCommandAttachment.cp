/*
	File:		PhotoDocCommandAttachment.cp

	Contains:	Implementation of a command relating to the document.

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		14 Jun 2000		drd		Removed unnecessary ExecuteCommandNumber
*/

#include "PhotoDocCommandAttachment.h"
#include "PhotoPrintDoc.h"

/*
PhotoDocCommandAttachment
*/
PhotoDocCommandAttachment::PhotoDocCommandAttachment(const CommandT inCommand, PhotoPrintDoc* inDoc) 
	: ECommandAttachment (inCommand)
	, mDoc (inDoc)
{
	mDoc->AddAttachment(this);
}//end ct

/*
~PhotoDocCommandAttachment
*/
PhotoDocCommandAttachment::~PhotoDocCommandAttachment() 
{
}//end dt
