/*
File:		AltoStratus.java

Contains:	Main class of AltoStratus, the graphical interface to Stratus

Written by:	David Dunham
			Electric Fish, Inc. <http://www.electricfish.com>

Copyright:	Copyright ©2000-2001 by Applied Microsystems Corporation.  All Rights reserved.

Change History (most recent first):

	05 Apr 2001		drd	Packages
	02 Apr 2001		drd	Handle nothing on command line
	29 Mar 2001		drd	Better error handling
	29 Mar 2001		drd	Use File.pathSeparator
	22 Mar 2001		drd	Update the tree in trackFile
	21 Mar 2001		drd	Handle STRATUS_HOME startup; added name
	20 Mar 2001		drd	Set com.apple.macos.useScreenMenuBar
	14 Mar 2001		drd	trackFile, gFilesSeen
	12 Mar 2001		drd	Added preferences, use it for L&F
	09 Mar 2001		drd	Made the -l command line option work again
	06 Mar 2001		drd	Moved all commands (inner classes) to Commands; removeAllPanels,
					removePanel
	05 Mar 2001		drd	Started Panel menu; removed fDisplay and support tabbed panels
	02 Mar 2001		drd	Renamed button to "Commit," added tooltips
	02 Mar 2001		drd	Switch to using SafeAction; moved error reporting to Error class
	01 Mar 2001		drd	showCommFailure; DoEditParameter
	26 Feb 2001		drd	DoCancelChanges, DoCommitChanges sends message to DisplayPanel
	22 Feb 2001		drd	Better error trapping; DoCancelChanges
	21 Feb 2001		drd	Added DoCommitChanges
	20 Feb 2001		drd	Added some error-trapping at startup
	01 Feb 2001		drd	Be sure to start in Metal look
	31 Jan 2001		drd	Missing command line args; handle return in Find dialog
	29 Jan 2001		drd	Allow URL on command line
	29 Jan 2001		drd	Catch Throwable for MRJ; new arg for find
	25 Jan 2001		drd	Use FileDialog rather than JFileChooser; Hooked up Find; DoFindNext
	23 Jan 2001		drd	First cut at DoNumeric.actionPerformed
	23 Jan 2001		drd	DoViewOptions talks to strip
	21 Dec 2000		drd	Pass on format_Bar
	18 Dec 2000		drd	DoOpen calls fDisplay.open (where XML stuff now lives)
	15 Dec 2000		drd	Moved version() to main class
	07 Dec 2000		drd	DoOpen, DoSave
	01 Dec 2000		drd	Made a separate group for Numeric (and moved Bar Chart here);
						gNoNumberFormatMenu, renamed to gNoNumberFormatMenu
	29 Nov 2000		drd	Added Bar Chart, View Options menu items
	28 Nov 2000		drd	Don't make test Strips
	17 Nov 2000		drd	gContainer now public
	16 Nov 2000		drd	Command line argument takes precedence for IOR file
	15 Nov 2000		drd	CORBA is now working: save gContainer and pass it on
	06 Nov 2000		drd	Use URL to get ior, and added IDL files
	03 Nov 2000		drd	neitherHexNorDecimal, gNoFormatMenu
	02 Nov 2000		drd	Use CommandAction for all our actions
	02 Nov 2000		drd	Use CommandMenu for better JRadioButtonMenuItem/Action support
	01 Nov 2000		drd	DoAddParameter allows a parameter to be added more than once
	30 Oct 2000		drd	fDisplay is now a DisplayPanel; add and remove parameters; tweaks for icons
	23 Oct 2000		drd	Several more actions
	20 Oct 2000		drd	Added Hex, Decimal commands in radio menu
	18 Oct 2000		drd	Proper ellipses; DoFind; make Parameters
	17 Oct 2000		drd	Added buttons underneath parameter browser; DoExit
	13 Oct 2000		drd	Add Find to correct menu
	04 Oct 2000		drd	Use CommandRegistrar instead (since we're not JDK 1.3); DoAbout
	03 Oct 2000		drd	Use BarTender
	02 Oct 2000		drd	Put in a package; send setDividerLocation after pack
	28 Sep 2000		drd	Created
*/

