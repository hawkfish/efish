// PhotoItemProperties.h
// Copyright © 2000 Electric Fish, Inc.  All Rights Reserved
#pragma once
#include "HORef.h"

class PhotoItemProperties {
	protected:
		
		Boolean	canRotate;
		Boolean	maximize;
		Boolean	maintainAspect;

	public:
		
				PhotoItemProperties();
				PhotoItemProperties(Boolean inRotate, Boolean inAspect, Boolean inMax);
				PhotoItemProperties(const PhotoItemProperties& other);
		virtual ~PhotoItemProperties();
		
		
			Boolean	GetRotate() const;
			Boolean	GetMaximize() const;
			Boolean GetAspect() const;
			
			void 	SetRotate(Boolean inVal);
			void	SetMaximize(Boolean inVal);
			void 	SetAspect(Boolean inVal);
		
	};//end class PhotoItemProperties
