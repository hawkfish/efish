/*
	File:		PhotoItemProperties.cp

	Contains:	properties that an item might use, but which aren't intrinsic to an Item

	Written by:	Dav Lion

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		14 Jun 2000		dml		Added AlignmentType (and associated map classes), alphabetized
*/
#include "PhotoItemProperties.h"
#include "xmlinput.h"
#include "xmloutput.h"


#pragma mark -
bool PhotoItemProperties::AlignmentMapper::sInitialized = false;
PhotoItemProperties::AlignmentMap PhotoItemProperties::AlignmentMapper::sAlignmentMap;

const char*
PhotoItemProperties::AlignmentMapper::Find(AlignmentType key) {
	if (!sInitialized) {
		Initialize();
		}//endif need to construct

		AlignmentMap::const_iterator	i (sAlignmentMap.find (key));
		if (i != sAlignmentMap.end ()) 
			return (*i).second;
		else
			return 0;
	}//end Find


void
PhotoItemProperties::AlignmentMapper::Initialize() {
    sAlignmentMap[kAlignNone] = "AlignNone";
    sAlignmentMap[kAlignVerticalCenter] = "AlignVerticalCenter";
    sAlignmentMap[kAlignTop] ="AlignTop";
    sAlignmentMap[kAlignBottom] ="AlignBottom";
    sAlignmentMap[kAlignHorizontalCenter] ="AlignHorizontalCenter";
    sAlignmentMap[kAlignAbsoluteCenter] ="AlignAbsoluteCenter";
    sAlignmentMap[kAlignCenterTop] ="AlignCenterTop";
    sAlignmentMap[kAlignCenterBottom] ="AlignCenterBottom";
    sAlignmentMap[kAlignLeft] ="AlignLeft";
    sAlignmentMap[kAlignCenterLeft] ="AlignCenterLeft";
    sAlignmentMap[kAlignTopLeft] ="AlignTopLeft";
    sAlignmentMap[kAlignBottomLeft] ="AlignBottomLeft";
    sAlignmentMap[kAlignRight] ="AlignRight";
    sAlignmentMap[kAlignCenterRight] = "AlignCenterRight";
    sAlignmentMap[kAlignTopRight] = "AlignTopRight";
    sAlignmentMap[kAlignBottomRight] = "AlignBottomRight";
	sInitialized = true;
	}//end Initialize


AlignmentType
PhotoItemProperties::AlignmentMapper::Lookup(const char* text) {
	if (!sInitialized) {
		Initialize();
		}//endif need to construct

	for (AlignmentMap::const_iterator	i = sAlignmentMap.begin(); i != sAlignmentMap.end(); ++i) {
		if (strcmp((*i).second, text) == 0) {
			return (*i).first;
			}//endif
		}//end

	return kAlignNone;
}//end Lookup

#pragma mark -
//----------------------------------------
// empty ct is most permissive
//----------------------------------------
PhotoItemProperties::PhotoItemProperties() {
	SetRotate(true);
	SetMaximize(false);
	SetAspect(false);	
	SetFullSize(false);
	SetAlignment(kAlignNone);
	}//end empty ct


//----------------------------------------
// fully specified ct
//----------------------------------------
PhotoItemProperties::PhotoItemProperties(bool inRotate, bool inResize, bool inMove,
											bool inMax, bool inAspect, 
											bool inFullSize, AlignmentType inAlignment)
{
	SetRotate(inRotate);
	SetResize(inResize);
	SetMove(inMove);
	SetMaximize(inMax);
	SetAspect(inAspect);	
	SetFullSize(inFullSize);
	SetAlignment(inAlignment);
	}//end fully specified ct
	

//----------------------------------------
// copy ct
//----------------------------------------
PhotoItemProperties::PhotoItemProperties(const PhotoItemProperties& other) {
	SetRotate(other.GetRotate());
	SetResize(other.GetResize());
	SetMove(other.GetMove());
	SetMaximize(other.GetMaximize());
	SetAspect(other.GetAspect());
	SetAlignment(other.GetAlignment());
	}//end copy ct

//----------------------------------------
// ~PhotoItemProperties
//----------------------------------------
PhotoItemProperties::~PhotoItemProperties() {
}//end dt


//----------------------------------------
// Getters.  all trivial
//----------------------------------------
AlignmentType 	PhotoItemProperties::GetAlignment() const {return mAlignment;};
bool 			PhotoItemProperties::GetAspect() const {return mMaintainAspect;};
bool 			PhotoItemProperties::GetFullSize() const {return mFullSize;};
bool			PhotoItemProperties::GetMaximize() const {return mMaximize;};
bool			PhotoItemProperties::GetMove() const {return mCanMove;};
bool			PhotoItemProperties::GetResize() const {return mCanResize;};
bool			PhotoItemProperties::GetRotate() const {return mCanRotate;};

//----------------------------------------
// Setters  
//----------------------------------------
void	PhotoItemProperties::SetAlignment(AlignmentType inAlign) {mAlignment = inAlign;};
void 	PhotoItemProperties::SetAspect(bool inVal) {mMaintainAspect = inVal;};

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

void	PhotoItemProperties::SetMaximize(bool inVal) {mMaximize = inVal;};
void 	PhotoItemProperties::SetMove(bool inVal) {mCanMove = inVal;};
void 	PhotoItemProperties::SetResize(bool inVal) {mCanResize = inVal;};
void 	PhotoItemProperties::SetRotate(bool inVal) {mCanRotate = inVal;};

	
	
	
#pragma mark -
//------------------------------------
// I/O		based on xmlio library
//------------------------------------
void PhotoItemProperties::Write(XML::Output &out) const
{
	// <name>(X,Y)</name>
	out.WriteElement("alignment", mAlignmentMap.Find(mAlignment));
	out.WriteElement("aspect", mMaintainAspect);
	out.WriteElement("fullSize", mFullSize);
	out.WriteElement("maximize", mMaximize);
	out.WriteElement("move", mCanMove);
	out.WriteElement("resize", mCanResize);
	out.WriteElement("rotate", mCanRotate);
}

void PhotoItemProperties::Read(XML::Element &elem)
{
	XML::Handler handlers[] = {
		XML::Handler("alignment", sParseAlignment, &mAlignment),
		XML::Handler("aspect", &mMaintainAspect),
		XML::Handler("fullSize", &mFullSize),
		XML::Handler("maximize", &mMaximize),
		XML::Handler("move", &mCanMove),
		XML::Handler("resize", &mCanResize),
		XML::Handler("rotate", &mCanRotate),
		XML::Handler::END
		}; //handlers
	elem.Process(handlers, this);
}


void
PhotoItemProperties::ParseAlignment(XML::Element &elem, void *userData) {
	AlignmentType* pAlignment ((AlignmentType*)userData);

	XML::Char tmp[64];
	size_t len = elem.ReadData(tmp, sizeof(tmp));
	tmp[len] = 0;
	
	*pAlignment = mAlignmentMap.Lookup(tmp);	
	}//end ParseAlignment


void
PhotoItemProperties::sParseAlignment(XML::Element &elem, void *userData) {
	((PhotoItemProperties *)userData)->ParseAlignment(elem, userData);
	}// StaticParseBound


void	
PhotoItemProperties::sParseProperties(XML::Element &elem, void *userData)
{
	PhotoItemProperties*	props = (PhotoItemProperties*)userData;
	props->Read(elem);
	
}//end

	