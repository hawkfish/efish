/*
	File:		XMLItemParser.cp

	Contains:	Implementation of an XML parser for an image.

	Written by:	Richard Wesley

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):
	
		19 aug 2001		dml		275, 282 add relative CropZoom fields
		26 Jul 2001		rmgw	Add EUserMessage. Bug #228.
		26 Jul 2001		rmgw	Factored from PhotoPrintItem. Bug #228.

*/

#include "XMLItemParser.h"

#include "PhotoExceptionHandler.h"

//	Epp
#include "EUserMessage.h"
#include "EUserMessageServer.h"

#include "xmlinput.h"

#include "MP2CStr.h"

#include <cstdio>

const	ResIDT	TEXT_XMLFileFailure	= 1142;

// ---------------------------------------------------------------------------
//	¥ XMLItemParser
// ---------------------------------------------------------------------------

XMLItemParser::XMLItemParser (

	XML::Input&			inInput,
	EUserMessage&		inMessage) 
	
	: mInput (inInput)
	, mMessage (inMessage)
	
	{ // begin XMLItemParser
		
		mMessage.SetDetails (new MNewHandle ((Size) 0));
		
	} // XMLItemParser

// ---------------------------------------------------------------------------
//	¥ ~XMLItemParser
// ---------------------------------------------------------------------------

XMLItemParser::~XMLItemParser (void) 
	
	{ // begin ~XMLItemParser
		
		try {
			if (mMessage.GetDetails () && mMessage.GetDetails ()->GetSize ()) 
				EUserMessageServer::GetSingleton ()->QueueUserMessage (mMessage);
			} // try
			
		catch (...) {
			//	Ick!
			} // catch
			
	} // end ~XMLItemParser

#pragma mark -

// ---------------------------------------------------------------------------
//	¥ HandleRect 													[static]
// ---------------------------------------------------------------------------
void 	
XMLItemParser::HandleRect (

	XML::Element&	elem, 
	void *			userData) 
	
	{ // begin HandleRect
	
		XML::Char tmp[64];
		size_t len = elem.ReadData(tmp, sizeof(tmp));
		tmp[len] = 0;
		
		Rect*	pRect ((Rect*)userData);

		SInt16 howMany = std::sscanf(tmp, "%hd,%hd,%hd,%hd", &pRect->top, &pRect->left, &pRect->bottom, &pRect->right);
		if (howMany != 4) {
			int line = elem.GetInput().GetLine();
			int col = elem.GetInput().GetColumn();
			throw new XML::InvalidValue(elem.GetInput(), line, col);
		}//endif unhappy		

	}//end HandleRect

// ---------------------------------------------------------------------------
//	¥ HandleItem
// ---------------------------------------------------------------------------

void
XMLItemParser::HandleItem (

	XML::Element &elem,
	void*		userData)

	{ // begin HandleItem
		
		XMLItemParser*		that = static_cast<XMLItemParser*>(userData);
		that->OnItemRead (that->ReadItem (elem));
		
	} // end HandleItem
	
// ---------------------------------------------------------------------------
//	¥ HandleItems
// ---------------------------------------------------------------------------

void
XMLItemParser::HandleItems (

	XML::Element&	elem, 
	void* 			userData) 
	
	{ // begin HandleItems
	
		XML::Handler handlers[] = {
			XML::Handler("photo", HandleItem),
			XML::Handler::END
			};
			
		elem.Process (handlers, userData);

	} // HandleItems

// ---------------------------------------------------------------------------
//	¥ HandleObjects
// ---------------------------------------------------------------------------

void
XMLItemParser::HandleObjects (

	XML::Element&	elem, 
	void* 			userData) 
	
	{ // begin HandleObjects
	
		XML::Handler handlers[] = {
			XML::Handler("Objects", HandleItems),
			XML::Handler::END
			};
			
		elem.Process (handlers, userData);

	} // HandleObjects

// ---------------------------------------------------------------------------
//	¥ ParseObjects
// ---------------------------------------------------------------------------

void
XMLItemParser::ParseObjects (void) 
	
	{ // begin ParseItems
	
		XML::Handler handlers[] = {
			XML::Handler("Objects", HandleItems),
			XML::Handler::END
		};
			
		mInput.Process (handlers, static_cast<void*>(this));

	} // ParseObjects

// ---------------------------------------------------------------------------
//	¥ ReadItem
// ---------------------------------------------------------------------------

