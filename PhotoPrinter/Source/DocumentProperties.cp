/*
	File:		DocumentProperties.cp

	Contains:	Implementation of 

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		05 sep 2001		dml		348 terminate handlers list in Read
		05 Oct 2000		drd		Use std:: with map, less
		14 sep 2000		dml		add header/footer support
		21 Aug 2000		drd		Use TitlePositionMapper
		21 Aug 2000		drd		Added title-related data (serialization not quite complete)
		11 Aug 2000		drd		Removed mFull, mEmpty
*/

#include "DocumentProperties.h"

#include "PhotoPrintPrefs.h"
#include "xmlinput.h"
#include "xmloutput.h"
#include "PhotoPrintApp.h"

//---------------------------------------------------------------
// support for the map between TitlePositionT and text
typedef	std::map<TitlePositionT, char*, std::less<TitlePositionT> > TitlePositionMap;

class TitlePositionMapper {
public :	
	static const char*			Find(const TitlePositionT key);
	static TitlePositionT		Lookup(const char* text);

protected:
	static	void				Initialize();

	static	bool				gInitialized;
	static	TitlePositionMap	gMap;
};//end class TitlePositionMapper
	
bool				TitlePositionMapper::gInitialized = false;
TitlePositionMap	TitlePositionMapper::gMap;

/*
Find
*/
const char*
TitlePositionMapper::Find(const TitlePositionT inKey) {
	if (!gInitialized) {
		Initialize();
	}//endif need to construct

		TitlePositionMap::const_iterator	i = gMap.find(inKey);
		if (i != gMap.end()) 
			return (*i).second;
		else
			return 0;
}//end Find
	
/*
Initialize
*/
void
TitlePositionMapper::Initialize() {
	gMap[kHeader] = "Header";
	gMap[kFooter] = "Footer";
	gMap[kNoTitle] = "none";
	gInitialized = true;
}//end

/*
Lookup
*/
TitlePositionT
TitlePositionMapper::Lookup(const char* text) {
	if (!gInitialized) {
		Initialize();
	}//endif need to construct

	for (TitlePositionMap::const_iterator	i = gMap.begin(); i != gMap.end(); ++i) {
		if (strcmp((*i).second, text) == 0) {
			return (*i).first;
		}//endif
	}//end

	return kNoTitle;
}//end Lookup

#pragma mark -

/*
DocumentProperties
	Constructor
*/
DocumentProperties::DocumentProperties(bool inDirty, DisplayStateT inState, TitlePositionT inPosition,
										const MPString inHeader, const MPString inFooter)
{
	SetDirty (inDirty);
	SetState (inState);
	SetTitlePosition(inPosition);
	SetHeader (inHeader);
	SetFooter (inFooter);
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
	this->SetHeader(other.GetHeader());
	this->SetFooter(other.GetFooter());
	this->SetTitlePosition(other.GetTitlePosition());
}//end copy ct

/*
DocumentProperties
	Default constructor
*/
DocumentProperties::DocumentProperties()
{
	SetDirty (false);
	SetState (kNormalState);
	SetHeader(PhotoPrintApp::gAnnoyanceText);
	SetFooter(PhotoPrintApp::gAnnoyanceText);
	SetTitlePosition(kNoTitle);
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

	*pos = TitlePositionMapper::Lookup(tmp);	
} // ParseTitlePosition

/*
Read
*/
void 		
DocumentProperties::Read(XML::Element &elem)
{
	char	header[256];
	header[0] = 0;
	char 	footer[256];
	footer[0] = 0;
	char	fontName[256];
	fontName[0] = 0;

	XML::Handler handlers[] = {
		XML::Handler("titlePosition", ParseTitlePosition, &mTitlePosition),
		XML::Handler("header", header, sizeof(header)),
		XML::Handler("footer", footer, sizeof(footer)),
		XML::Handler("fontName", fontName, sizeof(fontName)),
		XML::Handler("fontSize", &mFontSize),
		XML::Handler::END
	}; //handlers
	elem.Process(handlers, this);

	if (strlen(header)) {
		mHeader = header;
	}
	
	if (strlen(footer))
		mFooter = footer;

	if (strlen(fontName)) {
		MPString	name(fontName);
		this->SetFontName(name);
	}
}//end Read

/*
Write
*/
void 		
DocumentProperties::Write(XML::Output &out) const
{
	// Title
	MPString		terminated(mFooter);
	terminated += (unsigned char)'\0';
	out.WriteElement("footer", terminated.Chars());

	terminated = mHeader;
	terminated += (unsigned char)'\0';
	out.WriteElement("header", terminated.Chars());
	

	out.WriteElement("titlePosition", 	TitlePositionMapper::Find(this->GetTitlePosition()));
	
	Str255			font;
	::GetFontName(mFontNumber, font);
	terminated = font;
	terminated += (unsigned char)'\0';
	out.WriteElement("fontName", terminated.Chars());

	out.WriteElement("fontSize", mFontSize);
}//end Write
