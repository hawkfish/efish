/*
 * File: ./OBSERVEM/STATUSBITLISTHELPER.JAVA
 * From: OBSERVEDEF.IDL
 * Date: Fri Nov 10 16:17:22 2000
 *   By: C:\ALTOST~1\IDL\IDLTOJ~1.EXE Java IDL 1.2 Aug 18 1998 16:25:34
 */

package ObserveM;
public class StatusBitListHelper {
     // It is useless to have instances of this class
     private StatusBitListHelper() { }

    public static void write(org.omg.CORBA.portable.OutputStream out, short[] that)  {
          {
              out.write_long(that.length);
              out.write_ushort_array(that, 0, that.length);
          }
    }
    public static short[] read(org.omg.CORBA.portable.InputStream in) {
          short[] that;
          {
              int __length = in.read_long();
              that = new short[__length];
              in.read_ushort_array(that, 0, that.length);
          }
          return that;
    }
   public static short[] extract(org.omg.CORBA.Any a) {
     org.omg.CORBA.portable.InputStream in = a.create_input_stream();
     return read(in);
   }
   public static void insert(org.omg.CORBA.Any a, short[] that) {
     org.omg.CORBA.portable.OutputStream out = a.create_output_stream();
     a.type(type());
     write(out, that);
     a.read_value(out.create_input_stream(), type());
   }
   private static org.omg.CORBA.TypeCode _tc;
   synchronized public static org.omg.CORBA.TypeCode type() {
          if (_tc == null)
             _tc = org.omg.CORBA.ORB.init().create_alias_tc(id(), "StatusBitList", org.omg.CORBA.ORB.init().create_sequence_tc(0, org.omg.CORBA.ORB.init().get_primitive_tc(org.omg.CORBA.TCKind.tk_ushort)));
      return _tc;
   }
   public static String id() {
       return "IDL:ObserveM/StatusBitList:1.0";
   }
}
