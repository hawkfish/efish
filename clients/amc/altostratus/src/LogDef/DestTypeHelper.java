/*
 * File: ./LOGDEF/DESTTYPEHELPER.JAVA
 * From: LOGDEF.IDL
 * Date: Fri Nov 10 16:07:52 2000
 *   By: C:\ALTOST~1\IDL\IDLTOJ~1.EXE Java IDL 1.2 Aug 18 1998 16:25:34
 */

package LogDef;
public class DestTypeHelper {
     // It is useless to have instances of this class
     private DestTypeHelper() { }

    public static void write(org.omg.CORBA.portable.OutputStream out, LogDef.DestType that)  {
    out.write_long(that.value());
    }
    public static LogDef.DestType read(org.omg.CORBA.portable.InputStream in)  {
    return LogDef.DestType.from_int(in.read_long());
    }
   public static LogDef.DestType extract(org.omg.CORBA.Any a) {
     org.omg.CORBA.portable.InputStream in = a.create_input_stream();
     return read(in);
   }
   public static void insert(org.omg.CORBA.Any a, LogDef.DestType that) {
     org.omg.CORBA.portable.OutputStream out = a.create_output_stream();
     write(out, that);
     a.read_value(out.create_input_stream(), type());
   }
   private static org.omg.CORBA.TypeCode _tc;
   private static final int _memberCount = 4;
   private static String[] _members = {
                  "STDOUT",
                  "STDERR",
                  "DISKFILE",
                  "MEMORY"
   };
   synchronized public static org.omg.CORBA.TypeCode type() {
          if (_tc == null)
             _tc = org.omg.CORBA.ORB.init().create_enum_tc(id(), "DestType", _members);
      return _tc;
   }
   public static String id() {
       return "IDL:LogDef/DestType:1.0";
   }
}
