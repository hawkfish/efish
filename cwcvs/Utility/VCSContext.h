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
		virtual	void				GetProjectFile		(CWFileSpec& 				outProjectSpec) const = 0;

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
