/*
File:		DisplayParser.java

Contains:	Handles XML and adds Strips to a DisplayPanel

Written by:	David Dunham
		Electric Fish, Inc. <http://www.electricfish.com>

Copyright:	Copyright ©2000-2001 by Applied Microsystems Corporation.  All Rights reserved.

Change History (most recent first):

	05 Apr 2001		drd	Put into package
	29 Mar 2001		drd	Added Vectors for summarized error reporting as in 2.4.9.2
	27 Mar 2001		drd	makeStripIfNeeded checks for CORBA exceptions
	08 Mar 2001		drd	Do some validity checking for <format>
	21 Dec 2000		drd	makeStripIfNeeded actually does what it says
	18 Dec 2000		drd	Working, and error handling
	13 Dec 2000		drd	Created
*/

package amc.altostratus.xml;

import amc.altostratus.constants.ParamConstants;
import amc.altostratus.DisplayPanel;
import amc.altostratus.Parameter;
import amc.altostratus.Strip;
import java.util.Vector;
import org.xml.sax.Attributes;
import org.xml.sax.Locator;
import org.xml.sax.SAXException;
import org.xml.sax.SAXParseException;
import org.xml.sax.helpers.DefaultHandler;

public class DisplayParser extends DefaultHandler implements ParamConstants
{
	private	boolean		fError;
	private	DisplayPanel	fPanel;		// Parse for this object
	private	boolean		fInList;
	private	boolean		fInParameter;
	private	StringBuffer	fCurData;
	private	String		fCurTag;
	private	String		fLongName;
	private	String		fShortName;
	private	Strip			fStrip;
	private	Vector		fNameNotFound;
	private Vector		fStripsToAdd;
	private	Vector		fTypeMismatch;

	private	static	Locator	gLocator;	// A global so we can construct ParseException inner class

	public DisplayParser(DisplayPanel aPanel) {
		fPanel = aPanel;
		fNameNotFound = new Vector();
		fStripsToAdd = new Vector();
		fTypeMismatch = new Vector();
	}

 	/**
 	Receive notification of character data.
	*/
	public void characters(char[] aChars, int aStart, int aLength) throws SAXException {
		if (fCurTag == null) {
			// We shouldn't be getting data not inside of a tag (i.e. outside <parameter>)
		} else {
			if (fCurData == null)
				fCurData = new StringBuffer();
			fCurData.append(aChars, aStart, aLength);
		}
	}

	public void endDocument() throws SAXException {
		System.out.println("#endDocument");
	}

	/**
	Receive notification of the end of an element.
	*/
	public void endElement(String aNamespace, String aLocalName, String aTag) throws SAXException {
//		System.out.println("/" + aTag + " " + fCurData);
		try {
			if (aTag.equals("parameters")) {
				fInList = false;
			} else if (aTag.equals("parameter") && !fError) {
				fInParameter = false;
				this.makeStripIfNeeded();
				fStripsToAdd.addElement(fStrip);
				fPanel.add(fStrip);
				fStrip = null;
				fError = false;
			} else if (aTag.equals("longName")) {
				fLongName = fCurData.toString();
			} else if (aTag.equals("shortName")) {
				fShortName = fCurData.toString();
			} else if (aTag.equals("format") && !fError) {
				this.makeStripIfNeeded();
				char	theFormat = fCurData.charAt(0);			// We just grab the first character
				if (format_AllFormats.indexOf(theFormat) >= 0)
					fStrip.setFormat(theFormat);
				else
					throw new ParseException("unknown format \"" + fCurData + "\"");
			} else if (aTag.equals("max") && !fError) {
				this.makeStripIfNeeded();
				// !!!
			} else if (aTag.equals("min") && !fError) {
				this.makeStripIfNeeded();
				// !!!
			} else if (aTag.equals("radix") && !fError) {
				this.makeStripIfNeeded();
				fStrip.setRadix(fCurData.charAt(0));
			} else {
				System.err.println("unknown tag " + aTag);
				// ??? an unknown tag
			}
		} catch (StripException se) {
			System.out.println("caught StripException " + se);
			fNameNotFound.addElement(se.getMessage());
			fError = true;
		}
	}

