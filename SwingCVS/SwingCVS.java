//
//  SwingCVS.java
//

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

public class SwingCVS 
	
	{ // begin SwingCVS
	
	private static String labelPrefix = "Number of button clicks: ";
	private int numClicks = 0;
	
	public Component
	createComponents ()
		
		{ // begin createComponents
		
			final	JLabel	label = new JLabel (labelPrefix + "0     ");
			
			JButton button = new JButton ("I'm a Swing button!");
			button.addActionListener (new ActionListener () {
				public void
				actionPerformed (ActionEvent e)
					{label.setText (labelPrefix + ++numClicks);}
				});
			label.setLabelFor (button);
			
			JPanel pane = new JPanel ();
			pane.setBorder (BorderFactory.createEmptyBorder (30, 30, 10, 30));
			pane.setLayout (new GridLayout (0, 1));
			pane.add (button);
			pane.add (label);
			
			return pane;
			
		} // end createComponents
		    
    public static void 
	main (String args[]) 
	
		{ // begin main
			
			try {
				UIManager.setLookAndFeel (UIManager.getCrossPlatformLookAndFeelClassName ());
				} // try
				
			catch (Exception e) {
				} // catch
				
			JFrame	frame = new JFrame ("SwingCVS");
			SwingCVS app = new SwingCVS ();
			Component contents = app.createComponents ();
			frame.getContentPane ().add (contents, BorderLayout.CENTER);
			
			frame.addWindowListener (new WindowAdapter () {
				public void
				windowClosing (WindowEvent e)
					{System.exit (0);}
				});
				
			frame.pack ();
			frame.setVisible (true);
			
		} // end main

	} // end SwingCVS
