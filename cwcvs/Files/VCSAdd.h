#pragma once

#include "VCSFileCommand.h"

class VCSAdd : public VCSFileCommand

	{
		
	public:
		
									VCSAdd					(VCSContext&				inContext);
		virtual						~VCSAdd					(void);

		virtual	OSErr				AddDirectoryOfFile 		(const	FSSpec&				inDirFile);
		
		virtual	CWVCSItemStatus		ProcessRegularFile 		(CWVCSItem&					inItem);
	};
