/*
	File:		FileNotifier.cp

	Contains:	Redistributes notification of a file name change.

				There is a single global instance of FileNotifier.
				Objects interested in notification call Listen()
				Other objects send notification by calling Notify().
				The global then sends msg_FilenameChanged to any listeners.

	Written by:	David Dunham

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

	18 Jul 2001		drd		194 Created
*/

#include "FileNotifier.h"
#include "PhotoPrintCommands.h"

FileNotifier*	FileNotifier::gSingleton = nil;

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
void	FileNotifier::Notify(ConstStr255Param inOldName)
{
	Assert_(gSingleton != nil);

	gSingleton->BroadcastMessage(msg_FilenameChanged, (void*) inOldName);
} // Notify
