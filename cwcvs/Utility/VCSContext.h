/*
	File:		VCSContext.h

	Contains:	Interface to the CWCVSContext wrapper.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1998 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

        <14>     3/31/99    rmgw    Add AE idling.
        <13>     3/30/99    rmgw    Crash recovery merging.
        <12>     3/22/99    rmgw    MessageOutput should not be const.
        <11>     3/19/99    rmgw    ReportProgress should not be const.
        <10>     3/19/99    rmgw    Add handle sizing and project file access.
         <9>     3/18/99    rmgw    UpdateCheckoutState should not be const.
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

#include <AppleEvents.h>

class VCSContext {
		
									VCSContext			(const	VCSContext&);
		VCSContext&					operator=			(const	VCSContext&);
	
		
		static	VCSContext*			sAECallback;
		
		static	pascal Boolean 		AEIdleProc			(EventRecord*				theEvent, 
														 long*						sleepTime, 
														 RgnHandle*					mouseRgn);
														 
	public:
		
									VCSContext			(void);
		virtual						~VCSContext			(void);
			
			//	Plugin calls	
		virtual	long				GetAPIVersion		(void) const = 0;
		virtual	CWResult			GetIDEVersion		(CWIDEInfo&					outInfo) const = 0;
		virtual	CWResult			YieldTime			(void) = 0;
		virtual	long				GetRequest			(void) const = 0;
		
		virtual	CWMemHandle			GetNamedPreferences	(const char* 				prefsname) const = 0;
		virtual	void				GetProjectFile		(CWFileSpec& 				outProjectSpec) const = 0;

		virtual	long				GetMemHandleSize	(CWMemHandle				inHandle) const = 0;
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
														 int						inCompleted) = 0;
		virtual	void				ReportProgress 		(const char *				line1,
														 const char *				line2 = 0) = 0;
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
														 const long 				lBufferSize) = 0;							 
		virtual	void				UpdateCheckoutState (const CWFileSpec& 			inItem,
														 CWVCSCheckoutState 		eCheckoutState,
														 const CWVCSVersion& 		version) = 0;
		virtual	void				MessageOutput 		(short 						errorlevel,
														 const char *				line1, 
														 const char *				line2, 
														 long 						errorNumber = 0) = 0;
		virtual	CWResult			VisualDifference	(const CWFileSpec& 			fileFixedSpec,
														 const CWFileSpec& 			fileEditSpec) const = 0;
		virtual	CWResult			VisualDifference	(const char*				fileFixedName, 
														 const char*				fileFixedText, 
														 unsigned	long			fileFixedTextLength, 
														 const CWFileSpec& 			fileEditSpec) const = 0;
														 
			//	Non-IDE callbacks
		virtual	AEIdleUPP 			BeginAEIdle 		(void);
		virtual	CWResult			OnAEIdle			(EventRecord&				theEvent, 
														 long&						sleepTime, 
														 RgnHandle&					mouseRgn) = 0;
		virtual	void				EndAEIdle 			(AEIdleUPP&					inIdleProc);
	};
