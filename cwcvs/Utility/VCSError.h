// ===========================================================================
//	VCSError.h ©1997 Electric Fish, Inc. All rights reserved.
// ===========================================================================

#pragma once

#include "VCSContext.h"

enum ErrorStrings {
	kNoErr,

	kCvsErr,
	kCvsInfo,
	kCvsServer,
		
	kLongTaskErr,
	kShortTaskErr,
	
	kAbortedErr,
	kServerAbortedError,
	kServerMsg,
	
	kErrorStrings = 16003,
	
	kFnordError
	};
	
void 
VCSResTextError		(VCSContext&			inPB,
					 short					strListID, 
					 short					strIndex, 
					 OSErr					callerErr = noErr,
					 StringPtr				param1 = nil, 
					 StringPtr				param2 = nil,
					 StringPtr				param3 = nil, 
					 StringPtr				param4 = nil);

OSErr
VCSRaiseOSErr		(VCSContext&			inPB,
					 OSErr					e);

void 
VCSRaiseString		(VCSContext&			inPB,
					 short					strListID, 
					 short					strIndex, 
					 StringPtr				param1 = nil, 
					 StringPtr				param2 = nil,
					 StringPtr				param3 = nil, 
					 StringPtr				param4 = nil);

