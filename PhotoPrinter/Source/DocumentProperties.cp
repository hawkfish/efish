//	DocumentProperties.cp
//		Copyright © 2000 Electric Fish, Inc. All rights reserved.


/******** Change Log (most recent first ***************
		11 Aug 2000		drd		Removed mFull, mEmpty
*/

#include "DocumentProperties.h"

DocumentProperties::DocumentProperties(bool inDirty, DisplayState inState)
{
	SetDirty (inDirty);
	SetState (inState);
}//end full ct

					
DocumentProperties::DocumentProperties(DocumentProperties& other)
{
	SetDirty(other.GetDirty());
	SetState(other.GetState());
	}//end copy ct

DocumentProperties::DocumentProperties()
{
	SetDirty (false);
	SetState (kNormalState);
	}//end


DocumentProperties::~DocumentProperties() 
{}


//********** Getters **************/
bool	DocumentProperties::GetDirty(void) const {return mDirty;};
DocumentProperties::DisplayState 
		DocumentProperties::GetState(void) const {return mState;};

//********** Setters **************/
void		DocumentProperties::SetDirty(bool inVal) {mDirty = inVal;};
void		DocumentProperties::SetState(DisplayState inVal) {mState = inVal;};



void 		
DocumentProperties::Write(XML::Output &/*out*/) const
{
}//end Write

void 		
DocumentProperties::Read(XML::Element &/*elem*/)
{
}//end Read

