/*
	File:		CMCVS.cp

	Contains:	Implementation of the Projector CMM.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1997-9 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

         <7>      5/5/99    rmgw    Add scan code contextual menu override.
         <6>     3/30/99    rmgw    Crash recovery merging.
         <5>     3/19/99    rmgw    Remove ProjectDrag prefs junk.
         <4>     3/18/99    rmgw    Factor CMCVS code.
         <3>     3/18/99    rmgw    Remove bogus #include.
         <2>     3/18/99    rmgw    Add menu building.
         <1>     3/17/99    rmgw    first checked in.
*/


// Class Header
#include "CMCVS.h"

// Mac OS Includes
#include <AERegistry.h>
#include <CodeFragments.h>
#include <ContextualMenuPlugins.h>
#include <Resources.h>
#include <TextUtils.h>

// SOM Includes
#include <som.xh>

#include "VCSCMMContext.h"
#include "CWCVS.h"
#include "VCSUtil.h"
#include "MoreFilesExtras.h"

// Function declarations
extern pascal OSErr __initialize(CFragInitBlockPtr); // metrowerks's default initializer
extern pascal void __terminate(void);
pascal OSErr CMCVSInitialize(CFragInitBlockPtr init); // our initializer
pascal void CMCVSTerminate(void); // our terminator

//	Resources
const	ResID	kRegularCommandStrings 			= 128;
const	ResID	kAdvancedCommandStrings 		= 129;

const	ResID	kMenuRequests 					= 130;
const	ResID	kNotInDatabaseEnableRequests 	= 131;
const	ResID	kCheckedInEnableRequests	 	= 132;
const	ResID	kCheckedOutEnableRequests 		= 133;
const	ResID	kUnknownEnableRequests		 	= 134;
	
//	Types

const	long	kRequestCommandShift = 16;
const	long	kRecursiveCommandFlag = 0x02;
const	long	kAdvancedCommandFlag = 0x01;

struct CmdMap {
	ResID	strnID;
	short	strnIndex;
	short	request;
	Boolean	advanced;
	Boolean	recursive;
			
	long	GetCmdID	(void)  const;
	void	SetCmdID	(long	inCmdID);
	};

inline long
CmdMap::GetCmdID (void) const
	{return ((((long) request) << kRequestCommandShift) |
			(recursive ? kRecursiveCommandFlag : 0) | 
			(advanced ? kAdvancedCommandFlag : 0));};

inline void
CmdMap::SetCmdID (long inCmdID)
	{
		request = inCmdID >> kRequestCommandShift; 
		recursive = (inCmdID & kRecursiveCommandFlag) ? true : false;
		advanced = (inCmdID & kAdvancedCommandFlag) ? true : false;
		};
	

/*******************************************************************************

	AddCommandToAEDescList

*******************************************************************************/

static OSStatus 
AddCommandToAEDescList (

	AEDescList* 		ioCommandList,
	ConstStr255Param	inCommandString,
	SInt32 				inCommandID)

	{ // begin AddCommandToAEDescList
	
		OSStatus	e = noErr;
		
		AERecord	theCommandRecord = {typeNull, nil};
		
		do {
			// create an apple event record for our command
			e = ::AECreateList (nil, 0, true, &theCommandRecord);
			if (e != noErr) break;
			
			// stick the command text into the AERecord
			e = ::AEPutKeyPtr (&theCommandRecord, keyAEName, typeChar, inCommandString + 1, inCommandString[0]);
			if (e != noErr) break;
				
			// stick the command ID into the AERecord
			e = ::AEPutKeyPtr(&theCommandRecord, keyContextualMenuCommandID, typeLongInteger, &inCommandID, sizeof (inCommandID));
			if (e != noErr) break;
			
			// stick this record into the list of commands that we are passing back to CMM
			e = ::AEPutDesc(ioCommandList, 0, &theCommandRecord); 
			if (e != noErr) break;
			
			} while (false);
		
		// clean up after ourself; dispose of the AERecord
		::AEDisposeDesc (&theCommandRecord);

	    return e;
	    
	} // AddCommandToAEDescList

/*******************************************************************************

	AddMenuToAEDescList

*******************************************************************************/

