/*
 * File: ./OBSERVEM/CALLBACKOBJDEFHOLDER.JAVA
 * From: OBSERVEDEF.IDL
 * Date: Fri Nov 10 16:17:22 2000
 *   By: C:\ALTOST~1\IDL\IDLTOJ~1.EXE Java IDL 1.2 Aug 18 1998 16:25:34
 */

package ObserveM;
public final class CallbackObjDefHolder
     implements org.omg.CORBA.portable.Streamable{
    //	instance variable 
    public ObserveM.CallbackObjDef value;
    //	constructors 
    public CallbackObjDefHolder() {
	this(null);
    }
    public CallbackObjDefHolder(ObserveM.CallbackObjDef __arg) {
	value = __arg;
    }

    public void _write(org.omg.CORBA.portable.OutputStream out) {
        ObserveM.CallbackObjDefHelper.write(out, value);
    }

    public void _read(org.omg.CORBA.portable.InputStream in) {
        value = ObserveM.CallbackObjDefHelper.read(in);
    }

    public org.omg.CORBA.TypeCode _type() {
        return ObserveM.CallbackObjDefHelper.type();
    }
}
