/*
 * File: ./LOGDEF/UMESGHELPER.JAVA
 * From: LOGDEF.IDL
 * Date: Fri Nov 10 16:07:52 2000
 *   By: C:\ALTOST~1\IDL\IDLTOJ~1.EXE Java IDL 1.2 Aug 18 1998 16:25:34
 */

package LogDef;
public class UMesgHelper {
     // It is useless to have instances of this class
     private UMesgHelper() { }

    public static void write(org.omg.CORBA.portable.OutputStream out, byte[] that)  {
          {
              out.write_long(that.length);
              out.write_octet_array(that, 0, that.length);
          }
    }
    public static byte[] read(org.omg.CORBA.portable.InputStream in) {
          byte[] that;
          {
              int __length = in.read_long();
              that = new byte[__length];
              in.read_octet_array(that, 0, that.length);
          }
          return that;
    }
   public static byte[] extract(org.omg.CORBA.Any a) {
     org.omg.CORBA.portable.InputStream in = a.create_input_stream();
     return read(in);
   }
   public static void insert(org.omg.CORBA.Any a, byte[] that) {
     org.omg.CORBA.portable.OutputStream out = a.create_output_stream();
     a.type(type());
     write(out, that);
     a.read_value(out.create_input_stream(), type());
   }
   private static org.omg.CORBA.TypeCode _tc;
   synchronized public static org.omg.CORBA.TypeCode type() {
          if (_tc == null)
             _tc = org.omg.CORBA.ORB.init().create_alias_tc(id(), "UMesg", org.omg.CORBA.ORB.init().create_sequence_tc(0, org.omg.CORBA.ORB.init().get_primitive_tc(org.omg.CORBA.TCKind.tk_octet)));
      return _tc;
   }
   public static String id() {
       return "IDL:LogDef/UMesg:1.0";
   }
}
