/*
	File:		XMLHandleStream.h

	Contains:	A stream which outputs to a handle, compatible with the XML stuff

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):
	
		12 Dec 2000		drd		13 Added constructors; now also an InputStream
		30 Aug 2000		drd		Created
*/

#include "xmloutput.h"

class XMLHandleStream : public XML::InputStream, public XML::OutputStream, public LHandleStream
{
public:
		XMLHandleStream();
		XMLHandleStream(Handle inHandle);

protected:
	// InputStream
	virtual int read(XML_Char *buf, size_t bufLen);

	// OutputStream
	virtual	int write(const char *str, size_t len);
};
