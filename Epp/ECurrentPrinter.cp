/*
	File:		CurrentPrinter.c

	Contains:	Check the default printer under GX or old printing architecture.
				Print out the name, entityType, and zone.
	
				This sample is provided as-is. It's guaranteed to work on my
				mac this instant, and nothing more. It shows how to determine
				the currently selected printer now. Since this isn't an officially
				supported endeavor, things may change in the future, and this
				snippet will be for naught.

	Written by: Dave Polaschek and David Hayward	

	Copyright:	Copyright © 1995-1999 by Apple Computer, Inc., All Rights Reserved.

				You may incorporate this Apple sample source code into your program(s) without
				restriction. This Apple sample source code has been provided "AS IS" and the
				responsibility for its operation is yours. You are not permitted to redistribute
				this Apple sample source code as "Apple sample source code" after having made
				changes. If you're going to re-distribute the source, we require that you make
				it clear in the source that the code was descended from Apple sample source
				code, but that you've made changes.

	Change History (most recent first):
				7/23/1999	Karl Groethe	Updated for Metrowerks Codewarror Pro 2.1 fixed bug dealing witht
											copying more than 256 bytes into a pascal string.
				
				
	Electric Fish Changes:
		02 aug 2000	drd		ValidPrinter adds a Carbon test as well
		1 aug 2000 slithy:  ValidPrinter now initializes the NameString to empty (!)
							Changed OpenResFile to FSpOpenResFile for Carbon, using 
							MSpecialFolder, MFileSpec to deal w/ that mess			

*/


#include <stdio.h>
#include <string.h>
#include <Types.h>
#include <TextUtils.h>
#include <Files.h>
#include <Folders.h>
#include <Resources.h>
#include <Aliases.h>
#include <Gestalt.h>
#include <Memory.h>

#include "MFileSpec.h"
#include "MSpecialFolder.h"
#include <PMApplication.h>

#define gestaltGXVersion			'qdgx'			// in PrintingManager.h (old) or GXPrinting.h (new)
#define gestaltGXPrintingMgrVersion	'pmgr'			// in PrintingManager.h (old) or GXPrinting.h (new)
#define gestaltAliasMgrAttr			'alis'			// in Aliases.h

#define kPrintingResourceID			0xE000			// 0xE000 = -8192


static OSErr		GetCurrentPrinter(Str255 printer);
static Boolean		gxInstalled(void);
bool				ValidPrinter(void);


/*------------------------------------------------------------------------------*\
		This function returns true if QuickDraw GX printing is available
\*------------------------------------------------------------------------------*/
Boolean gxInstalled(void)
{
	/*
	long version;

	if (Gestalt(gestaltGXVersion, &version) == noErr)
		if (Gestalt(gestaltGXPrintingMgrVersion, &version) == noErr)
			return(true);
	return(false);
	*/
	return false;// GX printing no longer supported
}


/*------------------------------------------------------------------------------*\
		This function returns the network entity of the currently 
		selected printer, and as an added bonus, you get an error code, too!
\*------------------------------------------------------------------------------*/
OSErr GetCurrentPrinter(Str255 printer)
{
	StringHandle	theDriver;
	Handle			thePrinter;
	OSErr			theErr = noErr;
	short			resFileRefnum;

	theDriver = GetString(kPrintingResourceID);
	if (!theDriver) goto BADDriver;

	if (gxInstalled()) {
		short	vRefNum;
		long	dirID;
		FSSpec	theFile;
		long	resSize;

		theErr=FindFolder(kOnSystemDisk,kDesktopFolderType,kDontCreateFolder,&vRefNum,&dirID);
		if (theErr != noErr) goto BADDriver;

		theErr=FSMakeFSSpec(vRefNum,dirID,*theDriver,&theFile);
		if (theErr != noErr) goto BADDriver;

		resFileRefnum = FSpOpenResFile(&theFile,fsRdPerm);
		if (resFileRefnum == -1) goto BADResFile;

		thePrinter = Get1Resource('comm', 0);
		if (!thePrinter) goto BADPrinter;

		if (*((long *)*thePrinter) != 'PPTL') goto BADPrinter;

		resSize = GetHandleSize(thePrinter)-6;
		BlockMoveData((*thePrinter)+6,printer,resSize);
	} else {
		Boolean 	aliasCallsPresent;
		Boolean 	aliasResourcePresent;
		long		response;
		AliasHandle	theAlias;
		
		aliasCallsPresent = ((::Gestalt(gestaltAliasMgrAttr,&response) == noErr) &&
			(response & 1));

		theAlias = (AliasHandle)::GetResource('alis', kPrintingResourceID);
		aliasResourcePresent = !!(theAlias);
		
		if (aliasCallsPresent && aliasResourcePresent) {
			FSSpec	fileSpec;
			Boolean	wasChanged;

			theErr = ResolveAlias(NULL, theAlias,&fileSpec,&wasChanged);
			if (theErr != noErr) goto BADDriver;

			resFileRefnum = FSpOpenResFile(&fileSpec,fsRdPerm);
			if (resFileRefnum == -1) goto BADResFile;
		} else {
			MSpecialFolder extensionsFolder (kExtensionFolderType);
			MFileSpec driverSpec (extensionsFolder, *theDriver, false);
			resFileRefnum = FSpOpenResFile((FSSpec*)&driverSpec, fsRdPerm);
			if (resFileRefnum == -1) goto BADResFile;
		}
		thePrinter = ::Get1Resource('DRVR', kPrintingResourceID);
		if (!thePrinter) goto BADPrinter;
		::HLock(thePrinter);
		::BlockMoveData(*theDriver, printer, *theDriver[0]);
		::HUnlock(thePrinter);
	}
	::ReleaseResource(thePrinter);
	::CloseResFile(resFileRefnum);

	return(noErr);

// Error returns
BADPrinter:
	CloseResFile(resFileRefnum);
BADResFile:
	theErr = ResError();
BADDriver:
	return(theErr);
}



/*------------------------------------------------------------------------------*\
		This function tells if there's a valid printer selected (i.e. not
		aimed at a driver that's since been deleted or no printer selected
		since last System sw install) in the chooser
\*------------------------------------------------------------------------------*/
bool ValidPrinter(void)
{
	Str255	printerEntity = "\p";
	
	if (GetCurrentPrinter(printerEntity) != noErr)
		return false;
	else if (StrLength(printerEntity) == 0) {
		Boolean				isPS;
		LPrintSpec			spec;				// ??? depends on PowerPlant
		StPrintSession		session(spec);
		OSStatus			s = ::PMIsPostScriptDriver(&isPS);
		if (s == kPMNoError)
			return true;
		return false;
	} else
		return true;
} // ValidPrinter
