//	DocumentProperties.cp
//		Copyright © 2000 Electric Fish, Inc. All rights reserved.


/******** Change Log (most recent first ***************
*/

#include "DocumentProperties.h"


DocumentProperties::DocumentProperties(Boolean inDirty, DisplayState inState, 
										Boolean inFull, Boolean inEmpty)
{
	SetDirty (inDirty);
	SetState (inState);
	SetFull (inFull);
	SetEmpty (inEmpty);
}//end full ct

					
DocumentProperties::DocumentProperties(DocumentProperties& other)
{
	SetDirty(other.GetDirty());
	SetState(other.GetState());
	SetFull(other.GetFull());
	SetEmpty (other.GetEmpty());
	}//end copy ct

DocumentProperties::DocumentProperties() {
	SetFull (false);
	SetDirty (false);
	SetState (kNormalState);
	SetEmpty (true);
	}//end


DocumentProperties::~DocumentProperties() 
{}


//********** Getters **************/
Boolean		DocumentProperties::GetDirty(void) const {return dirty;};
Boolean 	DocumentProperties::GetFull(void) const {return full;};
DocumentProperties::DisplayState DocumentProperties::GetState(void) const {return state;};
Boolean		DocumentProperties::GetEmpty(void) const {return empty;};

//********** Setters **************/
void		DocumentProperties::SetDirty(Boolean inVal) {dirty = inVal;};
void		DocumentProperties::SetFull(Boolean inVal) {full = inVal;};
void		DocumentProperties::SetState(DisplayState inVal) {state = inVal;};
void		DocumentProperties::SetEmpty(Boolean inVal) {empty = inVal;};
