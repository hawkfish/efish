/*
File:		BarGraphDisplayer.java

Contains:	Bar graph display in an AltoStratus strip.

Written by:	David Dunham
		Electric Fish, Inc. <http://www.electricfish.com>

Copyright:	Copyright ©2000-2001 by Applied Microsystems Corporation.  All Rights reserved.

Change History (most recent first):

	05 Apr 2001		drd	Changes for packages
	07 Mar 2001		drd	Use IntegerEditor instead of JTextField, and do validation
	31 Jan 2001		drd	Better guess at fMax; dialog handles hex
	25 Jan 2001		drd	Tweaked label drawing
	23 Jan 2001		drd	Fixed updateMenus; handle fMin properly
	22 Jan 2001		drd	ViewOptionsDialog
	05 Jan 2001		drd	Draw the actual bar
	04 Jan 2001		drd	Draw labels under bar
	21 Dec 2000		drd	Support radix, etc.
	29 Nov 2000		drd	Created
*/

package amc.altostratus;

import amc.altostratus.constants.ParamConstants;
import amc.altostratus.framework.CommandRegistrar;
import amc.altostratus.xml.XMLWriter;
import java.awt.*;				// AWT classes
import java.awt.event.*;
import java.io.IOException;
import javax.swing.*;				// Swing components and classes

class BarGraphDisplayer extends JComponent implements Displayer, ParamConstants
{
// Constants
	static final int	kBarHeight = 12;
	static final int	kBorderThickness = 2;
	static final int	kHorizontalMargin = 4;	// On left and right
	static final int	kVerticalMargin = 1;	// On top and bottom

// Globals
	static Font		gLabelFont = null;		// Used for the text labels

// Instance data
	private	int			fMin;
	private	int			fMax;
	private int			fBaseline;			// For drawing labels
	private Parameter	fParameter;			// The parameter we display
	private char		fFormat;			// How parameter is displayed (e.g. format_Bar)
	private char		fRadix;				// How parameter is displayed (e.g. radix_Decimal)
	private char		fType;				// type_Integer, type_Float
	private ViewOptionsDialog	fDialog;

	public BarGraphDisplayer(Strip aStrip, Parameter aParameter) {
		// Make our font if we haven't done so
		if (gLabelFont == null) {
			gLabelFont = new Font("SansSerif", Font.PLAIN, 10);
		}

		FontMetrics		fm = this.getFontMetrics(gLabelFont);
		fBaseline = fm.getAscent();

		Dimension		d = new Dimension(aStrip.getWidth() - kHorizontalMargin * 2 -
			kBorderThickness * 2, fBaseline + fm.getDescent() + kBarHeight + kVerticalMargin * 2 +
			kBorderThickness);
		this.setPreferredSize(d);

		fParameter = aParameter;
		fFormat = aStrip.getFormat();		// Better be format_Bar (so do we need this???)
		fRadix = aStrip.getRadix();
		fType = aParameter.getType();

		// Here is where we cleverly get min and max from the parameter value…
		fMin = 0;
		int				value = fParameter.getValueInt();
		if (value < 255)
			fMax = 255;
		else {
			fMax = ((value + 99) / 100) * 100;
		}
	} // BarGraphDisplayer

	public void doViewOptions() {
		if (fDialog == null)
			this.makeDialog();
		fDialog.setupAndShow();
	} // doViewOptions

	/*
	Returns a value suitable for use as a label
	*/
	private String getLabel(int aValue) {
		if (fRadix == radix_Hex) {
			return Format.toHexString(aValue);
		} else {
			if (fType == type_Float) {
				return "" + (double)aValue;
			} else {
				return "" + aValue;
			}
		}
	}

	private String getMaxLabel() {
		return this.getLabel(fMax);
	}

	private String getMinLabel() {
		return this.getLabel(fMin);
	}

	/**
	Create the dialog box
	*/
	void makeDialog()
	{
		fDialog = new ViewOptionsDialog(this.getTopLevelAncestor());
	} // makeDialog

