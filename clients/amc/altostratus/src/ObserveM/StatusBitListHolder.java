/*
 * File: ./OBSERVEM/STATUSBITLISTHOLDER.JAVA
 * From: OBSERVEDEF.IDL
 * Date: Fri Nov 10 16:17:22 2000
 *   By: C:\ALTOST~1\IDL\IDLTOJ~1.EXE Java IDL 1.2 Aug 18 1998 16:25:34
 */

package ObserveM;
public final class StatusBitListHolder
    implements org.omg.CORBA.portable.Streamable
{
    //	instance variable 
    public short[] value;
    //	constructors 
    public StatusBitListHolder() {
	this(null);
    }
    public StatusBitListHolder(short[] __arg) {
	value = __arg;
    }
    public void _write(org.omg.CORBA.portable.OutputStream out) {
        ObserveM.StatusBitListHelper.write(out, value);
    }

    public void _read(org.omg.CORBA.portable.InputStream in) {
        value = ObserveM.StatusBitListHelper.read(in);
    }

    public org.omg.CORBA.TypeCode _type() {
        return ObserveM.StatusBitListHelper.type();
    }
}