	/**
	Handle a non-fatal error (by treating it as fatal)
	*/
	public void error(SAXParseException anException) throws SAXParseException {
		throw anException;
	}

	/**
	@return The total number
	*/
	public int getErrorCount() {
		return this.getNameNotFoundCount() + this.getTypeMismatchCount();
	} // getErrorCount

	public String getNameNotFound(int anIndex) {
		return (String) fNameNotFound.elementAt(anIndex);
	}

	public int getNameNotFoundCount() {
		return fNameNotFound.size();
	} // getNameNotFoundCount

	public int getGoodParameterCount() {
		return fStripsToAdd.size();
	} // getGoodParameterCount

	public int getTypeMismatchCount() {
		return fTypeMismatch.size();
	} // getTypeMismatchCount

	/**
	Be sure fStrip exists
	*/
	private void makeStripIfNeeded() throws ParseException, StripException {
		if (fStrip == null) {
			try {
				Parameter	p = new Parameter(fLongName, fShortName);
				fStrip = new Strip(p);
			} catch (org.omg.CORBA.SystemException se) {
//				se.printStackTrace();
				throw new StripException(fLongName);
			} catch (Exception ue) {
				ue.printStackTrace();
				throw new ParseException("\"" + fShortName + "\" generated " + ue);
			}
		}
	} // makeStripIfNeeded

	/**
	Receive an object for locating the origin of SAX document events.
		@param aLocator An object that can return the location of any SAX document event.
		@see org.xml.sax.Locator
	*/
	public void setDocumentLocator(Locator aLocator) {
		gLocator = aLocator;		// Stash it in case we need it
		super.setDocumentLocator(aLocator);
	}

	/**
	Receive notification of the beginning of an element.

	<p>The Parser will invoke this method at the beginning of every
	element in the XML document; there will be a corresponding
	{@link #endElement endElement} event for every startElement event
	(even when the element is empty). All of the element's content will be
	reported, in order, before the corresponding endElement
	event.</p>

		@param aNamespace The Namespace URI, or the empty string if the
			element has no Namespace URI or if Namespace processing is not being performed.
		@param aLocalName The local name (without prefix), or the
			empty string if Namespace processing is not being performed.
		@param aTag The qualified name (with prefix), or the
			empty string if qualified names are not available.
		@param aList The attributes attached to the element.  If
			there are no attributes, it shall be an empty Attributes object.
		@exception org.xml.sax.SAXException Any SAX exception, possibly wrapping another exception.
		@see #endElement
		@see org.xml.sax.Attributes
	*/
	public void startElement(String aNamespace, String aLocalName, String aTag, Attributes aList) throws SAXException {
//		System.out.println(aTag);
		
		if (aTag.equals("parameters")) {
			if (fInList) {
				throw new ParseException("\"parameters\" may not be nested");
			}
			fInList = true;
		} else if (aTag.equals("parameter")) {
			if (!fInList) {
				throw new ParseException("\"parameter\" must be inside \"parameters\"");
			} else if (fInParameter) {
				throw new ParseException("\"parameter\" may not be nested");
			}
			fInParameter = true;
			fCurTag = null;
			fError = false;
		} else if (fInParameter) {
			// We are allowing just about anything inside of <parameter>
			fCurTag = aTag;
			fCurData = null;
		} else {
			throw new ParseException("\"" + aTag + "\" not in \"parameter\"");
		}
	}

	/**
	Inner class which lets us report errors with a line number
	*/
	class ParseException extends SAXParseException
	{
		ParseException(String aString) {
			super(aString, gLocator);
		}
	}

	/**
	Inner class for reporting errors creating a Strip
	*/
	class StripException extends Exception
	{
		StripException(String aString) {
			super(aString);
		}
	}
}
