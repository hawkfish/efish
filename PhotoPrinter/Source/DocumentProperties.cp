/*
	File:		DocumentProperties.cp

	Contains:	Implementation of 

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		21 Aug 2000		drd		Added title-related data (serialization not quite complete)
		11 Aug 2000		drd		Removed mFull, mEmpty
*/

#include "DocumentProperties.h"

#include "PhotoPrintPrefs.h"
#include "xmlinput.h"
#include "xmloutput.h"

const char	*const DocumentProperties::gTitleLabels[] =
{
	"Header", "Footer", "none"
};

/*
DocumentProperties
	Constructor
*/
DocumentProperties::DocumentProperties(bool inDirty, DisplayStateT inState)
	: mTitlePosition(kFooter)
{
	SetDirty (inDirty);
	SetState (inState);

	PhotoPrintPrefs*	prefs = PhotoPrintPrefs::Singleton();
	this->SetFontNumber(prefs->GetFontNumber());
	this->SetFontSize(prefs->GetFontSize());
}//end full ct

/*
DocumentProperties
	Copy constructor
*/
DocumentProperties::DocumentProperties(DocumentProperties& other)
{
	SetDirty(other.GetDirty());
	SetState(other.GetState());

	this->SetFontNumber(other.GetFontNumber());
	this->SetFontSize(other.GetFontSize());
	this->SetTitlePosition(other.GetTitlePosition());
}//end copy ct

/*
DocumentProperties
	Default constructor
*/
DocumentProperties::DocumentProperties()
	: mTitlePosition(kFooter)
{
	SetDirty (false);
	SetState (kNormalState);

	PhotoPrintPrefs*	prefs = PhotoPrintPrefs::Singleton();
	this->SetFontNumber(prefs->GetFontNumber());
	this->SetFontSize(prefs->GetFontSize());
}//end

/*
~DocumentProperties
	Destructor
*/
DocumentProperties::~DocumentProperties() 
{
}

#pragma mark -

//********** Getters **************/
bool	DocumentProperties::GetDirty(void) const {return mDirty;};
DocumentProperties::DisplayStateT
		DocumentProperties::GetState(void) const {return mState;};

//********** Setters **************/
void		DocumentProperties::SetDirty(const bool inVal) {mDirty = inVal;}

/*
SetFontName
*/
void
DocumentProperties::SetFontName(ConstStr255Param inName)
{
	::GetFNum(inName, &mFontNumber);		// ??? doesn't check validity
} // SetFontName

void		DocumentProperties::SetState(const DisplayStateT inVal) {mState = inVal;};

#pragma mark -

/*
ParseProperties [static]
*/
void	
DocumentProperties::ParseProperties(XML::Element &elem, void *userData)
{
	DocumentProperties*		props = static_cast<DocumentProperties*>(userData);
	props->Read(elem);
} // ParseProperties

/*
ParseTitlePosition [static]
*/
void
DocumentProperties::ParseTitlePosition(XML::Element &elem, void *userData)
{
	TitlePositionT*		pos = static_cast<TitlePositionT*>(userData);

	XML::Char			tmp[64];
	size_t len = elem.ReadData(tmp, sizeof(tmp));
	tmp[len] = 0;

	*pos = kHeader;		// !!! do a lookup
} // ParseTitlePosition

/*
Read
*/
void 		
DocumentProperties::Read(XML::Element &elem)
{
	char	title[256];
	title[0] = 0;

	XML::Handler handlers[] = {
		XML::Handler("titlePosition", ParseTitlePosition, &mTitlePosition),
		XML::Handler("title", title, sizeof(title)),
		// SInt16			mFontNumber;	// !!! convert from name
		XML::Handler("fontSize", &mFontSize),
		}; //handlers
	elem.Process(handlers, this);

	if (strlen(title)) {
		mTitle = title;
	}
}//end Read

/*
Write
*/
void 		
DocumentProperties::Write(XML::Output &out) const
{
	// Title
	MPString		terminated(mTitle);
	terminated += (unsigned char)'\0';
	out.WriteElement("title", terminated.Chars());
	
	Str255			font;
	::GetFontName(mFontNumber, font);
	terminated = mTitle;
	terminated += (unsigned char)'\0';
	out.WriteElement("fontName", terminated.Chars());

	out.WriteElement("fontSize", mFontSize);
}//end Write
