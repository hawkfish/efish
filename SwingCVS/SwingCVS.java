//
//  SwingCVS.java
//

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.util.Enumeration;
import java.util.Vector;

public class SwingCVS 
	
	{ // begin SwingCVS
	
	public void
	execCVS (
		
		JTextArea	inTextArea,
		JScrollPane	inTextScroll,
		String[] 	inArgs)
		
		throws	java.io.IOException
		,		java.lang.InterruptedException
		
		{ // begin execCVS
			
			//	Start cvs
			Process 		cvs = Runtime.getRuntime ().exec (inArgs);
			DataInputStream	cvsout = new DataInputStream (cvs.getInputStream ());
			
			//	Read the lines
			JScrollBar	vScroll = inTextScroll.getVerticalScrollBar ();
			for (String line = cvsout.readLine (); line != null; line = cvsout.readLine ()) {
				inTextArea.append (line);
				inTextArea.append ("\n");
				vScroll.setValue (vScroll.getMaximum ());
				} // for
							
			//	Wait for it to quit
			cvs.waitFor ();
						
		} // end execCVS
		
	public Component
	createComponents ()
		
		{ // begin createComponents
		
			final	JTextArea	stdoutText = new JTextArea ();
			
			final	JScrollPane	stdOutScroll = new JScrollPane (stdoutText);
			stdOutScroll.setVerticalScrollBarPolicy (JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
			stdOutScroll.setPreferredSize (new Dimension (250, 250));
			
			JButton button = new JButton ("cvs --version");
			button.addActionListener (new ActionListener () {
				public void
				actionPerformed (ActionEvent event)
					{
						try {
							String[]	args = {"cvs", "--version"};
							execCVS (stdoutText, stdOutScroll, args);
							} // try
							
						catch (java.lang.Exception e) {
							stdoutText.append (e.toString ());
							stdoutText.append ("\n");
							} // catch
					}
				});
			
			JPanel pane = new JPanel ();
			pane.setBorder (BorderFactory.createEmptyBorder (30, 30, 10, 30));
			pane.setLayout (new GridLayout (0, 1));
			pane.add (stdOutScroll);
			pane.add (button);
			
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
