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
	}//end empty ct


//----------------------------------------
// fully specified ct
//----------------------------------------
PhotoItemProperties::PhotoItemProperties(Boolean inRotate, Boolean inMax, Boolean inAspect)
{
	SetRotate(inRotate);
	SetMaximize(inMax);
	SetAspect(inAspect);	
	}//end fully specified ct
	

//----------------------------------------
// copy ct
//----------------------------------------
PhotoItemProperties::PhotoItemProperties(const PhotoItemProperties& other) {
	SetRotate(other.GetRotate());
	SetMaximize(other.GetMaximize());
	SetAspect(other.GetAspect());
	}//end copy ct

//----------------------------------------
// ~PhotoItemProperties
//----------------------------------------
PhotoItemProperties::~PhotoItemProperties() {
}//end dt


//----------------------------------------
// Getters
//----------------------------------------
Boolean	PhotoItemProperties::GetRotate() const {return canRotate;};
Boolean	PhotoItemProperties::GetMaximize() const {return maximize;};
Boolean PhotoItemProperties::GetAspect() const {return maintainAspect;};

//----------------------------------------
// Setters
//----------------------------------------
void 	PhotoItemProperties::SetRotate(Boolean inVal) {canRotate = inVal;};
void	PhotoItemProperties::SetMaximize(Boolean inVal) {maximize = inVal;};
void 	PhotoItemProperties::SetAspect(Boolean inVal) {maintainAspect = inVal;};

