// ===========================================================================
//	VCSPrefs.h ©1997 Electric Fish, Inc. All rights reserved.
// ===========================================================================

#pragma once

#include "VCSContext.h"

#include "EnvPanel.h"

void 
VCSGetUserName (
	
	const VCSContext&		inPB,
	Str63					userName);

Boolean 
VCSPrefsGetEnv (
	
	const VCSContext&		inPB,
	ConstStr255Param		inKey,
	Str255					outValue,
	ConstStr255Param		inDefault);

OSErr 
VCSPrefsMakeEnvDescList (
	
	const VCSContext&		inPB,
	AEDescList*				outList);
