#pragma once

#include "VCSFileCommand.h"

#include <Resources.h>

class VCSDifference : public VCSFileCommand

	{
		
	protected:
		
		ResID						mCompareID;
		
		virtual	void				IterateFile		 		(const CInfoPBRec&		 	cpbPtr,
															 long						dirID,
															 Boolean&					quitFlag);
	public:
	
		enum {
			kTypeError = -1,
			kTypeIgnore = 0,
			kTypeIDE = 'CWIE',
			kTypeUnknown = '????',
			
			kIDEDiffResourceID = 16260,
			kNoIDEDiffResourceID = 16261
			};
			
			
									VCSDifference			(VCSContext&				inContext);
		virtual						~VCSDifference			(void);
			
			//	MacOS Diffs
		virtual	OSType 				GetFileCompareType 		(const	OSType				inType) const;
		virtual	OSType 				GetFileCompareType 		(const	FSSpec&				inItem) const;
		virtual	CWVCSItemStatus 	CompareIDEFile 			(CWVCSItem& 				inItem,
															 const	FSSpec&				inOldItem,
															 ConstStr255Param			inVersionName);
		virtual	CWVCSItemStatus 	CompareFile 			(CWVCSItem& 				inItem,
															 OSType						inCreator,
															 const	FSSpec&				inOldItem,
															 ConstStr255Param			inVersionName);
		virtual	Boolean				GetBaseVersion 			(StringPtr					outVersion,
															 const	FSSpec&				inSpec);

		virtual	CWVCSItemStatus		GetVersionFileFromUpdate (FSSpec&					outVersionSpec,
															 const	FSSpec&				inSpec,
															 ConstStr255Param			inVersion);
		virtual	CWVCSItemStatus		GetVersionFileFromCheckout (FSSpec&					outVersionSpec,
															 const	FSSpec&				inSpec,
															 ConstStr255Param			inVersion);
		virtual	CWVCSItemStatus		GetVersionFile 			(FSSpec&					outVersionSpec,
															 const	FSSpec&				inSpec,
															 ConstStr255Param			inVersion);

		virtual	CWVCSItemStatus		MacProcessRegularFile	(CWVCSItem&					inItem);

		virtual	CWVCSItemStatus		CVSProcessRegularFile 	(CWVCSItem&					inItem);
		virtual	CWVCSItemStatus		CVSProcessRegularFolder	(CWVCSItem&					inItem);

		virtual	CWVCSItemStatus		ProcessRegularFile 		(CWVCSItem&					inItem);
		virtual	CWVCSItemStatus		ProcessRegularFolder 	(CWVCSItem&					inItem);
	};
