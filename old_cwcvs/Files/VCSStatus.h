/*
	File:		VCSStatus.h

	Contains:	Interface to the CVS Status command.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1998 by Electric Fish, Inc.  All Rights Reserved.

*/


#pragma once

#include "VCSFileCommand.h"

class VCSStatus : public VCSFileCommand

	{
		
	public:
	
		enum StatusString {
			kIllegalIndex,
			kSuffixIndex,
			
			kStringsID = 16100
			};


									VCSStatus				(VCSContext&				inContext);
		virtual						~VCSStatus				(void);

		virtual	CWVCSItemStatus		ProcessRegularFolder 	(CWVCSItem&					inItem);
		virtual	CWVCSItemStatus		ProcessRegularFile 		(CWVCSItem&					inItem);
	};
