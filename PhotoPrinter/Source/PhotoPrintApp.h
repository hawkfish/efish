/*
	File:		PhotoPrintApp.h

	Contains:	Definition of the application class.

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		09 mar 2001		dml		add gOSX
		02 feb 2001		dml		add gCarbonVersion
		19 Jan 2001		drd		We don't need EventResume
		09 Nov 2000		drd		Override UpdateMenus
		08 Nov 2000		drd		Added gAqua; override HandleAppleEvent
		13 sep 2000		dml		added gIsRegistered
		12 sep 2000		dml		add gFlatPageFormat
		31 Aug 2000		drd		Override OpenOrPrintDocList
		11 aug 2000		dml		add parm to SetDocumentControllers
		11 Aug 2000		drd		Moved tool_, curs_ to other header files
		11 aug 2000		dml		add SetDocumentControllers
		03 aug 2000		dml		RefreshDocuments takes "ForceSort" parm
		02 aug 2000		dml		changed to RefreshDocuments(bool inAll, bool forceLayout);
		02 aug 2000		dml		added LayoutAllDocuments, GetSingleton
		24 Jul 2000		drd		Override AllowSubRemoval
		20 Jul 2000		drd		Added gCurTool
		19 Jul 2000		drd		Override EventResume
		18 Jul 2000		drd		Added gTools; override MakeMenuBar
		18 jul 2000		dml		changed gPrintSession to gCurPrintSession
		14 jul 2000		dml		add a singleton StPrintSession (later, will be per-document)
		10 Jul 2000		drd		Added gName, CheckPlatformSpec
		06 Jun 2000		drd		mPalette is now gPalette
		29 Jun 2000		drd		Override EventSuspend; added mPalette
		21 Jun 2000		drd		Override HandleCreateElementEvent
		15 Jun 2000		drd		Override Initialize
*/

#ifndef _H_CAppearanceApp
#define _H_CAppearanceApp

#pragma once

#include <CFBase.h>
#include <LDocApplication.h>

#include "HORef.h"
#include "UPrinting.h"
#include "EPrintSpec.h"
#include "MResFile.h"
#include "MNewHandle.h"
#include "MPString.h"

class PhotoPrintDoc;

class PhotoPrintApp : public LDocApplication {
public:
	enum {
		mbar_Carbon = 1128
	};

							PhotoPrintApp();
	virtual					~PhotoPrintApp();

	static PhotoPrintApp*		GetSingleton(void)	{ return gSingleton; }
	static void					RefreshDocuments(bool inForceSort = true, bool inForceLayout = true);
	static void					SetDocumentControllers(OSType inTool);

	// LCommander
	virtual Boolean			AllowSubRemoval(
								LCommander*			inSub);
	virtual void			FindCommandStatus(
								CommandT			inCommand,
								Boolean&			outEnabled,
								Boolean&			outUsesMark,
								UInt16&				outMark,
								Str255				outName);
	virtual Boolean			ObeyCommand(
								CommandT			inCommand,
								void*				ioParam = nil);	

	// LApplication
	virtual	void			MakeMenuBar();

	// LDocApplication
	virtual void			OpenDocument(
									FSSpec*				inMacFSSpec);
	virtual void			OpenOrPrintDocList(
									const AEDescList&	inDocList,
									SInt32				inAENumber);

	// LEventDispatcher
	virtual void			EventSuspend	(const EventRecord& inMacEvent);
	virtual void			UpdateMenus();

	// LModelObject
	virtual void			HandleAppleEvent(
									const AppleEvent	&inAppleEvent,
									AppleEvent			&outAEReply,
									AEDesc				&outResult,
									SInt32				inAENumber);
	virtual LModelObject*	HandleCreateElementEvent(
									DescType			inElemClass,
									DescType			inInsertPosition,
									LModelObject*		inTargetObject,
									const AppleEvent&	inAppleEvent,
									AppleEvent&			outAEReply);

	static	bool			CheckPlatformSpec();
	static	OSType			GetCurrentTool()					{ return gCurTool; }
	static	CFStringRef		Name()								{ return gName; }

protected:
	// LApplication
	virtual void			Initialize();
	virtual void			StartUp();

			void			AddCommands			(void);
			void			AddEvents			(void);
			void			RegisterClasses();

	// Class data
public:
	static LWindow*			gPalette;
	static LWindow*			gTools;
	static CFStringRef		gName;
	static StPrintSession*	gCurPrintSession;	// !!! should be document-specific
	static HORef<MNewHandle>	gFlatPageFormat;	// may want new documents to inherit MRU page format
	static OSType			gCurTool;
	static PhotoPrintDoc*	gPrintSessionOwner;
	static PhotoPrintApp*	gSingleton;
	static MCurResFile		gAppResFile;
	static bool				gAqua;				// Do we have Aqua layout?
	static bool				gOSX;				// is this really OSX, or is it Carbon?
	//
	static bool				gIsRegistered;
	static MPString			gAnnoyanceText;
	static long				gCarbonVersion;
};

#endif // _H_CAppearanceApp
