/*
 * File: ./LOGDEF/UMESGHOLDER.JAVA
 * From: LOGDEF.IDL
 * Date: Fri Nov 10 16:07:52 2000
 *   By: C:\ALTOST~1\IDL\IDLTOJ~1.EXE Java IDL 1.2 Aug 18 1998 16:25:34
 */

package LogDef;
public final class UMesgHolder
    implements org.omg.CORBA.portable.Streamable
{
    //	instance variable 
    public byte[] value;
    //	constructors 
    public UMesgHolder() {
	this(null);
    }
    public UMesgHolder(byte[] __arg) {
	value = __arg;
    }
    public void _write(org.omg.CORBA.portable.OutputStream out) {
        LogDef.UMesgHelper.write(out, value);
    }

    public void _read(org.omg.CORBA.portable.InputStream in) {
        value = LogDef.UMesgHelper.read(in);
    }

    public org.omg.CORBA.TypeCode _type() {
        return LogDef.UMesgHelper.type();
    }
}
