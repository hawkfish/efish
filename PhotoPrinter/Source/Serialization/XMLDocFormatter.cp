/*
	File:		XMLDocFormatter.cp

	Contains:	Implementation of an XML parser for an image.

	Written by:	Richard Wesley

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		31 aug 2001		dml		275, 282.  add new crop members to item writer
		21 Aug 2001		drd		340 Be more paranoid about using GetFileSpec() since it can be nil;
								use HARef<char>, not HORef<char>
		19 aug 2001		dml		add new relative crop-zoom fields to item serialization
		17 Aug 2001		rmgw	Pass in name to FormatDocument. Bug #330.
		01 Aug 2001		rmgw	Rename ImageCount property to ItemsPerPage.  Bug #265.
		31 Jul 2001		drd		256 Write maximumSize, minimumSize
		26 Jul 2001		rmgw	Factored from PhotoPrintItem and others. Bug #228.

	Description:
		
		A Builder pattern class for serialising our XML file format.
*/

#include "XMLDocFormatter.h"

#include "LayoutMapper.h"
#include "PhotoPrintDoc.h"
#include "PhotoPrintPrefs.h"		// Has gSizeLimitMap

#include "xmloutput.h"

#include "HARef.h"
#include "MP2CStr.h"

// ---------------------------------------------------------------------------
//	¥ FormatRect
// ---------------------------------------------------------------------------

void
XMLDocFormatter::FormatRect (

	const char* 	tagName, 
	const Rect& 	rect) 
	
	{ // begin FormatRect
	
		out.BeginElement(tagName, XML::Output::terse);
			out << rect.top << "," << rect.left << "," << rect.bottom << "," << rect.right;
		out.EndElement(XML::Output::terse);
	
	} // end FormatRect

// ---------------------------------------------------------------------------
//	¥ FormatItemProperties
// ---------------------------------------------------------------------------
//	TODO: factor from PhotoItemProperties

void 
XMLDocFormatter::FormatItemProperties (

	const	PhotoItemProperties&	inProps) 

	{ // begin FormatItemProperties
	
		out.BeginElement("properties", XML::Output::indent);
			inProps.Write(out);
		out.EndElement(XML::Output::indent);

	} // end FormatItemProperties

// ---------------------------------------------------------------------------
//	¥ FormatItem
// ---------------------------------------------------------------------------

void 
XMLDocFormatter::FormatItem (

	PhotoItemRef	inItem) 

	{ // begin FormatItem
	
		//	Read properties we store
		Rect							theDest (inItem->GetDestRect ());
		Rect							theCaptionRect (inItem->GetCaptionRect ());
		Rect							theImageRect (inItem->GetImageRect ());
		Rect							theFrameRect (inItem->GetFrameRect ());

		double							theXScale;
		double							theYScale;
		inItem->GetCropZoomScales (theXScale, theYScale);
		
		double							theTopCrop;
		double							theLeftCrop;
		double							theBottomCrop;
		double							theRightCrop;
		inItem->GetCrop (theTopCrop, theLeftCrop, theBottomCrop, theRightCrop);

		double							theTopOffset;
		double							theLeftOffset;
		inItem->GetCropZoomOffset (theTopOffset, theLeftOffset);
		
		double							theTopCZ;
		double							theLeftCZ;
		double							theBottomCZ;
		double							theRightCZ;
		inItem->GetCropZoom(theTopCZ, theLeftCZ, theBottomCZ, theRightCZ);

		double							theUserTopCrop ;
		double							theUserLeftCrop ;
		double							theUserBottomCrop ;
		double							theUserRightCrop ;
		inItem->GetUserCrop(theUserTopCrop, theUserLeftCrop, theUserBottomCrop, theUserRightCrop);

		PhotoItemProperties				theProperties (inItem->GetProperties ());

		double							theRot (inItem->GetRotation ());
		double							theSkew (inItem->GetSkew ());

		out.BeginElement("photo", XML::Output::indent);

			if ((!isTemplate) && (!inItem->IsEmpty())) {
				MFileSpec*				fs = inItem->GetFileSpec();
				if (fs == nil)
					Throw_(abortErr);			// Not a great error, but we should stop the save
				HARef<char> path (inItem->GetFileSpec()->MakePath());
				out.WriteElement ("filename", path);
			}//endif

			FormatRect("bounds", theDest);
			
			//crop stuff
			out.WriteElement("topCrop", theTopCrop);
			out.WriteElement("leftCrop", theLeftCrop);
			out.WriteElement("bottomCrop", theBottomCrop);
			out.WriteElement("rightCrop", theRightCrop);
			out.WriteElement("topOffset", theTopOffset);
			out.WriteElement("leftOffset", theLeftOffset);
			out.WriteElement("xScale", theXScale);
			out.WriteElement("yScale", theYScale);
			
			out.WriteElement("topCZ", theTopCZ);
			out.WriteElement("leftCZ", theLeftCZ);
			out.WriteElement("bottomCZ", theBottomCZ);
			out.WriteElement("rightCZ", theRightCZ);
			
			out.WriteElement("userTopCrop", theUserTopCrop);
			out.WriteElement("userLeftCrop", theUserLeftCrop);
			out.WriteElement("userBottomCrop", theUserBottomCrop);
			out.WriteElement("userRightCrop", theUserRightCrop);
			
			FormatRect("imageRect", theImageRect);
			FormatRect("captionRect", theCaptionRect);
			FormatRect("frameRect", theFrameRect);

			out.WriteElement("rotation", theRot);
			out.WriteElement("skew", theSkew);
			
			FormatItemProperties (theProperties);

		out.EndElement();

	} // end FormatItem

