//
//  SwingCVS.java
//

import java.awt.*;

public class SwingCVS extends Frame {
    public SwingCVS() {
	super("SwingCVS");
	Label l = new Label("Hello World!");;
	add(l);
	setVisible(true);
    }
    
    public static void main(String args[]) {
	new SwingCVS();
    }
}
