#include "PhotoDocCommandAttachment.h"
#include "PhotoPrintDoc.h"

PhotoDocCommandAttachment::PhotoDocCommandAttachment(CommandT inCommand, PhotoPrintDoc* inDoc) 
	: ECommandAttachment (inCommand)
	, mDoc (inDoc)
{
	mDoc->AddAttachment(this);
}//end ct


PhotoDocCommandAttachment::~PhotoDocCommandAttachment() 
{
}//end dt


void		
PhotoDocCommandAttachment::ExecuteCommandNumber	(CommandT			/*inCommand*/,
												 void*				/*inCommandData*/)
{
}
