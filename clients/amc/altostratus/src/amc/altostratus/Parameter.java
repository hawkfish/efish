/*
File:		Parameter.java

Contains:	Represents a Stratus parameter

Written by:	David Dunham
			Electric Fish, Inc. <http://www.electricfish.com>

Copyright:	Copyright ©2000-2001 by Applied Microsystems Corporation.  All Rights reserved.

Change History (most recent first):

	02 Apr 2001		drd	Send notifyObservers
	26 Mar 2001		drd	Removed error reporting in constructor
	07 Mar 2001		drd	No more format_Control
	05 Mar 2001		drd	Added finalize; handle Double_fixed
	02 Mar 2001		drd	Error reporting is now in the Error class
	01 Mar 2001		drd	Removed unused getCurrentInteger; setCurrentValue; report CORBA errors
	28 Feb 2001		drd	setValue
	27 Feb 2001		drd	writeChange, getValueLong
	21 Feb 2001		drd	fChangePending
	20 Feb 2001		drd	Constructor checks type for mutable/fixed
	06 Feb 2001		drd	Flavor-related changes (including fix of isDataFlavorSupported)
	05 Jan 2001		drd	getValueDouble, getValueInt
	21 Dec 2000		drd	Change to format_Number
	01 Dec 2000		drd	Use Format.toHexString
	30 Nov 2000		drd	type_Signed is now type_Integer; fName -> fLongName, fShortName
	29 Nov 2000		drd	Check for type_None; removed default constructor; getCurrentInteger
	28 Nov 2000		drd	Handle type_Float, and the fact that Stratus returns arrays
	27 Nov 2000		drd	retrieveValue; getValueString handles format_Name
	21 Nov 2000		drd	startObserving
	20 Nov 2000		drd	More hookup (added fObserver)
	17 Nov 2000		drd	CORBA (added fParameter)
	30 Oct 2000		drd	Observable
	24 Oct 2000		drd	Minimal Transferable stuff
	18 Oct 2000		drd	Added accessors; toString
	17 Oct 2000		drd	Created
*/

package amc.altostratus;

import amc.altostratus.constants.ParamConstants;
import java.awt.datatransfer.*;
import java.util.Observable;

// CORBA stuff
import ObserveM.*;
import ParameterM.*;

/**
Represents a Stratus parameter
*/
public class Parameter extends Observable implements ParamConstants, Transferable
{
// Constants

// Instance data
	private	boolean		fMutable;		// Can this be changed by user input?
	private	String		fLongName;
	private	String		fShortName;
	private	char			fType;		// type_Signed, type_Float, etc.
	private	Object		fValue;
	private	ParameterDef	fParameter;		// CORBA object
	private	Observer		fObserver;		// Watches fParameter
	private	boolean			fChangePending;	// Are any edits not yet sent to the model?

//	public static DataFlavor	gFlavor = new DataFlavor(Parameter.class, "Parameter");
	public static DataFlavor	gFlavor = new DataFlavor("application/x-java-jvm-local-objectref", "Parameter");
//	    DataFlavor flavors[] = { new DataFlavor(DataFlavor.javaJVMLocalObjectMimeType) };

	/**
	@param aName
	*/
	public Parameter(String aLongName, String aShortName) {
		fMutable = kImmutable;
		fType = type_None;
		fValue = null;
		fLongName = aLongName;
		fShortName = aShortName;

		if (AltoStratus.gContainer == null) {
			fParameter = null;
		} else {
			try {
				fParameter = AltoStratus.gContainer.getObject(fLongName);
//				System.out.println("fParameter=" + fParameter + " (" + fLongName + ")");
			} catch (org.omg.CORBA.SystemException e) {
				System.out.println("?Parameter(" + aLongName + ") " + e);

				// For some reason, trying again seems to work !!!
				fParameter = AltoStratus.gContainer.getObject(fLongName);
			}
			String		theType = fParameter.getType();
			System.out.println("Parameter " + aShortName + " type=" + theType);
			// In the original version of Stratus, there was no suffix, so we check
			// for the simple version
			if (theType.equals("String") || theType.equals("String_fixed")) {
				fType = type_String;
			} else if (theType.equals("Integer") || theType.equals("Integer_fixed")) {
				fType = type_Integer;
			} else if (theType.equals("Double") || theType.equals("Double_fixed")) {
				fType = type_Float;
			} else if (theType.equals("String_mutable")) {
				fType = type_String;
				fMutable = kMutable;
			} else if (theType.equals("Integer_mutable")) {
				fType = type_Integer;
				fMutable = kMutable;
			} else if (theType.equals("Double_mutable")) {
				fType = type_Float;
				fMutable = kMutable;
			}
			fObserver = null;
			fChangePending = false;
		}
	}

