#pragma once

#include "VCSFileCommand.h"

class VCSComment : public VCSFileCommand

	{
	
	public:
	
									VCSComment				(VCSContext&			inContext);
		virtual						~VCSComment				(void);

		virtual	CWVCSItemStatus		ProcessRegularFile 		(CWVCSItem&				inItem);
	};
