/*
File:		DisplayPanel.java

Contains:	Holds the strips

Written by:	David Dunham
		Electric Fish, Inc. <http://www.electricfish.com>

Copyright:	Copyright ©2000-2001 by Applied Microsystems Corporation.  All Rights reserved.

Change History (most recent first):

	05 Apr 2001		drd	Changes for packages
	29 Mar 2001		drd	Error handling: continueAfterError
	14 Mar 2001		drd	Use trackFile
	07 Mar 2001		drd	Moved showOpenError to Error class
	06 Mar 2001		drd	Added arg to open so it can handle appending
	05 Mar 2001		drd	Constructor sets layout; override setName and call with file name
	02 Mar 2001		drd	Report drop errors
	01 Mar 2001		drd	Report CORBA errors in commitChanges
	26 Feb 2001		drd	cancelChanges, commitChanges
	06 Feb 2001		drd	addAt
	05 Feb 2001		drd	Use FileDialog rather than JFileChooser; drop target
	25 Jan 2001		drd	Changed order in updateMenus
	21 Dec 2000		drd	setDecimal, setHex change radix, not format
	21 Dec 2000		drd	More robust open
	15 Dec 2000		drd	More saving
	07 Dec 2000		drd	save and related methods
	01 Dec 2000		drd	Handle new Numeric menu item, and separated concept of radix
	03 Nov 2000		drd	Use gNoFormatMenu to turn off all checkmarks
	02 Nov 2000		drd	updateMenus
	30 Oct 2000		drd	Created
*/

package amc.altostratus;

import amc.altostratus.constants.*;
import amc.altostratus.framework.CommandRegistrar;
import amc.altostratus.framework.Selection;
import amc.altostratus.xml.DisplayParser;
import amc.altostratus.xml.XMLWriter;
import java.awt.*;				// AWT classes
import java.awt.datatransfer.*;		// Flavors etc.
import java.awt.dnd.*;				// Drag & Drop
import java.io.*;
import javax.swing.*;				// Swing components and classes
import javax.swing.border.*;

// XML
import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;
import javax.xml.parsers.FactoryConfigurationError;
import javax.xml.parsers.ParserConfigurationException;
import org.xml.sax.SAXException;
import org.xml.sax.SAXParseException;

public class DisplayPanel extends JPanel implements DropTargetListener, MiscConstants, ParamConstants {
	private	boolean				fDirty;
	private DropTarget			fDropTarget;
	private	File				fFile;
	private	XMLWriter			fOutput;

	protected static Border		gDropBorder = new BevelBorder(BevelBorder.LOWERED);

	public DisplayPanel() {
		this.setName("Untitled");

		this.setLayout(new BoxLayout(this, BoxLayout.Y_AXIS));

		// Enable us to be a drop target
		fDropTarget = new DropTarget(this, this);
	} // DisplayPanel

	/**
	Adds a new strip based on the specified parameter. We don't refresh -- the caller
	should send <code>revalidate</code> when all parameters have been added.
		@param aParameter The parameter to display.
	*/
	public void add(Parameter aParameter) {
		Strip			theStrip = new Strip(aParameter);
		this.add(theStrip);
	}

	/**
	Adds a new strip. We don't refresh -- the caller
	should send <code>revalidate</code> when all parameters have been added.
		@param aStrip The strip to display.
	*/
	public void add(Strip aStrip) {
		this.add((JComponent) aStrip);
		fDirty = true;
	}

	/**
	Adds a new strip based on the specified parameter, at the position currently occupied
	by another strip. This is used for dragging onto a strip.
		@param aStrip The strip to move down.
		@param aParameter The parameter to display.
	*/
	public void addAt(Strip aStrip, Parameter aParameter) {
		Component[]		kids = this.getComponents();
		int				i;
		for (i = 0; i < kids.length; i++) {
			if (aStrip == kids[i])
				break;
		}
		Strip			theStrip = new Strip(aParameter);
		this.add(theStrip, i);
	} // addAt

	/**
	Cancel any pending changes. We pass this on to all our Strips.
	*/
	public void cancelChanges() {
		int		n = this.getComponentCount();
		for (int i = 0; i < n; i++) {
			Strip	theStrip = (Strip) this.getComponent(i);
			if (theStrip != null) {
				theStrip.cancelChanges();
			}
		}
	} // cancelChanges

