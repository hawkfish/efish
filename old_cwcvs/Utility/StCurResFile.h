/*
	File:		StCurResFile.h

	Contains:	Interface to the CurResFile resource holder.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1998 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

         <1>    11/18/98    rmgw    first checked in.
*/


#pragma once

#include <Resources.h>

class StCurResFile {

		ResFileRefNum	mSaveResFile;
		
	public:
	
						StCurResFile	(void);
						~StCurResFile	(void);
	};
	
