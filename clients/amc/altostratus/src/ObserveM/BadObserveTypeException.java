/*
 * File: ./OBSERVEM/BADOBSERVETYPEEXCEPTION.JAVA
 * From: OBSERVEDEF.IDL
 * Date: Fri Nov 10 16:17:22 2000
 *   By: C:\ALTOST~1\IDL\IDLTOJ~1.EXE Java IDL 1.2 Aug 18 1998 16:25:34
 */

package ObserveM;
public final class BadObserveTypeException
	extends org.omg.CORBA.UserException implements org.omg.CORBA.portable.IDLEntity {
    //	instance variables
    public String msg;
    //	constructors
    public BadObserveTypeException() {
	super();
    }
    public BadObserveTypeException(String __msg) {
	super();
	msg = __msg;
    }
}
