/*
 * File: ./ROOTFACTORYDEF/CREATEOBJECTEXCEPTION.JAVA
 * From: ROOTFACTORYDEF.IDL
 * Date: Wed Nov 08 16:42:26 2000
 *   By: C:\ALTOST~1\IDL\IDLTOJ~1.EXE Java IDL 1.2 Aug 18 1998 16:25:34
 */

package RootFactoryDef;
public final class CreateObjectException
	extends org.omg.CORBA.UserException implements org.omg.CORBA.portable.IDLEntity {
    //	instance variables
    public String msg;
    //	constructors
    public CreateObjectException() {
	super();
    }
    public CreateObjectException(String __msg) {
	super();
	msg = __msg;
    }
}
