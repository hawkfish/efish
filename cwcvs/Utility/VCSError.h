// ===========================================================================
//	VCSError.h ©1997 Electric Fish, Inc. All rights reserved.
// ===========================================================================

#pragma once

#include "VCSContext.h"

enum ErrorStrings {
	kNoErr,

	kCantFindProjectErr,
	kCantGetCKIDErr,

	kProjectorErr,
	kProjectorInfo,
	
	kCantDeleteErr,
	kNotCheckedOutErr,
	kCheckedOutErr,
	
	kLongTaskErr,
	kShortTaskErr,
	
	kAbortedErr,
	
	kErrorStrings = 16003,
	
	kFnordError
	};
	
void 
VCSResTextError		(const VCSContext&		inPB,
					 short					strListID, 
					 short					strIndex, 
					 OSErr					callerErr = noErr,
					 StringPtr				param1 = nil, 
					 StringPtr				param2 = nil,
					 StringPtr				param3 = nil, 
					 StringPtr				param4 = nil);

OSErr
VCSRaiseOSErr		(const VCSContext&		inPB,
					 OSErr					e);

void 
VCSRaiseString		(const VCSContext&		inPB,
					 short					strListID, 
					 short					strIndex, 
					 StringPtr				param1 = nil, 
					 StringPtr				param2 = nil,
					 StringPtr				param3 = nil, 
					 StringPtr				param4 = nil);

