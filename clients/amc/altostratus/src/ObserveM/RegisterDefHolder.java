/*
 * File: ./OBSERVEM/REGISTERDEFHOLDER.JAVA
 * From: OBSERVEDEF.IDL
 * Date: Fri Nov 10 16:17:22 2000
 *   By: C:\ALTOST~1\IDL\IDLTOJ~1.EXE Java IDL 1.2 Aug 18 1998 16:25:34
 */

package ObserveM;
public final class RegisterDefHolder
     implements org.omg.CORBA.portable.Streamable{
    //	instance variable 
    public ObserveM.RegisterDef value;
    //	constructors 
    public RegisterDefHolder() {
	this(null);
    }
    public RegisterDefHolder(ObserveM.RegisterDef __arg) {
	value = __arg;
    }

    public void _write(org.omg.CORBA.portable.OutputStream out) {
        ObserveM.RegisterDefHelper.write(out, value);
    }

    public void _read(org.omg.CORBA.portable.InputStream in) {
        value = ObserveM.RegisterDefHelper.read(in);
    }

    public org.omg.CORBA.TypeCode _type() {
        return ObserveM.RegisterDefHelper.type();
    }
}
