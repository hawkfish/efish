/*
	File:		FileNotifier.cp

	Contains:	Redistributes notification of a file name change.

				There is a single global instance of FileNotifier.
				Objects interested in notification call Listen()
				Other objects send notification by calling Notify().
				The global then sends msg_FilenameChanged to any listeners.

	Written by:	David Dunham

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		24 Jul 2001		rmgw	Broadcast new FSSpec.  Bug #219.
		18 Jul 2001		drd		194 Created
*/

#include "FileNotifier.h"

#include "PhotoPrintCommands.h"

FileNotifier*	
FileNotifier::gSingleton = 0;

/*
Listen [static]
*/
void	FileNotifier::Listen(LListener* inListener)
{
	if (gSingleton == nil)
		gSingleton = new FileNotifier();

	gSingleton->AddListener(inListener);
} // Listen

/*
Notify [static]
*/
void	FileNotifier::Notify(const	FSSpec& inNewSpec)
{
	Assert_(gSingleton != nil);

	gSingleton->BroadcastMessage(msg_FilenameChanged, (void*) &inNewSpec);
} // Notify
