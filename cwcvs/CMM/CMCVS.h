/*
	File:		CMCVS.h

	Contains:	Interface to the Projector CMM Plugin.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1997-9 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

         <3>     3/30/99    rmgw    Crash recovery merging.
         <2>     3/18/99    rmgw    Add menu building.
         <1>     3/17/99    rmgw    first checked in.
*/


#pragma once

// Parent Class Include
#include <AbstractCMPlugin.h>


class CMCVS : virtual AbstractCMPlugin {

#pragma SOMReleaseOrder (Initialize, ExamineContext, HandleSelection, PostMenuCleanup)

	FSSpec	mPluginFile;
	Boolean	mPluginInitialized;
	
public:

	virtual	OSStatus Initialize(
							Environment*,
							FSSpec *inFileSpec);
	virtual	OSStatus ExamineContext(
							Environment*,
							AEDesc *inContextDescriptor,
							SInt32 inTimeOutInTicks,
							AEDescList* ioCommands,
							Boolean* outNeedMoreTime);
	virtual	OSStatus HandleSelection(
							Environment*,
							AEDesc *inContextDescriptor,
							SInt32 inCommandID);
	virtual	OSStatus PostMenuCleanup(
							Environment*);
};

