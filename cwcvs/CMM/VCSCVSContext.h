/*
	File:		VCSCVSContext.h

	Contains:	Implementation of the CMM VCS context.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1997-9 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

*/


#pragma once

#include "VCSDialogContext.h"

#include "EnvList.h"

#include <Resources.h>

class VCSCVSContext : public VCSDialogContext {
		
									VCSCVSContext		(const	VCSCVSContext&);
		VCSCVSContext&				operator=			(const	VCSCVSContext&);
	
	protected:
	
		FSSpec						mLocalRoot;
		EnvironmentListHandle		mEnvList;
		
	public:
		
		static	OSErr				FindCVSDir 			(FSSpec&					outCVSDir,
														 const	FSSpec&				inContext);
		static	OSErr				FindLocalRoot 		(FSSpec&					outLocalRoot,
														 const	FSSpec&				inContext);
	
									VCSCVSContext		(h_CWVCSItemState			inItems,
														 long						inRequest,
														 Boolean					inAdvanced,
														 Boolean					inRecursive,
														 Boolean					inSupported = false);
		virtual						~VCSCVSContext		(void);
			
			//	Plugin calls	
		virtual	CWMemHandle			GetNamedPreferences	(const char* 				prefsname) const;
		virtual	void				GetProjectFile		(CWFileSpec& 				outProjectSpec) const;

			//	VCS calls
		virtual	void				GetDatabase			(CWVCSDatabaseConnection&	dbConnection) const;
														 
	};