	/**
	Flush any changes back to the model. We pass this on to all our Strips.
	*/
	public void commitChanges() {
		int		n = this.getComponentCount();
		try {
			for (int i = 0; i < n; i++) {
				Strip	theStrip = (Strip) this.getComponent(i);
				if (theStrip != null) {
					theStrip.commitChanges();
				}
			}
		} catch (org.omg.CORBA.COMM_FAILURE ce) {
			// We catch the exception here, rather than in Strip.commitChanges (or below),
			// so that we only get one report for all the changed parameters.
			Error.showCommFailure(ce);
		}
	} // commitChanges

	/**
	Ask the user whether or not to continue if there are errors
	*/
	private boolean continueAfterError(DisplayParser aHandler) {
		int			t = aHandler.getGoodParameterCount() + aHandler.getErrorCount();
		int			bn = aHandler.getNameNotFoundCount();
		int			bt = aHandler.getTypeMismatchCount();
		String		s = "There were " + t + " parameters, of which ";
		if (bn > 0) {
			s += bn + " name";
			if (bn != 1)
				s += "s";
			// ??? show name(s) here
			s += " could not be found";
			if (bn == 1) {
			}
			if (bt > 0)
				s += " and ";
		}
		if (bt > 0) {
			s += bt + " had a mismatched type";
		}
		int			r = JOptionPane.showConfirmDialog(null,
			s + ". Open this panel anyway (ignoring errors)?",
			"Proceed Despite Errors?",
			JOptionPane.YES_NO_OPTION);
		return r == JOptionPane.YES_OPTION || r == JOptionPane.OK_OPTION;
	}

	/**
	A Drag operation has encountered the DropTarget
	*/
	public void dragEnter(DropTargetDragEvent anEvent) {
		System.out.println("dragEnter");
		anEvent.acceptDrag(DnDConstants.ACTION_COPY);
		this.setBorder(gDropBorder);
	}

	/**
	The Drag operation has departed the DropTarget without dropping.
	*/
	public void dragExit(DropTargetEvent anEvent) {
		System.out.println("dragExit");
		this.setBorder(null);				// Restore default border
	}

	/**
	A Drag operation is ongoing on the DropTarget
	*/
	public void dragOver(DropTargetDragEvent anEvent) {
	}

	/**
	The Drag operation has terminated with a Drop on our DropTarget
	*/
	public void drop(DropTargetDropEvent anEvent) {
		this.setBorder(null);				// Restore default border

//		DropTargetContext		context = anEvent.getDropTargetContext();
//		System.out.println("DropTargetContext=" + context);

		DataFlavor[]			flavors = anEvent.getCurrentDataFlavors();
//		System.out.println("DataFlavor[0]=" + flavors[0]);

		Transferable			t = anEvent.getTransferable();
//		System.out.println("Transferrable=" + t);
		if (t.isDataFlavorSupported(DataFlavor.stringFlavor)) {
			System.out.println("accepting String");
			anEvent.acceptDrop(DnDConstants.ACTION_COPY);
			try {
				String	s = (String) t.getTransferData(DataFlavor.stringFlavor);
				System.out.println(s);

				// Get the two names
				String			longName = null;
				String			shortName = null;

				int		ln = s.length();
				int		first = 0;
				int		i;
				char	c;
				for (i = 0; i < ln; i++) {
					c = s.charAt(i);
					if (c == '\t') {
						// Remember, substring is OBO, it's EXCLUSIVE of endIndex
						longName = s.substring(0, i);
						shortName = s.substring(i + 1);
						break;
					}
				}
				this.add(new Parameter(longName, shortName));
				this.revalidate();
				anEvent.dropComplete(true);
			} catch (Exception e) {
				Error.showGenericFailure(e, "Drop");
				e.printStackTrace();
				anEvent.dropComplete(false);
			}
		} else if (t.isDataFlavorSupported(Parameter.gFlavor)) {
//			System.out.println("accepting Parameter");
			anEvent.acceptDrop(DnDConstants.ACTION_COPY);
			try {
				Parameter	param = (Parameter) t.getTransferData(Parameter.gFlavor);
				this.add(param);
				this.revalidate();
				
				anEvent.dropComplete(true);
			} catch (Exception e) {
				Error.showGenericFailure(e, "Drop");
				e.printStackTrace();
				anEvent.dropComplete(false);
			}
		} else {
			anEvent.rejectDrop();
		}
	}

	/**
	The user has modified the current drop gesture
	*/
	public void dropActionChanged(DropTargetDragEvent anEvent) {
	}

	public boolean getDirty() {
		return fDirty;
	}

	public boolean hasFile() {
		return fFile != null;
	}

