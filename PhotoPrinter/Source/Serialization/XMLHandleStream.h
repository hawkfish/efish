/*
	File:		XMLHandleStream.h

	Contains:	A stream which outputs to a handle, compatible with the XML stuff

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):
	
		30 Aug 2000		drd		Created
*/

#include "xmloutput.h"

class XMLHandleStream : public XML::OutputStream, public LHandleStream
{
	int write(const char *str, size_t len);
};
