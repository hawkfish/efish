/*
	File:		XMLItemParser.h

	Contains:	Interface to an XML parser for an item.

	Written by:	Richard Wesley

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):
	
		26 Jul 2001		rmgw	Add EUserMessage. Bug #228.
		26 Jul 2001		rmgw	Factored from PhotoPrintItem. Bug #228.

	Description:
	
		This is a Builder pattern for PhotoPrintItems.

*/

#pragma once

#include "PhotoPrintItem.h"

namespace XML {
	class Input;
	class Element;
	}

class EUserMessage;

class XMLItemParser

	{
	
	protected:
	
		XML::Input&				mInput;
		EUserMessage&			mMessage;
		
		static	void			HandleRect		(XML::Element &elem, void *userData);
		static	void			HandleItem		(XML::Element &elem, void* userData);	
		static	void			HandleItems		(XML::Element &elem, void* userData);	
		static	void			HandleObjects	(XML::Element &elem, void* userData);	
		
		PhotoItemRef			ReadItem		(XML::Element &elem);
	
		virtual	void			OnItemRead		(PhotoItemRef	inItem) = 0;

	public:
								
		explicit				XMLItemParser	(XML::Input&	inInput,
												 EUserMessage&	inMessage);
		virtual					~XMLItemParser	(void);
		
		EUserMessage&			GetMessage		(void) const {return mMessage;}
		
		virtual	void			ParseObjects	(void);
	};