/*
	File:		MakeIconCommand.cp

	Contains:	Implementation of the Make Icon command.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		18 Aug 2000		drd		Created
*/

#include "MakeIconCommand.h"

#include <Icons.h>
#include "MIconSuite.h"
#include "MResFile.h"
#include "PhotoPrintDoc.h"
#include <set>

/*
MakeIconCommand
*/
MakeIconCommand::MakeIconCommand(const CommandT inCommand, PhotoPrintDoc* inDoc)
	: PhotoDocCommandAttachment(inCommand, inDoc)
{
} // MakeIconCommand

/*
~MakeIconCommand
*/
MakeIconCommand::~MakeIconCommand()
{
} // ~MakeIconCommand

/*
ExecuteCommand
*/
void		
MakeIconCommand::ExecuteCommand(void* inCommandData)
{
#pragma unused(inCommandData)
	PhotoItemList		selection = mDoc->GetView()->Selection();
	PhotoIterator		i;
	set<MFileSpec>		files;			// The set makes sure we only get each file once
	for (i = selection.begin(); i != selection.end(); i++) {
		PhotoPrintItem*		image = *i;
		MFileSpec*			fs;
		fs = image->GetFileSpec();
		files.insert(MFileSpec(*fs));	// Construct a new one
	}

	MIconSuite				suite(kGenericRAMDiskIconResource, kSelectorAllAvailableData);
	set<MFileSpec>::iterator	f;
	for (f = files.begin(); f != files.end(); f++) {
		// Be sure we have a resource file
		FInfo		fi;
		(*f).GetFinderInfo(fi);
		(*f).CreateResFile(fi.fdType, fi.fdCreator);

		MResFile	theFile(*f, fsRdWrPerm);
		Handle		h;
		suite.GetIcon(h, 'ICN#');
		::DetachResource(h);
		theFile.Add(h, 'ICN#', kCustomIconResource);

		// Make sure the Finder knows it has a custom icon
		fi.fdFlags |= kHasCustomIcon;
		(*f).SetFinderInfo(fi);
	}
} // ExecuteCommand
									 
/*
FindCommandStatus {OVERRIDE}
*/
void		
MakeIconCommand::FindCommandStatus		(SCommandStatus*	ioStatus)
{
	*ioStatus->enabled = mDoc->GetView()->IsAnythingSelected();
} // FindCommandStatus
