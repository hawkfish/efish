// PhotoItemProperties.cp
// Copyright © 2000 Electric Fish, Inc.  All Rights Reserved
#include "PhotoItemProperties.h"
#include "xmlinput.h"
#include "xmloutput.h"

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
PhotoItemProperties::PhotoItemProperties(bool inRotate, bool inMax, 
											bool inAspect, bool inCenter,
											bool inFullSize)
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
bool	PhotoItemProperties::GetRotate() const {return canRotate;};
bool	PhotoItemProperties::GetMaximize() const {return maximize;};
bool PhotoItemProperties::GetAspect() const {return maintainAspect;};
bool PhotoItemProperties::GetCenter() const {return center;};
bool PhotoItemProperties::GetFullSize() const {return fullSize;};


//----------------------------------------
// Setters  (if simple and orthagonal, inlined immediately below.  else, expanded below
//----------------------------------------
void 	PhotoItemProperties::SetRotate(bool inVal) {canRotate = inVal;};
void	PhotoItemProperties::SetMaximize(bool inVal) {maximize = inVal;};
void 	PhotoItemProperties::SetAspect(bool inVal) {maintainAspect = inVal;};
void	PhotoItemProperties::SetCenter(bool inVal) {center = inVal;};

//------------------------------------
// SetFullSize.  If true, turn Maximize OFF, MaintainAspect ON
//------------------------------------
void	
PhotoItemProperties::SetFullSize(bool inVal) 
{
	fullSize = inVal;
	if (inVal == true) {
		SetMaximize(false);
		SetAspect(true);
		}//end
	}//end SetFullSize
	
	
	
#pragma mark -
//------------------------------------
// I/O		based on xmlio library
//------------------------------------


void PhotoItemProperties::Write(const char */*name*/, XML::Output &out) const
{
	// <name>(X,Y)</name>
	out.WriteElement("canRotate", canRotate);
	out.WriteElement("maximize", maximize);
	out.WriteElement("maintainAspect", maintainAspect);
	out.WriteElement("center", center);
	out.WriteElement("fullSize", fullSize);
}

void PhotoItemProperties::Read(XML::Element &elem)
{
	XML::Handler handlers[] = {
		XML::Handler("canRotate", &canRotate),
		XML::Handler("maximize", &maximize),
		XML::Handler("maintainAspect", &maintainAspect),
		XML::Handler("center", &center),
		XML::Handler("fullSize", &fullSize),
		XML::Handler::END
		}; //handlers
	elem.Process(handlers, this);
}

	