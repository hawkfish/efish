/*
	File:		VCSCommand.h

	Contains:	Interface to a VCS command handler object.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1997-8 Electric Fish Inc.  All Rights Reserved.

	Change History (most recent first):

        <16>    11/12/98    rmgw    Remove const from "Has" members to allow complex implementation
                                    determinations.
        <15>    10/14/98    rmgw    Convert to command object.
        <14>     10/8/98    rmgw    Convert to v7 API.
        <13>      6/9/98    rmgw    Add Recursive Purge command.
        <12>      6/5/98    rmgw    Add Recursive Properties.
        <11>      6/5/98    rmgw    Add Recursive Status.
        <10>      6/3/98    rmgw    Add Purge command.
         <9>     2/23/98    rmgw    Properties/Status implementation.
         <8>     2/19/98    rmgw    Add functionality for modifying comments.
         <7>     2/10/98    rmgw    Add VCSFileCancel.
         <6>      2/5/98    rmgw    Add VCSAbout.
         <5>    11/14/97    rmgw    first checked in.
         <4>    11/14/97    rmgw    Add Rename task.
         <3>    11/13/97    rmgw    Add History and Status commands.
         <2>    11/13/97    rmgw    Implement Database variables and Label commands.
         <1>    10/29/97    rmgw    first checked in
*/


#pragma once

#include "VCSContext.h"

class VCSCommand {

	protected:
	
		VCSContext&		mContext;
		
		Boolean			mHasCommand;
		Boolean			mHasRecursive;
		Boolean			mHasAdvanced;
		Boolean			mHasAdvancedRecursive;
		
	public:
	
									VCSCommand			(VCSContext&	inContext,
														 Boolean		inHasCommand = false,
														 Boolean		inHasRecursive = false,
														 Boolean		inHasAdvanced = false,
														 Boolean		inHasAdvancedRecursive = false);
		virtual						~VCSCommand			(void);

		virtual	CWVCSCommandStatus	RequestIsSupported 	(void);
		virtual	CWVCSCommandStatus	DoRequest 			(void);
		virtual	CWVCSCommandStatus	OnRequest 			(void);
	};

									 