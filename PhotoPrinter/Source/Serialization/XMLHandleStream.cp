/*
	File:		XMLHandleStream.cp

	Contains:	A stream which outputs to a handle, compatible with the XML stuff

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):
	
		30 Aug 2000		drd		Created
*/

#include "XMLHandleStream.h"

/*
write {OVERRIDE}
*/
int XMLHandleStream::write(const char *str, size_t len)
{
	SInt32	byteCount = len;
	this->PutBytes(str, byteCount);

	return byteCount;
} // write

