/*
	File:		PhotoExceptionHandler.cp

	Contains:	Implementation of chain-of-exception handlers (for PowerPlant LException)

	Written by:	Dav Lion (kibbitzed by richard wesley and david dunham)

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		25 Oct 2000		drd		ReportException uses StDesktopDeactivator (for non-Carbon apps)
		21 Sep 2000		drd		DefaultExceptionHandler uses ÔEstrÕ resources to get description;
								used resources for all strings; use EUtil::IsMemoryError; renamed
								base class
		21 sep 2000		dml		better string handling.  using chain.  class breakup.  add updPixMem
		20 Sep 2000		dml		Created
*/

#include "PhotoExceptionHandler.h"

#include "EUtil.h"

const ResIDT	alrt_TemplateFatal = 333;
const LStr255	emptyString = "\p";

// Globals
ExceptionHandler*	ExceptionHandler::gCurrent = nil;
LStr255				DefaultExceptionHandler::gDefaultOperation(str_ExceptionHandler, si_DefaultOperation);

/*
* ExceptionHandler::ct
*/
ExceptionHandler::ExceptionHandler(ConstStr255Param inOperation)
	: mPrevious (gCurrent)
	, mOperation (inOperation)
{	
	gCurrent = this;
}//end ct


/*
* ExceptionHandler::dt
*/
ExceptionHandler::~ExceptionHandler() {
	gCurrent = mPrevious;
	}//end


/*
* HandleKnownExceptions
*/
bool
ExceptionHandler::HandleKnownExceptions(LException& e)
{
	ExceptionHandler*	handler (gCurrent);
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

	LStr255			errorString (e.GetErrorCode());
	// There may be some danger trying to get a resource since we are trying to report
	// on an exception. However, our resource is preloaded and non-purgeable.
	LStr255			errorDescription(str_ExceptionHandler, si_UnknownError);

	// Look up the error in a resource (if we wanted to be clever, we'd look up 4 character
	// codes too, with a by-name lookup)
	StResource		estrRsrc('Estr', e.GetErrorCode(), Throw_No);
	StringHandle	estr = (StringHandle)(Handle)estrRsrc;
	if (estr != nil) {
		StHandleLocker	lock(estrRsrc);
		errorDescription = *estr;
	}

	this->ReportException(operation, errorDescription, errorString, emptyString);

	return true;
}//end HandleException
	
/*
* ReportException
*/
void
DefaultExceptionHandler::ReportException(const LStr255& parm0, const LStr255& parm1, 
										const LStr255& parm2, const LStr255& parm3)
{
	StDesktopDeactivator	blockForDialog;

	::ParamText(parm0, parm1, parm2, parm3);
	::InitCursor();
	::StopAlert(alrt_TemplateFatal, nil);
}//end ReportException										


#pragma mark -

/*
*MemoryExceptionHandler::HandleException
*/
bool
MemoryExceptionHandler::HandleException(LException& e, const LStr255& operation) {
	bool handled (false);
	
	LStr255 errorString (e.GetErrorCode());
	LStr255 errorDescription;
	
	if (EUtil::IsMemoryError(e.GetErrorCode())) {
			errorDescription.Assign(str_ExceptionHandler, si_MemoryError);
			this->ReportException(operation, errorDescription, errorString, emptyString);
			handled = true;
	}	

	return handled;
}//end HandleException