PhotoItemRef
XMLItemParser::ReadItem (

	XML::Element &elem)

	{ // begin ReadItem
	
		char	filename[256];
		filename[0] = 0;
		double	minVal (-360.0);
		double	maxVal (360.0);
		
		double 	scaleMin (0.0);
		double	scaleMax (100.0);
		
		//	Constructor arguments
		Rect							theDest;
		Rect							theCaptionRect;
		Rect							theImageRect;
		Rect							theFrameRect;

		double							theXScale;
		double							theYScale;
		double							theTopCrop;
		double							theLeftCrop;
		double							theBottomCrop;
		double							theRightCrop;
		double							theTopOffset;
		double							theLeftOffset;
		
		double							theTopCZ (0.0);
		double							theLeftCZ (0.0);
		double							theBottomCZ (0.0);
		double							theRightCZ (0.0);
		
		double							theUserTopCrop (0.0);
		double							theUserLeftCrop (0.0);
		double							theUserBottomCrop (0.0);
		double							theUserRightCrop (0.0);
		
		
		PhotoItemProperties				theProperties;

		double							theRot;
		double							theSkew;

		HORef<MFileSpec>				theFileSpec;
		
		//	Parser setup
		XML::Handler handlers[] = {
			XML::Handler("bounds", HandleRect, &theDest),
			//crop stuff
			XML::Handler("topCrop", &theTopCrop),
			XML::Handler("leftCrop", &theLeftCrop),
			XML::Handler("bottomCrop", &theBottomCrop),
			XML::Handler("rightCrop", &theRightCrop),
			XML::Handler("topOffset", &theTopOffset),
			XML::Handler("leftOffset", &theLeftOffset),
			XML::Handler("xScale", &theXScale, &scaleMin, &scaleMax),
			XML::Handler("yScale", &theYScale, &scaleMin, &scaleMax),
			XML::Handler("topCZ", &theTopCZ),
			XML::Handler("leftCZ", &theLeftCZ),
			XML::Handler("bottomCZ", &theBottomCZ),
			XML::Handler("rightCZ", &theRightCZ),
						
			XML::Handler("userTopCrop", &theUserTopCrop),
			XML::Handler("userLeftCrop", &theUserLeftCrop),
			XML::Handler("userBottomCrop", &theUserBottomCrop),
			XML::Handler("userRightCrop", &theUserRightCrop),

			XML::Handler("imageRect", HandleRect, (void*)&theImageRect),
			XML::Handler("captionRect", HandleRect, (void*)&theCaptionRect),
			XML::Handler("frameRect", HandleRect, (void*)&theFrameRect),		
			XML::Handler("filename", filename, sizeof(filename)),
			XML::Handler("rotation", &theRot, &minVal, &maxVal),
			XML::Handler("skew", &theSkew, &minVal, &maxVal),
			XML::Handler("properties", PhotoItemProperties::ParseProperties, (void*)&theProperties),
			XML::Handler::END
		};
		elem.Process(handlers, this);

		if (strlen(filename)) {
			try {
				theFileSpec = new MFileSpec(filename);
				} // try
				
			catch (const LException& e) {
				LStr255	str (e.GetErrorString ());
				LStr255	code;
				LStr255	desc;
				ExceptionHandler::GetErrorAndDescription (e, code, desc);
				
				EUserMessage::TextRef	oldDetails (mMessage.GetDetails ());
				EUserMessage::TextRef	moreDetails (EUserMessage::SetParamText (TEXT_XMLFileFailure, filename, MP2CStr (desc), MP2CStr (code), MP2CStr (str)));
				
				*oldDetails += *moreDetails;
				} // catch
			}//endif a file was specified (empty means template/placeholder)
		
		return new PhotoPrintItem  (theCaptionRect,
									theImageRect,
									theFrameRect,
									theDest,

									theXScale,
									theYScale,
									theTopCrop,
									theLeftCrop,
									theBottomCrop,
									theRightCrop,
									theTopOffset,
									theLeftOffset,
									
									theTopCZ,
									theLeftCZ,
									theBottomCZ,
									theRightCZ,
									
									theUserTopCrop,
									theUserLeftCrop,
									theUserBottomCrop,
									theUserRightCrop,

									theProperties,

									theRot,
									theSkew,

									theFileSpec);

	} // end ReadItem