package amc.altostratus;

import amc.altostratus.constants.*;
import amc.altostratus.framework.CommandRegistrar;
import amc.altostratus.framework.Preferences;
import java.awt.*;				// AWT classes
import java.awt.event.*;			// Basic event handling
import java.io.*;
import java.net.*;				// URL, etc.
import java.util.Iterator;
import java.util.TreeSet;
import javax.swing.*;				// Swing components and classes

// CORBA stuff
import org.omg.CORBA.ORB;
import RootFactoryDef.*;
import VteEngineServer.*;

import com.apple.mrj.*;

/**
The main class (application)
	@author David Dunham
*/
public class AltoStratus implements MiscConstants, ParamConstants, UIConstants
{
// Constants

// Instance data
	// User Interface
	JFrame		fWindow;		// Our main window
	JMenuBar		fMenuBar;		// Main window's menu bar
	JPanel		fBrowserHolder;
	ParameterTree	fBrowser;		// Parameter browser
	JScrollPane		fDisplayHolder;
	int				fPanelsMade;
	String			fSearchText;
	JTabbedPane		fPanelHolder;	// Holds DisplayPanels

// Globals
	public static		AltoStratus				gApplication = null;
	public static		JRadioButtonMenuItem	gNoDisplayFormatMenu = null;
	public static 		JRadioButtonMenuItem	gNoNumberFormatMenu = null;
	protected static	TreeSet					gFilesSeen;
	// CORBA
	public static		VteObjectContainer		gContainer = null;		// The main one

// Methods

	/**
	Convenience function for creating menu items
		@param aMenu the menu to add an item to
		@param anAction the action to add
		@param aMnemonic the letter to underline (0 if none)
		@param anAcceleratorKey the keystroke which activates this item
	*/
	static JMenuItem addMenuItem(JMenu aMenu, Action anAction, int aMnemonic, int anAcceleratorKey)
	{
		JMenuItem	item = aMenu.add(anAction);
		if (aMnemonic != 0)
			item.setMnemonic((char)aMnemonic);
		if (anAcceleratorKey != 0)
			item.setAccelerator(KeyStroke.getKeyStroke(anAcceleratorKey, java.awt.Event.CTRL_MASK));
		return item;
	} // addMenuItem

	/**
	Convenience function for creating radio button menu items
		@param aMenu the menu to add an item to
		@param anAction the action to add
		@param aMnemonic the letter to underline (0 if none)
		@param anAcceleratorKey the keystroke which activates this item
	*/
	static JRadioButtonMenuItem addRadioMenuItem(CommandMenu aMenu, ButtonGroup aGroup,
		Action anAction, int aMnemonic, int anAcceleratorKey) {
		JRadioButtonMenuItem	item = radioButtonItem(anAction, aMnemonic, anAcceleratorKey);
		if (aMenu != null) {
			aMenu.add(item, anAction);
		}
		aGroup.add(item);
		return item;
	} // addRadioMenuItem

