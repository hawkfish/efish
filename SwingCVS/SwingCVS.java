//
//  SwingCVS.java
//

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

public class SwingCVS 
	
	{ // begin SwingCVS
	
	public Component
	createComponents ()
		
		{ // begin createComponents
		
			final	JLabel	label = new JLabel ("Hello World!");
			
			JPanel pane = new JPanel ();
			pane.setBorder (BorderFactory.createEmptyBorder (30, 30, 10, 30));
			pane.setLayout (new GridLayout (0, 1));
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
