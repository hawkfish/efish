/*
File:		Bartender.java

Contains:	Manages the menus

Written by:	David Dunham

Copyright:	Copyright ©2000-2001 by Applied Microsystems Corporation.  All Rights reserved.

Change History (most recent first):

	05 Apr 2001		drd	Put into framework package
	03 Oct 2000		drd	Created
*/

package amc.altostratus.framework;

import java.awt.*;				// AWT classes
import javax.swing.*;				// Swing components and classes

public class Bartender extends JMenuBar
{
	/**
	Bartender
	*/
	public Bartender()
	{
	}

	/**
	findAction
		Search our menus for the specified command; return the associated Action.
		??? This is not recursive
	*/
	public Action findAction(String aCommand)
	{
		int					nMenus = this.getMenuCount();
		MenuElement[]			menus = this.getSubElements();
		for (int i = 0; i < nMenus; i++) {
			MenuElement			theMenu = menus[i];
			MenuElement[]		items = theMenu.getSubElements();
			int				nItems = items.length;
			for (int e = 0; e < nItems; e++) {
				MenuElement		theElement = items[e];
				if (theElement instanceof JMenuItem) {
					JMenuItem		theItem = (JMenuItem)theElement;
					String		theCommand = theItem.getActionCommand();
					if (theCommand.equals(aCommand)) {
						System.out.println("found=" + theCommand);
//						Action	theAction = theItem.getAction();
//						System.out.println("found=" + theAction);
//						return theAction;
					}
				}
			}
		}

		return null;
	} // findAction
}