// ---------------------------------------------------------------------------
//	¥ FormatItems
// ---------------------------------------------------------------------------

void 
XMLDocFormatter::FormatItems (

	ConstPhotoIterator	inBegin,
	ConstPhotoIterator	inEnd) 
	
	{ // begin FormatItems
			
		out.BeginElement ("Objects");
		for (ConstPhotoIterator i = inBegin; i != inEnd; ++i) 
			FormatItem (*i);
		out.EndElement ();	// Objects
	
	} // end FormatItems

// ---------------------------------------------------------------------------
//	¥ FormatDocumentProperties
// ---------------------------------------------------------------------------

void 
XMLDocFormatter::FormatDocumentProperties (

	const	DocumentProperties&	inProps)
	
	{ // begin FormatDocumentProperties
	
		out.BeginElement("Document_Properties");
			inProps.Write(out);
		out.EndElement();

	} // end FormatDocumentProperties
	
// ---------------------------------------------------------------------------
//	¥ FormatPrintProperties
// ---------------------------------------------------------------------------

void 
XMLDocFormatter::FormatPrintProperties (

	const	PrintProperties&	inProps)
	
	{ // begin FormatPrintProperties
	
		out.BeginElement("Print_Properties");
			inProps.Write (out);
		out.EndElement();

	} // end FormatPrintProperties
	
// ---------------------------------------------------------------------------
//	¥ FormatDocument
// ---------------------------------------------------------------------------

void 
XMLDocFormatter::FormatDocument (

	PhotoPrintDoc*		inDoc,
	ConstStr255Param	inName)
	
	{ // begin FormatDocument
		
		PhotoPrintView*				theView (inDoc->GetView ());
		PhotoPrintModel* 			theModel (theView->GetModel());
		
		double						theWidth (inDoc->GetWidth ());
		double						theHeight (inDoc->GetHeight ());
		SInt16						theDPI (inDoc->GetResolution ());
		SizeLimitT					maximumSize = inDoc->GetMaximumSize();
		SizeLimitT					minimumSize = inDoc->GetMinimumSize();
		const	DocumentProperties&	theDocProps (inDoc->GetProperties ());
		const	PrintProperties&	thePrintProps (inDoc->GetPrintProperties ());
		
		out.BeginDocument();
			out.writeLine("");

			out.BeginElementAttrs("Document");
					MPString title (inName);
					out.WriteAttr("name", MP2CStr (title));
				out.EndAttrs();

				out.WriteElement("layout", 	LayoutMapper::Find(theView->GetLayout()->GetType()));
				out.WriteElement("image_count", theView->GetLayout()->GetItemsPerPage());
				out.WriteElement("maximum_size", PhotoPrintPrefs::gSizeLimitMap[maximumSize]);
				out.WriteElement("minimum_size", PhotoPrintPrefs::gSizeLimitMap[minimumSize]);

				out.WriteElement("width", theWidth);
				out.WriteElement("height", theHeight);
				out.WriteElement("dpi", theDPI);
				
				FormatDocumentProperties (theDocProps);
				
				out.writeLine("");

				FormatPrintProperties (thePrintProps);

				out.writeLine("");

				// write objects
				FormatItems (theModel->begin (), theModel->end ());

			out.EndElement();	// Document

		out.EndDocument();
	
	} // end FormatDocument