#pragma once

#include "VCSCommand.h"

class VCSDatabaseConnect : public VCSCommand

	{
		
		CWVCSCommandStatus			Authorize				(const	FSSpec&	inCWD);
		
	public:
		
									VCSDatabaseConnect		(VCSContext&	inContext);
		virtual						~VCSDatabaseConnect		(void);
			
		virtual	CWVCSCommandStatus	DoRequest 				(void);
	};
		