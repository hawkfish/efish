/*
 * File: ./LOGDEF/UMESGLEVELLISTHELPER.JAVA
 * From: LOGDEF.IDL
 * Date: Fri Nov 10 16:07:52 2000
 *   By: C:\ALTOST~1\IDL\IDLTOJ~1.EXE Java IDL 1.2 Aug 18 1998 16:25:34
 */

package LogDef;
public class UMesgLevelListHelper {
     // It is useless to have instances of this class
     private UMesgLevelListHelper() { }

    public static void write(org.omg.CORBA.portable.OutputStream out, LogDef.UMesgLevelRec[] that)  {
          {
              out.write_long(that.length);
              for (int __index = 0 ; __index < that.length ; __index += 1) {
                  LogDef.UMesgLevelRecHelper.write(out, that[__index]);
              }
          }
    }
    public static LogDef.UMesgLevelRec[] read(org.omg.CORBA.portable.InputStream in) {
          LogDef.UMesgLevelRec[] that;
          {
              int __length = in.read_long();
              that = new LogDef.UMesgLevelRec[__length];
              for (int __index = 0 ; __index < that.length ; __index += 1) {
                  that[__index] = LogDef.UMesgLevelRecHelper.read(in);
              }
          }
          return that;
    }
   public static LogDef.UMesgLevelRec[] extract(org.omg.CORBA.Any a) {
     org.omg.CORBA.portable.InputStream in = a.create_input_stream();
     return read(in);
   }
   public static void insert(org.omg.CORBA.Any a, LogDef.UMesgLevelRec[] that) {
     org.omg.CORBA.portable.OutputStream out = a.create_output_stream();
     a.type(type());
     write(out, that);
     a.read_value(out.create_input_stream(), type());
   }
   private static org.omg.CORBA.TypeCode _tc;
   synchronized public static org.omg.CORBA.TypeCode type() {
          if (_tc == null)
             _tc = org.omg.CORBA.ORB.init().create_alias_tc(id(), "UMesgLevelList", org.omg.CORBA.ORB.init().create_sequence_tc(0, LogDef.UMesgLevelRecHelper.type()));
      return _tc;
   }
   public static String id() {
       return "IDL:LogDef/UMesgLevelList:1.0";
   }
}
