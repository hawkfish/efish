/*
	File:		PhotoExceptionHandler.cp

	Contains:	Implementation of chain-of-exception handlers (for PowerPlant LException)

	Written by:	Dav Lion (kibbitzed by richard wesley and david dunham)

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.
				Copyright ©2001 by A Sharp, LLC.

	Change History (most recent first):

		16 Jul 2001		rmgw	Switch to using EUserMessageServer.
		13 Jul 2001		rmgw	Add async parameter.
		27 Jun 2001		drd		93 Separated out GetErrorAndDescription
		20 May 2001		drd		HandleException now checks gMaxNumericError
		14 May 2001		drd		IsMemoryError is now a MemoryExceptionHandler function
		25 Oct 2000		drd		ReportException uses StDesktopDeactivator (for non-Carbon apps)
		21 Sep 2000		drd		DefaultExceptionHandler uses ÔEstrÕ resources to get description;
								used resources for all strings; use EUtil::IsMemoryError; renamed
								base class
		21 sep 2000		dml		better string handling.  using chain.  class breakup.  add updPixMem
		20 Sep 2000		dml		Created

	Discussion:
		ExceptionHandler is used to report errors to the user, in a meaningful fashion.

		ExceptionHandlers are typically created on the stack. Because they are chained,
		you typically make one for the application, and then make one specific to an
		operation you want to report errors for (such as an ECommandAttachment). Most of
		the time you'll want to use MemoryExceptionHandler, because it describes a wide
		range of memory-related errors the same way.

		You will need to pass exceptions to the ExceptionHandler chain by calling
		HandleKnownExceptions.

		Here is an example:

			MemoryExceptionHandler	commandHandler(commandName);

			try {
				...
			} catch (LException e) {
				if (!ExceptionHandler::HandleKnownExceptions(e, async))
					throw;
			}
			
		The async parameter specifies whether the error must be deferred.  The
		default value is false, meaining that the error can be reported at once.
*/

#include "PhotoExceptionHandler.h"

#include "EUserMessageServer.h"

const ResIDT	alrt_TemplateFatal = 333;
const LStr255	emptyString = "\p";

// Globals
ExceptionHandler*	ExceptionHandler::gCurrent = nil;
SInt32				ExceptionHandler::gMaxNumericError = 1000;	// Errors over this are treated as 4-character strings
LStr255				DefaultExceptionHandler::gDefaultOperation(str_ExceptionHandler, si_DefaultOperation);

/*
GetName
	Get a human-readable version of a command name
*/
StringPtr
ExceptionHandler::GetCommandName(

	CommandT	inCommand,
	Str255 		outDescriptor)
{
	// First try to get it from the menu
	ResIDT		theID;
	MenuHandle	theMacMenu;
	SInt16		theItem;
	LMenuBar::GetCurrentMenuBar()->FindMenuItem(inCommand, theID, theMacMenu, theItem);
	if (theItem != 0) {
		LMenu	*theMenu = LMenuBar::GetCurrentMenuBar()->FetchMenu(theID);
		::GetMenuItemText(theMacMenu, theItem, outDescriptor);
		// If the menu ends with an ellipsis, get rid of it
		if (outDescriptor[outDescriptor[0]] == 'É')
			outDescriptor[0]--;
	} else {
		// Not found in a menu, so come up with something (cryptic though it may be). If the
		// command seems to be numeric, show it as a number, otherwise as its 4-letter code.
		LStr255		fakeName;
		if (inCommand < '    ') {
			fakeName = (SInt32)inCommand;
		} else {
			fakeName = (FourCharCode)inCommand;
			fakeName = (UInt8)'Ô' + fakeName;
			fakeName += (UInt8)'Õ';
		}
		::BlockMoveData(fakeName, outDescriptor, fakeName.Length() + 1);
	}
	return outDescriptor;
} // GetName

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
* ExceptionHandler::ct
*/
ExceptionHandler::ExceptionHandler(MessageT	inCommand)
	: mPrevious (gCurrent)
{	
	gCurrent = this;
	
	GetCommandName (inCommand, mOperation);
	
}//end ct


/*
* ExceptionHandler::dt
*/
ExceptionHandler::~ExceptionHandler() {
	gCurrent = mPrevious;
	}//end

