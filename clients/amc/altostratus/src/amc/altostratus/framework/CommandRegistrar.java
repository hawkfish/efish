/*
File:		CommandRegistrar.java

Contains:	Tracks the relationship between commands and actions

Written by:	David Dunham

Copyright:	Copyright ©2000-2001 by Applied Microsystems Corporation.  All Rights reserved.

Change History (most recent first):

	05 Apr 2001		drd	Put into framework package
	25 Jan 2001		drd	Sanity-checking
	02 Nov 2000		drd	checkAction, uncheckAction; use CommandAction
	04 Oct 2000		drd	Created
*/

package amc.altostratus.framework;

import java.util.*;				// Utility classes and functions
import javax.swing.*;				// Swing components and classes

public class CommandRegistrar extends Hashtable {
// Globals
	public static CommandRegistrar gRegistrar = new CommandRegistrar();

// Methods
	/**
	Marks an action as being checked (selected)
	*/
	public synchronized void checkAction(String aCommand) {
		CommandAction	theAction = this.getAction(aCommand);
		if (theAction != null)
			theAction.setSelected(true);
	}
 
	/**
	Disables an action (making its associated menu item dim)
	*/
	public synchronized void disableAction(String aCommand) {
		Action	theAction = this.getAction(aCommand);
		if (theAction != null)
			theAction.setEnabled(false);
	} // disableAction

	/**
	enableAction
	*/
	public synchronized void enableAction(String aCommand) {
		Action	theAction = this.getAction(aCommand);
		if (theAction != null)
			theAction.setEnabled(true);
	} // enableAction

	/**
	getAction
		Returns the Action associated with the specified command.
		synchronized because Hashtable.get is.
	*/
	public synchronized CommandAction getAction(String aCommand) {
		CommandAction	theAction = (CommandAction) this.get(aCommand);
		if (theAction == null)
			System.out.println("%" + aCommand + " not found");
		return theAction;
	} // getAction

	/**
	putAction
		Adds an Action with the associated command string (which is a lookup key).
		synchronized because Hashtable.put is.
	*/
	public synchronized void putAction(String aCommand, CommandAction anAction) {
		this.put(aCommand, anAction);
	} // putAction

	/**
	Marks an action as being unchecked (deselected). Note that this probably doesn't do the right thing
	if we want to uncheck all items in a group.
	*/
	public synchronized void uncheckAction(String aCommand) {
		CommandAction	theAction = this.getAction(aCommand);
		theAction.setSelected(false);
	} // uncheckAction
}
