/*
File:		Strip.java

Contains:	Base class for an AltoStratus strip (which displays a parameter).
		Assumes our parent is a DisplayPanel.

Written by:	David Dunham
		Electric Fish, Inc. <http://www.electricfish.com>

Copyright:	Copyright ©2000-2001 by Applied Microsystems Corporation.  All Rights reserved.

Change History (most recent first):

	05 Apr 2001		drd	Changes for framework package
	07 Mar 2001		drd	Editing handles type_Float; no more format_Control; stopEditing
						checks hasBeenEdited and takes arg
	05 Mar 2001		drd	calcSizes sets preferredSize
	01 Mar 2001		drd	Hooked up stopEditing for integers, call it more often
	28 Feb 2001		drd	stopEditing calls setValue
	27 Feb 2001		drd	Call writeChange
	26 Feb 2001		drd	commitChanges
	23 Feb 2001		drd	Editor stuff; use revalidate instead of validate on parent
	15 Feb 2001		drd	Renamed doViewOptions to doDoubleClick
	07 Feb 2001		drd	2click brings up Options dialog
	06 Feb 2001		drd	Drag & Drop
	31 Jan 2001		drd	Fixed test in setFormat so we don't make Displayer for format_Name, and
					handle not having a parent
	23 Jan 2001		drd	Moved validate call to setFormat to handle removing a Displayer
	23 Jan 2001		drd	doViewOptions
	04 Jan 2001		drd	Make fDisplayer to BorderLayout.SOUTH 
	02 Jan 2001		drd	calcSizes
	21 Dec 2000		drd	Change to format_Number
	19 Dec 2000		drd	Save as XML
	01 Dec 2000		drd	fRadix
	30 Nov 2000		drd	type_Signed is now type_Integer
	29 Nov 2000		drd	fDisplayer and start of bar chart stuff
	28 Nov 2000		drd	Handle type_Float; right-justify display
	27 Nov 2000		drd	Set format to format_Name for string Parameters
	21 Nov 2000		drd	Send startObserving
	02 Nov 2000		drd	getFormat, setFormat; post updateMenus
	31 Oct 2000		drd	Disable, enable decimal and hex commands
	30 Oct 2000		drd	Observer
	26 Oct 2000		drd	Added getSelection; removed Drag & Drop; separate side/top margins
	24 Oct 2000		drd	Drag & Drop
	20 Oct 2000		drd	Hilite like tree; selection stuff
	19 Oct 2000		drd	Measure height; switched to two fonts
	18 Oct 2000		drd	fParameter
	17 Oct 2000		drd	Use SansSerif font
	16 Oct 2000		drd	Override paintComponent instead of paint!; use MouseAdapter
	04 Oct 2000		drd	Added String constructor
	29 Sep 2000		drd	Created
*/

package amc.altostratus;

import amc.altostratus.constants.*;
import amc.altostratus.framework.CommandRegistrar;
import amc.altostratus.framework.Selectable;
import amc.altostratus.framework.Selection;
import amc.altostratus.xml.XMLWriter;
import java.awt.*;				// AWT classes
import java.awt.datatransfer.*;		// Flavors etc.
import java.awt.dnd.*;				// Drag & Drop
import java.awt.event.*;			// AWT events
import java.io.*;
import java.util.*;				// Observable, Observer
import javax.swing.*;				// Swing components and classes
import javax.swing.border.*;

