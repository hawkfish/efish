/*
	File:		FileNotifier.h

	Contains:	Redistributes notification of a file name change

	Written by:	David Dunham

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

	18 Jul 2001		drd		194 Created
*/

class	FileNotifier : public LBroadcaster {
public:
	static	void	Listen(LListener* inListener);
	static	void	Notify(ConstStr255Param inOldName);

protected:
	static	FileNotifier*	gSingleton;
};
