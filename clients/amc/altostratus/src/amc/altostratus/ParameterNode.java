/*
File:		ParameterNode.java

Contains:	Represents a Stratus parameter saved in a TreeModel 

Written by:	David Dunham
		Electric Fish, Inc. <http://www.electricfish.com>

Copyright:	Copyright ©2000-2001 by Applied Microsystems Corporation.  All Rights reserved.

Change History (most recent first):

	05 Apr 2001		drd	Changes for packages
	02 Mar 2001		drd	Error reporting is now in the Error class
	01 Mar 2001		drd	Call showCommFailure if there's a CORBA exception
	26 Jan 2001		drd	Override children
	30 Nov 2000		drd	getDisplayName
	21 Nov 2000		drd	Fixed determineChildren
	17 Nov 2000		drd	getFullName
	15 Nov 2000		drd	Created
*/

package amc.altostratus;

import amc.altostratus.constants.ParamConstants;
import java.util.Enumeration;
import javax.swing.tree.*;			// JTree related classes (but not JTree)

// CORBA stuff
import VteEngineServer.*;

public class ParameterNode extends DefaultMutableTreeNode implements ParamConstants {
// Instance data
	protected	int				fLevel;
	protected	String			fDisplayName;
	protected	String			fFullName;
	protected	char				fHasChildren;
	protected	VteObjectContainer	fContainer;

	/**
	Constructor
		@param aString Tab-delimited string, indicating level and both names
	*/
	ParameterNode(String aString, VteObjectContainer aContainer) {
		fContainer = aContainer;
		fHasChildren = kUnknown;

		int		ln = aString.length();
		int		first = 0;
		int		nTabs = 0;
		int		i;
		char	c;

		// Determine the level by counting tabs
		for (i = 0; i < ln; i++) {
			c = aString.charAt(i);
			if (c == '\t') {
				first = i + 1;
				nTabs++;
			} else
				break;
		}
		fLevel = nTabs + 1;

		// Get the two names
		for (i = first; i < ln; i++) {
			c = aString.charAt(i);
			if (c == '\t') {
				// Remember, substring is OBO, it's EXCLUSIVE of endIndex
				fDisplayName = aString.substring(first, i);
				fFullName = aString.substring(i + 1);
				break;
			}
		}

		// Save in instance data for inherited
		userObject = fDisplayName;
	} // ParameterNode

	/**
	Creates and returns a forward-order enumeration of this node's children
			@return	an Enumeration of this node's children
	*/
    public Enumeration children() {
		switch (fHasChildren) {
			case kChildren:
				break;

			case kNoChildren:
				return EMPTY_ENUMERATION;

			case kUnknown:
				this.determineChildren();
		}
    	return super.children();
    }

	/**
	Called when we don't know if we have children or not. If we do, read them in.
	Either way, fHasChildren will be set to a known value.
	*/
	private boolean determineChildren() {
///		System.out.println("determineChildren: " + fDisplayName + " " + fLevel + " (" + fFullName + ")");
		String[]	items = null;
		try {
			items = fContainer.Catalog(fFullName, 1);
		} catch (org.omg.CORBA.SystemException ce) {
			// An exception here pretty much means we lost contact
			Error.showCommFailure(ce);
			throw ce;
		} catch (Exception e) {
			System.out.println(e);
			fHasChildren = kNoChildren;
			return false;
		}

		int			n = items.length;
		if (n == 1)
			fHasChildren = kNoChildren;
		else
			fHasChildren = kChildren;
		int			i;
		try {
			for (i = 1; i < n; i++) {
				ParameterNode	child = new ParameterNode(items[i], fContainer);
				this.add(child);
///				System.out.println(" [" + i + "]=" + items[i]);
			}
		} catch (org.omg.CORBA.SystemException ce) {
			// An exception here pretty much means we lost contact
			Error.showCommFailure(ce);
			throw ce;
		}

		return (fHasChildren == kChildren);
	}

	/**
	If we have previously calculated, return whether or not this node has kids. Otherwise,
	we determine this by asking the CORBA container.
	*/
	public boolean getAllowsChildren() {
		switch (fHasChildren) {
			case kChildren:
				return true;

			case kNoChildren:
				return false;

			case kUnknown:
				return this.determineChildren();
		}

		// Shouldn't get hereÉ
		return true;
	}

	/**
	Returns a more human-readable name
	*/
	public String getDisplayName() {
		return fDisplayName;
	} // getDisplayName

	/**
	*/
	public String getFullName() {
		return fFullName;
	}
}
