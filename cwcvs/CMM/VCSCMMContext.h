/*
	File:		VCSCMMContext.h

	Contains:	Implementation of the CMM VCS context.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1997-9 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

         <8>      5/5/99    rmgw    Add project file save routine.
         <7>     3/31/99    rmgw    Add AE idling.
         <6>     3/30/99    rmgw    Crash recovery merging.
         <5>     3/22/99    rmgw    Add results dialog.
         <4>     3/19/99    rmgw    First pass at GUI.
         <3>     3/19/99    rmgw    Implement CM project documents.
         <2>     3/18/99    rmgw    Add checkout state.
         <1>     3/17/99    rmgw    first checked in.
*/


#pragma once

#include "VCSDialogContext.h"

#include <Resources.h>

class VCSCMMContext : public VCSDialogContext {
		
									VCSCMMContext		(const	VCSCMMContext&);
		VCSCMMContext&				operator=			(const	VCSCMMContext&);
		
	protected:

	public:
		
		static	OSErr				FindProjectFile 	(FSSpec&					outProjectFile,
														 const	FSSpec&				inContext);
		static	OSErr				SaveProjectFile 	(VCSContext&				inContext,
														 ResID						inPrefsStrings,
														 const	FSSpec&				inProjectSpec);
		static	OSErr				MakeProjectFile 	(VCSContext&				inContext,
														 ResID						inPrefsStrings,
														 ConstStr255Param			inProjectName = 0,
														 Boolean					inPrompt = true);
		
									VCSCMMContext		(h_CWVCSItemState			inItems,
														 long						inRequest,
														 Boolean					inAdvanced,
														 Boolean					inRecursive,
														 Boolean					inSupported = false);
		virtual						~VCSCMMContext		(void);
			
			//	Plugin calls	
		virtual	CWMemHandle			GetNamedPreferences	(const char* 				prefsname) const;
		virtual	void				GetProjectFile		(CWFileSpec& 				outProjectSpec) const;

			//	VCS calls
		virtual	void				GetDatabase			(CWVCSDatabaseConnection&	dbConnection) const;
														 
	};

