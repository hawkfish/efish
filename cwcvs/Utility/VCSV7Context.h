/*
	File:		VCSV7Context.h

	Contains:	Interface to the v7 API context.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1998 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

          <>     3/19/99    rmgw    ReportProgress should not be const.
        <10>     3/31/99    rmgw    Add AE idling.
         <9>     3/30/99    rmgw    Crash recovery merging.
         <8>     3/22/99    rmgw    MessageOutput should not be const.
         <6>     3/19/99    rmgw    Add handle size and project file accessors.
         <5>     3/18/99    rmgw    UpdateCheckoutState no longer const.
         <4>     12/4/98    rmgw    Add GetIDEVersion.
         <3>     12/1/98    rmgw    Add memory based difference.
         <2>    11/12/98    rmgw    Return visual difference result code.
         <1>    11/10/98    rmgw    first checked in.
*/


#pragma once

#include "VCSContext.h"

class VCSV7Context : public VCSContext {
		
									VCSV7Context		(const	VCSV7Context&);
		VCSV7Context&				operator=			(const	VCSV7Context&);
		
	protected:
		
		CWPluginContext				mContext;
		
	public:
		
		static	CWResult			CheckResult			(CWResult					inResult);
		static	CWResult			CheckOptionalResult	(CWResult					inResult);
		
									VCSV7Context		(CWPluginContext			inContext);
		virtual						~VCSV7Context		(void);
			
			//	Plugin calls	
		virtual	long				GetAPIVersion		(void) const;
		virtual	CWResult			GetIDEVersion		(CWIDEInfo&					outInfo) const;
		virtual	CWResult			YieldTime			(void);
		virtual	long				GetRequest			(void) const;
		
		virtual	CWMemHandle			GetNamedPreferences	(const char* 				prefsname) const;
		virtual	void				GetProjectFile		(CWFileSpec& 				outProjectSpec) const;

		virtual	long				GetMemHandleSize	(CWMemHandle				inHandle) const;
		virtual	void*				LockMemHandle		(CWMemHandle				inHandle,
														 Boolean					inMoveHi = false) const;
		virtual	void				UnlockMemHandle		(CWMemHandle				inHandle) const;

		virtual	CWVCSCommandStatus	GetCommandStatus	(void) const;
		virtual	void				SetCommandStatus	(CWVCSCommandStatus 		inStatus);

		virtual	CWResult			PreDialog			(void) const;
		virtual	CWResult			PostDialog			(void) const;

		virtual	void				PreFileAction 		(const CWFileSpec& 			inFile) const;
		virtual	void				PostFileAction 		(const CWFileSpec& 			inFile) const;

		virtual	void				CompletionRatio 	(int						inTotal,
														 int						inCompleted);
		virtual	void				ReportProgress 		(const char *				line1,
														 const char *				line2 = 0);
		virtual	void				CreateDocument		(const	char*				inName,
														 Handle						inData,
														 Boolean					inDirty = false) const;
			
			//	VCS calls
		virtual	Boolean				CheckIfSupported	(void) const;
		virtual	Boolean				Advanced			(void) const;
		virtual	Boolean				Recursive			(void) const;

		virtual	void				GetDatabase			(CWVCSDatabaseConnection&	dbConnection) const;

		virtual	unsigned long		GetItemCount		(void) const;
		virtual	void				GetItem				(CWVCSItem&					outItem,
														 long						inItemNo) const;
		virtual	void				SetItem				(const	CWVCSItem&			inItem,
														 long						inItemNo);
		
		virtual	CWResult			GetComment			(const char *				pPrompt,
														 char *						pComment, 
														 const long 				lBufferSize);							 
		virtual	void				UpdateCheckoutState (const CWFileSpec& 			inItem,
														 CWVCSCheckoutState 		eCheckoutState,
														 const CWVCSVersion& 		version);
		virtual	void				MessageOutput 		(short 						errorlevel,
														 const char *				line1, 
														 const char *				line2, 
														 long 						errorNumber = 0);
		virtual	CWResult			VisualDifference	(const CWFileSpec& 			fileFixedSpec,
														 const CWFileSpec& 			fileEditSpec) const;
		virtual	CWResult			VisualDifference	(const char*				fileFixedName, 
														 const char*				fileFixedText, 
														 unsigned	long			fileFixedTextLength, 
														 const CWFileSpec& 			fileEditSpec) const;
														 
			//	Non-IDE callbacks
		virtual	CWResult			OnAEIdle			(EventRecord&				theEvent, 
														 long&						sleepTime, 
														 RgnHandle&					mouseRgn);
	};