	/**
	Builds the menu bar
	*/
	public void createMenus() {
		ActionListener	listener = new MenuItemActionListener(fWindow);

		JMenu		fileMenu = new JMenu("File");
		fileMenu.setMnemonic(kFileMnemonic);

		addMenuItem(fileMenu, new amc.altostratus.Commands.Open(this), 'O', KeyEvent.VK_O);
		addMenuItem(fileMenu, new Commands.Save(this, "Save", "save"), 'S', KeyEvent.VK_S);
		addMenuItem(fileMenu, new Commands.Save(this, "Save As\u2026", "saveAs"), 'A', 0);
		fileMenu.addSeparator();
		addMenuItem(fileMenu, new Commands.Exit(), 'X', 0);

		JMenu		editMenu = new JMenu("Edit");
		editMenu.setMnemonic(kEditMnemonic);
		// ??? basic 3 still need work
		editMenu.add(menuItem("Cut", listener, "cut", 'U', KeyEvent.VK_X));
		editMenu.add(menuItem("Copy", listener, "copy", 'C', KeyEvent.VK_C));
		editMenu.add(menuItem("Paste", listener, "paste", 'P', KeyEvent.VK_V));
		editMenu.addSeparator();
		addMenuItem(editMenu, new Commands.Find(this), 'F', KeyEvent.VK_F);
		addMenuItem(editMenu, new Commands.FindNext(this), 'N', KeyEvent.VK_G);

		CommandMenu		viewMenu = new CommandMenu("View");
		viewMenu.setMnemonic(kViewMnemonic);
		ButtonGroup	group = new ButtonGroup();
		addRadioMenuItem(viewMenu, group, new Commands.Decimal(this), 'D', 0);
		addRadioMenuItem(viewMenu, group, new Commands.Hexadecimal(this), 'H', 0);
		gNoNumberFormatMenu = addRadioMenuItem(null, group, new Commands.NeitherHexNorDecimal(), 0, 0);
		viewMenu.addSeparator();
		group = new ButtonGroup();
		addRadioMenuItem(viewMenu, group, new Commands.BarGraph(this), 'B', 0);
		addRadioMenuItem(viewMenu, group, new Commands.Numeric(this), 'N', 0);
		gNoDisplayFormatMenu = addRadioMenuItem(null, group, new Commands.NoFormat(), 0, 0);
		viewMenu.addSeparator();
		addMenuItem(viewMenu, new Commands.ViewOptions(), 'O', 0);
		try {
			// These menu items don't have anything to do with MRJ, but that seems like a
			// reasonable way to tell if we are on a Macintosh
			if (MRJApplicationUtils.isMRJToolkitAvailable()) {
				viewMenu.addSeparator();
				// View menu may not be best place for Look & feel, but this is really for
				// testing only, so it doesn't matter much.
				addMenuItem(viewMenu, new Commands.Aqua(this), 'A', 0);
				addMenuItem(viewMenu, new Commands.Metal(this), 'M', 0);
			}
		} catch (Throwable e) {
		}

		JMenu		paramMenu = new JMenu("Parameter");
		paramMenu.setMnemonic(kParamMnemonic);
		addMenuItem(paramMenu, new Commands.AddParameter(this), 'A', 0);
		addMenuItem(paramMenu, new Commands.RemoveParameter(this), 'R', 0);
		paramMenu.addSeparator();
		addMenuItem(paramMenu, new Commands.EditParameter(), 'E', 0);

		JMenu		panelMenu = new JMenu("Panel");
		panelMenu.setMnemonic(kPanelMnemonic);
		addMenuItem(panelMenu, new Commands.OpenPanel(this), 'O', 0);
		addMenuItem(panelMenu, new Commands.AddPanel(this), 'A', 0);
		addMenuItem(panelMenu, new Commands.RemovePanel(this), 'R', 0);
		addMenuItem(panelMenu, new Commands.RemoveAllPanels(this), 'E', 0);

		JMenu		helpMenu = new JMenu("Help");
		helpMenu.setMnemonic(kHelpMnemonic);
		addMenuItem(helpMenu, new Commands.About(this), 'A', 0);

		fMenuBar = new JMenuBar();
		fMenuBar.add(fileMenu);
		fMenuBar.add(editMenu);
		fMenuBar.add(viewMenu);
		fMenuBar.add(paramMenu);
		fMenuBar.add(panelMenu);
		fMenuBar.add(helpMenu);
		fWindow.setJMenuBar(fMenuBar);
	} // createMenus

