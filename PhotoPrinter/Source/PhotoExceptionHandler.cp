/*
	File:		PhotoExceptionHandler.cp

	Contains:	Implementation of chain-of-exception handlers

	Written by:	Dav Lion (kibbitzed by richard wesley and david dunham)

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		20 Sep 2000		dml		Created
*/

#include "PhotoExceptionHandler.h"

const ResIDT	alrt_TemplateFatal = 1000;

PhotoExceptionHandler* PhotoExceptionHandler::gCurrent = nil;


/*
* PhotoExceptionHandler::ct
*/
PhotoExceptionHandler::PhotoExceptionHandler(MPString inOperation)
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

	MPString errorString (e.GetErrorCode());
	MPString errorDescription;
	
	switch (e.GetErrorCode()) {
		case memFullErr:
		case cTempMemErr:		
			errorDescription = "\pinsufficient memory";
			break;
		default:
			errorDescription = "\an unexpected error";
		}//end switch		

	ReportException(mOperation, errorDescription, errorString, nil);

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
PhotoExceptionHandler::ReportException(const MPString& parm0, const MPString& parm1, 
										const MPString& parm2, const MPString& parm3)
{
	::ParamText(parm0, parm1, parm2, parm3);
	::Alert(alrt_TemplateFatal, nil);
	
}//end ReportException										

