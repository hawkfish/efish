/*
	File:		StCurResFile.cp

	Contains:	Implementation of the CurResFile resource holder.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1998 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

         <1>    11/18/98    rmgw    first checked in.
*/


#include "StCurResFile.h"

// ---------------------------------------------------------------------------
//		€ StCurResFile
// ---------------------------------------------------------------------------

StCurResFile::StCurResFile (void)
	
	: mSaveResFile (::CurResFile ())
	
	{ // begin StCurResFile
		
	} // end StCurResFile

// ---------------------------------------------------------------------------
//		€ ~StCurResFile
// ---------------------------------------------------------------------------

StCurResFile::~StCurResFile (void)
	
	{ // begin ~StCurResFile
		
		::UseResFile (mSaveResFile);
		
	} // end ~StCurResFile

