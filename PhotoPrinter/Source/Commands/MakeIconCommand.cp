/*
	File:		MakeIconCommand.cp

	Contains:	Implementation of the Make Icon command.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		07 Sep 2000		drd		Make more icon depths and sizes
		06 Sep 2000		drd		Actually create icons
		18 Aug 2000		drd		Created
*/

#include "MakeIconCommand.h"

#include <Icons.h>
#include <map>
#include "MIconSuite.h"
#include "MResFile.h"
#include "PhotoPrintDoc.h"

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

typedef std::map<MFileSpec, PhotoItemRef>		FileItemMap;

/*
ExecuteCommand
*/
void		
MakeIconCommand::ExecuteCommand(void* inCommandData)
{
#pragma unused(inCommandData)
	PhotoItemList		selection = mDoc->GetView()->Selection();
	PhotoIterator		i;
	PhotoItemRef		image;
	FileItemMap			files;			// The map makes sure we only get each file once
	for (i = selection.begin(); i != selection.end(); i++) {
		image = *i;
		const MFileSpec*	fs = image->GetFileSpec();
		
		FileItemMap::value_type		thePair(*fs, image);
		files.insert(thePair);
	}

	FileItemMap::iterator	f;
	for (f = files.begin(); f != files.end(); f++) {
		CInfoPBRec	pb;
		FInfo		fi;
		MFileSpec	theSpec((*f).first);	// Make a copy since we will use non-const methods
		theSpec.GetCatInfo(pb);
		unsigned long	origModDate = pb.hFileInfo.ioFlMdDat;
		theSpec.GetFinderInfo(fi);
		if (!theSpec.HasResourceFork()) {
			// Be sure we have a resource fork (so MResFile doesn't throw)
			theSpec.CreateResFile(fi.fdType, fi.fdCreator);
		}

		// Do resource stuff
		{
			MResFile	theFile(theSpec, fsRdWrPerm);
			Handle		h;
			image = (*f).second;
			h = image->MakeIcon('ICN#');
			theFile.Add(h, 'ICN#', kCustomIconResource);

			h = image->MakeIcon('ics#');
			theFile.Add(h, 'ics#', kCustomIconResource);

			h = image->MakeIcon('icl4');
			theFile.Add(h, 'icl4', kCustomIconResource);

			h = image->MakeIcon('ics4');
			theFile.Add(h, 'ics4', kCustomIconResource);

			h = image->MakeIcon('icl8');
			theFile.Add(h, 'icl8', kCustomIconResource);

			h = image->MakeIcon('ics8');
			theFile.Add(h, 'ics8', kCustomIconResource);
		} // Resource fork is now closed, so we can mess with meta-data

		// Make sure the Finder knows it has a custom icon, and clear the inited flag
		pb.hFileInfo.ioFlFndrInfo.fdFlags |= kHasCustomIcon;
		pb.hFileInfo.ioFlFndrInfo.fdFlags &= ~kHasBeenInited;

		// Reset modification date
		pb.hFileInfo.ioFlMdDat = origModDate;
		theSpec.SetCatInfo(pb);
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