static OSStatus 
AddMenuToAEDescList (

	AEDescList* 		ioCommandList,
	ConstStr255Param	inCommandString,
	const	AEDescList*	theSubmenuCommands)

	{ // begin AddMenuToAEDescList
	
		OSStatus e = noErr;
		
		AERecord theSupercommand = {typeNull, nil};
		
		do {
			// now, we need to create the supercommand which will "own" the
			// subcommands.  The supercommand lives in the root command list.
			// this looks very much like the AddCommandToAEDescList function,
			// except that instead of putting a command ID in the record,
			// we put in the subcommand list.
				
			// create an apple event record for our supercommand
			if (noErr != (e = ::AECreateList(NULL, 0, true, &theSupercommand))) break;
			
			// stick the command text into the AERecord
			if (noErr != (e = ::AEPutKeyPtr (&theSupercommand, keyAEName, typeChar, &inCommandString[1], inCommandString[0]))) break;
			
			// stick the subcommands into into the AERecord
			if (noErr != (e = ::AEPutKeyDesc (&theSupercommand, keyContextualMenuSubmenu, theSubmenuCommands))) break;
			
			// stick the supercommand into the list of commands that we are passing back to CMM
			if (noErr != (e = ::AEPutDesc(ioCommandList, 0, &theSupercommand))) break;
			} while (false);
		
		// clean up after ourself
		::AEDisposeDesc (&theSupercommand);

	    return e;
	    
	} // end AddMenuToAEDescList

/*******************************************************************************

	FindRequestIndex

*******************************************************************************/

const	ResType	type_RequestList = 'Req#';

static short 
FindRequestIndex (
	
	const	ResID		inListID,
	const	long		inRequest)
	
	{ // begin FindRequestIndex
	
		short	result = 0;
		
		do {
			long**	list = (long**) ::GetResource (type_RequestList, inListID);
			if ((list == nil) || (*list == nil)) break;
			
			Size	count = ::GetHandleSize ((Handle) list) / sizeof (**list);

			//	Find the string
			for (short i = 0; i < count; ++i) {
				if ((*list)[i] != inRequest) continue;
				
				result = i + 1;
				break;
				} // for
			} while (false);
			
		return result;
		
	} // end FindRequestIndex
	
/*******************************************************************************

	AppendVCSSubmenu

*******************************************************************************/

static OSStatus 
AppendVCSSubmenu (

	AEDescList* 				ioCommandList,
	const	ResID				inMenuList,
	const	CmdMap&				inCmd,
	const	CWVCSCheckoutState	inCheckoutState,
	const	Boolean				inEnableAll)
	
	{ // begin AppendVCSSubmenu
	
		OSStatus 				e = noErr;
		
		do
		{
			// Commands
			long**		requestList = (long**) ::GetResource (type_RequestList, inMenuList);
			if ((requestList == nil) || (*requestList == nil)) break;
			
			Size		requestCount = ::GetHandleSize ((Handle) requestList) / sizeof (**requestList);

			long		lastRequest = 0;
			Boolean		sepNeeded = false;
			for (short r = 0; r < requestCount; ++r) {
				CmdMap	cmd = inCmd;
				cmd.request = (*requestList)[r];
				
				//	Find the request
				if (cmd.request) {
					VCSCMMContext	context (0, cmd.request, cmd.advanced, cmd.recursive, true);
					if (cwCommandStatusSupported != CWCVSDispatch (context)) continue;
					
					ResID	enableID = kUnknownEnableRequests;
					if (inCheckoutState == cwCheckoutStateUnknown) 
						enableID = kUnknownEnableRequests;
						
					else {
						const	long	stateMask = ~(cwCheckoutStateMultiplyCheckedOutMask | cwCheckoutStateSharedMask | cwCheckoutStateBranchedMask | cwCheckoutStateExclusiveMask);
						switch (inCheckoutState & stateMask) {
							case cwCheckoutStateCheckedOut:
								enableID = kCheckedOutEnableRequests;
								break;
								
							case cwCheckoutStateNotCheckedOut:
								enableID = kCheckedInEnableRequests;
								break;
								
							case cwCheckoutStateNotInDatabase:
								enableID = kNotInDatabaseEnableRequests;
								break;
							} // switch
						} // else
						
					if (!cmd.recursive && !inEnableAll && ::FindRequestIndex (enableID, cmd.request) == 0) continue;
					} // if
					
				else if (lastRequest == 0) 
					continue;
				
				//	Did we have a seperator last time?
				Str255		cmdText;
				if (sepNeeded) {
					sepNeeded = false;

					//	Find the string
					cmd.strnIndex = FindRequestIndex (cmd.strnID, 0);
					if (!cmd.strnIndex) continue;
					::GetIndString (cmdText, cmd.strnID, cmd.strnIndex);
					
					//	Add to the menu
					if (noErr != (e = ::AddCommandToAEDescList (ioCommandList, cmdText, 0))) break;
					} // if
					
				//	Do we have one this time?
				if (!sepNeeded && (cmd.request == 0)) {
					sepNeeded = true;
					lastRequest = cmd.request;
					continue;
					} // if
				
				//	Find the string
				cmd.strnIndex = FindRequestIndex (cmd.strnID, cmd.request);
				if (!cmd.strnIndex) continue;
				::GetIndString (cmdText, cmd.strnID, cmd.strnIndex);
				
				//	Add to the menu
				if (noErr != (e = ::AddCommandToAEDescList (ioCommandList, cmdText, cmd.GetCmdID ()))) break;
				lastRequest = cmd.request;
				} // for
			
			} while (false);
		
	    return e;
	    
	} // AppendVCSSubmenu