/**
This class is the visual representation of a parameter. It occupies a horizontal strip in the display.
	@author David Dunham
*/
public class Strip extends JPanel implements DragGestureListener, DragSourceListener,
	DropTargetListener, Observer, MiscConstants, ParamConstants, Selectable {
// Constants
	static final int kBorderThickness = 2;
	static final int kHorizontalMargin = 1;	// On left and right
	static final int kVerticalMargin = 0;		// On top and bottom

// Globals
	static Font		gLabelFont = null;		// Used for the text label
	static Color	gSelectionColor = UIManager.getColor("Tree.selectionBorderColor");
		// ??? should get this from getBorderSelectionColor
	static Font		gValueFont = null;		// Used for the numeric value
	
// Instance data
	private int			fBaseline;			// For drawing label
	private DragSource	fDragSource;		// We need this for drag & drop
	private DropTarget			fDropTarget;
	private String		fLabel;
	private Parameter	fParameter;			// The parameter we display
	private char		fFormat;			// How parameter is displayed
	private char		fRadix;				// radix_Decimal or radix_Hex
	private boolean		fSelected;
	private	Displayer	fDisplayer;			// Optional subcomponent which displays stuff graphically
	private Editor		fEditor;

	protected static Border		gDropBorder = new BevelBorder(BevelBorder.LOWERED);

	/**
	Default constructor
	*/
	public Strip() {
		// Make our fonts if we haven't done so
		if (gLabelFont == null) {
			gLabelFont = new Font("SansSerif", Font.PLAIN, 12);
			gValueFont = new Font("monospaced", Font.PLAIN, 10);
		}

		this.setBackground(Color.white);

		this.setBorder(BorderFactory.createEmptyBorder(kBorderThickness, kBorderThickness,
			kBorderThickness, kBorderThickness));
		fFormat = format_Number;
		fParameter = null;
		fRadix = radix_Hex;
		fSelected = false;

		this.calcSizes();

		// Activate drag & drop -- notify us about copy or move from us
		fDragSource = DragSource.getDefaultDragSource();
		fDragSource.createDefaultDragGestureRecognizer(this, DnDConstants.ACTION_COPY_OR_MOVE, this);

		// Enable us to be a drop target
		fDropTarget = new DropTarget(this, this);

		this.addMouseListener(new HandleClick());
	}

	/**
	Constructor
		@param aParam the Parameter we display
	*/
	public Strip(Parameter aParam) {
		this();							// Default initialization
		fParameter = aParam;
		fParameter.startObserving();	// Hook up the Parameter to the server
		fParameter.addObserver(this);	// And tell it we want notifications
		this.setLabel(aParam.getShortName());
		if (fParameter.getType() == type_String) {
			fFormat = format_Name;
		} else if (fParameter.getType() == type_Float) {
			fRadix = radix_Decimal;
		}
	}

	/**
	Constructor for a label-only strip (not showing any Parameter)
		@param aString Initial label
	*/
	public Strip(String aString) {
		this();						// Call default constructor
		fParameter = null;
		this.setLabel(aString);
	}

	/**
	Recalc our preferred, min, max size
	*/
	private void calcSizes() {
		FontMetrics		fm = this.getFontMetrics(gLabelFont);
		fBaseline = fm.getAscent() + kVerticalMargin + kBorderThickness;
		int				h = fm.getHeight();
		if (fEditor != null) {
			Dimension		editorSize;
			editorSize = ((Component) fEditor).getPreferredSize();
			h = Math.max(h, editorSize.height);
		}
		h += kVerticalMargin * 2 + kBorderThickness * 2;
		fm = this.getFontMetrics(gValueFont);
		int				w = fm.stringWidth("0xFFFFFFFF") + kHorizontalMargin + kBorderThickness;

		if (fDisplayer != null) {
			Dimension		displayerSize;
			displayerSize = ((Component) fDisplayer).getPreferredSize();
			h += displayerSize.height;
		}
		Dimension		minSize = new Dimension(w, h);
		this.setMinimumSize(minSize);
		Dimension		maxSize = new Dimension(Integer.MAX_VALUE, h);
		this.setMaximumSize(maxSize);

		// If our preferred size is smaller than the minimum size, then things can get
		// scrunched when the window is resized. Be sure this can't happen.
		Dimension		preferredSize = this.getPreferredSize();
		preferredSize.height = Math.max(preferredSize.height, minSize.height);
		preferredSize.width = Math.max(preferredSize.width, minSize.width);
		this.setPreferredSize(preferredSize);
	} // calcSizes

	/**
	Cancel any pending changes. We pass this on to all our Strips.
	*/
	public void cancelChanges() {
		this.stopEditing(kCancel);
		if (this.hasBeenEdited()) {
			fParameter.setChangePending(false);
			this.repaint();
		}
	} // cancelChanges

	/**
	Flush any changes back to the model
	*/
	public void commitChanges() {
		this.stopEditing(kKeepChanges);
		if (this.hasBeenEdited()) {
			fParameter.writeChange();
			this.repaint();
		}
	} // commitChanges

	/**
	Disable any commands that we may have enabled
	*/
	void disableCommands() {
		// !!! move all format things somewhere else so they are grouped
		CommandRegistrar.gRegistrar.disableAction("barGraph");
		CommandRegistrar.gRegistrar.disableAction("decimal");
		CommandRegistrar.gRegistrar.disableAction("delParam");
		CommandRegistrar.gRegistrar.disableAction("hex");
		CommandRegistrar.gRegistrar.disableAction("numeric");
		CommandRegistrar.gRegistrar.disableAction("viewOptions");
	} // disableCommands

	/**
	Handle a double-click
	*/
	public void doDoubleClick() {
//		System.out.println("Strip.doDoubleClick");
		if (fDisplayer == null && fParameter.getMutable() == kMutable) {
			this.startEditing();
		} else {
			this.doViewOptions();
		}
	} // doDoubleClick

	/**
	Tells our Displayer to show the View Options dialog
	*/
	public void doViewOptions() {
		if (fDisplayer != null)
			fDisplayer.doViewOptions();
	} // doViewOptions

	/**
	as the operation completes (a DragSourceListener method)
	*/
	public void dragDropEnd(DragSourceDropEvent anEvent) {
		System.out.println("Strip.dragDropEnd success=" + anEvent.getDropSuccess() +
			" action=" + anEvent.getDropAction());
		if (anEvent.getDropSuccess() /* !!! && anEvent.getDropAction() == DnDConstants.ACTION_MOVE */) {
			DisplayPanel	display = (DisplayPanel) this.getParent();
			display.removeSelected();
			// !!! make a new selection
		}
	}

	public void dragEnter(DragSourceDragEvent anEvent) {
	}

	/**
	A Drag operation has encountered the DropTarget
	*/
	public void dragEnter(DropTargetDragEvent anEvent) {
		System.out.println("Strip.dragEnter");
		anEvent.acceptDrag(DnDConstants.ACTION_COPY_OR_MOVE);
		this.setBorder(gDropBorder);
	}

	public void dragExit(DragSourceEvent anEvent) {
	}

	/**
	The Drag operation has departed the DropTarget without dropping.
	*/
	public void dragExit(DropTargetEvent anEvent) {
		System.out.println("Strip.dragExit");
		this.setBorder(null);				// Restore default border
	}

	/**
	Beginning of a drag operation
	*/
	public void dragGestureRecognized(DragGestureEvent anEvent) {
		// ??? how do we deal with multiple selection? and should we just select?
		if (this.isSelected()) {
			System.out.println("Strip starting drag");
			anEvent.startDrag(DragSource.DefaultMoveDrop, fParameter, this);
		}
	} // dragGestureRecognized

	public void dragOver(DragSourceDragEvent anEvent) {
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
		Transferable			t = anEvent.getTransferable();
		if (t.isDataFlavorSupported(Parameter.gFlavor)) {
			System.out.println("accepting Parameter");
			anEvent.acceptDrop(DnDConstants.ACTION_MOVE);
			try {
				Parameter	param = (Parameter) t.getTransferData(Parameter.gFlavor);
				DisplayPanel	display = (DisplayPanel) this.getParent();
				display.addAt(this, param);
				
				anEvent.dropComplete(true);
			} catch (Exception e) {
				e.printStackTrace();
				anEvent.dropComplete(false);
			}
		} else {
			anEvent.rejectDrop();
		}
	}

	public void dropActionChanged(DragSourceDragEvent anEvent) {
	}

	/**
	The user has modified the current drop gesture
	*/
	public void dropActionChanged(DropTargetDragEvent anEvent) {
	}

	/**
	Enable any commands that apply when we are selected
	*/
	void enableCommands() {
		char	theType = fParameter.getType();

		// Not all can be in decimal
		if (theType != type_String && theType != type_None && theType != type_CharArray) {
			CommandRegistrar.gRegistrar.enableAction("barGraph");
			CommandRegistrar.gRegistrar.enableAction("decimal");
			CommandRegistrar.gRegistrar.enableAction("numeric");
		}
		CommandRegistrar.gRegistrar.enableAction("delParam");
		// Not all can be in hex
		if (theType == type_Integer || theType == type_Long)
			CommandRegistrar.gRegistrar.enableAction("hex");

		if (fFormat == format_Bar)
			CommandRegistrar.gRegistrar.enableAction("viewOptions");
	}

	/**
	@return The display format (format_Number, format_Control, etc.)
	*/
	public char getFormat() {
		return fFormat;
	} // getFormat

	/**
	@return
	*/
	public int getLabelWidth() {
		// ??? we might want to cache this for speed
		FontMetrics		fm = this.getFontMetrics(gLabelFont);
		int				width = fm.stringWidth(fLabel) + kHorizontalMargin + kBorderThickness;
		return width;
	} // getLabelWidth

	/**
	@return The parameter we display
	*/
	public Parameter getParameter() {
		return fParameter;
	} // getParameter

	public char getRadix() {
		return fRadix;
	} // getRadix

	/**
	Returns the selected item (note that we allow only a single selection)
		@return This object, or null
	*/
	public Object getSelection() {
		if (fSelected)
			return this;
		else
			return null;
	} // getSelection

	/**
	Returns whether or not we have been edited (and thus have pending changes)
	*/
	public boolean hasBeenEdited() {
		return fParameter.getChangePending();
	}

	/**
	Be sure the menus are updated
	*/
	public void invalidateMenus() {
		Runnable callUpdateMenus = new Runnable() {
			public void run() {
				DisplayPanel	display = (DisplayPanel) Strip.this.getParent();
				if (display != null)
					display.updateMenus();
			}
		};
		SwingUtilities.invokeLater(callUpdateMenus);
	} // invalidateMenus

	/**
		@return whether or not we are selected
	*/
	public boolean isSelected() {
		return fSelected;
	} // isSelected

	/**
	Create a helper object to draw some funky format
	*/
	protected void makeDisplayer() {
		// Remove existing one
		if (fDisplayer != null) {
			this.remove((Component) fDisplayer);
		}

		// The default layout is FlowLayout; we make a BorderLayout so that the Displayer
		// is automatically resized when our width changes
		BorderLayout		theLayout = new BorderLayout();
		this.setLayout(theLayout);

		// Eventually there will be more types of Displayer...
		fDisplayer = new BarGraphDisplayer(this, fParameter);
		this.calcSizes();
		this.add((Component) fDisplayer, BorderLayout.SOUTH);
	} // makeDisplayer

	/**
	paintComponent {OVERRIDE}
		@param aG
	*/
	public void paintComponent(Graphics aG) {
		// Erase the background
		if (this.hasBeenEdited()) {
			aG.setColor(new Color((float) 0.85, (float) 1.0, (float) 0.85));
		} else {
			aG.setColor(this.getBackground());
		}
		aG.fillRect(0, 0, this.getWidth(), this.getHeight());
		aG.setColor(this.getForeground());

		Insets	theInsets = this.getInsets();

		aG.setFont(gLabelFont);
		aG.drawString(fLabel, kHorizontalMargin + theInsets.left, fBaseline);

		if (fParameter != null && fDisplayer == null && fEditor == null) {
			String	value = fParameter.getValueString(fFormat, fRadix);

			if (value != null) {
				aG.setFont(gValueFont);
				FontMetrics		fm = this.getFontMetrics(gValueFont);
				int				valueWidth = fm.stringWidth(value) + kHorizontalMargin + kBorderThickness;
				aG.drawString(value, this.getWidth() - valueWidth, fBaseline);
			}
		}
	} // paintComponent

	/**
	Output this strip as XML
	*/
	public void save(XMLWriter aWriter) throws IOException {
		aWriter.startElement("parameter");
		aWriter.writeElement("longName", fParameter.getLongName());
		aWriter.writeElement("shortName", fParameter.getShortName());
		aWriter.writeElement("format", fFormat);
		if (fFormat != format_Name)
			aWriter.writeElement("radix", fRadix);
		// A bar chart (or whatever) may have its own settings to save
		if (fDisplayer != null)
			fDisplayer.save(aWriter);
		aWriter.endElement();
	} // save

	/**
	Change the display format
		@param aFormat The new format (format_Number, format_Bar, etc.)
	*/
	public void setFormat(char aFormat) {
		if (fFormat != aFormat) {
			fFormat = aFormat;

			if (!Format.isSimple(fFormat)) {
				this.makeDisplayer();
			} else {
				if (fDisplayer != null) {
					this.remove((Component) fDisplayer);
				}
				fDisplayer = null;
				this.calcSizes();
			}

			// See if we have a parent. If not, we're probably called as part of parsing
			// an XML document, so layout and menus don't need to be updated
			Component	parent = this.getParent();
			if (parent != null) {
				parent.validate();		// Do our layout

				// This probably has different menu items, so be sure they are enabled or disabled
				this.invalidateMenus();
			}
		}
	} // setFormat

	/**
	setLabel
		@param aString New text of label
	*/
	public void setLabel(String aString) {
		fLabel = aString;
		this.repaint();
	} // setLabel

	/**
	Switch display of numbers between decimal or hex
		@param aRadix <code>radix_Decimal</code> or <code>radix_Hex</code>
	*/
	public void setRadix(char aRadix) {
		if (fRadix != aRadix) {
			fRadix = aRadix;
			if (fDisplayer != null)
				fDisplayer.setRadix(aRadix);
			this.repaint();
		}
	} // setRadix

	/**
	Change the selection state of this strip.
	Called by Selection methods.
		@see Selection
	*/
	public void setSelected(boolean aState) {
		fSelected = aState;

		this.invalidateMenus();

		if (fSelected) {
			this.requestFocus();
			this.setBorder(BorderFactory.createMatteBorder(kBorderThickness, kBorderThickness,
				kBorderThickness, kBorderThickness, gSelectionColor));
			this.enableCommands();
		} else {
			this.stopEditing(kKeepChanges);
			this.setBorder(BorderFactory.createEmptyBorder(kBorderThickness, kBorderThickness,
				kBorderThickness, kBorderThickness));
			this.disableCommands();
		}
		this.repaint();
	}

	/**
	Create the appropriate sort of editor
	*/
	public void startEditing() {
		switch (fParameter.getType()) {
			case type_Float:
				fEditor = new FloatEditor(this, fParameter);
				break;

			case type_Integer:
				fEditor = new IntegerEditor(this, fParameter);
				break;

			case type_String:
				fEditor = new StringEditor(this, fParameter);
				break;

			default:
				System.err.println("?Can't edit this type");
		}
		if (fEditor != null) {
//			System.out.println(fEditor);
			this.setLayout(null);		// Don't do layout, put it where we want it
			JComponent	editor = (JComponent) fEditor;
			this.calcSizes();
			this.add(editor);
			
			editor.requestFocus();		// Typing should go here

			// We want the editor to take up the available width, after the label (with our
			// normal margin, and leaving space for the selection borders)
			editor.setLocation(this.getLabelWidth() + kHorizontalMargin, kBorderThickness);
			editor.setSize(this.getWidth() - this.getLabelWidth() - kHorizontalMargin * 2 -
				kBorderThickness, editor.getPreferredSize().height);
			this.revalidate();			// Defer layout
		}

		this.invalidateMenus();
	} // startEditing

	/**
	Remove the editor
	*/
	private void stopEditing(char aCommand) {
		if (fEditor != null) {
			System.out.println("stopEditing");
			// Get the value from the editor
			if (aCommand == kKeepChanges && fEditor.hasBeenEdited() && fEditor.validateCurrentValue()) {
				String		theValue = fEditor.getCurrentValue();
				switch (fParameter.getType()) {
					case type_Float:
						Double	doubleObject = Double.valueOf(theValue);
						double	theDouble = doubleObject.doubleValue();
						fParameter.setCurrentValue(theDouble);
						break;

					case type_Integer:
						int		theInt = Format.parseInt(theValue);
						fParameter.setCurrentValue(theInt);
						break;

					case type_String:
					fParameter.setValue(theValue);
					break;
				}
				fParameter.setChangePending(true);
			} else if (aCommand == kCancel) {
				fParameter.setChangePending(false);
				// !!! get value back from param
			}
			this.remove((Component) fEditor);
			fEditor = null;
			this.calcSizes();
			this.revalidate();			// Defer layout

			this.invalidateMenus();
		}
	}

	/**
	Our parameter has changed value.
		@param aParameter The parameter which is notifying us.
		@param aChange Information about what changed (not used at present).
	*/
	public void update(Observable aParameter, Object aChange) {
//		System.out.println("Strip.update from " + aParameter);
		this.repaint();		// !!! just the value portion, use getLabelWidth
	}

	/**
	Be sure the menus are current
	*/
	public void updateMenus() {
//		System.out.println("Strip.updateMenus");
		if (fEditor == null && fParameter.getMutable() == kMutable) {
			CommandRegistrar.gRegistrar.enableAction("editParameter");
		} else {
			CommandRegistrar.gRegistrar.disableAction("editParameter");
			CommandRegistrar.gRegistrar.checkAction("editParameter");
		}

		if (fDisplayer != null)
			fDisplayer.updateMenus();
	} // updateMenus

	/*
	HandleClick
		Inner class
	*/
	public class HandleClick extends MouseAdapter {
		/**
		Selects the strip
			@param anEvent The click
		*/
		public void mouseClicked(MouseEvent anEvent) {
			// ??? does parent component (DisplayPanel) need to know?
			if (anEvent.isShiftDown()) {
				if (fSelected) {
					Selection.removeFromSelection(Strip.this);
				} else {
					Selection.addToSelection(Strip.this);
				}
			} else {
				if (anEvent.getClickCount() == 2) {
					Strip.this.doDoubleClick();
				} else {
					Selection.setSelection(Strip.this);
				}
			}
//			System.out.println("clicked " + fLabel + " " + fSelected);
		}
	} // HandleClick
}
