/*
 * File: ./LOGDEF/LEVELHOLDER.JAVA
 * From: LOGDEF.IDL
 * Date: Fri Nov 10 16:07:52 2000
 *   By: C:\ALTOST~1\IDL\IDLTOJ~1.EXE Java IDL 1.2 Aug 18 1998 16:25:34
 */

package LogDef;
public final class LevelHolder
     implements org.omg.CORBA.portable.Streamable{
    //	instance variable 
    public LogDef.Level value;
    //	constructors 
    public LevelHolder() {
	this(null);
    }
    public LevelHolder(LogDef.Level __arg) {
	value = __arg;
    }

    public void _write(org.omg.CORBA.portable.OutputStream out) {
        LogDef.LevelHelper.write(out, value);
    }

    public void _read(org.omg.CORBA.portable.InputStream in) {
        value = LogDef.LevelHelper.read(in);
    }

    public org.omg.CORBA.TypeCode _type() {
        return LogDef.LevelHelper.type();
    }
}
