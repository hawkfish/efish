/*
	File:		PhotoExceptionHandler.cp

	Contains:	Implementation of chain-of-exception handlers

	Written by:	Dav Lion (kibbitzed by richard wesley and david dunham)

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		21 sep 2000		dml		better string handling.  using chain.  class breakup
		20 Sep 2000		dml		Created
*/

#include "PhotoExceptionHandler.h"

const ResIDT	alrt_TemplateFatal = 1000;
const LStr255	emptyString = "\p";

PhotoExceptionHandler* PhotoExceptionHandler::gCurrent = nil;


/*
* PhotoExceptionHandler::ct
*/
PhotoExceptionHandler::PhotoExceptionHandler(ConstStr255Param inOperation)
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
* HandleKnownExceptions
*/
bool
PhotoExceptionHandler::HandleKnownExceptions(LException& e)
{
	PhotoExceptionHandler* handler (gCurrent);
	ConstStr255Param	operationName (gCurrent->GetOperation());
	bool status (false);
	
	while (handler != nil) {
		status = handler->HandleException(e, operationName);
		if (status)
			break;
		handler = handler->GetUpstreamHandler();
		}//end while still handlers to try

	return status;	
}//end HandleKnownExceptions




#pragma mark -

/*
*DefaultExceptionHandler::HandleException
*/
bool
DefaultExceptionHandler::HandleException(LException& e, const LStr255& operation) {

	LStr255 errorString (e.GetErrorCode());
	LStr255 errorDescription = "\pan unexpected error";

	ReportException(operation, errorDescription, errorString, emptyString);

	return true;
	}//end HandleException
	
/*
* ReportException
*/
void
DefaultExceptionHandler::ReportException(const LStr255& parm0, const LStr255& parm1, 
										const LStr255& parm2, const LStr255& parm3)
{
	::ParamText(parm0, parm1, parm2, parm3);
	::Alert(alrt_TemplateFatal, nil);
	
}//end ReportException										


#pragma mark -

/*
*MemorytExceptionHandler::HandleException
*/
bool
MemoryExceptionHandler::HandleException(LException& e, const LStr255& operation) {
	bool handled (false);
	
	LStr255 errorString (e.GetErrorCode());
	LStr255 errorDescription;
	
	switch (e.GetErrorCode()) {
		case memFullErr:
		case cTempMemErr:	
		case cNoMemErr:
			errorDescription = "\pinsufficient memory";
			ReportException(operation, errorDescription, errorString, emptyString);
			handled = true;
			break;
		}//end switch		

	return handled;
	}//end HandleException	