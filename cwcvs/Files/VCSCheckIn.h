#pragma once

#include "VCSFileCommand.h"

class VCSCheckIn : public VCSFileCommand

	{
	
	protected:	
		
		OSErr						ParseResult 			(const	FSSpec&			root,
															 Handle					output,
															 Handle					file);
		CWVCSItemStatus				ParseOutput 			(const	FSSpec&			root,
															 Handle					output);

	public:
	
									VCSCheckIn				(VCSContext&			inContext);
		virtual						~VCSCheckIn				(void);

		virtual	CWVCSItemStatus		ProcessRegularFolder 	(CWVCSItem&				inItem);
		virtual	CWVCSItemStatus		ProcessRegularFile 		(CWVCSItem&				inItem);
	};
