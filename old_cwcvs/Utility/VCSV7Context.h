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
		virtual	CWResult			YieldTime			(void) const;
		virtual	long				GetRequest			(void) const;
		
		virtual	CWMemHandle			GetNamedPreferences	(const char* 				prefsname) const;
		virtual	void				GetProjectFile		(CWFileSpec& 				outProjectSpec) const;

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
														 int						inCompleted) const;
		virtual	void				ReportProgress 		(const char *				line1,
														 const char *				line2 = 0) const;
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
														 const long 				lBufferSize) const;							 
		virtual	void				UpdateCheckoutState (const CWFileSpec& 			inItem,
														 CWVCSCheckoutState 		eCheckoutState,
														 const CWVCSVersion& 		version) const;
		virtual	void				MessageOutput 		(short 						errorlevel,
														 const char *				line1, 
														 const char *				line2, 
														 long 						errorNumber = 0) const;
		virtual	CWResult			VisualDifference	(const CWFileSpec& 			fileFixedSpec,
														 const CWFileSpec& 			fileEditSpec) const;
		virtual	CWResult			VisualDifference	(const char*				fileFixedName, 
														 const char*				fileFixedText, 
														 unsigned	long			fileFixedTextLength, 
														 const CWFileSpec& 			fileEditSpec) const;
	};

