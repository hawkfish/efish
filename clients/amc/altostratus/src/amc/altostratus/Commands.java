/*
File:		Commands.java

Contains:	Menu commands for AltoStratus, the graphical interface to Stratus

Written by:	David Dunham
			Electric Fish, Inc. <http://www.electricfish.com>

Copyright:	Copyright ©2001 by Applied Microsystems Corporation.  All Rights reserved.

Change History (most recent first):

	05 Apr 2001		drd	Packages
	21 Mar 2001		drd	Use name() in case it changes
	20 Mar 2001		drd	AddPanel, OpenPanel no longer show file dialog
	12 Mar 2001		drd	Save Aqua/Metal setting
	08 Mar 2001		drd	Added "Prerelease" to about box
	06 Mar 2001		drd	Created (from AltoStratus.java); AddPanel, RemoveAllPanels, RemovePanel
*/

package amc.altostratus;

import amc.altostratus.constants.*;
import amc.altostratus.framework.*;

import java.awt.*;				// AWT classes
import java.awt.event.*;			// Basic event handling
import java.io.*;
import java.util.Vector;
import javax.swing.*;				// Swing components and classes
import javax.swing.border.*;		// Borders for Swing components

/**
The main class (application)
	@author David Dunham
*/
public interface Commands extends MiscConstants, ParamConstants, UIConstants
{
	/**
	About
	*/
	public static class About extends AppCommand
	{
		private	JDialog	fAboutBox;

		/**
		About
			Constructor
		*/
		public About(AltoStratus anApp) {
			// Define our name
			super(anApp, "About " + AltoStratus.name());
			CommandRegistrar.gRegistrar.putAction("about", this);

			fAboutBox = null;
		} // About

		/**
		performAction
		*/
		public void performAction(ActionEvent anEvent) {
			if (fAboutBox == null)
				this.makeAboutBox();
			fAboutBox.setVisible(true);
		}