/*
GetErrorAndDescription [static]
	
*/
void	ExceptionHandler::GetErrorAndDescription(
	const LException&	inE,
	LStr255&			outCode,
	LStr255&			outDescription)
{
	if (inE.GetErrorCode() <= gMaxNumericError) {
		outCode = (SInt32) inE.GetErrorCode();
	} else {
		outCode = (FourCharCode) inE.GetErrorCode();
		outCode = (UInt8) 'Ô' + outCode;
		outCode += (UInt8) 'Õ';
	}

	// There may be some danger trying to get a resource since we are trying to report
	// on an exception. However, our resource is preloaded and non-purgeable.
	outDescription.Assign(str_ExceptionHandler, si_UnknownError);

	// Look up the error in a resource (if we wanted to be clever, we'd look up 4 character
	// codes too, with a by-name lookup)
	StResource		estrRsrc('Estr', inE.GetErrorCode(), Throw_No);
	if (estrRsrc == nil) {
		// We didn't find an error message corresponding to that number. Look for a
		// resource whose name is the error code as a 4-letter code.
		LStr255		secondName((FourCharCode) inE.GetErrorCode());
		estrRsrc.GetResource('Estr', (ConstStringPtr) secondName, Throw_No);
	}
	StringHandle	estr = (StringHandle) (Handle) estrRsrc;
	if (estr != nil) {
		StHandleLocker	lock(estrRsrc);
		outDescription = *estr;
	}
} // GetErrorAndDescription

/*
HandleKnownExceptions [static]
*/
bool
ExceptionHandler::HandleKnownExceptions(LException& e, bool async)
{
	ExceptionHandler*	handler (gCurrent);
	ConstStr255Param	operationName (gCurrent->GetOperation());
	bool status (false);
	
	while (handler != nil) {
		status = handler->HandleException(e, operationName, async);
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
DefaultExceptionHandler::HandleException(LException& e, ConstStr255Param operation, bool async) {

	LStr255			errorString;
	LStr255			errorDescription;
	GetErrorAndDescription(e, errorString, errorDescription);

	this->ReportException(operation, errorDescription, errorString, emptyString, async);

	return true;
}//end HandleException
	
/*
* ReportException
*/
void
DefaultExceptionHandler::ReportException(ConstStr255Param parm0, ConstStr255Param parm1, 
									ConstStr255Param parm2, ConstStr255Param parm3,
									bool async)
{

	EUserMessage	msg (alrt_TemplateFatal, kStopIcon, parm0, parm1, parm2, parm3);
	
	if (async)
		EUserMessageServer::GetSingleton ()->QueueUserMessage (msg);
	else EUserMessageServer::GetSingleton ()->DisplayUserMessage (msg);
	
}//end ReportException										

#pragma mark -

/*
*MemoryExceptionHandler::HandleException
*/
bool
MemoryExceptionHandler::HandleException(LException& e, ConstStr255Param operation, bool async) {
	bool handled (false);
	
	LStr255 errorString (e.GetErrorCode());
	LStr255 errorDescription;
	
	if (IsMemoryError(e.GetErrorCode())) {
		errorDescription.Assign(str_ExceptionHandler, si_MemoryError);
		this->ReportException(operation, errorDescription, errorString, emptyString, async);
		handled = true;
	}	

	return handled;
}//end HandleException

/*
IsMemoryError [static]
	Several different Mac errors relate to being out of memory. This function
	lets us group them.
*/
bool
MemoryExceptionHandler::IsMemoryError(const OSErr inCode)
{
	// Note that there are a few more errors, but they seem to be very specialized
	switch (inCode) {
		case mFulErr:						/*memory full (open) or file won't fit (load)*/
		case memFullErr:					/*Not enough room in heap zone*/
		case cTempMemErr:	
		case cNoMemErr:
		case updPixMemErr:
		case noMemForPictPlaybackErr:
		case notEnoughBufferSpace:
		case kOTOutOfMemoryErr:
		case kUSBOutOfMemoryErr:
		case notEnoughMemoryToGrab:
		case telNoMemErr:
		case kHIDNotEnoughMemoryErr:
			return true;
	}
	return false;
} // IsMemoryError
