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
bool		DocumentProperties::GetDirty(void) const {return dirty;};
bool 	DocumentProperties::GetFull(void) const {return full;};
DocumentProperties::DisplayState DocumentProperties::GetState(void) const {return state;};
bool		DocumentProperties::GetEmpty(void) const {return empty;};

//********** Setters **************/
void		DocumentProperties::SetDirty(bool inVal) {dirty = inVal;};
void		DocumentProperties::SetFull(bool inVal) {full = inVal;};
void		DocumentProperties::SetState(DisplayState inVal) {state = inVal;};
void		DocumentProperties::SetEmpty(bool inVal) {empty = inVal;};



void 		
DocumentProperties::Write(XML::Output &/*out*/) const
{
}//end Write

void 		
DocumentProperties::Read(XML::Element &/*elem*/)
{
}//end Read