/*******************************************************************************

	CreateVCSSubmenu

*******************************************************************************/

static OSStatus 
CreateVCSSubmenu (

	AEDescList* 	ioCommandList,
	const	FSSpec*	inContext,
	const	Boolean	inEnableAll)
	
	{ // begin CreateVCSSubmenu
		
		OSErr					e = noErr;
		
		short					refNum = -1;
		AEDescList 				theRegularCommands = { typeNull, nil };
		AEDescList 				theAdvancedCommands = { typeNull, nil };
		
		do {
			//	Find the project file.  Fail silently if no match is found
			FSSpec	projectSpec;
			if (noErr != VCSCMMContext::FindProjectFile (projectSpec, *inContext)) break;
			refNum = ::FSpOpenResFile (&projectSpec, fsCurPerm);
			if (noErr != ::ResError ()) break;

			//	Determine whether it is a folder or not
			DInfo					dInfo;
			Boolean					isFolder = (dirNFErr != ::FSpGetDInfo (inContext, &dInfo));

			//	Determine the checkout state
			CWVCSCheckoutState			checkoutState = cwCheckoutStateUnknown;
			if (!isFolder) {
				VCSCMMContext			context (inContext, reqFileVersion, false, false);
				if (cwCommandStatusSucceeded != CWCVSDispatch (context)) break;
				checkoutState = context.GetCheckoutState ();
				} // if
				
			//	Create regular commmands
			Str255					cmdText;
			CmdMap	cmdRegular = {kRegularCommandStrings, 0, 0, false, isFolder};
			if (noErr != (e = ::AECreateList (nil, 0, false, &theRegularCommands))) break;
			
				//	Create advanced commmands
				CmdMap	cmdAdvanced = {kAdvancedCommandStrings, 0, 0, true, isFolder};
				if (noErr != (e = ::AECreateList (nil, 0, false, &theAdvancedCommands))) break;
				if (noErr != (e = ::AppendVCSSubmenu (&theAdvancedCommands, kMenuRequests, cmdAdvanced, checkoutState, inEnableAll))) break;

				// Add submenu
				::GetIndString (cmdText, cmdAdvanced.strnID, FindRequestIndex (cmdAdvanced.strnID, reqInitialize));
				if (noErr != (e = ::AddMenuToAEDescList (&theRegularCommands, cmdText, &theAdvancedCommands))) break;
				
				//	Add seperator
				::GetIndString (cmdText, cmdRegular.strnID, FindRequestIndex (cmdRegular.strnID, 0));
				if (noErr != (e = ::AddCommandToAEDescList (&theRegularCommands, cmdText, 0))) break;
				
				//	Add commands
				if (noErr != (e = ::AppendVCSSubmenu (&theRegularCommands, kMenuRequests, cmdRegular, checkoutState, inEnableAll))) break;

			// Add submenu
			::GetIndString (cmdText, cmdRegular.strnID, FindRequestIndex (cmdRegular.strnID, reqInitialize));
			if (noErr != (e = ::AddMenuToAEDescList (ioCommandList, cmdText, &theRegularCommands))) break;
			} while (false);
			
		// clean up after ourself
		::AEDisposeDesc (&theAdvancedCommands);
		::AEDisposeDesc (&theRegularCommands);
		
		if (refNum != -1) ::CloseResFile (refNum);
		refNum = -1;
		
		return e;

	} // CreateVCSSubmenu

#pragma mark -

/*******************************************************************************

	CMCVSInitialize
	
		All plugin SOM object must somehow register themselves so clients
		can instantiate them by name.  The best place that I've found to
		do it is in the fragment initializer.

*******************************************************************************/

pascal OSErr 
CMCVSInitialize (

	CFragInitBlockPtr init)

	{ // begin CMCVSInitialize

		OSErr e = __initialize (init);
		if (noErr != e) goto CleanUp;
			
		// register our class with SOM
		somNewClass(CMCVS);
	
	CleanUp:
	
		return e;
		
	} // end CMCVSInitialize