	/**
	paintComponent {OVERRIDE}
		@param aG
	*/
	public void paintComponent(Graphics aG) {
		// Erase the background
		aG.setColor(this.getBackground());
		aG.fillRect(0, 0, this.getWidth(), this.getHeight());
		aG.setColor(this.getForeground());
		aG.drawRect(kHorizontalMargin, kVerticalMargin,
			this.getWidth() - kHorizontalMargin * 2 - 1, kBarHeight - 1);

		aG.setFont(gLabelFont);
		int			y = kBarHeight + kVerticalMargin + kBorderThickness + fBaseline;
		aG.drawString(this.getMinLabel(), kBorderThickness + kHorizontalMargin, y);
		String		m = this.getMaxLabel();
		FontMetrics	fm = this.getFontMetrics(gLabelFont);
		int			w = fm.stringWidth(m);
		aG.drawString(m, this.getWidth() - kBorderThickness - kHorizontalMargin - w, y);

		if (fParameter != null) {
			double	percent = 0.0;
			if (fParameter.getType() == type_Integer) {
				percent = ((double) fParameter.getValueInt() - fMin) / (fMax - fMin);
			} else if (fParameter.getType() == type_Float) {
				percent = (fParameter.getValueDouble() - fMin) / (fMax - fMin);
			}
			int		x = (int) (kHorizontalMargin + (this.getWidth() - kHorizontalMargin * 2) * percent - 1);
//			System.out.println("percent=" + percent + " x=" + x);
			String	v = fParameter.getValueString(format_Number, fRadix);
			w = fm.stringWidth(v);
			aG.drawString(v, x - w / 2, y);
			aG.fillRect(kHorizontalMargin, kVerticalMargin, x, kBarHeight - 1);
		}
	} // paintComponent

	/**
	The underlying parameter has changed
	*/
	public void refresh() {
		this.repaint();
	}

	/**
	Output this graph as XML
	*/
	public void save(XMLWriter aWriter) throws IOException {
		aWriter.writeElement("min", fMin);
		aWriter.writeElement("max", fMax);
	}

	public void setRadix(char aRadix) {
		fRadix = aRadix;
		this.repaint();
	}

	public void setMax(int aValue) {
		fMax = aValue;
		this.repaint();
	}

	public void setMin(int aValue) {
		fMin = aValue;
		this.repaint();
	}

	/**
	Be sure the menus are current
	*/
	public void updateMenus() {
		System.out.println("BarGraphDisplayer.updateMenus");
		CommandRegistrar.gRegistrar.checkAction("barGraph");
		CommandRegistrar.gRegistrar.enableAction("viewOptions");
	} // updateMenus

	class ViewOptionsDialog extends JDialog {
		final int		kFieldWidth = 10;
		IntegerEditor	fMaxTextField;
		IntegerEditor	fMinTextField;

		public ViewOptionsDialog(Container aParent) {
			super((JFrame) aParent, "Bar Graph Options");	// @@@ localize
			JPanel			holder = new JPanel();
			this.getContentPane().add(holder);
			holder.setBorder(BorderFactory.createEmptyBorder(12, 12, 12, 12));

			JLabel			textLabel = new JLabel("Minimum:");
			holder.add(textLabel);
			fMinTextField = new IntegerEditor(kFieldWidth);
			holder.add(fMinTextField);

			textLabel = new JLabel("Maximum:");
			holder.add(textLabel);
			fMaxTextField = new IntegerEditor(kFieldWidth);
			holder.add(fMaxTextField);

			//button
			// !!! This actually should be 11 pixels from bottom and right
			JButton			button = new JButton("OK");
			button.setMnemonic('O');
			button.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent e) {
					if (!fMaxTextField.validateCurrentValue()) {
						Error.showParseError("Invalid maximum value", "Incorrect integer");
						fMaxTextField.selectAll();
						fMaxTextField.requestFocus();
					} else if (!fMinTextField.validateCurrentValue()) {
						Error.showParseError("Invalid minimum value", "Incorrect integer");
						fMinTextField.selectAll();
						fMinTextField.requestFocus();
					} else {
						BarGraphDisplayer.this.setMax(Format.parseInt(fMaxTextField.getText()));
						BarGraphDisplayer.this.setMin(Format.parseInt(fMinTextField.getText()));

						ViewOptionsDialog.this.setVisible(false);
					}
				}
			});
			holder.add(button);
			this.getRootPane().setDefaultButton(button);

			this.pack();
			this.setResizable(false);
			this.setVisible(false);
		} // ViewOptionsDialog

		/**
		Set up the fields and show the dialog
		*/
		public void setupAndShow() {
			fMaxTextField.setText(BarGraphDisplayer.this.getMaxLabel());
			fMinTextField.setText(BarGraphDisplayer.this.getMinLabel());
			fMinTextField.selectAll();
			fMinTextField.requestFocus();
			this.setVisible(true);
		} // setupAndShow
	} // ViewOptionsDialog
} // BarGraphDisplayer
