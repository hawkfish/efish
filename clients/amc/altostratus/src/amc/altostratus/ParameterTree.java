/*
File:		ParameterTree.java

Contains:	Class which displays a browsable list of parameters 

Written by:	David Dunham

Copyright:	Copyright ©2000-2001 by Applied Microsystems Corporation.  All Rights reserved.

Change History (most recent first):

	05 Apr 2001		drd	Changes for packages
	26 Mar 2001		drd	dragGestureRecognized does error reporting
	22 Mar 2001		drd	Added updateTrackedFiles, fFilesParent
	21 Mar 2001		drd	Handle no predefined displays
	20 Mar 2001		drd	Use FileNode and support predefined displays
	14 Mar 2001		drd	Add recent files to tree in createNodes
	02 Mar 2001		drd	Error reporting is now in the Error class
	01 Mar 2001		drd	Get Analysis from Stratus
	06 Feb 2001		drd	createDefaultDragGestureRecognizer with ACTION_COPY
	05 Feb 2001		drd	Try watch cursor during find; drag & drop
	29 Jan 2001		drd	find searches from selection and wraps
	25 Jan 2001		drd	find
	30 Nov 2000		drd	type_Unsigned is now type_Integer
	27 Nov 2000		drd	enableCommands no longer gets a Parameter object
	21 Nov 2000		drd	Parameter constructor no longer throws
	17 Nov 2000		drd	Changes for ParameterNode
	15 Nov 2000		drd	Begin CORBA
	31 Oct 2000		drd	Allow a parameter to be added more than once
	30 Oct 2000		drd	getSelectedParameter, parameterFromNode
	26 Oct 2000		drd	Removed drag & drop (OS X is buggy); convertValueToText, getSelection
	24 Oct 2000		drd	Started drag & drop
	20 Oct 2000		drd	Selection stuff
	28 Sep 2000		drd	Created
*/

package amc.altostratus;

import amc.altostratus.constants.MiscConstants;
import amc.altostratus.constants.ParamConstants;
import amc.altostratus.framework.CommandRegistrar;
import amc.altostratus.framework.Selectable;
import amc.altostratus.framework.Selection;
import java.awt.*;
import java.awt.Cursor;
import java.awt.datatransfer.*;		// Flavors etc.
import java.awt.dnd.*;				// Drag & Drop
import java.io.File;
import java.util.Enumeration;
import java.util.Iterator;
import javax.swing.*;				// Swing components and classes
import javax.swing.event.*;			// Swing events and listeners
import javax.swing.tree.*;			// JTree related classes (but not JTree)

// CORBA stuff
import ObserveM.*;
import VteEngineServer.*;