	/**
	Are we displaying the specified parameter?
		@param aParameter The parameter to check for.
		@return <code>true</code> if we are displaying it.
	*/
	public boolean isShowing(Parameter aParameter) {
		int		n = this.getComponentCount();
		for (int i = 0; i < n; i++) {
			Strip	theStrip = (Strip) this.getComponent(i);
			if (theStrip != null) {
				Parameter	theParameter = theStrip.getParameter();
				if (aParameter == theParameter)
					return true;
			}
		}
		return false;		// No find
	}

	/**
	Opens a file, replacing our previous contents.

	We use an SAXParser (and state-driven DisplayParser) rather than reading in a DOM with
	DocumentBuilder -- our format is very simple, and this seems easier.
		@param aFile The file to open
		@param aType How to open aFile: kOpen or kAppend
	*/
	public void open(File aFile, char aType) {
		SAXParserFactory	parserFactory = SAXParserFactory.newInstance();
//		parserFactory.setValidating(true);
		try {
			int			removeCount = this.getComponentCount();

			// Read in the XML file
			SAXParser	parser = parserFactory.newSAXParser();
			DisplayParser	handler = new DisplayParser(this);
			parser.parse(aFile, handler);
			if (handler.getErrorCount() > 0) {
				if (!this.continueAfterError(handler)) {
				}
			}
			
			fDirty = false;
			Selection.clearSelection();

			if (aType == kOpen) {
				fFile = aFile;				// Since we succeeded, this becomes our file

				// Remove all original items
				for (int i = removeCount - 1; i >= 0; i--) {
					this.remove(i);
				}
			}
		} catch (ParserConfigurationException pce) {
			System.out.println("Open failed (ParserConfigurationException)É");
			pce.printStackTrace();
			Error.showOpenError("Unable to configure XML parser");
		} catch (SAXParseException spe) {
			String		s = "XML parsing error at line " + spe.getLineNumber() + " of " + spe.getSystemId();
			s += System.getProperty("line.separator") + spe.getMessage();
			System.out.println(s);
			System.out.println(spe);
			Error.showOpenError(s);
		} catch (SAXException se) {
			String		s = "XML error";
			s += System.getProperty("line.separator") + se.getMessage();
			System.out.println(s);
			se.printStackTrace();
			Error.showOpenError(s);
		} catch (Throwable t) {
			// !!! this is an overly-broad catch (but that helps us debug for now)
			System.out.println("Open failedÉ");
			System.out.println(t);
			t.printStackTrace();
		} finally {
			this.setName(aFile.getName());
			this.revalidate();
			this.repaint();		// Be sure everything is redrawn
		}

		// If we were able to successfully open the file, add it to our list
		AltoStratus.trackFile(fFile);

/*
		DocumentBuilderFactory	factory = DocumentBuilderFactory.newInstance();
		try {
			factory.setValidating(true);
			DocumentBuilder	builder = factory.newDocumentBuilder();
			Document		document = builder.parse(aFile);

			NodeList		nodes = document.getElementsByTagName("parameters");
			Node			node = nodes.item(0);
			System.out.println("#=" + nodes.getLength() + " first=" + node);
			NodeList		kids = node.getChildNodes();
			Node			kid = kids.item(0);
			System.out.println("#=" + kids.getLength() + " first=" + kid);
		} catch (Throwable t) {
		}
*/
	} // open

	/**
	Remove all selected strips from the display.
	*/
	public void removeSelected() {
		// Iterate backwards so we can remove items
		for (int i = this.getComponentCount() - 1; i >= 0; i--) {
			Strip	theStrip = (Strip) this.getComponent(i);
			if (theStrip != null && theStrip.isSelected()) {
				this.remove(theStrip);
			}
		}
		fDirty = true;
		this.revalidate();
		this.repaint();		// Be sure everything is redrawn

		CommandRegistrar.gRegistrar.disableAction("delParam");
	}

