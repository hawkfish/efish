/*
	File:		XMLHandleStream.cp

	Contains:	A stream which outputs to a handle, compatible with the XML stuff

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):
	
		12 Dec 2000		drd		13 Added constructors, read
		30 Aug 2000		drd		Created
*/

#include "XMLHandleStream.h"

/*
XMLHandleStream
	Default constructor
*/
XMLHandleStream::XMLHandleStream()
	: LHandleStream()
{
} // XMLHandleStream

/*
XMLHandleStream
	Construct from an existing Handle
	The LHandleStream object assumes ownership of the Handle
*/
XMLHandleStream::XMLHandleStream(
	Handle	inHandle)
	: LHandleStream(inHandle)
{
} // XMLHandleStream

/*
read {OVERRIDE}
*/
int	XMLHandleStream::read(XML_Char *buf, size_t bufLen)
{
	SInt32	byteCount = bufLen;
	this->GetBytes(buf, byteCount);

	return byteCount;
} // read

/*
write {OVERRIDE}
*/
int XMLHandleStream::write(const char *str, size_t len)
{
	SInt32	byteCount = len;
	this->PutBytes(str, byteCount);

	return byteCount;
} // write

