/*
File:		Editor.java

Contains:	Interface for a value editor in an AltoStratus strip.

Written by:	David Dunham
		Electric Fish, Inc. <http://www.electricfish.com>

Copyright:	Copyright ©2001 by Applied Microsystems Corporation.  All Rights reserved.

Change History (most recent first):

	07 Mar 2001		drd	hasBeenEdited
	28 Feb 2001		drd	First actual methods: getCurrentValue, validateCurrentValue
	21 Feb 2001		drd	Created
*/

package amc.altostratus;

interface Editor
{
	public String	getCurrentValue();
	public boolean	hasBeenEdited();
	public boolean	validateCurrentValue();
}
