/*
 * File: ./LOGDEF/ERR.JAVA
 * From: LOGDEF.IDL
 * Date: Fri Nov 10 16:07:52 2000
 *   By: C:\ALTOST~1\IDL\IDLTOJ~1.EXE Java IDL 1.2 Aug 18 1998 16:25:34
 */

package LogDef;
public final class err
	extends org.omg.CORBA.UserException implements org.omg.CORBA.portable.IDLEntity {
    //	instance variables
    public String msg;
    //	constructors
    public err() {
	super();
    }
    public err(String __msg) {
	super();
	msg = __msg;
    }
}
