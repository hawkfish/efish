/*
 * File: ./OBSERVEM/CALLBACKOBJDEFHELPER.JAVA
 * From: OBSERVEDEF.IDL
 * Date: Fri Nov 10 16:17:22 2000
 *   By: C:\ALTOST~1\IDL\IDLTOJ~1.EXE Java IDL 1.2 Aug 18 1998 16:25:34
 */

package ObserveM;
public class CallbackObjDefHelper {
     // It is useless to have instances of this class
     private CallbackObjDefHelper() { }

    public static void write(org.omg.CORBA.portable.OutputStream out, ObserveM.CallbackObjDef that) {
        out.write_Object(that);
    }
    public static ObserveM.CallbackObjDef read(org.omg.CORBA.portable.InputStream in) {
        return ObserveM.CallbackObjDefHelper.narrow(in.read_Object());
    }
   public static ObserveM.CallbackObjDef extract(org.omg.CORBA.Any a) {
     org.omg.CORBA.portable.InputStream in = a.create_input_stream();
     return read(in);
   }
   public static void insert(org.omg.CORBA.Any a, ObserveM.CallbackObjDef that) {
     org.omg.CORBA.portable.OutputStream out = a.create_output_stream();
     write(out, that);
     a.read_value(out.create_input_stream(), type());
   }
   private static org.omg.CORBA.TypeCode _tc;
   synchronized public static org.omg.CORBA.TypeCode type() {
          if (_tc == null)
             _tc = org.omg.CORBA.ORB.init().create_interface_tc(id(), "CallbackObjDef");
      return _tc;
   }
   public static String id() {
       return "IDL:ObserveM/CallbackObjDef:1.0";
   }
   public static ObserveM.CallbackObjDef narrow(org.omg.CORBA.Object that)
	    throws org.omg.CORBA.BAD_PARAM {
        if (that == null)
            return null;
        if (that instanceof ObserveM.CallbackObjDef)
            return (ObserveM.CallbackObjDef) that;
	if (!that._is_a(id())) {
	    throw new org.omg.CORBA.BAD_PARAM();
	}
        org.omg.CORBA.portable.Delegate dup = ((org.omg.CORBA.portable.ObjectImpl)that)._get_delegate();
        ObserveM.CallbackObjDef result = new ObserveM._CallbackObjDefStub(dup);
        return result;
   }
}
