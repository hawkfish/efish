//	DocumentProperties.h
//		Copyright © 2000 Electric Fish, Inc. All rights reserved.

/******** Change Log (most recent first ***************
*/
#pragma once

class DocumentProperties {
	public:
		enum DisplayState {
			kNormalState = 0,
			kPreviewState,
			kPrintingState,
			kFnordState };

	protected:
		bool			dirty;
		DisplayState 	state;
		
		// we might want to enable/warn depending on the following.
		// it is possible to be both not-full and not-empty
		bool			full; // are all subs full?
		bool			empty; // is anything not-full 
	
	public:
					DocumentProperties(bool inDirty, DisplayState inState, 
										bool inFull, bool inEmpty);					
					DocumentProperties(DocumentProperties& other);
					DocumentProperties();
		virtual		~DocumentProperties();


					bool		 	GetDirty(void) const;
					bool			GetFull(void) const;
					DisplayState 	GetState(void) const;
					bool			GetEmpty(void) const;

					void		SetDirty(bool inVal);
					void		SetFull(bool inVal);
					void		SetState(DisplayState inVal);
					void		SetEmpty(bool inVal);
	};//end class DocumentProperties