#pragma once

#include "VCSStdDialog.h"

#include <Resources.h>

class VCSAdvancedOptionsDialog : public VCSStdDialog

	{
		
	public:
	
		enum {
			kVCSAdvancedFirstFreeItem = kVCSStdFirstFreeItem
			};
			
						VCSAdvancedOptionsDialog	(const VCSContext&		inPB,
													 ResID					inDLOGid);
		virtual			~VCSAdvancedOptionsDialog	(void);


		virtual	OSErr 	GetOptionsList 				(AEDescList&			outOptions,
													 ResID					inStrnID) const;
		virtual	OSErr 	SetOptionsList 				(const	AEDescList&		inOptions,
													 ResID					inStrnID);
													 
	};

