/*
File:		Error.java

Contains:	Error reporting for AltoStratus, the graphical interface to Stratus

Written by:	David Dunham
			Electric Fish, Inc. <http://www.electricfish.com>

Copyright:	Copyright ©2001 by Applied Microsystems Corporation.  All Rights reserved.

Change History (most recent first):

	07 Mar 2001		drd	Moved showOpenError here
	02 Mar 2001		drd	Created
*/

package amc.altostratus;

import javax.swing.JOptionPane;
import javax.swing.SwingUtilities;

/**
The main class (application)
	@author David Dunham
*/
public class Error
{
	// Globals
	private	static Exception	gException;
	private	static String		gString;

	/**
	Display an error message for the exception, which we presume to have some relation to CORBA.
	*/
	public static void showCommFailure(Exception anException) {
		JOptionPane.showMessageDialog(null, "Problem talking to the CORBA server." +
			System.getProperty("line.separator") + "(" + anException + ")",
			"Communication Failure", JOptionPane.ERROR_MESSAGE);
		// Note: we would have used fWindow except we are a static function, not a method
	}

	/**
	Display an error message for the exception.
	*/
	public static void showGenericFailure(Exception anException, String aCommand) {
		gException = anException;
		gString = aCommand;

		Runnable call = new Runnable() {
			public void run() {
				JOptionPane.showMessageDialog(null, gString + " failed." +
					System.getProperty("line.separator") + "(" + gException + ")",
					"Could not complete action", JOptionPane.ERROR_MESSAGE);
				// Note: we would have used fWindow except we are a static function, not a method
			}
		};
		SwingUtilities.invokeLater(call);
	}

	/**
	Report on an error opening a file
	*/
	public static void showOpenError(String aMessage) {
		JOptionPane.showMessageDialog(null, aMessage, "Open failed", JOptionPane.ERROR_MESSAGE);
	} // showOpenError

	/**
	Report on an error parsing
	*/
	public static void showParseError(String aTitle, String aMessage) {
		JOptionPane.showMessageDialog(null, aMessage, aTitle, JOptionPane.ERROR_MESSAGE);
	} // showOpenError

}