	/**
	Creates the user interface.
	We create this manually, rather than by using some RAD tool, for better platform
	independence in development.
	*/
	public void createUI() {
		// Mac stuff
		try {
			if (MRJApplicationUtils.isMRJToolkitAvailable()) {
				// If we're in the Aqua L&F, we want a Mac menu bar at the top of the screen
				java.util.Properties	props = System.getProperties();
				props.put("com.apple.macos.useScreenMenuBar", "true");
				System.setProperties(props);
//				System.out.println("com.apple.macos.useScreenMenuBar=" + System.getProperty("com.apple.macos.useScreenMenuBar"));

				// Use our previus UI setting (but default to Metal, which is no longer
				// the default as of Mac OS X 4K29)
				String	ui = (String) Preferences.getObjectForKey("ui");
				if (ui == null || ui.equals("metal")) {
					UIManager.setLookAndFeel(UIManager.getCrossPlatformLookAndFeelClassName());
				}
			}
		} catch (Throwable e) {
		}

		fWindow = new JFrame("SoftLOGIC Runtime Display");
		Container		container = fWindow.getContentPane();

		// In some Javas, I'm seeing the splitpane resizing cursor. Try to avoid this
		container.setCursor(Cursor.getDefaultCursor());

		try {
			fBrowser = new ParameterTree(gContainer);
		} catch (ObserveM.BadObserveTypeException e) {
			System.out.println(e);
		}
		JScrollPane		scroller = new JScrollPane(fBrowser);
		fBrowserHolder = new JPanel();
		fBrowserHolder.setLayout(new BorderLayout());
		fBrowserHolder.add(scroller);	// Will go in expandable center of BorderLayout

		// Cancel and OK are below the parameter browser
		JPanel	buttons = new JPanel();
		
		URL			url = AltoStratus.class.getResource("/images/x.gif");
//		System.out.println("x url=" + url);
		ImageIcon	cancelIcon = new ImageIcon(url);
		JButton		cancelButton = new CommandButton("Cancel", cancelIcon, new Commands.CancelChanges(this));
		CommandRegistrar.gRegistrar.disableAction("cancelChanges");
		// This is really a kludge, but there is a lot of white space on left and right,
		// and it's hard for the button to fit.
		Insets		margin = cancelButton.getMargin();
		margin.left = 2;
		margin.right = 4;
		cancelButton.setMargin(margin);
		cancelButton.setToolTipText("Discards all pending changes to parameters");
		buttons.add(cancelButton);
		url = AltoStratus.class.getResource("/images/check.gif");
		ImageIcon	okIcon = new ImageIcon(url);
		JButton		okButton = new CommandButton("Commit", okIcon, new Commands.CommitChanges(this));
		// This is really a kludge, but there is a lot of white space on left and right,
		// and it's hard for the button to fit.
		margin = okButton.getMargin();
		margin.left = 2;
		margin.right = 4;
		okButton.setMargin(margin);
		okButton.setToolTipText("Updates all changed parameters");
		CommandRegistrar.gRegistrar.disableAction("commitChanges");
		buttons.add(okButton);
		fBrowserHolder.add(buttons, BorderLayout.SOUTH);

		JComponent		scrolled = this.makeNewPanel();
		fDisplayHolder = new JScrollPane(scrolled);

		JSplitPane		splitPane = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT,
			fBrowserHolder, fDisplayHolder);
		container.add(splitPane);
		// A split pane's elements need to have a minimum size
		Dimension		minSize = new Dimension(100, 50);
		fBrowserHolder.setMinimumSize(minSize);
		fDisplayHolder.setMinimumSize(minSize);

		this.createMenus();

		fWindow.pack();					// Size it to its contents
		fWindow.setSize(750, 500);		// Was 600 * 400 to try to fit most screens

//		System.out.println("getDividerLocation " + splitPane.getDividerLocation() + " " + splitPane.getWidth());
		splitPane.setDividerLocation(0.3);	// Devote most space to display

		fWindow.show();

