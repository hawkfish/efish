/*
	File:		PhotoExceptionHandler.h

	Contains:	Implementation of chain-of-exception handlers (for PowerPlant LException)

	Written by:	Dav Lion (kibbitzed by richard wesley and david dunham)

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.
				Copyright ©2001 by A Sharp, LLC. All rights reserved.

	Change History (most recent first):

		27 Jun 2001		drd		93 GetErrorAndDescription
		20 May 2001		drd		gMaxNumericError
		14 May 2001		drd		Moved IsMemoryError to MemoryExceptionHandler
		21 Sep 2000		drd		Added gDefaultOperation (instead of string constant), resource
								enums; renamed base class
		21 sep 2000		dml		add GetUpstreamHandler accessor; initial class hierarchy
		21 sep 2000		dml		use LStr255, take ConstStr255Param as ct arg
		20 sep 2000		dml		added SilentExceptionEater (subclass)
		20 Sep 2000		dml		Created
*/

// abstract base class.  handles nothing
class ExceptionHandler {
protected:
	enum {
		str_ExceptionHandler = 333,
			si_DefaultOperation = 1,
			si_UnknownError,
			si_MemoryError
	};

	static	ExceptionHandler*	gCurrent;
	static	SInt32				gMaxNumericError;	// Errors over this are treated as 4-character strings
	
	ExceptionHandler*	mPrevious;
	LStr255				mOperation;

	virtual void	ReportException(const LStr255& parm0, const LStr255& parm1, 
									const LStr255& parm2, const LStr255& parm3) = 0;
	virtual bool	HandleException(LException& e, const LStr255& operation) = 0;
	
public:
			ExceptionHandler(ConstStr255Param inOperationName);
	virtual ~ExceptionHandler();

		ExceptionHandler*		GetUpstreamHandler(void) {return mPrevious;};
		ConstStr255Param		GetOperation(void) {return (ConstStr255Param)mOperation;}

	static	void	GetErrorAndDescription(const LException& inE, LStr255& outCode, LStr255& outDescription);
	static	bool	HandleKnownExceptions(LException& inException);
};//end
	

// Useful class. Looks up message in ‘Estr’ resources
class DefaultExceptionHandler : public ExceptionHandler {
protected:
	static	LStr255		gDefaultOperation;

	virtual bool	HandleException(LException& e, const LStr255& operation);
	virtual void	ReportException(const LStr255& parm0, const LStr255& parm1, 
									const LStr255& parm2, const LStr255& parm3);
public:
			DefaultExceptionHandler(ConstStr255Param inOperationName = gDefaultOperation) : ExceptionHandler(inOperationName) {};
	virtual ~DefaultExceptionHandler() {};

};//end SilentExceptionEater	


// class which never reports to user
class SilentExceptionEater : public DefaultExceptionHandler {
protected:
	virtual void	ReportException(const LStr255& /*parm0*/, const LStr255& /*parm1*/, 
									const LStr255& /*parm2*/, const LStr255& /*parm3*/) {};
public:
			SilentExceptionEater(ConstStr255Param inOperationName = gDefaultOperation) : DefaultExceptionHandler(inOperationName) {};
	virtual ~SilentExceptionEater() {};

};//end SilentExceptionEater


// class which can interpret out-of-memory situations	
class MemoryExceptionHandler : public DefaultExceptionHandler {
	protected:
		virtual bool	HandleException(LException& e, const LStr255& operation);
	public:
				MemoryExceptionHandler(ConstStr255Param inOperationName = gDefaultOperation) : DefaultExceptionHandler(inOperationName) {};
		virtual ~MemoryExceptionHandler() {};

	// Errors
	static	bool	IsMemoryError(const OSErr inCode);
	};//end SilentExceptionEater
