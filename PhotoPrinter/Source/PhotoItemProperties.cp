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
bool	PhotoItemProperties::GetRotate() const {return mCanRotate;};
bool	PhotoItemProperties::GetMaximize() const {return mMaximize;};
bool 	PhotoItemProperties::GetAspect() const {return mMaintainAspect;};
bool 	PhotoItemProperties::GetCenter() const {return mCenter;};
bool 	PhotoItemProperties::GetFullSize() const {return mFullSize;};


//----------------------------------------
// Setters  (if simple and orthagonal, inlined immediately below.  else, expanded below
//----------------------------------------
void 	PhotoItemProperties::SetRotate(bool inVal) {mCanRotate = inVal;};
void	PhotoItemProperties::SetMaximize(bool inVal) {mMaximize = inVal;};
void 	PhotoItemProperties::SetAspect(bool inVal) {mMaintainAspect = inVal;};
void	PhotoItemProperties::SetCenter(bool inVal) {mCenter = inVal;};

//------------------------------------
// SetFullSize.  If true, turn Maximize OFF, MaintainAspect ON
//------------------------------------
void	
PhotoItemProperties::SetFullSize(bool inVal) 
{
	mFullSize = inVal;
	if (inVal == true) {
		SetMaximize(false);
		SetAspect(true);
		}//end
	}//end SetFullSize
	
	
	
#pragma mark -
//------------------------------------
// I/O		based on xmlio library
//------------------------------------


void PhotoItemProperties::Write(XML::Output &out) const
{
	// <name>(X,Y)</name>
	out.WriteElement("canRotate", mCanRotate);
	out.WriteElement("maximize", mMaximize);
	out.WriteElement("maintainAspect", mMaintainAspect);
	out.WriteElement("center", mCenter);
	out.WriteElement("fullSize", mFullSize);
}

void PhotoItemProperties::Read(XML::Element &elem)
{
	XML::Handler handlers[] = {
		XML::Handler("canRotate", &mCanRotate),
		XML::Handler("maximize", &mMaximize),
		XML::Handler("maintainAspect", &mMaintainAspect),
		XML::Handler("center", &mCenter),
		XML::Handler("fullSize", &mFullSize),
		XML::Handler::END
		}; //handlers
	elem.Process(handlers, this);
}

void	
PhotoItemProperties::sParseProperties(XML::Element &elem, void *userData)
{
	PhotoItemProperties*	props = (PhotoItemProperties*)userData;
	props->Read(elem);
	
}//end

	