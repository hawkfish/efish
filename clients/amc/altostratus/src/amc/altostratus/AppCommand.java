/*
File:		AppCommand.java

Contains:	

Written by:	David Dunham
		Electric Fish, Inc. <http://www.electricfish.com>

Copyright:	Copyright ©2001 by Applied Microsystems Corporation.  All Rights reserved.

Change History (most recent first):

//  Created by david on Wed Apr 04 2001.
*/

package amc.altostratus;
import amc.altostratus.framework.SafeAction;

/**
Any of the commands that require access to the application descend from this
*/
abstract class AppCommand extends SafeAction {
	protected	AltoStratus		fApplication;

	protected AppCommand(AltoStratus anApp, String aName) {
		super(aName);
		fApplication = anApp;
	}
}

