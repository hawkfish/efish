// PhotoItemProperties.h
// Copyright © 2000 Electric Fish, Inc.  All Rights Reserved
#pragma once
#include "HORef.h"

namespace XML {
	class Output;
	class Element;
	class Handler;
}

class PhotoItemProperties {
	protected:
		
		bool	canRotate;
		bool	maximize;
		bool	maintainAspect;
		bool 	center;
		bool 	fullSize;

	public:
		
				PhotoItemProperties();
				PhotoItemProperties(bool inRotate, bool inAspect, 
									bool inMax, bool center, bool inFullSize);
				PhotoItemProperties(const PhotoItemProperties& other);
		virtual ~PhotoItemProperties();
		
		
			bool	GetRotate() const;
			bool	GetMaximize() const;
			bool 	GetAspect() const;
			bool	GetCenter() const;
			bool 	GetFullSize() const;
			
			void 	SetRotate(bool inVal);
			void	SetMaximize(bool inVal);
			void 	SetAspect(bool inVal);
			void 	SetCenter(bool inVal);		
			void	SetFullSize(bool inVal);

// IO
			void 	Write(XML::Output &out) const;
			void 	Read(XML::Element &elem);
			static	void	sParseProperties(XML::Element &elem, void *userData);


	};//end class PhotoItemProperties
