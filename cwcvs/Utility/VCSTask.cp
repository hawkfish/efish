/*
	File:		VCSTask.cp

	Contains:	<contents>

	Written by:	Richard Wesley

	Copyright:	Copyright ©1997-8 Electric Fish Inc.  All Rights Reserved.

	Change History (most recent first):

         <6>     10/8/98    rmgw    Convert to v7 API.
         <5>     9/22/98    rmgw    Convert to CWP3.1.
         <4>      2/6/98    rmgw    Convert VCSTask to stack object.
         <3>    11/13/97    rmgw    Implement.
         <2>    11/12/97    rmgw   Use const strings.
         <1>    10/29/97           first checked in
*/


#include "VCSTask.h"

#include "VCSUtil.h"

#include <TextUtils.h>

//	=== Members ===

VCSTask*
VCSTask::sTaskList = 0;

// ---------------------------------------------------------------------------
//		¥ GetTaskName [static]
// ---------------------------------------------------------------------------

StringPtr
VCSTask::GetTaskName (

	Str255	outTaskName)

	{ // begin GetTaskName
		
		if (sTaskList)
			BlockMoveData (sTaskList->mMessage, outTaskName, sizeof (sTaskList->mMessage));
		else outTaskName[0] = 0;
		
		return c2pstr ((char*) outTaskName);
		
	} // end GetTaskName
	
// ---------------------------------------------------------------------------
//		¥ VCSTask
// ---------------------------------------------------------------------------

VCSTask::VCSTask (

	VCSContext&				inPB,
	short 					strListID, 
	short		 			strIndex, 
	ConstStr255Param 		param1, 
	ConstStr255Param 		param2,
	ConstStr255Param 		param3, 
	ConstStr255Param 		param4)
	
	: mPB (inPB)
	, mNext (sTaskList)
	
	{ // begin VCSTask
		
		ReplaceInIndString ((StringPtr) mMessage, strListID, strIndex, param1, param2, param3, param4);	
		p2cstr ((StringPtr) mMessage);
		
		sTaskList = this;

		mPB.ReportProgress (sTaskList->mMessage);

	} // end VCSTask

// ---------------------------------------------------------------------------
//		¥ ~VCSTask
// ---------------------------------------------------------------------------

VCSTask::~VCSTask (void)
	
	{ // begin ~VCSTask
		
		char	null = 0;
		
		/* pop the task from the task stack */
		sTaskList = mNext;
		
		if (sTaskList) 
			mPB.ReportProgress (sTaskList->mMessage);
		else mPB.ReportProgress (&null);
		
	} // end ~VCSTask
