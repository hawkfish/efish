/*
	File:		IconUtil.cp

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

#include "IconUtil.h"

#include "EPixMapHandle.h"

#include "MBitMap.h"
#include "MNewHandle.h"
#include "MRect.h"
/*
#include <QDOffscreen.h>
#include <Quickdraw.h>
#include <Palettes.h>
#include <Memory.h>
#include <Resources.h>
*/
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

Handle 
MakeIcon (

	GWorldPtr 	inGWorld, 
	Rect *		srcRect, 
	short 		dstDepth, 
	short 		iconSize)

	{ // begin MakeIcon
		
		Handle			iconData = nil;
		
		StGDeviceSaver	saveWorld;
		SetGWorld (inGWorld, nil);
		
		// make a gworld for the icl resource
		MRect			iconFrame (0, 0, iconSize, iconSize);
		EPixMapHandle	pix (iconFrame, dstDepth);
		
		if (pix.Lock ()) {
			MPixMap		gPix (GetGWorldPixMap (inGWorld));
			if (gPix.Lock ()) {
				CopyBits (GetPortBitMapForCopyBits(inGWorld),	pix.AsBitMap (),
						  srcRect,								&iconFrame,
						  srcCopy | ditherCopy, 				nil);
		 
		 		gPix.Unlock ();
	 			} // if
	 			
		 	SInt16	bytesPerRow = pix.CalcRowBytes (pix.GetBounds (), dstDepth);
		 	Size	imageSize = pix.CalcImageSize (pix.GetBounds (), dstDepth);
			
			iconData = NewHandle (imageSize);
			ThrowIfMemFail_(iconData);
			BlockMoveData (pix.GetBaseAddr(), *iconData, imageSize);
			HNoPurge (iconData);

			pix.Unlock ();
			} // if
			
		return iconData;

	} // end MakeIcon

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	this returns a handle to the image data for an icon MASK of dimension iconSize x iconSize.
	The icon is created by copying the srcRect of inGWorld into a PixMap.
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Handle 
MakeIconMask (

	GWorldPtr inGWorld, 
	Rect *srcRect, 
	short iconSize)
	
	{ // begin MakeIconMask
	
		MDisposeHandle<Handle>	iconBits (MakeIcon (inGWorld, srcRect, 1, iconSize));			
		ThrowIfNil_(iconBits);
		
		iconBits.Lock ();

		// set up a plain jane bitmap based on the data we just got from MakeIcon
		BitMap	srcBitMap;
		SetRect (&srcBitMap.bounds, 0, 0, iconSize, iconSize);
		srcBitMap.rowBytes = iconSize / 8;
		srcBitMap.baseAddr = *iconBits;
		
		MBitMap		maskBitMap (srcBitMap.bounds);
		maskBitMap.SetMask (srcBitMap);
		iconBits.Unlock ();

		ThrowIfOSErr_(::PtrToXHand (maskBitMap.GetBits (), iconBits, MBitMap::CalcImageSize (srcBitMap.bounds)));

		iconBits.NoPurge ();
		
		Handle	result (iconBits);
		iconBits.Detach ();
		
		return result;

	} // end MakeIconMask

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	this returns a handle to the image data for an icon and its mask, of size iconDimension x iconDimension.
	The icon is created by copying the srcRect of inGWorld into a PixMap.
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Handle	
MakeICN_pound (

	GWorldPtr	inGWorld, 
	Rect *srcRect, 
	short iconDimension)

	{ // begin MakeICN_pound
	
		MDisposeHandle<Handle>	icon (MakeIcon (inGWorld, srcRect, 1, iconDimension));			
		ThrowIfNil_(icon);
		
		MNewHandle				iconMask (MakeIconMask (inGWorld, srcRect, iconDimension));	
		ThrowIfNil_(iconMask);
			
		ThrowIfOSErr_(HandAndHand (iconMask, icon));	// Concatenate iconMask at end of icon
		
		Handle					result (icon);
		icon.Detach ();
		
		return result;

	} // end MakeICN_pound

