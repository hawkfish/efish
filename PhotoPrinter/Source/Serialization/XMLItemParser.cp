/*
	File:		XMLItemParser.cp

	Contains:	Implementation of an XML parser for an image.

	Written by:	Richard Wesley

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):
	
		26 Jul 2001		rmgw	Factored from PhotoPrintItem. Bug #228.

*/

#include "XMLItemParser.h"

#include "xmlinput.h"

#include <cstdio>

// ---------------------------------------------------------------------------
//	¥ XMLItemParser
// ---------------------------------------------------------------------------

XMLItemParser::XMLItemParser (

	XML::Input&			inInput) 
	
	: mInput (inInput)
	
	{ // begin XMLItemParser
	
	} // XMLItemParser

// ---------------------------------------------------------------------------
//	¥ ~XMLItemParser
// ---------------------------------------------------------------------------

XMLItemParser::~XMLItemParser (void) 
	
	{ // begin ~XMLItemParser
	
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
			theFileSpec = new MFileSpec(filename);	
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

									theProperties,

									theRot,
									theSkew,

									theFileSpec);

	} // end ReadItem

