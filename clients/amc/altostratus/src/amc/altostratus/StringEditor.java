/*
File:		StringEditor.java

Contains:	Interface for a string value editor in an AltoStratus strip.

Written by:	David Dunham
		Electric Fish, Inc. <http://www.electricfish.com>

Copyright:	Copyright ©2001 by Applied Microsystems Corporation.  All Rights reserved.

Change History (most recent first):

	05 Apr 2001		drd	Changes for packages
	07 Mar 2001		drd	Constructor for int; added fHasBeenEdited, fEditingParameter
	01 Mar 2001		drd	New constructor; start selected
	28 Feb 2001		drd	getCurrentValue, validateCurrentValue
	22 Feb 2001		drd	Created
*/

package amc.altostratus;

import amc.altostratus.constants.ParamConstants;
import amc.altostratus.framework.CommandRegistrar;
import java.awt.*;				// AWT classes
import javax.swing.*;				// Swing components and classes
import javax.swing.text.*;

class StringEditor extends JTextField implements Editor, ParamConstants
{
	public		boolean		fEditingParameter;
	protected	boolean		fHasBeenEdited;

	/**
	This is the "base" constructor, used by ourself and subclasses
	*/
	protected StringEditor(String aStartingValue) {
		super(aStartingValue);

		this.setHorizontalAlignment(RIGHT);

		this.selectAll();
		fHasBeenEdited = false;
	}

	/**
	Constructs a new empty StringEditor with the specified number of columns.
	A default model is created and the initial string is set to null.

	@param aColumns  the number of columns to use to calculate the preferred width.
		If aColumns is set to zero, the preferred width will be whatever naturally
		results from the component implementation.
	*/ 
	public StringEditor(int aColumns) {
		super(aColumns);

		// Don't make right-justified
	}

	/**
	This is the public constructor
	*/
	public StringEditor(Strip aStrip, Parameter aParameter) {
		this(aParameter.getValueString(format_Name, radix_Decimal));
		fEditingParameter = true;
	} // StringEditor

	/**
	Creates the default implementation of the model to be used at construction if one isn't
	explicitly given.
		@return the default model implementation
	*/
	protected Document createDefaultModel() {
		return new StringDocument();
	}

	public String	getCurrentValue() {
		return this.getText();
	} // getCurrentValue

	public boolean	hasBeenEdited() {
		return fHasBeenEdited;
	}

	public void	setHasBeenEdited(boolean aValue) {
		fHasBeenEdited = aValue;
	}

	public boolean	validateCurrentValue() {
		return true;		// Strings don't need much validity checking!
	}

	/**
	This inner class represents the data in the field. We subclass the usual one
	so that we can track whether or not it's been edited
	*/
	protected class StringDocument extends PlainDocument
	{
		/**
		Inserts some content into the document.
			@param anOffset the starting offset >= 0
			@param aString the string to insert; does nothing with null/empty strings
			@param anAttr the attributes for the inserted content
			@exception BadLocationException  anOffset is not a valid position within the document
			@see Document#insertString
		*/
		public void insertString(int anOffset, String aString, AttributeSet anAttr)
				throws BadLocationException {
			super.insertString(anOffset, aString, anAttr);
			StringEditor.this.setHasBeenEdited(true);

			if (fEditingParameter) {
				CommandRegistrar.gRegistrar.enableAction("commitChanges");
				CommandRegistrar.gRegistrar.enableAction("cancelChanges");
			}
		}
	}
}
