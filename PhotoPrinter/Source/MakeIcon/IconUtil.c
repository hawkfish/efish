/*
	File:		IconUtil.c

	Contains:	Routines to create icons of any pixel depth, and masked Icons.

	Written by: Brigham Stevens	

	Copyright:	Copyright © 1984-1999 by Apple Computer, Inc., All Rights Reserved.

				You may incorporate this Apple sample source code into your program(s) without
				restriction. This Apple sample source code has been provided "AS IS" and the
				responsibility for its operation is yours. You are not permitted to redistribute
				this Apple sample source code as "Apple sample source code" after having made
				changes. If you're going to re-distribute the source, we require that you make
				it clear in the source that the code was descended from Apple sample source
				code, but that you've made changes.

	Change History (most recent first):
				07 Sep 2000		drd			Fixed calc of bytesPerRow
				07 Sep 2000		drd			Cleanup; make sure mask is full-sized
				7/9/1999	Karl Groethe	Updated for Metrowerks Codewarror Pro 2.1
				

*/

#include <QDOffscreen.h>
#include <Quickdraw.h>
#include <Palettes.h>
#include <Memory.h>
#include <Resources.h>
#include "OldBitMap.h"
#include "MiscCode.h"

#include "IconUtil.h"

Handle MakeIconMask(GWorldPtr inGWorld, Rect *srcRect, short iconSize)
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	this returns a handle to the image data for an icon MASK of dimension iconSize x iconSize.
	The icon is created by copying the srcRect of inGWorld into a PixMap.
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
{
	Handle	iconBits;
	BitMap	srcBitMap, maskBitMap;
	short	err;
	long	numBytes;
	short	rowbytes;
	
	iconBits = MakeIcon(inGWorld, srcRect, 1, iconSize);
	if (!iconBits)	return nil;
	HLock(iconBits);

	// set up a plain jane bitmap based on the data we just got from MakeIcon
	SetRect(&srcBitMap.bounds, 0, 0, iconSize, iconSize);
	srcBitMap.rowBytes = iconSize / 8;
	srcBitMap.baseAddr = *iconBits;
	
	NewMaskedBitMap(&srcBitMap, &maskBitMap, &srcBitMap.bounds);
	if (!maskBitMap.baseAddr) {
		DisposeHandle(iconBits);
		return nil;
	}
	
	HUnlock(iconBits);

	CalcOffScreen(&srcBitMap.bounds, &numBytes, &rowbytes);
	err = PtrToXHand(maskBitMap.baseAddr, iconBits, numBytes);
	if (err) {
		/* we can at least return a handle to the original image,
			which may be considered better than nothing.
			OR - we can dump the image and return nil.
		*/
		DebugStr("\pError in PtrToXHand");
	}
	
	FreeBitMap(&maskBitMap);

	HNoPurge(iconBits);
	return iconBits;
} // MakeIconMask


Handle	MakeICN_pound(GWorldPtr	inGWorld, Rect *srcRect, short iconDimension)
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	this returns a handle to the image data for an icon and its mask, of size iconDimension x iconDimension.
	The icon is created by copying the srcRect of inGWorld into a PixMap.
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
{
	OSErr	err;
	Handle	icon;
	Handle	iconMask;
	
	icon = MakeIcon(inGWorld, srcRect, 1, iconDimension);			
	
	if (icon) {
		Size	s = GetHandleSize(icon);
		if (s == 0) s++;
		iconMask = MakeIconMask(inGWorld, srcRect, iconDimension);	
		
		if (iconMask) {
			s = GetHandleSize(iconMask);
			if (s == 0) s++;
			err = HandAndHand(iconMask, icon);	// Concatenate iconMask at end of icon
			CheckError("\pHandAndHand fail.", err);
			DisposeHandle(iconMask);
		}
	}
	
	return icon;
}

/*
MakeIcon
	Creates a handle to the image data for an icon, or nil if an error.
	The source image is specified by GWorld and rectangle defining the area
	to create the icon from.
	The type of icon is specified by the depth and Size paramters.
	iconSize is used for both height and width.
	For example, to create an Icl8 pass 8 for dstDepth and 32 for iconSize.
	to create an ics8 pass 8 for the dstDepth and 16 for iconSize.
	to create an ICON pass 1 for the dstDepth and 32 for iconSize.
*/
Handle MakeIcon(GWorldPtr inGWorld, Rect *srcRect, short dstDepth, short iconSize)
{
	GWorldPtr		saveWorld;
	GDHandle		saveDevice;
	long			bytesPerRow;
	long			imageSize;
	Handle			dstHandle;
	PixMapHandle	pix;
	Rect			iconFrame;
	QDErr			err;
	
	GetGWorld(&saveWorld, &saveDevice);		// save Graphics env state

	SetGWorld(inGWorld, nil);
	
	iconFrame.top = 0;
	iconFrame.left = 0;
	iconFrame.bottom = iconSize;
	iconFrame.right = iconSize;
	
	// make a gworld for the icl resource
	pix = (PixMapHandle)NewHandle(sizeof(PixMap));
	
	/* See Tech Note #120 - for info on creating a PixMap by hand as SetUpPixMap
		does.  SetUpPixMap was taken from that Tech Note....
	*/
	err =  SetUpPixMap(dstDepth, &iconFrame, GetCTable(dstDepth), pix);

	if (err) {
		//asm {move.w	err, d0 }
		DebugStr("\pQuickDraw error.");
		return nil;
	}
		
	LockPixels(GetGWorldPixMap(inGWorld));
	LockPixels(pix);
			
	CopyBits(GetPortBitMapForCopyBits(inGWorld),
				(BitMapPtr)*pix,
				srcRect,
				&iconFrame,
				srcCopy | ditherCopy, nil);
 
 	UnlockPixels(GetGWorldPixMap(inGWorld));
 
 	bytesPerRow = ((dstDepth * ((**pix).bounds.right - (**pix).bounds.left) + 15) / 16) * 2;	// Was 31, 32, * 4 but this was giving us ics# that were too big
	imageSize  = (bytesPerRow) * ((**pix).bounds.bottom - (**pix).bounds.top);

	dstHandle = NewHandle(imageSize);
	err = MemError ();
	if (err || dstHandle == nil)
	{
		//asm { move.w err, d0}
		DebugStr("\pI am fuckered.");
		return nil;	
	}
	HLock((Handle)dstHandle);
	BlockMove(GetPixBaseAddr(pix),*dstHandle,imageSize);
	HUnlock(dstHandle);
	UnlockPixels(pix);
	TearDownPixMap(pix);
	// Restore graphics env to previous state
	SetGWorld(saveWorld, saveDevice);
	
	HNoPurge(dstHandle);
	return dstHandle;
} // MakeIcon

void TearDownPixMap(PixMapHandle pix)
{
	// We really need to do more....  BUT It is the thought that counts.. 
	DisposeHandle((Handle)pix);	
} // TearDownPixMap
