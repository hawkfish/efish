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
		Boolean			dirty;
		DisplayState 	state;
		
		// we might want to enable/warn depending on the following.
		// it is possible to be both not-full and not-empty
		Boolean			full; // are all subs full?
		Boolean			empty; // is anything not-full 
	
	public:
					DocumentProperties(Boolean inDirty, DisplayState inState, 
										Boolean inFull, Boolean inEmpty);					
					DocumentProperties(DocumentProperties& other);
					DocumentProperties();
		virtual		~DocumentProperties();


					Boolean		 	GetDirty(void) const;
					Boolean			GetFull(void) const;
					DisplayState 	GetState(void) const;
					Boolean			GetEmpty(void) const;

					void		SetDirty(Boolean inVal);
					void		SetFull(Boolean inVal);
					void		SetState(DisplayState inVal);
					void		SetEmpty(Boolean inVal);
	};//end class DocumentProperties