/*
 * File: ./LOGDEF/ERRHELPER.JAVA
 * From: LOGDEF.IDL
 * Date: Fri Nov 10 16:07:52 2000
 *   By: C:\ALTOST~1\IDL\IDLTOJ~1.EXE Java IDL 1.2 Aug 18 1998 16:25:34
 */

package LogDef;
public class errHelper {
     // It is useless to have instances of this class
     private errHelper() { }

    public static void write(org.omg.CORBA.portable.OutputStream out, LogDef.err that) {
    out.write_string(id());

	out.write_string(that.msg);
    }
    public static LogDef.err read(org.omg.CORBA.portable.InputStream in) {
        LogDef.err that = new LogDef.err();
         // read and discard the repository id
        in.read_string();

	that.msg = in.read_string();
    return that;
    }
   public static LogDef.err extract(org.omg.CORBA.Any a) {
     org.omg.CORBA.portable.InputStream in = a.create_input_stream();
     return read(in);
   }
   public static void insert(org.omg.CORBA.Any a, LogDef.err that) {
     org.omg.CORBA.portable.OutputStream out = a.create_output_stream();
     write(out, that);
     a.read_value(out.create_input_stream(), type());
   }
   private static org.omg.CORBA.TypeCode _tc;
   synchronized public static org.omg.CORBA.TypeCode type() {
       int _memberCount = 1;
       org.omg.CORBA.StructMember[] _members = null;
          if (_tc == null) {
               _members= new org.omg.CORBA.StructMember[1];
               _members[0] = new org.omg.CORBA.StructMember(
                 "msg",
                 org.omg.CORBA.ORB.init().get_primitive_tc(org.omg.CORBA.TCKind.tk_string),
                 null);
             _tc = org.omg.CORBA.ORB.init().create_exception_tc(id(), "err", _members);
          }
      return _tc;
   }
   public static String id() {
       return "IDL:LogDef/err:1.0";
   }
}
