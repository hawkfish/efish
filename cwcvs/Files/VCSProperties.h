#pragma once

#include "VCSFileCommand.h"

#include <AEDataModel.h>

class VCSProperties : public VCSFileCommand

	{
		
	public:
	
									VCSProperties			(VCSContext&				inContext);
		virtual						~VCSProperties			(void);

		virtual	CWVCSItemStatus		ProcessRegularFile 		(CWVCSItem&					inItem);
		virtual	CWVCSItemStatus		ProcessRegularFolder 	(CWVCSItem&					inItem);
	};
