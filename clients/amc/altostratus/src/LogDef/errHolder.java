/*
 * File: ./LOGDEF/ERRHOLDER.JAVA
 * From: LOGDEF.IDL
 * Date: Fri Nov 10 16:07:52 2000
 *   By: C:\ALTOST~1\IDL\IDLTOJ~1.EXE Java IDL 1.2 Aug 18 1998 16:25:34
 */

package LogDef;
public final class errHolder
     implements org.omg.CORBA.portable.Streamable{
    //	instance variable 
    public LogDef.err value;
    //	constructors 
    public errHolder() {
	this(null);
    }
    public errHolder(LogDef.err __arg) {
	value = __arg;
    }

    public void _write(org.omg.CORBA.portable.OutputStream out) {
        LogDef.errHelper.write(out, value);
    }

    public void _read(org.omg.CORBA.portable.InputStream in) {
        value = LogDef.errHelper.read(in);
    }

    public org.omg.CORBA.TypeCode _type() {
        return LogDef.errHelper.type();
    }
}
