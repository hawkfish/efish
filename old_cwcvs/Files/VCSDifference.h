#pragma once

#include "VCSFileCommand.h"

class VCSDifference : public VCSFileCommand

	{
		
	public:
	
									VCSDifference			(VCSContext&				inContext);
		virtual						~VCSDifference			(void);

		virtual	CWVCSItemStatus		ProcessRegularFile 		(CWVCSItem&					inItem);
		virtual	CWVCSItemStatus		ProcessRegularFolder 	(CWVCSItem&					inItem);
	};
