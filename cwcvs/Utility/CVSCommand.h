#pragma once

#include "VCSContext.h"

#include <AEDataModel.h>
#include <Processes.h>

OSErr
CVSCreateCommand	(AEDesc*						outCommand, 
					 const	char*					inName);

OSErr
CVSDisposeCommand	(AEDesc*						inCommand);

OSErr
CVSAddCStringArg	(AEDesc*						command, 
					 const	char*					string);
	
OSErr
CVSAddPStringArg	(AEDesc*						command, 
					 ConstStr255Param				string);
	
OSErr
CVSAddCommentArg	(AEDesc*						outCommand, 
					 ConstStr255Param				inComment);

OSErr
CVSAddListArgs		(AEDesc*						inCommand,
					 AEDescList* 					inList);
					 
OSErr 
CVSGetProcess 		(const VCSContext&				inContext,
					 ProcessSerialNumber*			psn);

OSErr
CVSSendCommand 		(const VCSContext&				inContext,
					 Handle*						outResult,
					 const	ProcessSerialNumber*	inPSN,
					 const	AEDesc*					inCommand,
					 const	FSSpec*					inCVSLocal);