	/**
	Puts up a dialog if necessary and saves as XML
	*/
	public void save(String anAction) {
		if (anAction.equals("saveAs") || fFile == null) {
			FileDialog		dialog = new FileDialog((Frame) this.getTopLevelAncestor(), "",
								FileDialog.SAVE);
			dialog.show();
			String			dir = dialog.getDirectory();
			String			fileName = dialog.getFile();
			System.out.println("dir=" + dir + " fileName=" + fileName);
			if (fileName != null) {
				fFile = new File(dir + fileName);

			// JFileChooser doesn't give as good a user experience, so we don't use it
/*
			JFileChooser	chooser = new JFileChooser();
			int				result = chooser.showSaveDialog(null);
			if (result == JFileChooser.APPROVE_OPTION) {
				fFile = chooser.getSelectedFile();
*/
				try {
					FileOutputStream	theStream = new FileOutputStream(fFile);
					// Should be a TEXT file for Mac OS (handled globally by MRJFileUtils)
					fOutput = new XMLWriter(theStream, "UTF8");
					fOutput.write("<?xml version='1.0' encoding='UTF-8'?>");
					fOutput.nl();
					fOutput.writeComment("AltoStratus " + AltoStratus.version());
					fOutput.startElement("parameters", "version='1.0'");
					int		n = this.getComponentCount();
					for (int i = 0; i < n; i++) {
						Strip	theStrip = (Strip) this.getComponent(i);
						if (theStrip != null) {
							theStrip.save(fOutput);
						}
					}
					fOutput.endElement();

					fOutput.close();
					fDirty = false;
				} catch (IOException e) {
				}
				this.setName(fileName);
				AltoStratus.trackFile(fFile);
			}
		}
	}

	/**
	Handle saving before opening a new set
		@return true if it's OK to continue
	*/
	public boolean saveFirst() {
		if (fDirty) {
			int			r = JOptionPane.showConfirmDialog(null,
				"Save the current panel before opening a new one?",
				"Save Display?",
				JOptionPane.YES_NO_CANCEL_OPTION);
			if (r == JOptionPane.YES_OPTION || r == JOptionPane.OK_OPTION) {
				this.save("save");
			}
			if (r == JOptionPane.CANCEL_OPTION || r == JOptionPane.CLOSED_OPTION)
				return false;
		}

		return true;
	}

	/**
	Display all selected strips in decimal
	*/
	public void setDecimal() {
		int		n = this.getComponentCount();
		for (int i = 0; i < n; i++) {
			Strip	theStrip = (Strip) this.getComponent(i);
			if (theStrip != null && theStrip.isSelected()) {
				theStrip.setRadix(radix_Decimal);
			}
		}
	} // setDecimal

	public void setFormat(char aFormat) {
		int		n = this.getComponentCount();
		for (int i = 0; i < n; i++) {
			Strip	theStrip = (Strip) this.getComponent(i);
			if (theStrip != null && theStrip.isSelected()) {
				theStrip.setFormat(aFormat);
			}
		}
	}

	/**
	Display all selected strips in hexadecimal
	*/
	public void setHex() {
		int		n = this.getComponentCount();
		for (int i = 0; i < n; i++) {
			Strip	theStrip = (Strip) this.getComponent(i);
			if (theStrip != null && theStrip.isSelected()) {
				theStrip.setRadix(radix_Hex);
			}
		}
	} // setHex

	/**
	Sets the name of the component to the specified string.
		@param aName  The string that is to be this component's name.
	*/
    public void setName(String aName) {
    	super.setName(aName);		// ??? might want to remove extension

		Component		parent = this.getParent();
		if (parent instanceof JTabbedPane) {
			JTabbedPane		thePane = (JTabbedPane) parent;
			int				i = thePane.indexOfComponent(this);
			thePane.setTitleAt(i, this.getName());
		}
    }

	/**
	Be sure the menus are current
	*/
	public void updateMenus() {
//		System.out.println("DisplayPanel.updateMenus");
		AltoStratus.gNoNumberFormatMenu.setSelected(true);
		AltoStratus.gNoDisplayFormatMenu.setSelected(true);

		boolean		allDecimal = false;
		boolean		allHex = false;
		boolean		allNumeric = false;
		int		n = this.getComponentCount();
		if (n > 0) {
			allDecimal = true;
			allHex = true;
			allNumeric = true;
			for (int i = 0; i < n; i++) {
				Strip	theStrip = (Strip) this.getComponent(i);
				if (theStrip != null && theStrip.isSelected()) {
					char	r = theStrip.getRadix();
					if (r != radix_Decimal)
						allDecimal = false;
					if (r != radix_Hex)
						allHex = false;
					if (!Format.isNumeric(theStrip.getFormat()))
						allNumeric = false;
					theStrip.updateMenus();
				}
			}
		} else {
		}

//		System.out.println("allDecimal=" + allDecimal + " allHex=" + allHex + " allNumeric=" + allNumeric);

		if (allDecimal)
			CommandRegistrar.gRegistrar.checkAction("decimal");
		if (allHex)
			CommandRegistrar.gRegistrar.checkAction("hex");
		if (allNumeric)
			CommandRegistrar.gRegistrar.checkAction("numeric");
	}
}
