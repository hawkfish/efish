//	DocumentProperties.h
//		Copyright © 2000 Electric Fish, Inc. All rights reserved.

/******** Change Log (most recent first ***************
		11 Aug 2000		drd		Removed mFull, mEmpty
*/

#pragma once

namespace XML {
	class Output;
	class Element;
	class Handler;
}

class DocumentProperties {
	public:
		enum DisplayState {
			kNormalState = 0,
			kPreviewState,
			kPrintingState,
			kFnordState };

	protected:
		bool			mDirty;
		DisplayState 	mState;
		
	public:
					DocumentProperties(bool inDirty, DisplayState inState);					
					DocumentProperties(DocumentProperties& other);
					DocumentProperties();
		virtual		~DocumentProperties();


					bool		 	GetDirty(void) const;
					DisplayState 	GetState(void) const;

					void		SetDirty(bool inVal);
					void		SetState(DisplayState inVal);

// IO
					void 		Write(XML::Output &out) const;
					void 		Read(XML::Element &elem);
}; //end class DocumentProperties
