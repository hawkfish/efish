/*
	File:		VCSContext.h

	Contains:	Interface to the CWCVSContext wrapper.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1998 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

         <8>     12/4/98    rmgw    Add GetIDEVersion.
         <7>     12/1/98    rmgw    Add memory based difference.
         <6>    11/12/98    rmgw    Return visual difference result code.
         <5>    11/10/98    rmgw    Convert to abstract base class.
         <4>     11/9/98    rmgw    Make CompletionRatio/Pre/PostFile work when not implemented.
         <3>    10/19/98    rmgw    Pass 0 for location spec in MessageOutput.
         <2>    10/12/98    rmgw    Add visual difference.
         <1>     10/8/98    rmgw    Convert to v7 API.
*/


#pragma once

#include <DropInVCS.h>

class VCSContext {
		
									VCSContext			(const	VCSContext&);
		VCSContext&					operator=			(const	VCSContext&);
		
	public:
		
									VCSContext			(void);
		virtual						~VCSContext			(void);
			
			//	Plugin calls	
		virtual	long				GetAPIVersion		(void) const = 0;
		virtual	CWResult			GetIDEVersion		(CWIDEInfo&					outInfo) const = 0;
		virtual	CWResult			YieldTime			(void) const = 0;
		virtual	long				GetRequest			(void) const = 0;
		
		virtual	CWMemHandle			GetNamedPreferences	(const char* 				prefsname) const = 0;

		virtual	void*				LockMemHandle		(CWMemHandle				inHandle,
														 Boolean					inMoveHi = false) const = 0;
		virtual	void				UnlockMemHandle		(CWMemHandle				inHandle) const = 0;

		virtual	CWVCSCommandStatus	GetCommandStatus	(void) const = 0;
		virtual	void				SetCommandStatus	(CWVCSCommandStatus 		inStatus) = 0;

		virtual	CWResult			PreDialog			(void) const = 0;
		virtual	CWResult			PostDialog			(void) const = 0;

		virtual	void				PreFileAction 		(const CWFileSpec& 			inFile) const = 0;
		virtual	void				PostFileAction 		(const CWFileSpec& 			inFile) const = 0;

		virtual	void				CompletionRatio 	(int						inTotal,
														 int						inCompleted) const = 0;
		virtual	void				ReportProgress 		(const char *				line1,
														 const char *				line2 = 0) const = 0;
		virtual	void				CreateDocument		(const	char*				inName,
														 Handle						inData,
														 Boolean					inDirty = false) const = 0;
			
			//	VCS calls
		virtual	Boolean				CheckIfSupported	(void) const = 0;
		virtual	Boolean				Advanced			(void) const = 0;
		virtual	Boolean				Recursive			(void) const = 0;

		virtual	void				GetDatabase			(CWVCSDatabaseConnection&	dbConnection) const = 0;

		virtual	unsigned long		GetItemCount		(void) const = 0;
		virtual	void				GetItem				(CWVCSItem&					outItem,
														 long						inItemNo) const = 0;
		virtual	void				SetItem				(const	CWVCSItem&			inItem,
														 long						inItemNo) = 0;
		
		virtual	CWResult			GetComment			(const char *				pPrompt,
														 char *						pComment, 
														 const long 				lBufferSize) const = 0;							 
		virtual	void				UpdateCheckoutState (const CWFileSpec& 			inItem,
														 CWVCSCheckoutState 		eCheckoutState,
														 const CWVCSVersion& 		version) const = 0;
		virtual	void				MessageOutput 		(short 						errorlevel,
														 const char *				line1, 
														 const char *				line2, 
														 long 						errorNumber = 0) const = 0;
		virtual	CWResult			VisualDifference	(const CWFileSpec& 			fileFixedSpec,
														 const CWFileSpec& 			fileEditSpec) const = 0;
		virtual	CWResult			VisualDifference	(const char*				fileFixedName, 
														 const char*				fileFixedText, 
														 unsigned	long			fileFixedTextLength, 
														 const CWFileSpec& 			fileEditSpec) const = 0;
	};
