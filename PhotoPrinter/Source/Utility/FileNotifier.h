/*
	File:		FileNotifier.h

	Contains:	Redistributes notification of a file name change

	Written by:	David Dunham

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		24 Jul 2001		rmgw	Broadcast new FSSpec.  Bug #219.
		18 Jul 2001		drd		194 Created
*/

#pragma once

#include <LBroadcaster.h>

class	FileNotifier : public LBroadcaster {

public:
	
	enum {
		msg_FilenameChanged		= 'file'			//	const	FSSpec*
		};
	
	static	void	Listen	(LListener* 	inListener);
	static	void	Notify	(const	FSSpec& inNewSpec);

protected:
	static	FileNotifier*	gSingleton;
};
