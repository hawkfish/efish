// PhotoItemProperties.cp
// Copyright © 2000 Electric Fish, Inc.  All Rights Reserved
#include "PhotoItemProperties.h"


//----------------------------------------
// empty ct is most permissive
//----------------------------------------
PhotoItemProperties::PhotoItemProperties() {
	SetRotate(true);
	SetMaximize(false);
	SetAspect(false);	
	SetCenter(false);	
	SetFullSize(false);
	}//end empty ct


//----------------------------------------
// fully specified ct
//----------------------------------------
PhotoItemProperties::PhotoItemProperties(Boolean inRotate, Boolean inMax, 
											Boolean inAspect, Boolean inCenter,
											Boolean inFullSize)
{
	SetRotate(inRotate);
	SetMaximize(inMax);
	SetAspect(inAspect);	
	SetCenter(inCenter);
	SetFullSize(inFullSize);
	}//end fully specified ct
	

//----------------------------------------
// copy ct
//----------------------------------------
PhotoItemProperties::PhotoItemProperties(const PhotoItemProperties& other) {
	SetRotate(other.GetRotate());
	SetMaximize(other.GetMaximize());
	SetAspect(other.GetAspect());
	SetCenter(other.GetCenter());
	}//end copy ct

//----------------------------------------
// ~PhotoItemProperties
//----------------------------------------
PhotoItemProperties::~PhotoItemProperties() {
}//end dt


//----------------------------------------
// Getters.  all trivial
//----------------------------------------
Boolean	PhotoItemProperties::GetRotate() const {return canRotate;};
Boolean	PhotoItemProperties::GetMaximize() const {return maximize;};
Boolean PhotoItemProperties::GetAspect() const {return maintainAspect;};
Boolean PhotoItemProperties::GetCenter() const {return center;};
Boolean PhotoItemProperties::GetFullSize() const {return fullSize;};


//----------------------------------------
// Setters  (if simple and orthagonal, inlined immediately below.  else, expanded below
//----------------------------------------
void 	PhotoItemProperties::SetRotate(Boolean inVal) {canRotate = inVal;};
void	PhotoItemProperties::SetMaximize(Boolean inVal) {maximize = inVal;};
void 	PhotoItemProperties::SetAspect(Boolean inVal) {maintainAspect = inVal;};
void	PhotoItemProperties::SetCenter(Boolean inVal) {center = inVal;};

//------------------------------------
// SetFullSize.  If true, turn Maximize OFF, MaintainAspect ON
//------------------------------------
void	
PhotoItemProperties::SetFullSize(Boolean inVal) 
{
	fullSize = inVal;
	if (inVal == true) {
		SetMaximize(false);
		SetAspect(true);
		}//end
	}//end SetFullSize