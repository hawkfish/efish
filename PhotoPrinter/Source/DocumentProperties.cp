//	DocumentProperties.cp
//		Copyright © 2000 Electric Fish, Inc. All rights reserved.


/******** Change Log (most recent first ***************
*/

#include "DocumentProperties.h"


DocumentProperties::DocumentProperties(bool inDirty, DisplayState inState, 
										bool inFull, bool inEmpty)
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
bool	DocumentProperties::GetDirty(void) const {return mDirty;};
DocumentProperties::DisplayState 
		DocumentProperties::GetState(void) const {return mState;};
bool		DocumentProperties::GetEmpty(void) const {return mEmpty;};
bool 	DocumentProperties::GetFull(void) const {return mFull;};

//********** Setters **************/
void		DocumentProperties::SetDirty(bool inVal) {mDirty = inVal;};
void		DocumentProperties::SetFull(bool inVal) {mFull = inVal;};
void		DocumentProperties::SetState(DisplayState inVal) {mState = inVal;};
void		DocumentProperties::SetEmpty(bool inVal) {mEmpty = inVal;};



void 		
DocumentProperties::Write(XML::Output &/*out*/) const
{
}//end Write

void 		
DocumentProperties::Read(XML::Element &/*elem*/)
{
}//end Read



