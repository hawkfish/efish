#pragma once

#include <MacTypes.h>

class StPtr {

		Ptr			mP;
		
					StPtr		(const	StPtr&	other);
		StPtr&		operator=	(const	StPtr&	other);
		
	public:
	
					StPtr		(Ptr	p = nil);
					~StPtr		(void);
		
		operator	Ptr			(void) const {return mP;}
	};
	
