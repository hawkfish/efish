/*
File:		XMLWriter.java

Contains:	Outputs XML

Written by:	David Dunham
		Electric Fish, Inc. <http://www.electricfish.com>

Copyright:	©2000-2001 by Applied Microsystems Corporation.  All Rights reserved.

Change History (most recent first):

	05 Apr 2001		drd	Put into package
	04 Jan 2001		drd	writeElement for int
	13 Dec 2000		drd	Created
*/

package amc.altostratus.xml;

import java.io.*;
import java.util.*;

public class XMLWriter extends OutputStreamWriter
{
	static final int	kIndent = 4;

	private	int		fIndent;	// Current indent level (spaces)
	private	Stack		fTags;	// XML elements processed so far

    /**
     * Create an OutputStreamWriter that uses the named character encoding.
     *
     * @param  out  An OutputStream
     * @param  enc  Name of the encoding to be used
     *
     * @exception  UnsupportedEncodingException
     *             If the named encoding is not supported
     */
	public XMLWriter(OutputStream out, String enc)
	throws UnsupportedEncodingException
	{
		super(out, enc);
		fIndent = 0;
		fTags = new Stack();
	}

	/**
	Complete processing of an XML element
	*/
	public void endElement() throws IOException {
		fIndent -= kIndent;
		this.writeIndentation();
		String	theTag = (String) fTags.pop();
		this.write("</" + theTag + ">");
		this.nl();
	}

	/**
	Output a newline
	*/
	public void nl() throws IOException {
		String		newline = System.getProperty("line.separator");
		this.write(newline);
	}

	/**
	Begin processing of an XML element
		@param aTag The name of the element
	*/
	public void startElement(String aTag) throws IOException {
		this.writeIndentation();
		this.write("<" + aTag + ">");
		fTags.push(aTag);
		fIndent += kIndent;
		this.nl();
	} // startElement

	/**
	Begin processing of an XML element
		@param aTag The name of the element
		@param anAttribute List of atributes
	*/
	public void startElement(String aTag, String anAttribute) throws IOException {
		this.writeIndentation();
		this.write("<" + aTag + " "+ anAttribute + ">");
		fTags.push(aTag);
		fIndent += kIndent;
		this.nl();
	} // startElement

	/**
	Outputs a comment
		@param aString The text
	*/
	public void writeComment(String aString) throws IOException {
		this.writeIndentation();
		this.write("<!-- " + aString + " -->");
		this.nl();
	} // writeComment

	/**
	Outputs a tagged value
		@param aTag The name of the element
		@param aChar The data
	*/
	public void writeElement(String aTag, char aChar) throws IOException {
		this.writeIndentation();
		this.write("<" + aTag + ">" + aChar + "</" + aTag + ">");
		this.nl();
	}

	/**
	Outputs a tagged value
		@param aTag The name of the element
		@param aChar The data
	*/
	public void writeElement(String aTag, int aValue) throws IOException {
		this.writeIndentation();
		this.write("<" + aTag + ">" + aValue + "</" + aTag + ">");
		this.nl();
	}

	/**
	Outputs a tagged value
		@param aTag The name of the element
		@param aString The data
	*/
	public void writeElement(String aTag, String aString) throws IOException {
		this.writeIndentation();
		this.write("<" + aTag + ">" + aString + "</" + aTag + ">");
		this.nl();
	}

	/**
	Outputs the current amount of indentation
	*/
	protected void writeIndentation() throws IOException {
		int		i;
		for (i = 1; i <= fIndent; i++) {
			this.write(" ");
		}
	}
}