		/**
		makeAboutBox
			See: aardvark Version.java
		*/
		protected void makeAboutBox() {
			GridBagLayout		gridbag = new GridBagLayout();
			GridBagConstraints	c = new GridBagConstraints();
			
			fAboutBox = new JDialog(fApplication.fWindow, "About " + AltoStratus.name());
			JPanel			holder = new JPanel();
			fAboutBox.getContentPane().add(holder);
			holder.setLayout(gridbag);
			holder.setBorder(BorderFactory.createEmptyBorder(12, 12, 12, 12));
			
			//top label
			String			topText = "<html><h1>" +
								"<font color=green>" + AltoStratus.name() + "</font></h1></html>";
			JLabel			topLabel = new JLabel(topText);	
			c.anchor = GridBagConstraints.NORTH;
			c.gridwidth = GridBagConstraints.REMAINDER;
			gridbag.setConstraints(topLabel, c);
			holder.add(topLabel);
			
			// Any graphics goes here ???
			
			//text
			String	initialText = "<html><h2>" +
				"<font color=red>Prerelease</font> " +
				"Version " + AltoStratus.version() + "<br>Copyright \u00a92001 Applied Microsystems Corporation</h2>";
			initialText += "Engineering by Electric Fish Inc.<br>This product includes software developed by the Apache Software Foundation (http://www.apache.org/).<br>";
			JLabel	textLabel = new JLabel(initialText);
			c.gridwidth = GridBagConstraints.REMAINDER; //end of row
			c.weighty = 1.0;
			c.weightx = 1.0;
			c.fill = GridBagConstraints.NONE;
			gridbag.setConstraints(textLabel, c);
			holder.add(textLabel);
			
			//button
			// !!! This actually should be 11 pixels from bottom and right
			JButton	button = new JButton("Close");
			button.setMnemonic('C');
			button.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent e) {
					fAboutBox.setVisible(false);
				}
			});
			c.gridwidth = GridBagConstraints.REMAINDER;
			c.anchor = GridBagConstraints.SOUTH;
			gridbag.setConstraints(button, c);
			holder.add(button);
			fAboutBox.getRootPane().setDefaultButton(button);

			fAboutBox.pack();
			fAboutBox.setResizable(false);
			fAboutBox.setVisible(false);
		}//end makeAboutBox
	}

	/**
	AddPanel
	*/
	public static class AddPanel extends AppCommand {
		/**
		Constructor
		*/
		public AddPanel(AltoStratus anApp) {
			// Define our name
			super(anApp, "Append");
			CommandRegistrar.gRegistrar.putAction("addPanel", this);

			this.setEnabled(false);		// Start out dim
		} // AddPanel

		/**
		Add to current panel
		*/
		public void performAction(ActionEvent anEvent) {
			FileDialog		dialog = new FileDialog(fApplication.fWindow, "", FileDialog.LOAD);
			dialog.show();
			String			dir = dialog.getDirectory();
			String			fileName = dialog.getFile();
			System.out.println("dir=" + dir + " fileName=" + fileName);
			if (fileName != null) {
				File		theFile = new File(dir + fileName);
				fApplication.getCurrentPanel().open(theFile, kAppend);
			}
		}
	} // AddPanel

	/**
	AddParameter
	*/
	public class AddParameter extends AppCommand {
		/**
		Constructor
		*/
		public AddParameter(AltoStratus anApp) {
			// Define our name
			super(anApp, "Add");
			CommandRegistrar.gRegistrar.putAction("addParam", this);

			this.setEnabled(false);		// Start out dim
		} // AddParameter

		/**
		performAction
		*/
		public void performAction(ActionEvent anEvent) {
			Parameter	theParameter = fApplication.fBrowser.getSelectedParameter();
			if (theParameter != null) {
				fApplication.getCurrentPanel().add(theParameter);
				fApplication.getCurrentPanel().revalidate();
			}
		}
	}

	/**
	Aqua
	*/
	public class Aqua extends AppCommand {
		/**
		Constructor
		*/
		public Aqua(AltoStratus anApp) {
			// Define our name
			super(anApp, "Aqua");
			CommandRegistrar.gRegistrar.putAction("aqua", this);
//			this.putValue("ActionCommandKey" /*Action.ACTION_COMMAND_KEY*/, "aqua");
//			this.putValue("MnemonicKey" /*Action.MNEMONIC_KEY*/, "A" /*'A'*/);
		}

		public void performAction(ActionEvent anEvent) {
			try {
				// Play with the Mac OS X (Aqua) UI
				UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
				SwingUtilities.updateComponentTreeUI(fApplication.fWindow);
				// ??? If we keep this command, we should also update dialogs
				Preferences.setObjectForKey("aqua", "ui");
				this.setEnabled(false);
				// Enable the other one
				CommandRegistrar.gRegistrar.enableAction("metal");
			} catch (Exception e) {
				// Don't worry about the exception, we can handle the default look & feel
			}
		}
	}

	/**
	Handles the Bar Graph command
	*/
	public class BarGraph extends AppCommand
	{
		/**
		Constructor
		*/
		public BarGraph(AltoStratus anApp) {
			// Define our name
			super(anApp, "Bar Graph");
			CommandRegistrar.gRegistrar.putAction("barGraph", this);
//			this.putValue("ActionCommandKey" /*Action.ACTION_COMMAND_KEY*/, "aqua");
//			this.putValue("MnemonicKey" /*Action.MNEMONIC_KEY*/, "A" /*'A'*/);

			this.setEnabled(false);		// Start out dim
		} // BarGraph

		/**
		performAction
		*/
		public void performAction(ActionEvent anEvent) {
			fApplication.getCurrentPanel().setFormat(format_Bar);
		}
	} // BarGraph

	/**
	Handles the Cancel Changes command (for button)
	*/
	public class CancelChanges extends AppCommand
	{
		/**
		Constructor
		*/
		public CancelChanges(AltoStratus anApp) {
			// Define our name
			super(anApp, "Cancel Changes");
			CommandRegistrar.gRegistrar.putAction("cancelChanges", this);

			// Unlike the typical action, don't start out dim (since the button needs to
			// be set up to listen first)
		} // CancelChanges

		/**
		performAction
		*/
		public void performAction(ActionEvent anEvent) {
			fApplication.getCurrentPanel().cancelChanges();
			this.setEnabled(false);
			CommandRegistrar.gRegistrar.disableAction("commitChanges");
		}
	} // CancelChanges

	/**
	Handles the Commit Changes command (for button)
	*/
	public class CommitChanges extends AppCommand
	{
		/**
		Constructor
		*/
		public CommitChanges(AltoStratus anApp) {
			// Define our name
			super(anApp, "Commit Changes");
			CommandRegistrar.gRegistrar.putAction("commitChanges", this);

			// Unlike the typical action, don't start out dim (since the button needs to
			// be set up to listen first)
		} // CommitChanges

		/**
		performAction
		*/
		public void performAction(ActionEvent anEvent) {
			fApplication.getCurrentPanel().commitChanges();
			this.setEnabled(false);
			CommandRegistrar.gRegistrar.disableAction("cancelChanges");
		}
	} // CommitChanges

	/**
	Handles the Decimal command
	*/
	public class Decimal extends AppCommand
	{
		/**
		Constructor
		*/
		public Decimal(AltoStratus anApp) {
			// Define our name
			super(anApp, "Decimal");
			CommandRegistrar.gRegistrar.putAction("decimal", this);
//			this.putValue("ActionCommandKey" /*Action.ACTION_COMMAND_KEY*/, "aqua");
//			this.putValue("MnemonicKey" /*Action.MNEMONIC_KEY*/, "A" /*'A'*/);

			this.setEnabled(false);		// Start out dim
		} // Decimal

		/**
		performAction
		*/
		public void performAction(ActionEvent anEvent) {
			fApplication.getCurrentPanel().setDecimal();
		}
	} // Decimal


	/**
	Handles the Edit command
	*/
	public static class EditParameter extends SafeAction
	{
		/**
		Constructor
		*/
		public EditParameter() {
			// Define our name
			super("Edit");
			CommandRegistrar.gRegistrar.putAction("editParameter", this);
//			this.putValue("ActionCommandKey" /*Action.ACTION_COMMAND_KEY*/, "aqua");
//			this.putValue("MnemonicKey" /*Action.MNEMONIC_KEY*/, "A" /*'A'*/);

			this.setEnabled(false);		// Start out dim
		} // EditParameter

		/**
		performAction
		*/
		public void performAction(ActionEvent anEvent) {
			// We need to forward this down, probably to a Displayer
			Vector	theSel = Selection.getSelection();
			if (theSel != null && theSel.size() == 1) {
				Object		selected = theSel.elementAt(0);
				Strip		theStrip = (Strip) selected;
				if (theStrip != null) {
					theStrip.startEditing();
				}
			}
		}
	} // EditParameter

	/**
	Handles the Exit (quit) command
	*/
	public static class Exit extends SafeAction
	{
		/**
		Constructor
		*/
		public Exit() {
			// Define our name
			super("Exit");
			CommandRegistrar.gRegistrar.putAction("quit", this);
//			this.putValue("ActionCommandKey" /*Action.ACTION_COMMAND_KEY*/, "aqua");
//			this.putValue("MnemonicKey" /*Action.MNEMONIC_KEY*/, "A" /*'A'*/);
		} // Exit

		/**
		performAction
		*/
		public void performAction(ActionEvent anEvent) {
			// ??? worry about unsaved changes

			System.exit(0);				// Standard exit (leaves other JVM apps running)
		}
	}

	/**
	Find
	*/
	public class Find extends AppCommand
	{
		final int		kFieldWidth = 10;

		JDialog		fDialog;
		JTextField		fTextField;

		/**
		Constructor
		*/
		public Find(AltoStratus anApp) {
			// Define our name
			super(anApp, "Find\u2026");
			CommandRegistrar.gRegistrar.putAction("find", this);
//			this.putValue("ActionCommandKey" /*Action.ACTION_COMMAND_KEY*/, "aqua");
//			this.putValue("MnemonicKey" /*Action.MNEMONIC_KEY*/, "A" /*'A'*/);

			fDialog = null;
		} // Find

		/**
		Execute the command (by displaying the Find dialog)
		*/
		public void performAction(ActionEvent anEvent) {
			if (fDialog == null)
				this.makeDialog();
			fDialog.setVisible(true);
			fTextField.selectAll();
			fTextField.requestFocus();
		}

		public void find() {
			fApplication.fSearchText = fTextField.getText();
			if (fApplication.fSearchText.length() > 0) {
				CommandRegistrar.gRegistrar.enableAction("findNext");
				fApplication.fBrowser.find(fApplication.fSearchText, kFirstPass);
			}
		} // find

		/**
		Create the dialog box
		*/
		private void makeDialog() {
			fDialog = new JDialog(fApplication.fWindow, "Find");
			JPanel			holder = new JPanel();
			fDialog.getContentPane().add(holder);
			holder.setBorder(BorderFactory.createEmptyBorder(12, 12, 12, 12));

			JLabel	textLabel = new JLabel("Parameter:");
			holder.add(textLabel);
			fTextField = new JTextField(kFieldWidth);
			holder.add(fTextField);
			fTextField.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent e) {
					Find.this.find();
				}
			});

			//button
			// !!! This actually should be 11 pixels from bottom and right
			JButton	button = new JButton("Find");
			button.setMnemonic('F');
			button.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent e) {
					Find.this.find();
				}
			});
			holder.add(button);
			fDialog.getRootPane().setDefaultButton(button);

			fDialog.pack();
			fDialog.setResizable(false);
			fDialog.setVisible(false);
		} // makeDialog
	}

	/**
	FindNext
	*/
	public class FindNext extends AppCommand
	{
		/**
		Constructor
		*/
		public FindNext(AltoStratus anApp) {
			// Define our name
			super(anApp, "Find Next");
			CommandRegistrar.gRegistrar.putAction("findNext", this);
//			this.putValue("ActionCommandKey" /*Action.ACTION_COMMAND_KEY*/, "aqua");
//			this.putValue("MnemonicKey" /*Action.MNEMONIC_KEY*/, "A" /*'A'*/);

			// Start disabled (Find will enable us)
			CommandRegistrar.gRegistrar.disableAction("findNext");
		} // Find

		/**
		Execute the command
		*/
		public void performAction(ActionEvent anEvent) {
			fApplication.fBrowser.find(fApplication.fSearchText, kFirstPass);
		}
	}

	/**
	Handles the Hexadecimal command
	*/
	public class Hexadecimal extends AppCommand
	{
		/**
		Constructor
		*/
		public Hexadecimal(AltoStratus anApp) {
			// Define our name
			super(anApp, "Hexadecimal");
			CommandRegistrar.gRegistrar.putAction("hex", this);
//			this.putValue("ActionCommandKey" /*Action.ACTION_COMMAND_KEY*/, "hex");
//			this.putValue("MnemonicKey" /*Action.MNEMONIC_KEY*/, "A" /*'A'*/);

			this.setEnabled(false);		// Start out dim
		} // Hexadecimal

		/**
		performAction
		*/
		public void performAction(ActionEvent anEvent) {
			fApplication.getCurrentPanel().setHex();
		}
	} // Hexadecimal

	/**
	Metal
	*/
	public class Metal extends AppCommand {
		/**
		Metal
			Constructor
		*/
		public Metal(AltoStratus anApp) {
			// Define our name
			super(anApp, "Metal");
			CommandRegistrar.gRegistrar.putAction("metal", this);
//			this.putValue(Action.ACTION_COMMAND_KEY, "metal");
//			this.putValue("MnemonicKey" /*Action.MNEMONIC_KEY*/, "M" /*'A'*/);

			// We are defaulting to Metal, so disable it in the menus
			this.setEnabled(false);
		}

		public void performAction(ActionEvent anEvent) {
			try {
				// Back to cross-platform (Metal) UI
				UIManager.setLookAndFeel(UIManager.getCrossPlatformLookAndFeelClassName());
				SwingUtilities.updateComponentTreeUI(fApplication.fWindow);
				// ??? If we keep this command, we should also update dialogs
				Preferences.setObjectForKey("metal", "ui");
				this.setEnabled(false);
				// Enable the other one
				CommandRegistrar.gRegistrar.enableAction("aqua");
			} catch (Exception e) {
				// Don't worry about the exception, we can handle the default look & feel
			}
		}
	}

	/**
	Dummy action used to uncheck the menus
	*/
	public static class NeitherHexNorDecimal extends SafeAction
	{
		/**
		Constructor
		*/
		public NeitherHexNorDecimal() {
			// Define our name
			super("Neither");
			CommandRegistrar.gRegistrar.putAction("neitherHexNorDecimal", this);
		} // NeitherHexNorDecimal

		/**
		performAction
		*/
		public void performAction(ActionEvent anEvent) {
		}
	} // NeitherHexNorDecimal

	/**
	Dummy action used to uncheck the menus
	*/
	public static class NoFormat extends SafeAction
	{
		/**
		Constructor
		*/
		public NoFormat() {
			// Define our name
			super("Neither1");
			CommandRegistrar.gRegistrar.putAction("noParticularFormat", this);
		} // NoFormat

		/**
		performAction
		*/
		public void performAction(ActionEvent anEvent) {
		}
	} // NoFormat

	/**
	Handles the Numeric command
	*/
	public class Numeric extends AppCommand
	{
		/**
		Constructor
		*/
		public Numeric(AltoStratus anApp) {
			// Define our name
			super(anApp, "Numeric");
			CommandRegistrar.gRegistrar.putAction("numeric", this);

			this.setEnabled(false);		// Start out dim
		} // Numeric

		/**
		performAction
		*/
		public void performAction(ActionEvent anEvent) {
			fApplication.getCurrentPanel().setFormat(format_Number);
		}
	} // Numeric

	/**
	Open
	*/
	static public class Open extends AppCommand {
		/**
		Constructor
		*/
		public Open(AltoStratus anApp) {
			// Define our name
			super(anApp, "Open\u2026");
			CommandRegistrar.gRegistrar.putAction("open", this);
		} // Open

		/**
		performAction
		*/
		public void performAction(ActionEvent anEvent) {
			if (fApplication.getCurrentPanel().saveFirst()) {
				FileDialog		dialog = new FileDialog(fApplication.fWindow, "", FileDialog.LOAD);
				dialog.show();
				String			dir = dialog.getDirectory();
				String			fileName = dialog.getFile();
				System.out.println("dir=" + dir + " fileName=" + fileName);
				if (fileName != null) {
					File		theFile = new File(dir + fileName);
					fApplication.getCurrentPanel().open(theFile, kOpen);
				}

				// JFileChooser doesn't give as good a user experience, so we don't use it
/*
				JFileChooser	chooser = new JFileChooser();
				int				result = chooser.showOpenDialog(null);
				if (result == JFileChooser.APPROVE_OPTION) {
					File		theFile = chooser.getSelectedFile();
					System.out.println("file=" + theFile);
					fApplication.getCurrentPanel().open(theFile);
				}
*/
			}
		}
	} // Open

	/**
	OpenPanel
	*/
	public static class OpenPanel extends AppCommand {
		/**
		Constructor
		*/
		public OpenPanel(AltoStratus anApp) {
			// Define our name
			super(anApp, "Open");
			CommandRegistrar.gRegistrar.putAction("openPanel", this);

			this.setEnabled(false);		// Start out dim
		} // OpenPanel

		/**
		Always adds a new panel
		*/
		public void performAction(ActionEvent anEvent) {
			FileNode		fileNode = (FileNode) fApplication.fBrowser.getSelection();

			if (fileNode != null) {
				File		theFile = fileNode.getFile();
				fApplication.makeNewPanel();
				fApplication.getCurrentPanel().open(theFile, kOpen);
			}
		}
	} // OpenPanel

	/**
	RemoveAllPanels
	*/
	public static class RemoveAllPanels extends AppCommand {
		/**
		Constructor
		*/
		public RemoveAllPanels(AltoStratus anApp) {
			// Define our name
			super(anApp, "Remove All");
			CommandRegistrar.gRegistrar.putAction("removeAllPanels", this);
			this.setEnabled(false);
		} // RemoveAllPanels

		/**
		Always adds a new panel
		*/
		public void performAction(ActionEvent anEvent) {
			fApplication.removeAllPanels();
		}
	} // RemoveAllPanels

	/**
	RemovePanel
	*/
	public static class RemovePanel extends AppCommand {
		/**
		Constructor
		*/
		public RemovePanel(AltoStratus anApp) {
			// Define our name
			super(anApp, "Remove");
			CommandRegistrar.gRegistrar.putAction("removePanel", this);
			this.setEnabled(false);
		} // RemovePanel

		/**
		Always adds a new panel
		*/
		public void performAction(ActionEvent anEvent) {
			fApplication.removeCurrentPanel();
		}
	} // RemovePanel

	/**
	RemoveParameter
	*/
	public static class RemoveParameter extends AppCommand {
		/**
		Constructor
		*/
		public RemoveParameter(AltoStratus anApp) {
			// Define our name
			super(anApp, "Remove");
			CommandRegistrar.gRegistrar.putAction("delParam", this);

			this.setEnabled(false);		// Start out dim
		} // RemoveParameter

		/**
		performAction
		*/
		public void performAction(ActionEvent anEvent) {
			fApplication.getCurrentPanel().removeSelected();
		}
	}

	/**
	Handles the Save and Save As... commands
	*/
	public static class Save extends AppCommand
	{
		private String		fAction;

		/**
		Constructor
		*/
		public Save(AltoStratus anApp, String aName, String anAction) {
			// Define our name
			super(anApp, aName);
			fAction = anAction;
			CommandRegistrar.gRegistrar.putAction(anAction, this);
		} // Save

		/**
		performAction
		*/
		public void performAction(ActionEvent anEvent) {
			fApplication.getCurrentPanel().save(fAction);
		}
	} // Save

	/**
	Handles the Options command
	*/
	public static class ViewOptions extends SafeAction
	{
		/**
		Constructor
		*/
		public ViewOptions() {
			// Define our name
			super("Options");
			CommandRegistrar.gRegistrar.putAction("viewOptions", this);
//			this.putValue("ActionCommandKey" /*Action.ACTION_COMMAND_KEY*/, "aqua");
//			this.putValue("MnemonicKey" /*Action.MNEMONIC_KEY*/, "A" /*'A'*/);

			this.setEnabled(false);		// Start out dim
		} // ViewOptions

		/**
		performAction
		*/
		public void performAction(ActionEvent anEvent) {
			// We need to forward this down, probably to a Displayer
			Vector	theSel = Selection.getSelection();
			if (theSel != null && theSel.size() == 1) {
				Object		selected = theSel.elementAt(0);
				Strip	theDisplayer = (Strip) selected;
				if (theDisplayer != null) {
					theDisplayer.doViewOptions();
				}
			}
		}
	} // ViewOptions
}