	/**
	This constructor takes an integer (it's really for development only)
	*/
	public Parameter(String aName, char aType, int aValue, boolean aMutable) {
		fLongName = fShortName = aName;
		fType = aType;
		fValue = new Integer(aValue);
		fMutable = aMutable;
	} // Parameter

	/**
	Called by the garbage collector on an object when garbage collection
	determines that there are no more references to the object.
	*/
	protected void finalize() throws Throwable {
		System.out.println("Parameter.finalize");
		fParameter = null;
		fObserver = null;

		super.finalize();	// Doesn't propagate like a C++ destructor…
	}

	public boolean getChangePending() {
		return fChangePending;
	} // getChangePending

	/**
	Returns whether or not this Parameter can be edited by the user
	*/
	public boolean getMutable() {
		return fMutable;
	} // getMutable

	/**
	Returns the complete (dotted) name
	*/
	public String getLongName() {
		return fLongName;
	} // getLongName

	/**
	Returns the short name
	*/
	public String getShortName() {
		return fShortName;
	} // getShortName

	public Object getTransferData(DataFlavor aFlavor) {
//		System.out.println("getTransferData " + aFlavor);
		return this;
	}

	public DataFlavor[] getTransferDataFlavors() {
	    DataFlavor flavors[] = { gFlavor };
	    return flavors;
	}

	/**
	Returns a code indicating the type of data
	*/
	public char getType() {
		return fType;
	} // getType

	/**
	Returns the numeric value
	*/
	public Object getValue() {
		return fValue;
	} // getValue

	/**
	Returns the value coerced to a double
	*/
	public double getValueDouble() {
		double		theInt = ((double[]) fValue)[kCurrentValue];
		return theInt;
	}

	/**
	Returns the value coerced to an integer
	Assumes that we are an integer.
	*/
	public int getValueInt() {
		int		theInt = ((int[]) fValue)[kCurrentValue];
		return theInt;
	}

	/**
	Returns the value coerced to a long
	*/
	public long getValueLong() {
		long	theLong = ((long[]) fValue)[kCurrentValue];
		return theLong;
	} // getValueLong

	/**
	Returns the numeric value, suitably formatted for display.
	@param aFormat How to display the value
	@return The numeric value, formatted
	*/
	public String getValueString(char aFormat, char aRadix) {		
		String		theString = null;
		Integer		theInt;
		Double		theFloat;

		if (fValue == null || fType == type_None)
			return null;
			
		switch (aFormat) {
			case format_Number:
				switch (fType) {
					case type_Integer: {
						theInt = new Integer(((int[]) fValue)[kCurrentValue]);
						switch (aRadix) {
							case radix_Decimal:
								theString = theInt.toString();
								break;
							case radix_Hex:
								theString = Format.toHexString(theInt.intValue());
								break;
						}
						break;
					}
						
					case type_Float:
						double[] f = (double[]) fValue;
						theFloat = new Double(f[kCurrentValue]);
						theString = theFloat.toString();
						break;
				}
				break;

			case format_Name:
				theString = (String) fValue;
		}

		return theString;
	} // getValueString

	/**
	Returns whether the requested flavor is supported by this object.
		@param flavor the requested flavor for the data
	*/
	public boolean isDataFlavorSupported(DataFlavor aFlavor) {
		return (aFlavor.equals(gFlavor));
	} // isDataFlavorSupported