/*******************************************************************************

	CMCVS::Initialize

*******************************************************************************/

OSStatus  
CMCVS::Initialize (
	
	Environment*,
	FSSpec* 		inFileSpec)
	
	{ // begin Initialize

		// Remember our resource file
		mPluginFile = *inFileSpec;
		
		mPluginInitialized = false;
		
	    return noErr;
	    
	} // end Initialize

/*******************************************************************************

	CMCVS::ExamineContext

*******************************************************************************/

OSStatus  
CMCVS::ExamineContext (

	Environment*,
	AEDesc*				inContextDescriptor,
	SInt32 				/* inTimeOutInTicks */,
	AEDescList* 		ioCommands,
	Boolean*			outNeedMoreTime)

	{ // begin ExamineContext
		
		OSErr	e = noErr;
		SInt16	refNum = -1;
		
		do {
			// make sure the descriptor isn't null
			if (!inContextDescriptor) break;
			
			//	Make sure it is a file type
			AEDesc 		theFSSDesc = {typeNull, nil};
			if (noErr != ::AECoerceDesc (inContextDescriptor, typeFSS, &theFSSDesc)) break;
			FSSpec		theFSS = **(FSSpec **) theFSSDesc.dataHandle;
			::AEDisposeDesc (&theFSSDesc);
				
			// Open our resource file
			refNum = ::FSpOpenResFile (&mPluginFile, fsCurPerm);
			if (noErr != (e = ::ResError ())) break;
			
			// Just for kicks, lets create a submenu for our plugin
			::CreateVCSSubmenu (ioCommands, &theFSS, ::CheckScanCode (kAlphaScan));
			} while (false);
		
		if (refNum != -1) ::CloseResFile (refNum);
		
		*outNeedMoreTime = false;

		return e;
		
	} // CMCVS::ExamineContext

/*******************************************************************************

	CMCVS::HandleSelection

*******************************************************************************/
// here is where you would actually carry out the action that the user
// requested.

OSStatus 
CMCVS::HandleSelection (

	Environment*,
	AEDesc*			inContextDescriptor,
	SInt32 			inCommandID)

	{ // begin HandleSelection
		
		OSErr	e = noErr;

		SInt16	pluginRefNum = -1;
		SInt16	projectRefNum = -1;
		
		do {
			// make sure the descriptor isn't null
			if (!inContextDescriptor) break;
			
			//	Make sure it is a file type
			AEDesc 		theFSSDesc = {typeNull, NULL };
			if (noErr != ::AECoerceDesc (inContextDescriptor, typeFSS, &theFSSDesc)) break;
			FSSpec		theFSS = **(FSSpec **) theFSSDesc.dataHandle;
			::AEDisposeDesc (&theFSSDesc);
				
			// Open our resource file
			pluginRefNum = ::FSpOpenResFile (&mPluginFile, fsCurPerm);
			if (noErr != (e = ::ResError ())) break;
			
			//	Initialize
			if (!mPluginInitialized) {
				VCSCMMContext	initContext (0, reqInitialize, false, false);
				if (cwCommandStatusSucceeded != CWCVSDispatch (initContext)) break;
				
				mPluginInitialized = true;
				} // if
				
			//	Find the project file
			FSSpec	projectSpec;
			if (noErr != (e = VCSCMMContext::FindProjectFile (projectSpec, theFSS))) break;
			projectRefNum = ::FSpOpenResFile (&projectSpec, fsCurPerm);
			if (noErr != (e = ::ResError ())) break;
			
			//	Dispatch the request
			CmdMap			cmd;
			cmd.SetCmdID (inCommandID);

			VCSCMMContext	context (&theFSS, cmd.request, cmd.advanced, cmd.recursive);
			CWCVSDispatch (context);
			} while (false);
		
		if (projectRefNum != -1) ::CloseResFile (projectRefNum);
		projectRefNum = 0;
		
		if (pluginRefNum != -1) ::CloseResFile (pluginRefNum);
		pluginRefNum = 0;
		
	    return e;
	    
	} // end HandleSelection

/*******************************************************************************

	CMCVS::PostMenuCleanup

*******************************************************************************/
// we didn't allocate any buffers or cache any data in ExamineContext,
// so we don't need to clean anything up here

OSStatus
CMCVS::PostMenuCleanup (

	Environment*)

	{ // begin PostMenuCleanup
		
	    return noErr;
	    
	} // end PostMenuCleanup

