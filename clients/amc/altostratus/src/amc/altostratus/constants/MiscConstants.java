/*
File:		MiscConstants.java

Contains:	Place to put constants so they don't have to be qualified

Written by:	David Dunham

Copyright:	Copyright ©2001 by Applied Microsystems Corporation.  All Rights reserved.

Change History (most recent first):

	05 Apr 2001		drd	Constants are their own package
	07 Mar 2001		drd	Added kCancel, kKeepChanges
	06 Mar 2001		drd	Added kOpen, kAppend
	29 Jan 2001		drd	Created
*/

package amc.altostratus.constants;

public interface MiscConstants
{
	boolean	kFirstPass = true;
	boolean	kSecondPass = false;

	// For open
	char	kAppend = 'A';
	char	kOpen = 'O';

	char	kCancel = 'x';
	char	kKeepChanges = 'K';
}
