/*
 * File: ./OBSERVEM/REGISTERDEFHELPER.JAVA
 * From: OBSERVEDEF.IDL
 * Date: Fri Nov 10 16:17:22 2000
 *   By: C:\ALTOST~1\IDL\IDLTOJ~1.EXE Java IDL 1.2 Aug 18 1998 16:25:34
 */

package ObserveM;
public class RegisterDefHelper {
     // It is useless to have instances of this class
     private RegisterDefHelper() { }

    public static void write(org.omg.CORBA.portable.OutputStream out, ObserveM.RegisterDef that) {
        out.write_Object(that);
    }
    public static ObserveM.RegisterDef read(org.omg.CORBA.portable.InputStream in) {
        return ObserveM.RegisterDefHelper.narrow(in.read_Object());
    }
   public static ObserveM.RegisterDef extract(org.omg.CORBA.Any a) {
     org.omg.CORBA.portable.InputStream in = a.create_input_stream();
     return read(in);
   }
   public static void insert(org.omg.CORBA.Any a, ObserveM.RegisterDef that) {
     org.omg.CORBA.portable.OutputStream out = a.create_output_stream();
     write(out, that);
     a.read_value(out.create_input_stream(), type());
   }
   private static org.omg.CORBA.TypeCode _tc;
   synchronized public static org.omg.CORBA.TypeCode type() {
          if (_tc == null)
             _tc = org.omg.CORBA.ORB.init().create_interface_tc(id(), "RegisterDef");
      return _tc;
   }
   public static String id() {
       return "IDL:ObserveM/RegisterDef:1.0";
   }
   public static ObserveM.RegisterDef narrow(org.omg.CORBA.Object that)
	    throws org.omg.CORBA.BAD_PARAM {
        if (that == null)
            return null;
        if (that instanceof ObserveM.RegisterDef)
            return (ObserveM.RegisterDef) that;
	if (!that._is_a(id())) {
	    throw new org.omg.CORBA.BAD_PARAM();
	}
        org.omg.CORBA.portable.Delegate dup = ((org.omg.CORBA.portable.ObjectImpl)that)._get_delegate();
        ObserveM.RegisterDef result = new ObserveM._RegisterDefStub(dup);
        return result;
   }
}
