/*
	File:		PhotoExceptionHandler.h

	Contains:	Implementation of chain-of-exception handlers

	Written by:	Dav Lion (kibbitzed by richard wesley and david dunham)

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		21 sep 2000		dml		use LStr255, take ConstStr255Param as ct arg
		20 sep 2000		dml		added SilentExceptionEater (subclass)
		20 Sep 2000		dml		Created
*/


class PhotoExceptionHandler {
	protected:
		static PhotoExceptionHandler*	gCurrent;	
		
		PhotoExceptionHandler*	mPrevious;
		LStr255					mOperation;

		void	ReportException(const LStr255& parm0, const LStr255& parm1, 
								const LStr255& parm2, const LStr255& parm3);
		bool	HandleException(LException& e);
		
	public:
				PhotoExceptionHandler(LStr255 inOperationName = "\pthe command");
		virtual ~PhotoExceptionHandler();
	static bool	HandleKnownExceptions(LException& inException);
	};//end
	
	
	
class SilentExceptionEater : public PhotoExceptionHandler {
	protected:
		void	ReportException(const LStr255& /*parm0*/, const LStr255& /*parm1*/, 
								const LStr255& /*parm2*/, const LStr255& /*parm3*/) {};
	public:
				SilentExceptionEater() : PhotoExceptionHandler() {};
		virtual ~SilentExceptionEater() {};

	};//end SilentExceptionEater