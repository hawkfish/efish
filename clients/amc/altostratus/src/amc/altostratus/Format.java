/*
File:		Format.java

Contains:	Utilities related to formatting

Written by:	David Dunham
		Electric Fish, Inc. <http://www.electricfish.com>

Copyright:	Copyright ©2000-2001 by Applied Microsystems Corporation.  All Rights reserved.

Change History (most recent first):

	07 Mar 2001		drd	No more format_Control
	31 Jan 2001		drd	isSimple, parseInt
	21 Dec 2000		drd	Got rid of radix-related formats (now just format_Number and format_Control)
	01 Dec 2000		drd	Created
*/

package amc.altostratus;

import amc.altostratus.constants.ParamConstants;

class Format implements ParamConstants {
	/**
	Test whether or not a format is considered numeric
	*/
	public static boolean isNumeric(char inFormat) {
		return (inFormat == format_Number);
	}

	/**
	Test whether or not a format needs a Displayer
	*/
	public static boolean isSimple(char inFormat) {
		return (inFormat == format_Name || inFormat == format_Number);
	}

	/**
	Calls Integer.parseInt depending on
	*/
	public static int parseInt(String aString) throws NumberFormatException {
		if (aString.startsWith("0x") || aString.startsWith("0X")) {
			return Integer.parseInt(aString.substring(2), 16);
		} else {
			return Integer.parseInt(aString);
		}
	}

	/**
	Converts a 32 bit quantity to hex
	*/
	public static String toHexString(int inValue) {
		String	hex = Integer.toHexString(inValue);
		return "0x" + hex;
	}
}
