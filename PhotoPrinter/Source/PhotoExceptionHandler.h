/*
	File:		PhotoExceptionHandler.h

	Contains:	Implementation of chain-of-exception handlers

	Written by:	Dav Lion (kibbitzed by richard wesley and david dunham)

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		21 sep 2000		dml		add GetUpstreamHandler accessor; initial class hierarchy
		21 sep 2000		dml		use LStr255, take ConstStr255Param as ct arg
		20 sep 2000		dml		added SilentExceptionEater (subclass)
		20 Sep 2000		dml		Created
*/


// abstract base class.  handles nothing
class PhotoExceptionHandler {
	protected:
		static PhotoExceptionHandler*	gCurrent;	
		
		PhotoExceptionHandler*	mPrevious;
		LStr255					mOperation;

		virtual void	ReportException(const LStr255& parm0, const LStr255& parm1, 
										const LStr255& parm2, const LStr255& parm3) = 0;
		virtual bool	HandleException(LException& e, const LStr255& operation) = 0;
		
	public:
				PhotoExceptionHandler(ConstStr255Param inOperationName);
		virtual ~PhotoExceptionHandler();

			PhotoExceptionHandler*	GetUpstreamHandler(void) {return mPrevious;};
			ConstStr255Param		GetOperation(void) {return (ConstStr255Param)mOperation;};

	static bool	HandleKnownExceptions(LException& inException);
	};//end
	

// barely useful class.  always gives "unexpected error" message
class DefaultExceptionHandler : public PhotoExceptionHandler {
	protected:
		virtual bool	HandleException(LException& e, const LStr255& operation);
		virtual void	ReportException(const LStr255& parm0, const LStr255& parm1, 
										const LStr255& parm2, const LStr255& parm3);
	public:
				DefaultExceptionHandler(ConstStr255Param inOperationName = "\pthe command") : PhotoExceptionHandler(inOperationName) {};
		virtual ~DefaultExceptionHandler() {};

	};//end SilentExceptionEater	
	

// class which never reports to user
class SilentExceptionEater : public DefaultExceptionHandler {
	protected:
		virtual void	ReportException(const LStr255& /*parm0*/, const LStr255& /*parm1*/, 
										const LStr255& /*parm2*/, const LStr255& /*parm3*/) {};
	public:
				SilentExceptionEater(ConstStr255Param inOperationName = "\pthe command") : DefaultExceptionHandler(inOperationName) {};
		virtual ~SilentExceptionEater() {};

	};//end SilentExceptionEater
	
	
// class which can interpret out-of-memory situations	
class MemoryExceptionHandler : public DefaultExceptionHandler {
	protected:
		virtual bool	HandleException(LException& e, const LStr255& operation);
	public:
				MemoryExceptionHandler(ConstStr255Param inOperationName = "\pthe command") : DefaultExceptionHandler(inOperationName) {};
		virtual ~MemoryExceptionHandler() {};

	};//end SilentExceptionEater	