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
		
		bool	mCanRotate;
		bool	mCanResize;
		bool	mCanMove;
		
		bool	mMaximize;
		bool	mMaintainAspect;
		bool 	mCenter;
		
		bool 	mFullSize;

	public:
		
				PhotoItemProperties();
				PhotoItemProperties(bool inRotate, bool inResize, bool inMove,
									bool inAspect, bool inMax, bool center, 
									bool inFullSize);
				PhotoItemProperties(const PhotoItemProperties& other);
		virtual ~PhotoItemProperties();
		
		
			bool	GetRotate() const;
			bool	GetResize() const;
			bool	GetMove() const;
			
			bool	GetMaximize() const;
			bool 	GetAspect() const;
			bool	GetCenter() const;
			bool 	GetFullSize() const;
			
			void 	SetRotate(bool inVal);
			void	SetResize(bool inVal);
			void	SetMove	(bool inVal);

			void	SetMaximize(bool inVal);
			void 	SetAspect(bool inVal);
			void 	SetCenter(bool inVal);		
			void	SetFullSize(bool inVal);

// IO
			void 	Write(XML::Output &out) const;
			void 	Read(XML::Element &elem);
			static	void	sParseProperties(XML::Element &elem, void *userData);


	};//end class PhotoItemProperties
