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

#include "VCSContext.h"
#include "CMProgressDialog.h"
#include "CMResultsDialog.h"

#include <Resources.h>

class VCSCMMContext : public VCSContext {
		
									VCSCMMContext		(const	VCSCMMContext&);
		VCSCMMContext&				operator=			(const	VCSCMMContext&);
		
	protected:

		CMResultsDialog				mResultsDialog;
		CMProgressDialog			mProgressDialog;
		Boolean						mHaveResults;
		CWResult					mYieldResult;
		
		long						mItemCount;
		CWVCSItem					mItem;
		const	long				mRequest;
		const	Boolean				mAdvanced;
		const	Boolean				mRecursive;
		const	Boolean				mSupported;

		CWVCSCommandStatus			mCurrentStatus;
		CWVCSCheckoutState			mCheckoutState;
		char						mUserName[256];
		char						mPassword[256];
		
		Boolean						mUseForAll;
		Str255						mLastComment;
		
	public:
		
		static	CWResult			CheckResult			(CWResult					inResult);
		static	CWResult			CheckOptionalResult	(CWResult					inResult);
		static	OSErr				FindProjectFile 	(FSSpec&					outProjectFile,
														 const	FSSpec&				inContext);
		static	OSErr				SaveProjectFile 	(VCSContext&				inContext,
														 ResID						inPrefsStrings,
														 const	FSSpec&				inProjectSpec);
		static	OSErr				MakeProjectFile 	(VCSContext&				inContext,
														 ResID						inPrefsStrings,
														 ConstStr255Param			inProjectName = 0);
		
									VCSCMMContext		(const	FSSpec*				inContext,
														 long						inRequest,
														 Boolean					inAdvanced,
														 Boolean					inRecursive,
														 Boolean					inSupported = false);
		virtual						~VCSCMMContext		(void);
			
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

			//	Context specific
		virtual	CWVCSCheckoutState	GetCheckoutState	(void) const;
	};

