/*
 * File: ./OBSERVEM/STATUSBITSHELPER.JAVA
 * From: OBSERVEDEF.IDL
 * Date: Fri Nov 10 16:17:22 2000
 *   By: C:\ALTOST~1\IDL\IDLTOJ~1.EXE Java IDL 1.2 Aug 18 1998 16:25:34
 */

package ObserveM;
public class StatusBitsHelper {
     // It is useless to have instances of this class
     private StatusBitsHelper() { }

    public static void write(org.omg.CORBA.portable.OutputStream out, short that)  {
          out.write_ushort(that);
    }
    public static short read(org.omg.CORBA.portable.InputStream in) {
          short that;
          that = in.read_ushort();
          return that;
    }
   public static short extract(org.omg.CORBA.Any a) {
     org.omg.CORBA.portable.InputStream in = a.create_input_stream();
     return read(in);
   }
   public static void insert(org.omg.CORBA.Any a, short that) {
     org.omg.CORBA.portable.OutputStream out = a.create_output_stream();
     a.type(type());
     write(out, that);
     a.read_value(out.create_input_stream(), type());
   }
   private static org.omg.CORBA.TypeCode _tc;
   synchronized public static org.omg.CORBA.TypeCode type() {
          if (_tc == null)
             _tc = org.omg.CORBA.ORB.init().create_alias_tc(id(), "StatusBits", org.omg.CORBA.ORB.init().get_primitive_tc(org.omg.CORBA.TCKind.tk_ushort));
      return _tc;
   }
   public static String id() {
       return "IDL:ObserveM/StatusBits:1.0";
   }
}