	/**
	Fetch our value from the CORBA object
	*/
	protected Object retrieveValue() {
		switch (fType) {
			case type_CharArray:
				fValue = fParameter.getByteArray();
				break;

			case type_Float:
				fValue = fParameter.getDouble();
				break;

			case type_Long:
				fValue = fParameter.getLongLong();
				break;

			case type_Integer:
				fValue = fParameter.getInteger();
				break;

			case type_String:
				fValue = fParameter.getString();
				break;

			default:
				System.out.println("?retrieveValue-unknown type '" + fType + "'");
		}

		this.setChanged();				// Be sure Observer notification happens
//		System.out.println("%retrieveValue-fType='" + fType + "' value=" + fValue);
		return fValue;
	}

	public void setChangePending(boolean inValue) {
		fChangePending = inValue;
	} // setChangePending

	/**
	Set just the current value (leaving kMaxValue, etc. unchanged)
	*/
	public void setCurrentValue(double aValue) {
		double[]	theArray = ((double[]) fValue);
		theArray[kCurrentValue] = aValue;
		System.out.println("setCurrentValue " + aValue + ", " + this.getValueDouble());
	} // setCurrentValue

	/**
	Set just the current value (leaving kMaxValue, etc. unchanged)
	*/
	public void setCurrentValue(int aValue) {
		int[]	theArray = ((int[]) fValue);
		theArray[kCurrentValue] = aValue;
		System.out.println("setCurrentValue " + aValue + ", " + this.getValueInt());
	} // setCurrentValue

	/**
	Replace our instance data
	*/
	public void setValue(Object aValue) {
		fValue = aValue;
	}

	/**
	Begin receiving notifications of value changed
	*/
	public void startObserving() {
		fObserver = null;						// In case of failure
		try {
			if (fParameter != null) {
				fObserver = new Observer(fParameter);
//				System.out.println("%startObserving " + this + " via " + fObserver);
				this.retrieveValue();	// ??? not needed
			}
		} catch (ObserveM.BadObserveTypeException e) {
			System.out.println("?startObserving-" + e);
		}
	}

	/**
	Convert to printable representation
	*/
	public String toString() {
		String	theString = super.toString() + " ";
		// Get rid of the package prefix
		int		i = theString.lastIndexOf(".");
		theString = theString.substring(i + 1);
		theString += fType + "=" + fValue;
		if (fMutable)
			theString += " (mutable)";
		return theString;
	} // toString

	/**
	Let the Stratus model know about our value
	*/
	public void writeChange() {
		System.out.println("%writeChange-" + this);
		switch (fType) {
			case type_CharArray:
				fParameter.setByteArray((byte []) fValue);
				break;

			case type_Float:
				 fParameter.setDouble(this.getValueDouble());
				break;

			case type_Long:
				fParameter.setLongLong(this.getValueLong());
				break;

			case type_Integer:
				fParameter.setInteger(this.getValueInt());
				break;

			case type_String:
				fParameter.setString((String)fValue);	// getValueString is a formatting call!
				break;

			default:
				System.out.println("?writeChange-unknown type '" + fType + "'");
		}

		this.setChangePending(false);
	} // writeChange

	/**
	Inner class which allows the Parameter to get callbacks. This is an inner class so that
	it can inherit (the default bindings use the inheritance model).
	*/
	class Observer extends _CallbackObjDefImplBase {
		/**
		Constructor
		*/
		public Observer(ParameterDef aParameter) throws ObserveM.BadObserveTypeException {
			short[] types = { ObserveM.ALL_TYPE.value };
			aParameter.subscribe(this, types);
		 }

		/**
		Parameter has changed value
		*/
	    public void update(org.omg.CORBA.Any obj, short[] observerTypes) {
	    	// ??? We need to be sure the parameter is marked as changed, right now
	    	// ??? this is a side-effect of retrieveValue()
	    	Object	v = Parameter.this.retrieveValue();
	    	String	s = Parameter.this.getValueString(format_Number, radix_Hex);
			System.out.println("update " + s);
			if (!fChangePending) {
//				System.out.println("notifying " + Parameter.this.countObservers() + " observers");
				Parameter.this.notifyObservers();
			}
		}
	} // Observer inner class
}