public class ParameterTree extends JTree implements DragGestureListener, DragSourceListener,
	MiscConstants, ParamConstants, Selectable {
// Instance data
	// CORBA
	VteObjectContainer		fContainer;		// Our source
	DefaultMutableTreeNode	fFilesParent;
	Observer				fObserver;		// Watches the fContainer
	DragSource				fDragSource;	// We need this for drag & drop

	/**
	ParameterTree
		Constructor
	*/
	public ParameterTree(VteObjectContainer aContainer) throws ObserveM.BadObserveTypeException {
		super(createNodes(aContainer));

		this.updateTrackedFiles();

		fContainer = aContainer;
		if (fContainer != null) {
			fObserver = new Observer(fContainer);
		}

		((DefaultTreeModel) this.getModel()).setAsksAllowsChildren(true);	// Need to ask the nodes
		this.setRootVisible(false);
		this.setShowsRootHandles(true);

		// We don't want to display folders (or any other icon) next to them
		DefaultTreeCellRenderer	renderer = new DefaultTreeCellRenderer();
		renderer.setClosedIcon(null);
		renderer.setLeafIcon(null);
		renderer.setOpenIcon(null);
		renderer.setIconTextGap(1);		// Less space between gadget and text
		this.setCellRenderer(renderer);

		// Be sure only one item can be selected
		this.getSelectionModel().setSelectionMode(TreeSelectionModel.SINGLE_TREE_SELECTION);

		// Talk with the central selection
		this.addTreeSelectionListener(new TreeSelectionListener() {
			public void valueChanged(TreeSelectionEvent anEvent) {
//				System.out.println("valueChanged " + anEvent.getPath() + " " + anEvent.isAddedPath());
				if (ParameterTree.this.isSelectionEmpty()) {
					Selection.removeFromSelection(ParameterTree.this);
				} else {
					Selection.setSelection(ParameterTree.this);
					// The Selection object doesn't know much about changes within ourself;
					// be sure we update the menus properly
					ParameterTree.this.enableCommands(anEvent.getPath());
				}
			}
		});

		// Activate drag & drop -- notify us about copy or move from us
		fDragSource = DragSource.getDefaultDragSource();
		fDragSource.createDefaultDragGestureRecognizer(this, DnDConstants.ACTION_COPY, this);

//		System.out.println("ParameterTree.min=" + this.getMinimumSize());
//		System.out.println("ParameterTree.max=" + this.getMaximumSize());
//		System.out.println("ParameterTree.preferredSize=" + this.getPreferredSize());
//		System.out.println("ParameterTree.size=" + this.getSize());
	} // ParameterTree

	/**
	Make a bunch of tree nodes for test purposes
	*/
	private static TreeNode createNodes(VteObjectContainer aContainer) {
		DefaultMutableTreeNode root;
		DefaultMutableTreeNode grandparent;
		DefaultMutableTreeNode parent;
		DefaultMutableTreeNode child;
		DefaultMutableTreeNode toy;
		DefaultMutableTreeNode part;

		root = new DefaultMutableTreeNode("AltoStratus");

		if (aContainer == null) {
			grandparent = new DefaultMutableTreeNode("System");
			root.add(grandparent);

			parent = new DefaultMutableTreeNode("port1");
			grandparent.add(parent);

			child = new DefaultMutableTreeNode("rxPwr");
			parent.add(child);
			toy = new DefaultMutableTreeNode("---");
			child.add(toy);

			child = new DefaultMutableTreeNode("boardCntlr");
			parent.add(child);

			toy = new DefaultMutableTreeNode("cpu");
			child.add(toy);
			part = new DefaultMutableTreeNode("---");
			toy.add(part);
			toy = new DefaultMutableTreeNode("AtoD");
			child.add(toy);
			part = new DefaultMutableTreeNode("---");
			toy.add(part);
			toy = new DefaultMutableTreeNode("dualPortedRAM");
			child.add(toy);
			part = new DefaultMutableTreeNode("---");
			toy.add(part);
			toy = new DefaultMutableTreeNode("i2c");
			child.add(toy);
			part = new DefaultMutableTreeNode("---");
			toy.add(part);
			toy = new DefaultMutableTreeNode("tempSense");
			child.add(toy);
			part = new DefaultMutableTreeNode("---");
			toy.add(part);
			toy = new DefaultMutableTreeNode("seeprom");
			child.add(toy);
			part = new DefaultMutableTreeNode("---");
			toy.add(part);
			toy = new DefaultMutableTreeNode("cpic");
			child.add(toy);
			part = new DefaultMutableTreeNode("---");
			toy.add(part);
			toy = new DefaultMutableTreeNode("internalMemoryMap");
			child.add(toy);
			part = new DefaultMutableTreeNode("---");
			toy.add(part);

			child = new DefaultMutableTreeNode("txPwr");
			parent.add(child);
			toy = new DefaultMutableTreeNode("---");
			child.add(toy);

			child = new DefaultMutableTreeNode(new Parameter("port1.bcFPGA", "port1.bcFPGA"));
			parent.add(child);
			toy = new DefaultMutableTreeNode("0xf0f00008 Control (LO)");
			child.add(toy);
			part = new DefaultMutableTreeNode("---");
			toy.add(part);
			toy = new DefaultMutableTreeNode(new Parameter("0xf0f00008 Control (HI)", "0xf0f00008 Control (HI)"));
			child.add(toy);
			part = new DefaultMutableTreeNode(new Parameter("CSR", "CSR"));
			toy.add(part);
			part = new DefaultMutableTreeNode(new Parameter("GPIO LO REQ", type_Integer, 32, kMutable));
			toy.add(part);
			part = new DefaultMutableTreeNode(new Parameter("GPIO HI REQ", type_Integer, 0, kMutable));
			toy.add(part);

			grandparent = new DefaultMutableTreeNode("Analysis");
			root.add(grandparent);

			parent = new DefaultMutableTreeNode("RomLog");
			grandparent.add(parent);
			child = new DefaultMutableTreeNode("---");
			parent.add(child);
			parent = new DefaultMutableTreeNode("I2Clog");
			grandparent.add(parent);
			child = new DefaultMutableTreeNode("---");
			parent.add(child);
	} else {
			try {
				String[]	items = aContainer.Catalog("System", 1);
				grandparent = new ParameterNode(items[0], aContainer);
				root.add(grandparent);

				items = aContainer.Catalog("Analysis", 1);
				grandparent = new ParameterNode(items[0], aContainer);
				root.add(grandparent);
			} catch (org.omg.CORBA.COMM_FAILURE ce) {
				Error.showCommFailure(ce);
			}
		}


		grandparent = new DefaultMutableTreeNode("Run Control");
		root.add(grandparent);

		parent = new DefaultMutableTreeNode("Elapsed Time", false);
		grandparent.add(parent);
		parent = new DefaultMutableTreeNode("Events Processed", false);
		grandparent.add(parent);
		parent = new DefaultMutableTreeNode("Stop Time", false);
		grandparent.add(parent);
		parent = new DefaultMutableTreeNode("Events Remaining", false);
		grandparent.add(parent);
		parent = new DefaultMutableTreeNode("Command Line", false);
		grandparent.add(parent);

		return root;
	}

	/**
	Disable any commands that we may have enabled
	*/
	void disableCommands() {
		CommandRegistrar.gRegistrar.disableAction("addParam");
		CommandRegistrar.gRegistrar.disableAction("addPanel");
		CommandRegistrar.gRegistrar.disableAction("openPanel");
	} // disableCommands

	public void dragDropEnd(DragSourceDropEvent anEvent) {
	}

	public void dragEnter(DragSourceDragEvent anEvent) {
	}

	public void dragExit(DragSourceEvent anEvent) {
	}

	/**
	Beginning of a drag operation
	*/
	public void dragGestureRecognized(DragGestureEvent anEvent) {
		System.out.println("starting drag");
		DefaultTreeModel		model = (DefaultTreeModel) this.getModel();
		DefaultMutableTreeNode	root = (DefaultMutableTreeNode) model.getRoot();
		if (root != null) {
			try {
				Parameter			param = this.getSelectedParameter();

				TreePath			curPath = this.getSelectionPath();
//				System.out.println(curPath);
				Rectangle			bounds = this.getPathBounds(curPath);
//				System.out.println(bounds);
				Image				image = this.createImage(bounds.width, bounds.height);
				Graphics			g = image.getGraphics();
				g.fillRect(0, 0, 25, 19);
					// !!! draw
				Point				offset = new Point(0, 0);	// offset of the image origin from the
																// cursor hotspot at the instant of the trigger

				offset = anEvent.getDragOrigin();
//				System.out.println(offset);
				offset.y = -offset.y;
				anEvent.startDrag(DragSource.DefaultMoveDrop, image, offset,
					param, this);
//				new StringSelection(param.getLongName() + '\t' + param.getShortName()), this);
			} catch (org.omg.CORBA.COMM_FAILURE ce) {
				Error.showCommFailure(ce);
			} catch (org.omg.CORBA.SystemException e2) {
			}
		}
	} // dragGestureRecognized

	public void dragOver(DragSourceDragEvent anEvent) {
	}

	public void dropActionChanged(DragSourceDragEvent anEvent) {
	}

	/**
	Enable any commands that apply to the current selection
		@param aPath Path to the selection
	*/
	public void enableCommands(TreePath aPath) {
		if (aPath != null) {
			Object		theNode = aPath.getLastPathComponent();
//			System.out.println("enableCommands " + theNode);
			if (theNode instanceof FileNode) {
				// Note that it is OK to add a parameter more than once
				CommandRegistrar.gRegistrar.enableAction("addPanel");
				CommandRegistrar.gRegistrar.enableAction("openPanel");
				CommandRegistrar.gRegistrar.disableAction("addParam");
			} else {
				// Note that it is OK to add a parameter more than once
				CommandRegistrar.gRegistrar.enableAction("addParam");
				CommandRegistrar.gRegistrar.disableAction("addPanel");
				CommandRegistrar.gRegistrar.disableAction("openPanel");
			}
		}
	} // enableCommands

	/**
	Searches (case insensitive) for text in the display name of a parameter. If it's found,
	expands the tree to show the found item and selects it.
		@param aString The string to search for
	*/
	public void find(String aString, boolean aFirstTime) {
//		fDialog.setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));
		this.setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));
		String					searchString = aString.toLowerCase();
		DefaultTreeModel		model = (DefaultTreeModel) this.getModel();
		DefaultMutableTreeNode	root = (DefaultMutableTreeNode) model.getRoot();
		if (root != null) {
			TreePath			curPath = this.getSelectionPath();
			Enumeration			e = root.preorderEnumeration();
			boolean				searchIsActive = false;
			Object				curNode = null;
			if (curPath == null || aFirstTime == kSecondPass)
				searchIsActive = true;
			else
				curNode = curPath.getLastPathComponent();
			while (e.hasMoreElements()) {
				TreeNode		o = (TreeNode) e.nextElement();
				if (!searchIsActive) {
					if (o.equals(curNode)) {
						searchIsActive = true;
					}
					continue;
				}
				String			name;
				if (o instanceof ParameterNode) {
					ParameterNode	node = (ParameterNode) o;
					name = node.getDisplayName().toLowerCase();
				} else {
					name = o.toString();
				}
//				System.out.println("testing " + name);
				if (name.indexOf(searchString) >= 0) {
//					System.out.println("found in " + o);
					TreeNode[]	paths = model.getPathToRoot(o.getParent());
					System.out.println("found in " + paths);
					
					TreePath	path = new TreePath(paths);
//					System.out.println("found in " + path);
					this.expandPath(path);
					this.setSelectionPath(path.pathByAddingChild(o));
					this.restoreCursor();
					return;
				}
			}
			System.out.println("not found in pass");
			if (aFirstTime == kFirstPass)
				this.find(aString, kSecondPass);
			else {
				this.restoreCursor();
				JOptionPane.showMessageDialog(this.getParent(), "Could not find \u201c" + aString + "\u201d in parameters.",
					"Text Not Found", JOptionPane.INFORMATION_MESSAGE);
			}
		}
	} // find

	/**
	Returns the selected item (note that we allow only a single selection)
		@return The parameter, or null
	*/
	public Parameter getSelectedParameter() {
		return parameterFromNode(this.getSelection());
	} // getSelectedParameter

	/**
	Returns the selected item (note that we allow only a single selection)
		@return The tree node, or null
	*/
	public Object getSelection() {
		TreePath	path = this.getLeadSelectionPath();
//		System.out.println("path " + path);
		if (path == null)
			return null;
		else {
			Object	o = path.getLastPathComponent();
//			System.out.println("return " + o);
			return o;
		}
	}

	/**
		@return whether or not we are selected
	*/
	public boolean isSelected() {
		return !this.isSelectionEmpty();
	}

	/**
	Gets the parameter related to the specified tree node
		@param aValue
		@return The associated Parameter
	*/
	static public Parameter parameterFromNode(Object aValue) {
		if (aValue != null) {
			if (aValue instanceof ParameterNode) {
				ParameterNode	n = (ParameterNode) aValue;
				String			s = n.getFullName();
				System.out.println("%parameterFromNode name=" + s);
				Parameter		p = new Parameter(s, n.getDisplayName());
				return p;
			}
			if (aValue instanceof DefaultMutableTreeNode) {
				DefaultMutableTreeNode	n = (DefaultMutableTreeNode) aValue;
				System.out.println("DefaultMutableTreeNode as parameter " + n);
				Object			o = n.getUserObject();
				if (o instanceof Parameter) {
					Parameter	p = (Parameter) o;
					return p;
				}
			}
		}
		return null;		// Couldn't get a parameter
	}

	private void restoreCursor() {
//		fDialog.setCursor(null);
		this.setCursor(null);
	}

	/**
	Change the selection state.
	*/
	public void setSelected(boolean anArg) {
		if (anArg) {
			// Our superclass has already handled selection
			this.enableCommands(this.getLeadSelectionPath());
		} else {
			this.clearSelection();
			this.disableCommands();
		}
	}

	/**
	Keep track of all files opened or saved (simply refreshing)
	*/
	public void updateTrackedFiles() {
		DefaultTreeModel		model = (DefaultTreeModel) this.getModel();
		if (fFilesParent != null) {
			model.removeNodeFromParent(fFilesParent);
		}
		Iterator				i = AltoStratus.getTrackedFiles();
		if (i != null) {
			fFilesParent = new DefaultMutableTreeNode("Recent Panels", false);
			while (i.hasNext()) {
				File			theFile = (File) i.next();
				FileNode		parent = new FileNode(theFile);
				fFilesParent.setAllowsChildren(true);
				fFilesParent.add(parent);
			}
			model.insertNodeInto(fFilesParent, (MutableTreeNode) model.getRoot(), 3);
		}
	}

	class Observer extends _CallbackObjDefImplBase {
		/**
		Constructor
		*/
		public Observer(VteObjectContainer aContainer) throws ObserveM.BadObserveTypeException {
			short[] types = { ObserveM.ALL_TYPE.value };
			aContainer.subscribe(this, types);
		 }

		/**
		Parameter has changed value
		*/
	    public void update(org.omg.CORBA.Any obj, short[] observerTypes) {
			System.out.println("%ParameterTree.Observer-obj " + obj + " types " + observerTypes);
		}
	}
}