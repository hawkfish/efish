/*
	File:		MiscCode.c

	Contains:	

	Written by: 	

	Copyright:	Copyright © 1992-1999 by Apple Computer, Inc., All Rights Reserved.

				You may incorporate this Apple sample source code into your program(s) without
				restriction. This Apple sample source code has been provided "AS IS" and the
				responsibility for its operation is yours. You are not permitted to redistribute
				this Apple sample source code as "Apple sample source code" after having made
				changes. If you're going to re-distribute the source, we require that you make
				it clear in the source that the code was descended from Apple sample source
				code, but that you've made changes.

	Change History (most recent first):
				07 Sep 00	drd				Removed DrawImage rather than Carbonize it
				7/9/1999	Karl Groethe	Updated for Metrowerks Codewarror Pro 2.1
				

*/
#include "MiscCode.h"
#include <QDOffscreen.h>
#include <Resources.h>

void CheckError(Str255 err, short code)
{
	if(code)
	{
		//asm { move.w	code, d0 }
		DebugStr(err);
	}
}

void TryRemoveResource(ResType	type, short	ID)
{
	Handle	h;
		
	h = GetResource(type,ID);
	if(h)
		RemoveResource(h);
}
