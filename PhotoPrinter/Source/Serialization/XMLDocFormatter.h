/*
	File:		XMLDocFormatter.h

	Contains:	Interface to an XML formatter for all parts of a document.

	Written by:	Richard Wesley

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):
	
		26 Jul 2001		rmgw	Factored from PhotoPrintItem and others. Bug #228.

	Description:
	
*/

#pragma once

#include "PhotoPrintItem.h"

namespace XML {
	class Output;
	class Element;
	class Handler;
	}

class PhotoItemProperties;
class DocumentProperties;
class PrintProperties;
class PhotoPrintDoc;

class XMLDocFormatter

	{
	
	protected:
	
		XML::Output&	out;
		const bool 		isTemplate;
		
	public:
		
						XMLDocFormatter			(XML::Output& 	inOut,
												 bool			inIsTemplate = false)
												: out (inOut), isTemplate (inIsTemplate) {}
		
		void			FormatRect				(const char* 	tagName, 
												 const Rect& 	rect);
			
		void			FormatItemProperties	(const	PhotoItemProperties&	inProps);
		void			FormatItem				(PhotoItemRef					inItem);
		void			FormatItems				(ConstPhotoIterator				inBegin,
												 ConstPhotoIterator				inEnd);

		void			FormatDocumentProperties(const	DocumentProperties&		inProps);
		void			FormatPrintProperties	(const	PrintProperties&		inProps);
		void			FormatDocument			(PhotoPrintDoc*					inDoc);
	};