		// Do anything that requires the frame to have a size
		// Annoyingly, the width of splitPane is not correct until the window is actually
		// shown. Luckily, I'm not seeing any flicker or other effects by doing this late.
		// If this is a problem, then we could always use the version which takes an integer
		// number of pixels
//		System.out.println("getDividerLocation " + splitPane.getDividerLocation() + " " + splitPane.getWidth());
		splitPane.setDividerLocation(0.3);	// Devote most space to display
//		System.out.println("getDividerLocation " + splitPane.getDividerLocation() + " " + splitPane.getWidth());
//		splitPane.setDividerLocation(200);	// Devote most space to display
	} // createUI

	/**
	Return the active DisplayPanel
	*/
	public DisplayPanel getCurrentPanel() {
		// Get the Component being scrolled
		Component			child = fDisplayHolder.getViewport().getView();
		if (child instanceof DisplayPanel) {
			// Just return child
		} else if (child instanceof JTabbedPane) {
			JTabbedPane		thePane = (JTabbedPane) child;
			child = thePane.getSelectedComponent();
		} else {
			child = null;
		}
		return (DisplayPanel) child;
	} // getCurrentPanel

	public static String getIOR(boolean aPass) {
		String			separator;
		String			home = System.getProperty("stratus.home");
		System.out.println("stratus.home=" + home);
		if (home == null) {
			System.err.println("?property 'stratus.home' must be set");
			return null;
		}
		if (home.startsWith("file://") || home.startsWith("http://")) {
			// No matter what the native OS uses for a separator, we should be using a
			// slash for a URL.
			separator = "/";
		} else {
			separator = File.separator;
		}
		if (!home.endsWith(separator))
			home += separator;

		String			serverPath = home + "_stratus.tmp";
		String			ior = null;
		Exception		theErr = null;
		String			errText= "?Could not launch " + name();

		try {
			ior = readFirstLine(serverPath);
		} catch (FileNotFoundException fnf) {
			if (aPass == kFirstPass) {
				String		serverScript = home + "run_stratus";
				Runtime		os = Runtime.getRuntime();
				try {
					Process		p = os.exec(serverScript);
					System.out.println("started " + p);
					InputStream	errors = p.getErrorStream();
					if (errors.available() > 0) {
						char[]	remoteErr = new char[errors.available()];
						InputStreamReader	r = new InputStreamReader(errors);
						r.read(remoteErr, 0, errors.available());
						System.out.println("result: " + String.valueOf(remoteErr));
					}
					ior = getIOR(kSecondPass);
				} catch (IOException e) {
					errText = "?Could not start server";
					theErr = e;
				}
			} else {
				theErr = fnf;
			}
		} catch (MalformedURLException e) {
			theErr = e;
		} catch (IOException e) {
			theErr = e;
		}

		if (theErr != null) {
			System.err.println(errText + ".");
			System.err.println(theErr);
			System.exit(0);
		}

		return ior;
	}

	public static Iterator getTrackedFiles() {
		return gFilesSeen.iterator();
	}

	protected static void initializePreferences() {
		// Make a preference file in ~/AltoStratus/AltoStratus.prefs
		Preferences.makePreferences("AltoStratus", "AltoStratus.prefs");	// !!!

		gFilesSeen = new TreeSet();
		String		theString = Preferences.getObjectForKey("recentFiles.count");
		if (theString != null) {
			try {
				int		count = Integer.parseInt(theString);
				int		i;
				for (i = 1; i <= count; i++) {
					theString = Preferences.getObjectForKey("recentFiles." + i);
					File	theFile = new File(theString);
					// Make sure the file exists -- this lets us weed out invalid entries
					if (theFile.exists())
						gFilesSeen.add(theFile);
				}
			} catch (Exception e) {
				e.printStackTrace();
			}
		}

	} // initializePreferences

	/**
	Entry point for application
	*/
	public static void main(String args[]) {
		try {
		InetAddress	addr = InetAddress.getLocalHost();
		String		myHostAddr = addr.getHostAddress();
		System.out.println(myHostAddr + " " + addr);
		} catch (Exception e) {
		}
		String		ior = getIOR(kFirstPass);
		if (ior != null) {
			System.out.println("ior=" + ior);
			org.omg.CORBA.ORB		orb = null;
			try {
				orb = org.omg.CORBA.ORB.init(args, null /*props*/);
			} catch (Exception e) {
				System.err.println("?Unable to initialize ORB. " + e);
				System.exit(0);		// Bail out
			}

			// "destringify" the object
			RootFactory				theFactory = null;
			VteObjectContainer[]	containers = null;
			try {
				theFactory = RootFactoryHelper.narrow(orb.string_to_object(ior));
				containers = theFactory.getAllVteObjectContainers();
			} catch (Exception e) {
				System.err.println("?Unable to talk with server (it may not be running).");
				System.err.println(" " + e);
				System.exit(0);		// Bail out
			}
			gContainer = containers[0];
//			System.out.println("theContainer=" + gContainer);
		}

		// Mac stuff
		try {
			if (MRJApplicationUtils.isMRJToolkitAvailable()) {
				MRJOSType	newType = new MRJOSType("TEXT");
				MRJOSType	newCreator = new MRJOSType("CWIE");
				MRJFileUtils.setDefaultFileType(newType);
				MRJFileUtils.setDefaultFileCreator(newCreator);
			}
		} catch (Throwable e) {
		}

		initializePreferences();

		gApplication = new AltoStratus();
		try {
		gApplication.createUI();

		// For ease in debugging, start with one
		Parameter	p = new Parameter("System.registers.Reg0x4000", "0x4000_Data Register");
//		gApplication.getCurrentPanel().add(p);
//		gApplication.getCurrentPanel().revalidate();

		} catch (Exception e) {
		System.out.println(e);
		Parameter	p = new Parameter("System.registers.Reg0x4000", "0x4000_Data Register");
		p.startObserving();
		System.out.println(p.getValueString(format_Number, radix_Hex));
		}
	} // main

	/**
	Converts to tabs if necessary
	*/
	public JComponent makeNewPanel() {
		DisplayPanel	theDisplay = new DisplayPanel();

		if (fPanelsMade > 0) {
			if (fPanelHolder == null)
				fPanelHolder = new JTabbedPane();

			if (fPanelsMade == 1) {
				// We need to get the first DisplayPanel out of the scroll
				DisplayPanel	firstDisplay = this.getCurrentPanel();
				// Scroll the tabs
				fDisplayHolder.getViewport().setView(fPanelHolder);
				// Put the first DisplayPanel into the tabs
				fPanelHolder.addTab(firstDisplay.getName(), firstDisplay);
			}
			// Stick the new DisplayPanel into the tabs
			fPanelHolder.addTab(theDisplay.getName(), theDisplay);
			fPanelHolder.setSelectedIndex(fPanelHolder.getTabCount() - 1);
			fPanelsMade++;;
			CommandRegistrar.gRegistrar.enableAction("removeAllPanels");
			CommandRegistrar.gRegistrar.enableAction("removePanel");
			return fPanelHolder;
		} else {
			fPanelsMade++;;
			return theDisplay;
		}
	}

	/**
	Convenience function for creating menu items
		@param aLabel Text of the menu item
		@param aListener Object whose actionPerformed() method will be called
		@param aCommand String which identifies the action
		@param aMnemonic the letter to underline (0 if none)
		@param anAcceleratorKey the keystroke which activates this item
	*/
	static JMenuItem menuItem(String aLabel, ActionListener aListener, String aCommand,
		int aMnemonic, int anAcceleratorKey)
	{
		JMenuItem	item = new JMenuItem(aLabel);
		item.addActionListener(aListener);
		item.setActionCommand(aCommand);
		if (aMnemonic != 0)
			item.setMnemonic((char)aMnemonic);
		if (anAcceleratorKey != 0)
			item.setAccelerator(KeyStroke.getKeyStroke(anAcceleratorKey, java.awt.Event.CTRL_MASK));
		return item;
	} // menuItem

	static String name() {
		return "SoftLOGIC";
	}

	/**
	Convenience function for creating menu items
		@param aLabel Text of the menu item
		@param aListener Object whose actionPerformed() method will be called
		@param aCommand String which identifies the action
		@param aMnemonic the letter to underline (0 if none)
		@param anAcceleratorKey the keystroke which activates this item
	*/
	static JRadioButtonMenuItem radioButtonItem(Action anAction, int aMnemonic, int anAcceleratorKey) {
		JRadioButtonMenuItem	item = new JRadioButtonMenuItem(
			(String)anAction.getValue(Action.NAME), false);		// Start disabled
		item.addActionListener(anAction);
		if (aMnemonic != 0)
			item.setMnemonic((char)aMnemonic);
		if (anAcceleratorKey != 0)
			item.setAccelerator(KeyStroke.getKeyStroke(anAcceleratorKey, java.awt.Event.CTRL_MASK));
		return item;
	} // radioButtonItem

	/**
	Read the first line from the specified file
		@param aPath Path of the file, either as a URL or a file system path
		@return The first line
	*/
	protected static String readFirstLine(String aPath) throws IOException, MalformedURLException {
		BufferedReader	in;
		if (aPath.startsWith("file://") || aPath.startsWith("http://")) {
			URL			theURL = new URL(aPath);
			in = new BufferedReader(new InputStreamReader(theURL.openStream()));
		} else {
			in = new BufferedReader(new FileReader(aPath));
		}
		return in.readLine();	// We're only interested in the first line
	} // readFirstLine

	/**
	Remove all panels from the display.
	*/
	public void removeAllPanels() {
		int			r = JOptionPane.showConfirmDialog(fDisplayHolder,
			"Are you sure you want to remove all of the subpanels?",
			"Remove All Panels?",
			JOptionPane.OK_CANCEL_OPTION);
		if (r == JOptionPane.YES_OPTION || r == JOptionPane.OK_OPTION) {
			fDisplayHolder.setViewportView(new DisplayPanel());
			fPanelHolder = null;
			fPanelsMade = 1;
			// Don't allow removing the now empty DisplayPanel
			CommandRegistrar.gRegistrar.disableAction("removeAllPanels");
			CommandRegistrar.gRegistrar.disableAction("removePanel");
		}
	}

	/**
	Remove the current panel from the display.
	*/
	public void removeCurrentPanel() {
		int			r = JOptionPane.showConfirmDialog(fDisplayHolder,
			"Are you sure you want to remove the currently active subpanel?",
			"Remove Panel?",
			JOptionPane.OK_CANCEL_OPTION);
		if (r == JOptionPane.YES_OPTION || r == JOptionPane.OK_OPTION) {
			fPanelHolder.remove(fPanelHolder.getSelectedComponent());
			fPanelsMade--;
			// If we just dropped down to one, we don't need tabs any more
			if (fPanelsMade == 1) {
				Component	c = fPanelHolder.getSelectedComponent();
				fDisplayHolder.setViewportView(c);
				fPanelHolder = null;
				// Don't allow removing the last one
				CommandRegistrar.gRegistrar.disableAction("removeAllPanels");
				CommandRegistrar.gRegistrar.disableAction("removePanel");
			}
		}
	}

	/**
	Keep track of all files opened or saved
	*/
	public static void trackFile(File aFile) {
		if (gFilesSeen == null)
			gFilesSeen = new TreeSet();

		if (!gFilesSeen.contains(aFile)) {
			try {
				gFilesSeen.add(aFile);
				int				count = gFilesSeen.size();
				
				Preferences.setObjectForKeyNoWrite("" + count, "recentFiles.count");
				Iterator		iter = getTrackedFiles();
				int				i;
				for (i = 1; i <= count; i++) {
					File		theFile = (File) iter.next();
					String		thePath = theFile.getCanonicalPath();
					Preferences.setObjectForKeyNoWrite(thePath, "recentFiles." + i);
				}
			} catch (Exception e) {
				e.printStackTrace();
			}
			Preferences.write();

			// Put it in the tree
			gApplication.fBrowser.updateTrackedFiles();
		}
	}

	/**
	Human-readable application version
	*/
	public static String version() {
		return "1.0d28";
	}


	/**
	MenuItemActionListener {Inner Class}
		Responds to menu choices
		!!! This should go away once we implement everything
	*/
	public static class MenuItemActionListener implements ActionListener
	{
		Component	fParent;		// Whose menu?

		public MenuItemActionListener(Component aParent)	{ fParent = aParent; }

		public void actionPerformed(ActionEvent anEvent) {
			JMenuItem		item = (JMenuItem)anEvent.getSource();
			String		cmd = item.getActionCommand();
			JOptionPane.showMessageDialog(fParent, cmd + " was selected.");
		}
	}
}
