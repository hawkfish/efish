/*
	File:		PhotoItemProperties.h

	Contains:	properties that an item might use, but which aren't intrinsic to an Item

	Written by:	Dav Lion

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		16 jun 2000 	dml		factored alignment stuff into AlignmentGizmo
		14 Jun 2000		dml		Added AlignmentType (and associated map classes), removed mCenter
*/

#pragma once
#include "HORef.h"

namespace XML {
	class Output;
	class Element;
	class Handler;
}


class PhotoItemProperties {
	protected:
		
		// what are we allowed to do
		bool	mCanMove;
		bool	mCanResize;
		bool	mCanRotate;
		
		// what do we want to do
		bool	mMaintainAspect;
		bool	mMaximize;
		
		// how do we want to do it?
		AlignmentType	mAlignment;
		bool 			mFullSize;
			

		void			ParseAlignment(XML::Element &elem, void *userData);
		static 	void	sParseAlignment(XML::Element &elem, void *userData);

	
	public:
		
				PhotoItemProperties();
				PhotoItemProperties(bool inRotate, bool inResize, bool inMove,
									bool inAspect, bool inMax, 
									bool inFullSize, AlignmentType inAlign);
				PhotoItemProperties(const PhotoItemProperties& other);
		virtual ~PhotoItemProperties();
		
		
			AlignmentType	GetAlignment() const;
			bool 			GetAspect() const;
			bool 			GetFullSize() const;
			bool			GetMaximize() const;
			bool			GetMove() const;
			bool			GetResize() const;
			bool			GetRotate() const;
			

			void	SetAlignment(AlignmentType inAlign);
			void 	SetAspect(bool inVal);
			void	SetFullSize(bool inVal);
			void	SetMaximize(bool inVal);
			void	SetMove	(bool inVal);
			void	SetResize(bool inVal);
			void 	SetRotate(bool inVal);

// IO
			void 	Read(XML::Element &elem);
			void 	Write(XML::Output &out) const;
			static	void	sParseProperties(XML::Element &elem, void *userData);


	};//end class PhotoItemProperties
