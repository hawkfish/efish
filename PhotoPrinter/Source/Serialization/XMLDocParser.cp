/*
	File:		XMLDocParser.cp

	Contains:	Implementation of an XML parser for a document.

	Written by:	Richard Wesley

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		31 Jul 2001		drd		256 Fixed types so HandleMinMax works
		31 Jul 2001		drd		256 Parse maximumSize, minimumSize
		26 Jul 2001		rmgw	Add EUserMessage. Bug #228.
		26 Jul 2001		rmgw	Factored from PhotoPrintDoc.  Bug #228.
*/

#include "XMLDocParser.h"

#include "LayoutMapper.h"
#include "PhotoPrintDoc.h"
#include "PhotoPrintPrefs.h"

#include "StDisableBroadcaster.h"

#include "xmlinput.h"

// ---------------------------------------------------------------------------
//	¥ XMLDocParser
// ---------------------------------------------------------------------------

XMLDocParser::XMLDocParser (

	XML::Input&			inInput,
	EUserMessage&		inMessage,
	PhotoPrintDoc*		inDoc) 
	
	: XMLItemParser (inInput, inMessage)
	
	, mDoc (inDoc)
	
	{ // begin XMLDocParser
	
	} // end XMLDocParser

// ---------------------------------------------------------------------------
//	¥ ~XMLDocParser
// ---------------------------------------------------------------------------

XMLDocParser::~XMLDocParser (void) 
	
	{ // begin ~XMLDocParser
	
	} // end ~XMLDocParser

#pragma mark -

// ---------------------------------------------------------------------------
//	¥ HandleDocument
// ---------------------------------------------------------------------------

void
XMLDocParser::HandleDocument (

	XML::Element&	elem, 
	void* 			userData) 
	
	{ // begin HandleDocument
	
		XMLDocParser*		that = static_cast<XMLDocParser*>(userData);
		that->OnDocumentRead (that->ReadDocument (elem));

	} // end HandleDocument

// ---------------------------------------------------------------------------
//	¥ HandleLayout
// ---------------------------------------------------------------------------

void
XMLDocParser::HandleLayout (

	XML::Element&	elem, 
	void *			userData)

	{ // begin HandleLayout
	
		OSType* pLayout ((OSType*) userData);

		XML::Char tmp[64];
		size_t len = elem.ReadData(tmp, sizeof(tmp));
		tmp[len] = 0;
		
		*pLayout = LayoutMapper::Lookup(tmp);	

	} // end HandleLayout

// ---------------------------------------------------------------------------
//	¥ HandleMinMax
// ---------------------------------------------------------------------------
void
XMLDocParser::HandleMinMax (
	XML::Element&	elem, 
	void *			outUserData)
{
	SInt16*		pEnum = (SInt16*) outUserData;

	XML::Char	bufr[64];
	size_t		len = elem.ReadData(bufr, sizeof(bufr));
	bufr[len] = 0;

	SizeLimitMap::const_iterator	i;
	for (i = PhotoPrintPrefs::gSizeLimitMap.begin(); i != PhotoPrintPrefs::gSizeLimitMap.end(); ++i) {
		if (std::strcmp((*i).second, bufr) == 0) {
			*pEnum = (*i).first;
			return;
		} //endif
	} // end for
} // HandleMinMax

// ---------------------------------------------------------------------------
//	¥ OnDocumentRead
// ---------------------------------------------------------------------------

void
XMLDocParser::OnDocumentRead (

	PhotoPrintDoc*	/*inDoc*/)

	{ // begin OnDocumentRead
	
	} // end OnDocumentRead

// ---------------------------------------------------------------------------
//	¥ OnItemRead
// ---------------------------------------------------------------------------

void
XMLDocParser::OnItemRead (

	PhotoItemRef	inItem)

	{ // begin OnItemRead
	
		PhotoPrintModel*	model = mDoc->GetView()->GetModel();
		model->AdoptNewItem (inItem, model->end ());	

	} // end OnItemRead

// ---------------------------------------------------------------------------
//	¥ ParseObjects
// ---------------------------------------------------------------------------

void
XMLDocParser::ParseDocument (void) 
	
	{ // begin ParseDocument
	
		XML::Handler handlers[] = {
			XML::Handler("Document", HandleDocument),
			XML::Handler::END
			};
			
		// 95 Get rid of current data before reading from the file
		mDoc->GetView()->GetModel()->RemoveAllItems();

		mInput.Process (handlers, static_cast<void*>(this));

	} // ParseObjects

// ---------------------------------------------------------------------------
//	¥ ReadDocument
// ---------------------------------------------------------------------------

PhotoPrintDoc* 
XMLDocParser::ReadDocument (

	XML::Element &elem)

	{ // begin ReadDocument

		double				minVal (0.0);
		double				maxVal (200000.0);
		Layout::LayoutType	layoutType;
		UInt16				imageCount = 1;
		
		DocumentProperties	theDocProps (mDoc->GetProperties ());
		PrintProperties		thePrintProps (mDoc->GetPrintProperties ());
		double				theWidth (mDoc->GetWidth ());
		double				theHeight (mDoc->GetHeight ());
		SInt16				theDPI (mDoc->GetResolution ());
		SInt16				maximumSize = mDoc->GetMaximumSize();
		SInt16				minimumSize = mDoc->GetMinimumSize();
		
		XML::Handler handlers[] = {
			XML::Handler("Document_Properties", DocumentProperties::ParseProperties, &theDocProps),
			XML::Handler("Print_Properties", PrintProperties::sParseProperties, &thePrintProps),
			XML::Handler("Objects", HandleItems),
			XML::Handler("width", &theWidth, &minVal, &maxVal),
			XML::Handler("height", &theHeight, &minVal, &maxVal),
			XML::Handler("dpi", &theDPI),
			XML::Handler("layout", HandleLayout, &layoutType),
			XML::Handler("image_count", &imageCount),
			XML::Handler("maximum_size", HandleMinMax, (void*)&maximumSize),
			XML::Handler("minimum_size", HandleMinMax, (void*)&minimumSize),
			XML::Handler::END
		};
			
		elem.Process (handlers, this);
		
		mDoc->GetProperties () = theDocProps;
		mDoc->GetPrintProperties () = thePrintProps;
		mDoc->SetWidth (theWidth);
		mDoc->SetHeight (theHeight);
		mDoc->SetMaximumSize((SizeLimitT) maximumSize, PhotoPrintDoc::kMinimal);
		mDoc->SetMinimumSize((SizeLimitT) minimumSize, PhotoPrintDoc::kMinimal);
		
		// Set the orientation to match the implicitly saved one
		EPrintSpec*		spec = mDoc->GetPrintRec();
		OSType			orientation;
		if (theWidth > theHeight)
			orientation = kLandscape;
		else
			orientation = kPortrait;

		spec->SetOrientation(orientation, PhotoUtility::gNeedDoubleOrientationSetting);
		mDoc->MatchViewToPrintRec(mDoc->GetPageCount ());

		// 206.  Call SwitchLayout with full info
		mDoc->GetView()->SwitchLayout(layoutType, imageCount);
		
		mDoc->SetResolution (theDPI);

		return mDoc;
		
	} // end ReadDocument
