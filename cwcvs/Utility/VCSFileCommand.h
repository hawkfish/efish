/*
	File:		VCSFileCommand.h

	Contains:	Interface to the generic file command handler.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1998 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

         <1>    10/14/98    rmgw    first checked in.
*/


#pragma once

#include "VCSCommand.h"

class VCSFileCommand : public VCSCommand

	{
		
		static	pascal	void		IterateProc		 		(const CInfoPBRec * const 	cpbPtr,
															 long						dirID,
															 Boolean*					quitFlag,
															 void*						yourDataPtr);
	
	protected:
	
		virtual	void				IterateFile		 		(const CInfoPBRec&		 	cpbPtr,
															 long						dirID,
															 Boolean&					quitFlag);
		virtual	void				IterateFolder		 	(const CInfoPBRec&		 	cpbPtr,
															 long						dirID,
															 Boolean&					quitFlag);
		virtual	void				IterateFilter		 	(const CInfoPBRec&		 	cpbPtr,
															 long						dirID,
															 Boolean&					quitFlag);
															 
	public:
		
		static	UInt16				sMaxRecursion;
		
									VCSFileCommand			(VCSContext&				inContext,
															 Boolean					inHasCommand = false,
															 Boolean					inHasRecursive = false,
															 Boolean					inHasAdvanced = false,
															 Boolean					inHasAdvancedRecursive = false);
		virtual						~VCSFileCommand			(void);


		virtual	CWVCSItemStatus		ProcessRegularFile 		(CWVCSItem&					inItem);
		virtual	CWVCSItemStatus		ProcessAdvancedFile 	(CWVCSItem&					inItem);
		virtual	CWVCSItemStatus		ProcessFile 			(CWVCSItem&					inItem);
		
		virtual	CWVCSItemStatus		ProcessRegularFolder 	(CWVCSItem&					inItem);
		virtual	CWVCSItemStatus		ProcessAdvancedFolder 	(CWVCSItem&					inItem);
		virtual	CWVCSItemStatus		ProcessFolder 			(CWVCSItem&					inItem);
		
		virtual	CWVCSItemStatus		ProcessItem 			(CWVCSItem&					inItem);
		
		virtual	CWVCSCommandStatus	DoRequest 				(void);
	};
