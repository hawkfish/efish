#pragma once

#include "VCSFileCommand.h"

class VCSCheckIn : public VCSFileCommand

	{
	
	protected:	
		
		OSErr						ParseResult 			(const	FSSpec&			root,
															 const	FSSpec&			project,
															 Handle					output,
															 Handle					file);
		CWVCSItemStatus				ParseOutput 			(const	FSSpec&			root,
															 const	FSSpec&			project,
															 Handle					output);

	public:
	
									VCSCheckIn				(VCSContext&			inContext);
		virtual						~VCSCheckIn				(void);

		virtual	CWVCSItemStatus		ProcessRegularFolder 	(CWVCSItem&				inItem);
		virtual	CWVCSItemStatus		ProcessRegularFile 		(CWVCSItem&				inItem);
	};
