/*
	File:		PhotoExceptionHandler.cp

	Contains:	Implementation of chain-of-exception handlers

	Written by:	Dav Lion (kibbitzed by richard wesley and david dunham)

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		21 sep 2000		dml		better null strings
		20 Sep 2000		dml		Created
*/

#include "PhotoExceptionHandler.h"

const ResIDT	alrt_TemplateFatal = 1000;
const LStr255	emptyString = "\p";

PhotoExceptionHandler* PhotoExceptionHandler::gCurrent = nil;


/*
* PhotoExceptionHandler::ct
*/
PhotoExceptionHandler::PhotoExceptionHandler(LStr255 inOperation)
	: mPrevious (gCurrent)
	, mOperation (inOperation)
{	
	gCurrent = this;
}//end ct



/*
* PhotoExceptionHandler::dt
*/
PhotoExceptionHandler::~PhotoExceptionHandler() {
	gCurrent = mPrevious;
	}//end




/*
*HandleException
*/
bool
PhotoExceptionHandler::HandleException(LException& e) {

	LStr255 errorString (e.GetErrorCode());
	LStr255 errorDescription;
	
	switch (e.GetErrorCode()) {
		case memFullErr:
		case cTempMemErr:	
		case cNoMemErr:
			errorDescription = "\pinsufficient memory";
			break;
		default:
			errorDescription = "\pan unexpected error";
		}//end switch		

	ReportException(mOperation, errorDescription, errorString, emptyString);

	return true;
	}//end HandleException


/*
* HandleKnownExceptions
*/
bool
PhotoExceptionHandler::HandleKnownExceptions(LException& e)
{
	if (gCurrent != nil)
		return gCurrent->HandleException(e);
	else
	{
		return false;
		}//else die with an unknown exception
		
}//end HandleKnownExceptions




/*
* ReportException
*/
void
PhotoExceptionHandler::ReportException(const LStr255& parm0, const LStr255& parm1, 
										const LStr255& parm2, const LStr255& parm3)
{
	::ParamText(parm0, parm1, parm2, parm3);
	::Alert(alrt_TemplateFatal, nil);
	
}//end ReportException										

