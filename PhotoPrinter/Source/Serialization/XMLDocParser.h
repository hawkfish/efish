/*
	File:		XMLDocParser.h

	Contains:	Interface to an XML parser for a document.

	Written by:	Richard Wesley

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):
	
		26 Jul 2001		rmgw	Add EUserMessage. Bug #228.
		26 Jul 2001		rmgw	Factored from PhotoPrintDoc.  Bug #228.

	Description:
	
		This is a Builder pattern for PhotoPrintDocs.  

*/

#pragma once

#include "XMLItemParser.h"

class PhotoPrintDoc;

class XMLDocParser : public XMLItemParser

	{
	
	protected:
		
		PhotoPrintDoc*			mDoc;
		
		static	void			HandleLayout	(XML::Element &elem, void* userData);	
		static	void			HandleDocument	(XML::Element &elem, void* userData);	

		PhotoPrintDoc*			ReadDocument	(XML::Element&	elem);

		virtual	void			OnItemRead		(PhotoItemRef	inItem);
		virtual	void			OnDocumentRead	(PhotoPrintDoc*	inDoc);

	public:
								
								XMLDocParser	(XML::Input&	inInput,
												 EUserMessage&	inMessage,
												 PhotoPrintDoc*	inDoc);
		virtual					~XMLDocParser	(void);

		virtual	void			ParseDocument	(void);
	};