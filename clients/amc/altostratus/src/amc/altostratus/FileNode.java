/*
File:		FileNode.java

Contains:	Represents a file saved in a TreeModel 

Written by:	David Dunham
		Electric Fish, Inc. <http://www.electricfish.com>

Copyright:	Copyright ©2001 by Applied Microsystems Corporation.  All Rights reserved.

Change History (most recent first):

	14 Mar 2001		drd	Created
*/

package amc.altostratus;

import java.io.File;
import javax.swing.tree.*;

public class FileNode extends DefaultMutableTreeNode {
	protected	File	fFile;

	/**
	Constructor
		@param aFile The file to save
	*/
	public FileNode(File aFile) {
		super(aFile, false);	// We never have kids
		fFile = aFile;

		// Save in instance data for inherited
		userObject = fFile.getName();
	}

	public File getFile() {
		return fFile;
	}
}
