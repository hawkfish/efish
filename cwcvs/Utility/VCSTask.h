/*
	File:		VCSTask.h

	Contains:	Interface for the Task feedback class.

	Written by:	Richard Wesley

	Copyright:	©1997-8 Electric Fish, Inc. All rights reserved.

	Change History (most recent first):

        <14>    10/19/98    rmgw    Add examining task.
        <13>     10/8/98    rmgw    Convert to v7 API.
        <12>     9/24/98    rmgw    Add comment tasks.
        <11>      6/3/98    rmgw    Add Purge task.
        <10>     2/23/98    rmgw    Properties/Status implementation.
         <9>      2/6/98    rmgw    Convert VCSTask to stack object.
         <8>    11/17/97    rmgw    Delete old labels.
         <7>    11/14/97    rmgw    Add Rename task.
         <6>    11/13/97    rmgw    Add History and Status commands.
         <5>    11/13/97    rmgw    Implement Database variables and Label commands.
         <4>    11/13/97    rmgw    Implement our task stack.
         <3>    11/12/97    rmgw    Use const strings.
         <2>    10/31/97    rmgw    Add comment tasks.
         <1>    10/29/97    rmgw    first checked in.
*/


#pragma once

#include "VCSContext.h"

enum TaskStrings {
	kIllegalTask,

	kConnectTask,
	kVariablesTask,
	
	kAddTask,
	kCheckOutTask,
	kCheckInTask,
	kCommentTask,
	kDeleteTask,
	kDifferenceTask,
	kGetTask,
	kHistoryTask,
	kLabelTask,
	kPropertiesTask,
	kRenameTask,
	kStatusTask,
	kUndoCheckOutTask,
	kVersionTask,
	
	kFindingClientTask,

	kTaskStrings = 16001,
	
	kFnordTask
	};
	

class VCSTask {

		static	VCSTask*		sTaskList;
		
		VCSContext&				mPB;
		VCSTask*				mNext;
		char					mMessage [256];
	
	public:
	
								VCSTask			(VCSContext&			inPB,
												 short 					strListID, 
												 short		 			strIndex, 
												 ConstStr255Param 		param1 = nil, 
												 ConstStr255Param 		param2 = nil,
												 ConstStr255Param 		param3 = nil, 
												 ConstStr255Param 		param4 = nil);
		virtual					~VCSTask		(void);
		
		static	StringPtr		GetTaskName		(Str255					outTaskName);
	};
