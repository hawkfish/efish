/*
	File:		PhotoExceptionHandler.h

	Contains:	Implementation of chain-of-exception handlers

	Written by:	Dav Lion (kibbitzed by richard wesley and david dunham)

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		20 sep 2000		dml		added SilentExceptionEater (subclass)
		20 Sep 2000		dml		Created
*/

#include "MPString.h"

class PhotoExceptionHandler {
	protected:
		static PhotoExceptionHandler*	gCurrent;	
		
		PhotoExceptionHandler*	mPrevious;
		MPString				mOperation;

		void	ReportException(const MPString& parm0, const MPString& parm1, 
								const MPString& parm2, const MPString& parm3);
		bool	HandleException(LException& e);
		
	public:
				PhotoExceptionHandler(MPString inOperationName = "\pthe command");
		virtual ~PhotoExceptionHandler();
	static bool	HandleKnownExceptions(LException& inException);
	};//end
	
	
	
class SilentExceptionEater : public PhotoExceptionHandler {
	protected:
		void	ReportException(const MPString& /*parm0*/, const MPString& /*parm1*/, 
								const MPString& /*parm2*/, const MPString& /*parm3*/) {};
	public:
				SilentExceptionEater();
		virtual ~SilentExceptionEater();

	};//end SilentExceptionEater