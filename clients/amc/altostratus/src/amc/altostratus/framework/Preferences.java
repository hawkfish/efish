/*
File:		Preferences.java

Contains:	Handles

Written by:	David Dunham
		Electric Fish, Inc. <http://www.electricfish.com>

Copyright:	Copyright ©2001 by Applied Microsystems Corporation.  All Rights reserved.

Change History (most recent first):

	05 Apr 2001		drd	Put into framework package
	14 Mar 2001		drd	setObjectForKeyNoWrite; changed args to String since we are Properties
	12 Mar 2001		drd	Created
*/

package amc.altostratus.framework;

import java.io.*;
import java.util.Properties;

public class Preferences extends Properties {
	// Class globals
	private static Preferences	gPrefs;

	// Instance data
	private File	fFile;
	private	String	fPath;

	private Preferences(String aFolder, String aName) {
		// Figure out where our preferences lives
		String					homePath = System.getProperty("user.home");
		if (aFolder != null) {
			try {
				// We may need to make a directory
				File				theFolder = new File(homePath, aFolder);
				if (!theFolder.exists()) {
					theFolder.mkdirs();		// Recursively make directories as needed
				}
				homePath = theFolder.getCanonicalPath();
			} catch (IOException e) {
				e.printStackTrace();
				// !!! do something, maybe alert
			}
		}
		fFile = new File(homePath, aName);
//		System.out.println("fFile= " + fFile);

		try {
			FileInputStream		theStream = new FileInputStream(fFile);
			this.load(theStream);
		} catch (IOException e) {
			e.printStackTrace();
			// !!! do something, maybe delete
		}
		this.list(System.out);
	}

	public void finalize() throws Throwable {
		this.flush();
		super.finalize();
	}

	protected void flush() {
		try {
			FileOutputStream	theStream = new FileOutputStream(fFile);
			this.save(theStream, "AltoStratus Preferences");
		} catch (IOException e) {
			e.printStackTrace();
			// !!! do something, maybe alert
		}
	}

	/**
	Retrieves a value from the preferences
		@param aKey The value's key
	*/
	public static String getObjectForKey(String aKey) {
		return gPrefs.getProperty(aKey);
	}

	/**
	Creates a new global preferences manager
		@param aFolder Name of folder (may be null)
		@param aName File name
	*/
	public static void makePreferences(String aFolder, String aName) {
		gPrefs = new Preferences(aFolder, aName);
	}

	/**
	Stores a value into the preferences
		@param anObject The value to save
		@param aKey The value's key
	*/
	public static void setObjectForKey(String anObject, String aKey) {
		setObjectForKeyNoWrite(anObject, aKey);
		gPrefs.flush();
	}

	/**
	Stores a value into the preferences
		@param anObject The value to save
		@param aKey The value's key
	*/
	public static void setObjectForKeyNoWrite(String anObject, String aKey) {
		gPrefs.put(aKey, anObject);
		// gPrefs.setProperty(aKey, anObject);
	}

	public static void write() {
		gPrefs.flush();
	}

}
